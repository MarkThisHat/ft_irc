#include "Ping.hpp"
#include "ClientService.hpp"
#include "Client.hpp"

Ping::Ping(Server* server) : ACommand(server, false) {}
Ping::~Ping() {}

void Ping::execute(Client* client, std::vector<std::string> args) {
  if (args.empty())
    ClientService::send_message(client, "PONG");
  ClientService::send_message(client, "PONG " + args[0]);
}

bool Ping::_has_valid_parameters(Client* client, const std::vector<std::string>& args) {
  (void)client;
  (void)args;
  return true;
}
