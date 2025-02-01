#include "Socket.hpp"

Socket::Socket() : socketFD(-1) {}

Socket::~Socket() {
    if (socketFD != -1) {
        ::close(socketFD); // Esses :: significa que estou chamando o close() global, parte do unistd
    }
}


int Socket::getSocket() const {
    return socketFD;
}

void Socket::setSocket(int socket) {
    socketFD = socket;
}


void Socket::create() {
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == -1) {
        throw std::runtime_error("Erro ao criar o socket");
    }
}

void Socket::bind(const struct sockaddr_in& address) {
    if (::bind(socketFD, (struct sockaddr*)&address, sizeof(address)) == -1) {
        throw std::runtime_error("Erro ao fazer bind do socket");
    }
}

void Socket::listen(int backlog) {
    if (::listen(socketFD, backlog) == -1) {
        throw std::runtime_error("Erro ao ouvir por conexões");
    }
}

int Socket::accept(struct sockaddr_in& clientAddress, socklen_t& clientAddrLen) {
    int clientSocket = ::accept(socketFD, (struct sockaddr*)&clientAddress, &clientAddrLen);
    if (clientSocket == -1) {
        throw std::runtime_error("Erro ao aceitar conexão");
    }
    return clientSocket;
}

void Socket::close() {
    if (socketFD != -1) {
        ::close(socketFD);
    }
}
#include <iostream>
void Socket::setNonBlocking() {
    std::cout << "socket fd " << socketFD <<std:: endl; 
    int flags = fcntl(socketFD, F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error("Erro ao obter flags do socket");
    }

    if (fcntl(socketFD, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("Erro ao definir o socket como não bloqueante");
    }
}

void Socket::configure(const std::string& host, int port) {
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(host.c_str());
    
    if (address.sin_addr.s_addr == INADDR_NONE) {
        throw std::runtime_error("Endereço IP inválido ou erro na conversão");
    }
    
    bind(address);
}
