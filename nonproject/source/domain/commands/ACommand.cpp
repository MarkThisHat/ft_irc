#include "commands/ACommand.hpp"
#include "Channel.hpp"
#include "ChannelService.hpp"
#include "Client.hpp"
#include "ClientService.hpp"
#include "Macros.hpp"


ACommand::ACommand(Server* server, bool auth) : _auth(auth), _server(server) {}

ACommand::~ACommand() {}


bool    ACommand::auth_required() const { return _auth; }


// Funções auxiliares

bool ACommand::_is_on_channel(Client* client, Channel* channel) {
    std::cout << "ACOMMAND::Validate if client is on channel..." << std::endl;
    if (channel == client->get_channel())
        return true;
    ClientService::send_message(client, ERR_NOTONCHANNEL(client->get_nickname(), channel->get_name()));
    return false;
}

bool ACommand::_is_valid_channel(Client* client, Channel* channel, const std::string& target) {
    std::cout << "ACOMMAND::Validate if a channel exists..." << std::endl;
    if (channel)
        return true;
    ClientService::send_message(client, ERR_NOSUCHCHANNEL(client->get_nickname(), target));
    return false;
}

bool ACommand::_has_channel_privileges(Client* client, Channel* channel) {
    std::cout << "ACOMMAND::Validate if client has privileges..." << std::endl;
    Client* admin = channel->get_admin();
    if (client != admin && (channel->get_operators().first && !channel->get_operator(client))) {
        ClientService::send_message(client, ERR_NOPRIVILEGES(client->get_nickname()));
        return false;
    }
    return true;
}

bool ACommand::_is_already_registered(Client* client) {
    std::cout << "ACOMMAND::Validate if client is already registered..." << std::endl;
    if (client->get_state() != REGISTERED)
        return false;
    ClientService::send_message(client, ERR_ALREADYREGISTERED(client->get_nickname()));
    return true;
}

bool ACommand::_is_valid_client(Client* client, Client* dest, Channel* channel, const std::string& name) {
    std::cout << "ACOMMAND::Validate if client exists and is on channel..." << std::endl;
    if (!dest) {
        ClientService::send_message(client, ERR_NOSUCHNICK(client->get_nickname(), dest->get_nickname()));
        return false;
    }

    if (!dest->get_channel() || dest->get_channel() != channel) {
        ClientService::send_message(client, ERR_USERNOTINCHANNEL(client->get_nickname(), dest->get_nickname(), name));
        return false;
    }
    
    return true;
}
