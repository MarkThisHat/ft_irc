#include "Pong.hpp"
#include "ClientService.hpp"
#include "Client.hpp"

Pong::Pong(Server* server) : ACommand(server, false) {}
Pong::~Pong() {}

void Pong::execute(Client* client, std::vector<std::string> args) {
  if (!args.empty())
    ClientService::send_message(client, args[0]);
}

bool Pong::_has_valid_parameters(Client* client, const std::vector<std::string>& args) {
  (void)client;
  (void)args;
  return true;
}
