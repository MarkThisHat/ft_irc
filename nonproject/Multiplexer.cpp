#include "Multiplexer.hpp"
#include "Server.hpp"


Multiplexer::Multiplexer() {
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
}


// ADICIONA UM NOVO FD PARA SER MONITORADO
void Multiplexer::subscribe_fd_for_monitoring(int fd) {
    epoll_event event;

    event.events = EPOLLIN;
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
void Multiplexer::wait_for_events(Server& server) {
    while (server.is_running()) {

        int total_events = epoll_wait(_epoll_fd, _events, MAX_EVENTS, 0); //implementar o sinal de sair do programa
        if (total_events == -1)
            throw std::runtime_error("Error while polling with epoll");

        handle_events(server.get_server_fd(), total_events);

        std::cout << "DIACHO DE LOOPING QUE NÃO DESLIGA." << std::endl;
        sleep(3);
    }

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
void Multiplexer::handle_events(int server_fd, int total_events) {
    for (int i = 0; i < total_events; i++) {

        // Se o evento for de desconexão ou erro do client
        if ((_events[i].events & EPOLLERR) || (_events[i].events & EPOLLHUP))
            disconnect_client(_events[i].data.fd);


        if (_events[i].events & EPOLLIN) {
            // Nova conexão (novo client tentando conectar)
            if (_events[i].data.fd == server_fd) {
                connect_client(server_fd);
            }
            // Mensagem a ser lida
            else
                read_client_message(_events[i].data.fd);
        }
    }
}

// DISCONECTA O CLIENT DO SERVER
void Multiplexer::disconnect_client(int client_fd) {
    unsubscribe_fd_for_monitoring(client_fd);
    ::close(client_fd);
    std::cout << "Client disconnected." << std::endl;
}

// CONECTA UM CLIENT AO SERVER
void Multiplexer::connect_client(int server_fd) {
    sockaddr_in addr = {};
    socklen_t   size = sizeof(addr);

    int client_fd = ::accept(server_fd, (sockaddr*)&addr, &size);
    if (client_fd == -1)
        throw std::runtime_error("Error to accept connection");

    subscribe_fd_for_monitoring(client_fd);
    std::cout << "Client connected." << std::endl;

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

// MINI GNL
void Multiplexer::read_client_message(int client_fd) {
    std::string message;
    
    char buffer[100];
    bzero(buffer, 100);

    while (!strstr(buffer, "\n"))
    {
        bzero(buffer, 100);

        if ((recv(client_fd, buffer, 100, 0) < 0) and (errno != EWOULDBLOCK))
            throw std::runtime_error("Error while reading buffer from a client!");

        message.append(buffer);
    }

    if (!message.empty() && message[message.size() - 1] == '\n')
        message.erase(message.size() - 1);

    std::cout << message << std::endl;
}
