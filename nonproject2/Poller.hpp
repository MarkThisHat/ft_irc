#ifndef POLLER_HPP
#define POLLER_HPP

#include <vector>
#include <sys/epoll.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <set>

class Poller {
    private:
        int epoll_fd;  // Descritor do epoll
        // int timeout;   // Timeout em milissegundos
        // std::vector<struct epoll_event> monitored_clients;  // Lista de eventos monitorados
        std::set<int> clients; // Track client file descriptors

    public:
        Poller();   // Construtor
        ~Poller();                       // Destruidor

        void    addSocket(int socket_fd, int events);  // Adiciona um socket à lista de monitoramento
        void    removeSocket(int socket_fd);             // Remove um socket da lista
        void    pollSockets();                          // Monitora os sockets
        // bool    isReadable(int socket_fd);               // Verifica se o socket está pronto para leitura
        // bool    isWritable(int socket_fd);               // Verifica se o socket está pronto para escrita
        // bool    isClosed(int socket_fd);                 // Verifica se o socket foi fechado
        // size_t  getSocketCount() const;               // Retorna o número de sockets monitorados
        // struct epoll_event getSocket(size_t index) const;  // Retorna o objeto epoll_event correspondente ao índice
};

#endif  // POLLER_HPP
