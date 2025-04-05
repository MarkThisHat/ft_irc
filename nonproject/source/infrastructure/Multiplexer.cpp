#include "Multiplexer.hpp"
#include "Client.hpp"
#include "CommandHandler.hpp"
#include "Utils.hpp"


Multiplexer::Multiplexer(int server_fd) {
    _server_fd = server_fd;
    _epoll_fd = epoll_create1(0);
    if (_epoll_fd == -1)
        throw std::runtime_error("Failed to create epoll file descriptor");

    /*
        int epoll_create1(int size);
        - size: O número máximo de descritores de arquivo que você espera monitorar
            (no Linux, pode ser ignorado, mas deve ser fornecido).
        - Retorna um fd para o "monitor" de epoll. Se falhar, retorna -1.
    */
}

Multiplexer::~Multiplexer() {
    if (_epoll_fd != -1)
        ::close(_epoll_fd);

    // Destruir todos os clients
    std::map<int, Client*>::iterator client_it;
    for (client_it = _clients.begin(); client_it != _clients.end(); ++client_it) {
        Client* client = client_it->second;
        delete client;
    }
    _clients.clear();
}


// Getters

int                         Multiplexer::get_epoll_fd() const { return _epoll_fd; }

std::map<int, Client*>&     Multiplexer::get_clients() { return _clients; }

Client*                     Multiplexer::get_client(std::string target) {
    std::map<int, Client*>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); ++it) {
        Client* client = it->second;
        if (client->get_nickname() == target)
            return client;
    }

    return NULL;
}


// Métodos

// ADICIONA UM NOVO FD PARA SER MONITORADO
void Multiplexer::subscribe_fd_for_monitoring(int fd) {
    epoll_event event;

    bzero(&event, sizeof(epoll_event));
    event.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;
    event.data.fd = fd;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
        throw std::runtime_error("Failed to add fd to epoll");

    /*
        EPOLLIN
        - Conexões de clientes:
            No início, você está interessado em saber quando um cliente tenta 
            se conectar ao servidor (quando o socket de servidor tem uma 
            solicitação de conexão pendente). Nesse caso, você usaria EPOLLIN, 
            que indicaria que o servidor deve chamar accept() para aceitar a 
            conexão do cliente.
        - Mensagens de clientes já conectados:
            Depois que um cliente se conecta ao servidor e você aceita a conexão, 
            o socket do cliente precisa ser monitorado para ler mensagens 
            enviadas por esse cliente. Isso também requer o uso de EPOLLIN, porque 
            você está interessado em dados que o cliente envia ao servidor.

        - epoll_event é uma struct que descreve o que você quer monitorar 
            para o socket em questão. Ele contém o atributo "events" para o tipo
            de evento que quer monitorar (EPOLLIN, EPOLLOUT, EPOLLERR, etc.) e
            "data" onde são armazenados dados associados ao fd (como o ID do socket).
        - epoll_ctl é usada para modificar o conjunto de arquivos monitorados 
            pelo epoll_fd.
        - EPOLL_CTL_ADD especifica que estamos adicionando um novo fd (socket)
            para ser monitorado.
    */
}

// REMOVE UM FD DO MONITORAMENTO
void Multiplexer::unsubscribe_fd_for_monitoring(int fd) {
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL) < 0)
        throw std::runtime_error("Error while removing client fd from epoll");

    /*
        - EPOLL_CTL_DEL especifica que estamos removendo um fd (socket)
            da lista de eventos monitorados.
    */
}

