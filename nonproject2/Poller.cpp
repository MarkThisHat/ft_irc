#include "Poller.hpp"
#include "EventHandler.hpp"

void Poller::addSocket(int socket_fd, int events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = socket_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &ev) == -1)
        throw std::runtime_error("Failed to add socket to epoll");

    clients.insert(socket_fd);
}

void Poller::removeSocket(int socket_fd) {
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket_fd, NULL) == -1)
        throw std::runtime_error("Failed to remove socket from epoll");

    clients.erase(socket_fd); // Stop tracking the file descriptor
    close(socket_fd); // Close the file descriptor
}

void Poller::pollSockets() {
    const int MAX_EVENTS = 10;
    struct epoll_event events[MAX_EVENTS];

    int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (num_events == -1)
        throw std::runtime_error("epoll_wait failed");

    for (int i = 0; i < num_events; ++i) {
        int fd = events[i].data.fd;
        if (events[i].events & EPOLLIN) {
            // Handle read event
            std::cout << "Data available on fd: " << fd << std::endl;
            try {
                std::string data = EventHandler::readData(fd);
                if (data.empty())
                    removeSocket(fd);
                std::cout << "***read:\n" << data << "\nend of data***" << std::endl;
            } catch (std::exception &e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
        if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP) {
            // Handle error or hang-up
            std::cout << "Error or hang-up on fd: " << fd << std::endl;
            removeSocket(fd); // Clean up the client
        }
    }
}

Poller::Poller() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        throw std::runtime_error("Failed to create epoll instance");
    }
}

Poller::~Poller() {
    for (std::set<int>::iterator it = clients.begin(); it != clients.end(); ++it) {
        close(*it);
    }
    close(epoll_fd);
}
