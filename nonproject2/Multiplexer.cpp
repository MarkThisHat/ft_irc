#include "Multiplexer.hpp"

Multiplexer::Multiplexer() {
    running = false;

    // Associa eventos a funções de tratamento no construtor
    eventHandlers[EPOLLIN] = &Multiplexer::handleReadable;
    eventHandlers[EPOLLOUT] = &Multiplexer::handleWritable;
    eventHandlers[EPOLLHUP] = &Multiplexer::handleClosed;
}

Multiplexer::~Multiplexer() {}

void Multiplexer::start() {
    running = true;
    std::cout << "Multiplexer iniciado, aguardando eventos..." << std::endl;
}

void Multiplexer::stop() {
    running = false;
    std::cout << "Multiplexer parado." << std::endl;
}

void Multiplexer::addSocket(int socketFD, short events) {
    poller.addSocket(socketFD, events);
}

void Multiplexer::removeSocket(int socketFD) {
    poller.removeSocket(socketFD);
}

bool Multiplexer::isRunning() const {
    if (!running)//marcos: eu tiraria o throw
        std::cerr << "Multiplexer não está em execução!" << std::endl;
    return running;
}

void Multiplexer::processPollResult(int ret) {
    if (ret < 0) {
        throw std::runtime_error("Erro ao executar epoll_wait no processPollResult");
    } else if (ret == 0) {
        std::cout << "Nenhum evento ocorrido no tempo de espera" << std::endl;
    }
}

void Multiplexer::processSocketEvents(int socketFD, uint32_t events) {
    for (std::map<short, EventHandler>::iterator it = eventHandlers.begin(); it != eventHandlers.end(); ++it) {
        if (events & it->first) {
            (this->*(it->second))(socketFD);
        }
    }
}

void Multiplexer::handleEvents() {
    if (!isRunning())
        return;

    int ret = poller.pollSockets();
    processPollResult(ret);

    for (size_t i = 0; i < poller.getSocketCount(); ++i) {
        int socketFD = poller.getSocket(i).data.fd;
        uint32_t events = poller.getSocket(i).events;
        processSocketEvents(socketFD, events);
    }
}

void Multiplexer::handleReadable(int socketFD) {
    std::cout << "Dados disponíveis para leitura no socket " << socketFD << std::endl;
    // Adicione aqui o código para ler dados
}

void Multiplexer::handleWritable(int socketFD) {
    std::cout << "Socket " << socketFD << " pronto para escrita." << std::endl;
    // Adicione aqui o código para escrever dados
}

void Multiplexer::handleClosed(int socketFD) {
    std::cout << "Socket " << socketFD << " foi fechado." << std::endl;
    // Adicione aqui o código para fechar a conexão
}
