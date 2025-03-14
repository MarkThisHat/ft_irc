#include "commands/server/Nick.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "ClientService.hpp"
#include "ChannelService.hpp"
#include "Macros.hpp"
#include "Server.hpp"


Nick::Nick(Server* server) : ACommand(server, false) {}
Nick::~Nick() {}


// syntax: NICK <nickname>
void    Nick::execute(Client* client, std::vector<std::string> args) {
    if (!_has_valid_parameters(client, args))
        return;

    std::string nickname = args[0];
    std::cout << "client nickname" << args[0] << std::endl;
    if (_is_nickname_taken(nickname, client))
        return;

    client->set_nickname(nickname);

    _set_client_state(client);
    std::cout << "SUCCEDED NICK" << std::endl;
}


// Funções auxiliares

bool Nick::_has_valid_parameters(Client* client, const std::vector<std::string>& args) {
    std::cout << "NICK::Validate parameters..." << std::endl;
    if (args.size() == 1)
        return true;
    ClientService::send_message(client, ERR_NEEDMOREPARAMS(client->get_nickname(), "NICK"));
    return false;
}

bool Nick::_is_nickname_taken(const std::string& nickname, Client* client) {
    std::cout << "NICK::Validate if nickname is already in use..." << std::endl;
    if (!_server->get_client(nickname))
        return false;
    ClientService::send_message(client, ERR_NICKNAMEINUSE(client->get_nickname()));
    return true;
}

void Nick::_set_client_state(Client* client) {
    std::cout << "NICK::Setting register state..." << std::endl;
    if (client->get_state() != LOGGED_IN || client->get_username().empty() || client->get_realname().empty())
        return;

    client->set_state(REGISTERED);
    std::string server = "ft_irc";
    ClientService::send_message(client, RPL_MYINFO(client->get_nickname()));
    ClientService::send_message(client, RPL_WELCOME(server, client->get_nickname()));
}
