#include "Client.hpp"

# include <iostream>

Client::Client(int fd, const std::string& host, int port) {
    _fd = fd;
    _state = CONNECTED;
    _channel = NULL;

    memset(&_address, 0, sizeof(_address));   // Limpa a estrutura sockaddr_in
    _address.sin_family = AF_INET;            // Define o tipo de família de endereço (default: IPv4)
    _address.sin_port = htons(port);          // Inicializa a porta

    _address.sin_addr.s_addr = inet_addr(host.c_str()); // Define o IP do client
    if (_address.sin_addr.s_addr == INADDR_NONE)
        throw std::runtime_error("Error while converting or invalid address");
}

Client::~Client() {}


int Client::get_fd() const { return _fd; }
std::string Client::get_port() const { return "_address"; } //implementar

std::string Client::get_nickname() const { return _nickname; }
std::string Client::get_username() const { return _username; }
std::string Client::get_hostname() const { return _hostname; }
std::string Client::get_realname() const { return _realname; }
ClientState Client::get_state() const { return _state; }


std::string Client::get_prefix() const {
    std::string username = _username.empty() ? "" : "!" + _username;
    std::string hostname = _hostname.empty() ? "" : "@" + _hostname;

    return _nickname + username + hostname;
}

Channel* Client::get_channel() const { return _channel; }

void Client::set_nickname(const std::string& nickname) { _nickname = nickname; }
void Client::set_username(const std::string& username) { _username = username; }
void Client::set_realname(const std::string& realname) { _realname = realname; }
void Client::set_state(ClientState state) { _state = state; }
void Client::set_channel(Channel* channel) { _channel = channel; }
