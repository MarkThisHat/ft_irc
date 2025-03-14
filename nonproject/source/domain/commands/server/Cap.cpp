#include "Cap.hpp"
#include "ClientService.hpp"
#include "Client.hpp"

Cap::Cap(Server* server) : ACommand(server, false) {}
Cap::~Cap() {}

void Cap::execute(Client* client, std::vector<std::string> args) {
  if (args[0] == "LS" && args[1] == "302")
      ClientService::send_message(client, "PING guest");
}

bool Cap::_has_valid_parameters(Client* client, const std::vector<std::string>& args) {
  (void)client;
  (void)args;
  return true;
}