// LOOPING QUE FICA MONITORANDO EVENTOS
int Multiplexer::check_for_events() {

    int total_events = epoll_wait(_epoll_fd, _events, MAX_EVENTS, -1); //implementar o sinal de sair do programa
    if (total_events == -1) {
        if (errno == EINTR) {
            // Epoll was interrupted by a signal, return 0 to indicate no new events
            return 0;
        }
        throw std::runtime_error("Error while polling with epoll");
    }


    return total_events;

    /*
        - epoll_wait é uma função que bloqueia o thread até que um evento 
            ocorra em um dos fds monitorados pelo epoll.
        - _events: Um array onde os eventos que ocorreram serão armazenados. 
            A função preencherá esse array com informações sobre os descritores 
            de arquivos que estão prontos para serem lidos/escritos.
        - MAX_EVENTS: Número máximo de eventos que podem ser retornados 
            na chamada de epoll_wait. Caso ultrapasse, o evento será perdido.
        - timeout (-1): Valor para bloqueio indefinido (espera até que pelo 
            menos um evento aconteça). Ou seja, o servidor vai "esperar" sem 
            fazer nada até que um evento relevante (como dados para ler, uma 
            nova conexão, etc.) ocorra em um dos fds monitorados. 
    */

}

// PARSER PARA GERENCIAR OS DIFERENTES EVENTOS
void Multiplexer::handle_events(int total_events, CommandHandler* handler) {
    for (int i = 0; i < total_events; i++) {

        // Eventos do servidor
        if (_events[i].data.fd == _server_fd) {
            // Nova conexão (novo client tentando conectar)
            if (_events[i].events & EPOLLIN)
                connect_client(_events[i].data.fd);


        // Eventos do client
        } else {
            // Se o evento for de desconexão ou erro do client
            if ((_events[i].events & (EPOLLHUP | EPOLLERR)))
                disconnect_client(_events[i].data.fd);

            if (_events[i].events & EPOLLIN)
                handle_client(_events[i].data.fd, handler);

        }
    }
}

// DISCONECTA O CLIENT DO SERVER
void Multiplexer::disconnect_client(int client_fd) {
    unsubscribe_fd_for_monitoring(client_fd);

    std::map<int, Client*>::iterator it = _clients.find(client_fd);
    if (it != _clients.end()) {
        Client* client = it->second;
        _clients.erase(it);
        delete client;
    }

    ::close(client_fd);

    std::string datetime = Utils::get_time();
    std::cout << "[" << datetime << "] Client disconnected." << std::endl;
}

// CONECTA UM CLIENT AO SERVER
int Multiplexer::connect_client(int server_fd) {
    sockaddr_in addr = {};
    socklen_t   size = sizeof(addr);

    int client_fd = _accept_connection(server_fd, &addr, &size);
    Client* client = _create_client(client_fd, addr);

    _clients.insert(std::make_pair(client_fd, client));
    subscribe_fd_for_monitoring(client_fd);

    std::string datetime = Utils::get_time();
    std::cout << "[" << datetime << "] Client connected." << std::endl;
    return client_fd;
}


// HANDLE CLIENTS
void Multiplexer::handle_client(int client_fd, CommandHandler* handler) {
    try {
        Client*     client = _clients.at(client_fd);
        std::string message = read_client_message(client_fd);
        
        handler->handle_command(client, message);
    } catch (const std::exception& e) {
        std::cout << "Error while handling the client message! " << e.what() << std::endl;
    }
}

// MINI GNL
std::string Multiplexer::read_client_message(int client_fd) {
    std::string message;
    try {
        
        char buffer[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);

        // Vai salvando a mensagem no buffer, e depois agrega em "message"
        while (!strstr(buffer, "\n"))
        {
            bzero(buffer, BUFFER_SIZE);

            /* CODIGO COMENTADO POIS EWOULDBLOCK VIOLARIA A REGRA DE USAR O ERRNO COM EQUIVALENTE AO poll()
            if ((recv(client_fd, buffer, BUFFER_SIZE, 0) <= 0) and (errno != EWOULDBLOCK)) {
                disconnect_client(client_fd);
                return "";
            }
            */
            ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
            if (bytes_received == 0) {
                // Client has closed the connection
                disconnect_client(client_fd);
                return "";
            } else if (bytes_received < 0) {
                // Just return and wait for epoll_wait() to signal readiness again
                return "";
            }
            

            /*
                ssize_t recv(int sockfd, void *buf, size_t len, int flags);
                - sockfd é o fd do client de quem está vindo a mensagem.
                - buf é o ponteiro para o buffer, o bloco de memória com os dados a 
                    serem lidos.
                - len é o tamanho do buffer, definido o tamanhno máximo através 
                    da constante BUFFER_SIZE.
                - flags permite especificar algumas opções adicionais, mas 0 significa 
                    que nenhuma foi definida.
                - Se retornar -1, significa erro. Se retornar 0, significa que a
                    conexão foi encerrada por parte do client. E o valor positivo 
                    corresponde ao número de bytes recebidos.
            */ 

            message.append(buffer);
        }

        return message;
        // Apaga a última quebra de linha
        // if (!message.empty() && message[message.size() - 1] == '\n')
        //     message.erase(message.size() - 1);

        // Imprime a mensagem do terminal, identificada pelo FD do usuário
        // std::cout << client_fd << ": " << message << std::endl;

        // Apenas para testar. Se o usuário escreve exit(), é removido

    } catch (const std::exception& e) {
        std::cout << "Error processing the message: " << e.what() << std::endl;
        return message;
    }
}

