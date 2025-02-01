#include "ServerService.hpp"


ServerService::ServerService(Server& serv, Socket& sock)
    : server(serv), socket(sock) {}

ServerService::~ServerService() {
    if (server.isRunning()) {
        stop();
    }
}


void ServerService::configureSocket(const std::string& host, int port) {
    socket.create();
    socket.configure(host, port);
}


void ServerService::listenForConnections() {
    try {
        socket.listen(10);  // 10 é o backlog de conexões
    }
    catch (const std::exception& e) {
        throw ServerException(e, "Erro ao escutar por conexões");
    }
}

void ServerService::acceptConnection() {
    try {
        struct sockaddr_in clientAddress;
        socklen_t clientAddrLen = sizeof(clientAddress);
        int clientSocket = socket.accept(clientAddress, clientAddrLen);
        std::cout << "Cliente conectado: " << inet_ntoa(clientAddress.sin_addr) << std::endl;
        close(clientSocket);  // Fecha a conexão após o processamento
    }
    catch (const std::exception& e) {
        throw ServerException(e, "Erro ao aceitar conexão");
    }
}


void ServerService::start(const std::string& host, int port) {
    if (server.isRunning()) {
        std::cout << "Servidor já está rodando!" << std::endl;
        return;
    }

    try {
        configureSocket(host, port);
        socket.setNonBlocking();
        socket.listen(10);  // 10 é o backlog de conexões
        server.setRunning(true);
        std::cout << "Servidor iniciado e aguardando conexões..." << std::endl;
    }
    catch (const std::exception& e) {
        throw ServerException(e, "Erro ao iniciar o servidor");
    }
}

void ServerService::stop() {
    if (!server.isRunning()) {
        std::cout << "Servidor já está parado!" << std::endl;
        return;
    }

    try {
        socket.close();
        server.setRunning(false);
        std::cout << "Servidor parado." << std::endl;
    }
    catch (const std::exception& e) {
        throw ServerException(e, "Erro ao parar o servidor");
    }
}