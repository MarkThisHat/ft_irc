#include "commands/operator/Invite.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "ClientService.hpp"
#include "ChannelService.hpp"
#include "Macros.hpp"
#include "Server.hpp"


Invite::Invite(Server* server) : ACommand(server, true) {}
Invite::~Invite() {}


// syntax: INVITE <nickname> #<channel>
void Invite::execute(Client* client, std::vector<std::string> args) {
    if (!_has_valid_parameters(client, args))
        return;

    std::string target = args[0];
    std::string channel_name = args[1];
    if (channel_name.at(0) != '#') {
        ClientService::send_message(client, ERR_NOSUCHCHANNEL(client->get_nickname(), channel_name));
        return;
    }
    channel_name.erase(0,1);

    Channel* channel = _server->get_channel(channel_name);
    if (!_is_valid_channel(client, channel, channel_name))
        return;

    Client* dest = _server->get_client(target);
    if (!_is_valid_client(client, dest, channel, channel_name))
        return;

    if (!_has_channel_privileges(client, channel))
        return;

    std::cout << "INVITE::Sending invite..." << std::endl;
    channel->add_to_inviteds(dest);
    ClientService::send_message(dest, RPL_INVITE(client->get_prefix(), target, channel_name));
    std::cout << "SUCCEDED INVITE" << std::endl;
}


// Funções auxiliares

bool Invite::_has_valid_parameters(Client* client, const std::vector<std::string>& args) {
    std::cout << "INVITE::Validate parameters..." << std::endl;
    if (args.size() == 2)
        return true;
    ClientService::send_message(client, ERR_NEEDMOREPARAMS(client->get_nickname(), "KICK"));
    return false;
}
