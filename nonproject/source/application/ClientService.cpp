#include "ClientService.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "ChannelService.hpp"
#include "Multiplexer.hpp"
#include "Macros.hpp"


void ClientService::send_message(Client* client, const std::string& message) {
    Multiplexer::send_client_message(client->get_fd(), message);
}

void ClientService::reply_message(Client* client, const std::string& reply) {
    send_message(client, ":" + client->get_prefix() + " " + reply);
}

void ClientService::join_channel(Client* client, Channel* channel) {
    std::cout << "ClientService::Joining to a channel..." << std::endl;
    _update_client_channel(client, channel);
    _broadcast_join_message(client, channel);
    std::cout << "ClientService::Client joined." << std::endl;

    // std::string users = _gather_channel_users(channel);
    // _send_end_of_names_reply(client, channel);
    // _send_nickname_reply(client, channel, users);
}

void ClientService::leave_channel(Client* client, Channel* channel) {
    std::cout << "ClientService::Leaving for a channel..." << std::endl;
    _update_client_channel(client, NULL);
    _broadcast_part_message(client, channel);
    std::cout << "ClientService::Client left." << std::endl;
}


// Funções auxiliares

void ClientService::_update_client_channel(Client* client, Channel* channel) {
    client->set_channel(channel);
}

void ClientService::_broadcast_join_message(Client* client, Channel* channel) {
    ChannelService::broadcast(channel, RPL_JOIN(client->get_prefix(), channel->get_name(), client->get_nickname()));
}

void ClientService::_broadcast_part_message(Client* client, Channel* channel) {
    ClientService::send_message(client, RPL_PART(client->get_prefix(), channel->get_name()));
    ChannelService::broadcast(channel, RPL_PART(client->get_prefix(), channel->get_name()));
}


// std::string ClientService::_gather_channel_users(Channel* channel) {
//     std::string users = "";
//     std::vector<std::string> nicknames = ChannelService::get_nicknames(channel);
//     for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); ++it) {
//         users.append(*it + " ");
//     }
//     return users;
// }

// void ClientService::_send_nickname_reply(Client* client, Channel* channel, const std::string& users) {
//     send_message(client, RPL_NAMREPLY(client->get_nickname(), channel->get_name(), users));
// }


// void ClientService::_send_end_of_names_reply(Client* client, Channel* channel) {
//     send_message(client, RPL_ENDOFNAMES(client->get_nickname(), channel->get_name()));
// }
