#include "commands/misc/Cap.hpp"
#include "ClientService.hpp"
#include "Client.hpp"
#include "Macros.hpp"


Cap::Cap(Server* server) : ACommand(server, false) {}
Cap::~Cap() {}


// syntax: CAP <args>
void Cap::execute(Client* client, std::vector<std::string> args) {
    if (!_has_valid_parameters(client, args))
        return;

    if (args[0] == "LS" /*&& args[1] == "302"*/) {
        ClientService::send_message(client, "CAP * LS");
    }
    ClientService::send_message(client, "CAP * END");
}

bool Cap::_has_valid_parameters(Client* client, const std::vector<std::string>& args) {
    std::cout << "CAP::Validate parameters..." << std::endl;
    if (args.size() < 3)
        return true;
    ClientService::send_message(client, ERR_NEEDMOREPARAMS(std::string("CAP")));
    return false;
}
