#include "commands/operator/Kick.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "ClientService.hpp"
#include "ChannelService.hpp"
#include "Macros.hpp"
#include "Server.hpp"


Kick::Kick(Server* server) : ACommand(server, true) {}
Kick::~Kick() {}


// syntax: KICK #<channel> <client> :<message>
void Kick::execute(Client* client, std::vector<std::string> args) {
    if (!_has_valid_parameters(client, args))
        return;

    std::string target = args[1];
    std::string reason = _extract_reason(args);
    std::string channel_name = args[0];
    if (channel_name.at(0) != '#') {
        ClientService::send_message(client, ERR_NOSUCHCHANNEL(client->get_nickname(), channel_name));
        return;
    }
    channel_name.erase(0,1);

    Channel* channel = client->get_channel();
    if (!_is_valid_channel(client, channel, channel_name))
        return;

    if (!_has_channel_privileges(client, channel))
        return;

    Client* dest = _server->get_client(target);
    if (!_is_valid_client(client, dest, channel, channel_name))
        return;

    std::cout << "KICK::Kicking client..." << std::endl;
    ChannelService::kick_client(channel, client, dest, reason);
    ClientService::leave_channel(client, channel);
    std::cout << "SUCCEDED KICK" << std::endl;
}


// Funções auxiliares

bool Kick::_has_valid_parameters(Client* client, const std::vector<std::string>& args) {
    std::cout << "KICK::Validate parameters..." << std::endl;
    if (args.size() > 1)
        return true;
    ClientService::send_message(client, ERR_NEEDMOREPARAMS(client->get_nickname(), "KICK"));
    return false;
}

std::string Kick::_extract_reason(const std::vector<std::string>& args) {
    std::cout << "KICK::Extracting reason..." << std::endl;
    if (!(args.size() >= 3 && (args[2][0] != ':' || args[2].size() > 1)))
        return "No reason specified!";

    std::string reason = "";
    for (size_t i = 2; i < args.size(); ++i) {
        reason.append(args[i]);
        reason.append(" ");
    }
    return reason;
}
