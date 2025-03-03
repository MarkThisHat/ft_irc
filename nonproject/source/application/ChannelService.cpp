#include "ChannelService.hpp"


void ChannelService::broadcast(Channel* channel, const std::string& message, Client* exclude) {
    std::set<Client*>& clients = channel->get_clients();

    for (std::set<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (*it != exclude)
            ClientService::send_message(*it, message);
    }
}

int ChannelService::add_client(Channel* channel, Client* client) {
    if (_can_add_client(channel, client)) {
        channel->add_to_clients(client);
        _announce_client_join(channel, client);
        return 0;
    }

    return -1;
}

void ChannelService::remove_client(Channel* channel, Client* client) {
    std::set<Client*>& clients = channel->get_clients();
    std::set<Client*>::iterator it = clients.find(client);
    if (it != clients.end())
        clients.erase(it);

    _change_admin_if_needed(channel, client);
    _announce_client_leave(channel, client);
}

void ChannelService::kick_client(Channel* channel, Client* client, Client* target, const std::string& reason) {
    if (channel->get_admin() != client)
        return;

    channel->add_to_black_list(target);
    remove_client(channel, target);
    target->set_channel(NULL);

    _announce_client_kick(channel, client, target, reason);
}


// Funções auxiliares

bool ChannelService::_can_add_client(Channel* channel, Client* client) {
    std::map<Client*, int>& black_list = channel->get_black_list();
    std::map<Client*, int>::iterator it = black_list.find(client);

    return (it == black_list.end() || it->second < MAX_BLACKLIST_VIOLATIONS);
    /* O Client só será adicionado ao canal se:
        - Não estiver na black list, ou;
        - Tiver valor menor que três na black list.
    */
}

void ChannelService::_change_admin_if_needed(Channel* channel, Client* client) {
    if (client != channel->get_admin())
        return;

    std::set<Client*>& clients = channel->get_clients();

    if (!clients.empty()) {
        std::set<Client*>::iterator it = clients.begin();
        channel->set_admin(*it);
        _announce_admin_change(channel, *it);
    }
}

void ChannelService::_announce_client_join(Channel* channel, Client* client) {
    std::string nicknameClient = client->get_nickname();
    std::string channelName = channel->get_name();
    broadcast(channel, nicknameClient + MESSAGE_CLIENT_JOIN(channelName, nicknameClient));
}

void ChannelService::_announce_client_leave(Channel* channel, Client* client) {
    std::string nicknameClient = client->get_nickname();
    std::string channelName = channel->get_name();
    broadcast(channel, nicknameClient + MESSAGE_CLIENT_LEAVE(channelName, nicknameClient));
}

void ChannelService::_announce_admin_change(Channel* channel, Client* client) {
    std::string nicknameClient = client->get_nickname();
    std::string channelName = channel->get_name();
    broadcast(channel, nicknameClient + MESSAGE_ADMIN_CHANGE(channelName, nicknameClient));
}

void ChannelService::_announce_client_kick(Channel* channel, Client* client, Client* target, const std::string& reason) {
    std::string nicknameClient = client->get_nickname();
    std::string nicknameTarget = target->get_nickname();
    std::string channelName = channel->get_name();
    broadcast(channel, nicknameClient + MESSAGE_CLIENT_KICK(channelName, nicknameClient, nicknameTarget, reason));
}
