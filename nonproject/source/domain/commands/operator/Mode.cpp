#include "commands/operator/Mode.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "ClientService.hpp"
#include "ChannelService.hpp"
#include "Macros.hpp"
#include "Server.hpp"


Mode::Mode(Server* server) : ACommand(server, true) {}
Mode::~Mode() {}


// syntax: MODE #<channel> <flag> <args>
void Mode::execute(Client* client, std::vector<std::string> args) {
    if (!_has_valid_parameters(client, args))
        return;

    std::string channel_name = args[0];    
    Channel* channel = _server->get_channel(channel_name);
    if (!_is_valid_channel(client, channel, channel_name))
        return;

    if (args.size() == 1) {
        _send_active_modes(client, channel);
        std::cout << "SUCCEDED MODE NO-ARGS" << std::endl;
        return;
    }

    if (!_has_channel_privileges(client, channel))
        return;

    std::string flag = args[1];
    if (flag.at(0) != '+' && flag.at(0) != '-')
        return;

    _process_modes(client, args, channel);
    std::cout << "SUCCEDED MODE" << std::endl;
}

void Mode::_send_active_modes(Client* client, Channel* channel) {
    std::string active_modes = channel->get_active_modes();
    std::string mode_params = channel->get_mode_params();

    ClientService::send_message(client, RPL_CHANNELMODEIS(client->get_nickname(), channel->get_name(), active_modes, mode_params));
}

// Funções auxiliares

bool Mode::_has_valid_parameters(Client* client, const std::vector<std::string>& args) {
    std::cout << "MODE::Validate parameters..." << std::endl;
    if (args.size() > 0)
        return true;
    ClientService::send_message(client, ERR_NEEDMOREPARAMS(client->get_nickname(), std::string("MODE")));
    return false;
}

void Mode::_process_modes(Client* client, std::vector<std::string> args, Channel* channel) {
    std::cout << "MODE::Parsing modes..." << std::endl;
    std::string target = args[1];
    if (/*(target.at(0) != '+' && target.at(0) != '-') || */target.length() != 2)
        return;
    bool enable_mode = (target.at(0) == '+') ? true : false;
    char flag = target.at(1);

    switch (flag) {
        case 't':
            _set_mode_t(client, channel, enable_mode);
            break;
        case 'i':
            _set_mode_i(client, channel, enable_mode);
            break;
        case 'l':
            _set_mode_l(client, channel, enable_mode, args[2]);
            break;
        case 'k':
            _set_mode_k(client, channel, enable_mode, args[2]);
            break;
        case 'o':
            _set_mode_o(client, channel, enable_mode, args);
            break;
        default:
            // _udefined_mode(client, channel, enable_mode, args, p);
            break;
    }
}


// Modes

void Mode::_set_mode_t(Client* client, Channel* channel, bool enable_mode) {
    channel->set_topic(enable_mode, "", client->get_nickname());
    ChannelService::broadcast(channel, RPL_MODE(client->get_info(), channel->get_name(), (enable_mode ? "+t" : "-t"), ""));
}

void Mode::_set_mode_i(Client* client, Channel* channel, bool enable_mode) {
    channel->set_guests(enable_mode);
    ChannelService::broadcast(channel, RPL_MODE(client->get_info(), channel->get_name(), (enable_mode ? "+i" : "-i"), ""));
}

void Mode::_set_mode_l(Client* client, Channel* channel, bool enable_mode, std::string& arg) {
    int limit = arg.empty() ? 0 : atoi(arg.c_str());
    if (enable_mode && (arg.empty() || limit == 0))
        return ; //defina o limite maior que 0
    channel->set_limit(enable_mode, limit);
    ChannelService::broadcast(channel, RPL_MODE(client->get_info(), channel->get_name(), (enable_mode ? "+l" : "-l"), (enable_mode ? arg : "")));
}

void Mode::_set_mode_k(Client* client, Channel* channel, bool enable_mode, std::string& arg) {
    if (enable_mode && arg.empty())
        return ; //defina uma key
    std::string channel_key = arg.empty() ? "" : arg;
    channel->set_key(enable_mode, channel_key);
    ChannelService::broadcast(channel, RPL_MODE(client->get_info(), channel->get_name(), (enable_mode ? "+k" : "-k"), ""));
}

void Mode::_set_mode_o(Client* client, Channel* channel, bool enable_mode, std::vector<std::string>& args) {
    if (enable_mode && args[2].empty())
        return; //defina um ou mais operators

    channel->set_operators(enable_mode);

    if (!enable_mode)
        return;

    for (size_t i = 2; i < args.size(); ++i) {
        Client* dest = _server->get_client(args[i]);
        if (!dest) {
            ClientService::send_message(client, ERR_NOSUCHNICK(client->get_nickname(), dest->get_nickname()));
            continue;
        }

        std::set<Client*> channel_clients = channel->get_clients();
        if (channel_clients.find(dest) == channel_clients.end()) {
            ClientService::send_message(client, ERR_NOTONCHANNEL(client->get_nickname(), dest->get_nickname()));
            continue;
        }

        channel->add_to_operators(dest);
    }
    ChannelService::broadcast(channel, RPL_MODE(client->get_info(), channel->get_name(), (enable_mode ? "+o" : "-o"), ""));
}
