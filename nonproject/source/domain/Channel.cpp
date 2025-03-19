#include "Channel.hpp"
#include "Client.hpp"
#include <ctime>
#include <sstream>

Channel::Channel(Client* admin, const std::string& name) :
        _name(name),
        _admin(admin),
        _operators(std::make_pair(false, std::set<Client*>())),
        _inviteds(std::make_pair(false, std::set<Client*>())),
        _topic(std::make_pair(false, "")),
        _key(std::make_pair(false, "")),
        _limit(std::make_pair(false, 0)) {

        std::time_t now = std::time(NULL);  // Get current timestamp
        std::ostringstream oss;
        oss << now;  // Convert int to string
        _created = (std::make_pair(admin->get_nickname(), oss.str()));
    }

Channel::~Channel() {}

// Getters
std::string                 Channel::get_name() const { return _name; }
Client*                     Channel::get_admin() const { return _admin; }
std::set<Client*>&          Channel::get_clients() { return _clients; }
std::map<Client*, int>&     Channel::get_black_list() { return _black_list; }


std::pair<bool, std::set<Client*> >&     Channel::get_operators() { return _operators; }
std::pair<bool, std::set<Client*> >&     Channel::get_inviteds() { return _inviteds; }
std::pair<bool, std::string>&            Channel::get_topic() { return _topic; }
std::pair<bool, std::string>&            Channel::get_key() { return _key; }
std::pair<bool, int>&                    Channel::get_limit() { return _limit; }


// Setters

void Channel::set_admin(Client* admin) { _admin = admin; }
void Channel::set_name(const std::string& name) { _name = name; }

void Channel::set_operators(bool state) { _operators.first = state; }
void Channel::set_inviteds(bool state) { _inviteds.first = state; }
void Channel::set_topic(bool state, const std::string& topic) { _topic.first = state; _topic.second = topic; }
void Channel::set_key(bool state, const std::string& key) { _key.first = state; _key.second = key; }
void Channel::set_limit(bool state, int limit) { _limit.first = state; _limit.second = limit; }


// Métodos

Client* Channel::get_operator(Client* target) {
    if (_operators.first && _operators.second.find(target) != _operators.second.end())
        return target;
    return NULL;
}

Client* Channel::get_invited(Client* target) {
    if (_inviteds.first && _inviteds.second.find(target) != _inviteds.second.end())
        return target;
    return NULL;
}

std::string Channel::get_creator() {
    return _created.first;
}

std::string Channel::get_creation() {
    return _created.second;
}

void Channel::add_to_black_list(Client* client) {
    _black_list.find(client) != _black_list.end() 
        ? _black_list[client] += 1 
        : _black_list[client] = 1;
}

std::string Channel::get_active_modes() {
    std::string modes = "+";

    if (_topic.first)
        modes += "t";
    if (_inviteds.first)
        modes += "i";
    if (_limit.first)
        modes += "l";
    if (_key.first)
        modes += "k";
    if (!_operators.second.empty())
        modes += "o";

    return modes;
}

std::string Channel::get_mode_params() {
    std::string params;
    std::stringstream ss;

    if (_limit.first) {
        ss << _limit.second;
        params += ss.str() + " ";
    }

    if (_key.first)
        params += _key.second + " ";


    return params;
}


void Channel::add_to_clients(Client* client) { _clients.insert(client); }
void Channel::add_to_inviteds(Client* client) { _inviteds.second.insert(client); }
void Channel::add_to_operators(Client* client) { _operators.second.insert(client); }

void Channel::remove_from_clients(Client* client) { _clients.erase(client); }
void Channel::remove_from_inviteds(Client* client) { _inviteds.second.erase(client); }
void Channel::remove_from_operators(Client* client) { _operators.second.erase(client); }
