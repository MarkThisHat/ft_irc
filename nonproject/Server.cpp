#include "Server.hpp"
#include "SignalManager.hpp"

Server::Server(const std::string& host, int port, const std::string& pass) : 
    Socket(host, port),
    _running(false), 
    _password(pass),
    _multiplexer(this->Socket::_socket_fd) {}

Server::~Server() {}


// INICIA O SERVER
void Server::turn_on() {
    try {
        int total_events;
        _running = true;
        std::cout << "Server is now running." << std::endl;

        _multiplexer.subscribe_fd_for_monitoring(Socket::_socket_fd);

        SignalManager::initialize(&_running);

        while(_running) {
            total_events = _multiplexer.check_for_events();
            _multiplexer.handle_events(total_events);
        }

    } catch (const std::exception& e) {
        throw ServerException(e, "Unable to turn on the server.");
    }
}

// INTERROMPE O SERVER
void Server::turn_off() {
    try {
        if (_running) {
            _running = false;
            _multiplexer.unsubscribe_fd_for_monitoring(Socket::_socket_fd);
        }

        std::cout << "Server has been shut down." << std::endl;
    } catch (const std::exception& e) {
        throw ServerException(e, "Unable to turn off the server.");
    }
}

// Getters
bool    Server::is_running() const { return _running; }
int     Server::get_server_fd() const { return Socket::_socket_fd; }
const   std::string Server::get_password() const { return _password; }