void Multiplexer::send_client_message(int client_fd, const std::string& message) {
    std::string buffer = message + "\r\n";
    if (send(client_fd, buffer.c_str(), buffer.length(), 0) == -1)
        throw std::runtime_error("Error while sending a message to a client!");
    /*
        ssize_t send(int sockfd, const void *buf, size_t len, int flags);
        Enquanto o recv lê mensagens, o send envia dados através de um 
        socket.
        - sockfd é o fd do client que está enviando a mensagem.
        - buf é o ponteiro para o buffer, o bloco de memória com os dados a 
            serem enviados. Foi convertido para c-string.
        - len é o tamanho do buffer.
        - flags permite especificar algumas opções adicionais, mas 0 significa 
            que nenhuma foi definida.
        - Se retornar -1, significa erro. E o valor positivo corresponde ao
            número de bytes enviados.
    */
}


// MÉTODOS AINDA NÃO UTILIZADOS
void Multiplexer::handle_read_event(int fd) {
    if (fd != _server_fd)
        read_client_message(fd);
    else
        connect_client(fd);
}

void Multiplexer::handle_error_event(int fd) {
    if (fd != _server_fd)
        disconnect_client(fd);
}

void Multiplexer::handle_hangup_event(int fd) {

    if (fd != _server_fd)
        disconnect_client(fd);
}

void Multiplexer::handle_write_event(int fd) {
    // send_message_to_client(fd);
    (void)fd;
    return ;
}


// Funções auxiliares

int Multiplexer::_accept_connection(int server_fd, sockaddr_in* addr, socklen_t* size) {
    int client_fd = ::accept(server_fd, (sockaddr*)addr, size);
    if (client_fd == -1)
        throw std::runtime_error("Error to accept connection");
    return client_fd;
    /*
        int ::accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
        - sockfd é o socket do servidor.
        - addr é um ponteiro para uma estrutura sockaddr onde as informações 
            sobre o endereço do cliente serão armazenadas (como o IP e a 
            porta de onde a conexão foi feita).
        - addrlen é um ponteiro para a variável que contém o tamanho da 
            estrutura sockaddr. Quando accept() for bem-sucedido, ele 
            preencherá essa estrutura com o endereço do cliente.
        - retorna o fd do client.
    */   
}

std::string Multiplexer::_get_client_hostname(const struct in_addr& client_ip) {
    struct hostent* host = gethostbyname(inet_ntoa(client_ip));
    
    if (host && host->h_name) {
        return std::string(host->h_name);
    }
    return std::string(inet_ntoa(client_ip));
}


Client* Multiplexer::_create_client(int client_fd, const sockaddr_in& addr) {
    std::string client_host = _get_client_hostname(addr.sin_addr);
    int client_port = ntohs(addr.sin_port);

//    std::cout << "Creating client:" << std::endl;
//    std::cout << "Client FD: " << client_fd << std::endl;
//    std::cout << "Client IP: " << client_host << std::endl;
//    std::cout << "Client Port: " << client_port << std::endl;

    return new Client(client_fd, client_host, client_port);
}