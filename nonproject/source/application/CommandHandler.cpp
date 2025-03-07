#include "CommandHandler.hpp"
#include "ACommand.hpp"
#include "Macros.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "ClientService.hpp"
#include "Server.hpp"

CommandHandler::CommandHandler(Server* server) : _server(server) {
    // Server
    // _commands["PASS"] = new Pass(*_server);
    // _commands["NICK"] = new Nick(*_server);
    // _commands["USER"] = new User(*_server);
    // _commands["QUIT"] = new Quit(*_server);

    // // General
    // _commands["JOIN"] = new Join(*_server);
    // _commands["PART"] = new Part(*_server);
	// _commands["PRIVMSG"] = new PrivMsg(*_server);

    // // Operator
    // _commands["MODE"] = new Mode(*_server);
    // _commands["KICK"] = new Kick(*_server);
	// _commands["TOPIC"] = new Topic(*_server);
    // _commands["INVITE"] = new Invite(*_server);
}

CommandHandler::~CommandHandler () {
    for (std::map<std::string, ACommand *>::iterator it = _commands.begin(); it != _commands.end(); ++it)
        delete it->second;
}


// Métodos

std::string     CommandHandler::_trim(const std::string& str) {
    std::string WHITESPACE = " \n\r\t\f\v";
    std::string result = "";

    size_t  start = str.find_first_not_of(WHITESPACE);
    if (start != std::string::npos)
        result = str.substr(start);

    size_t  end = result.find_last_not_of(WHITESPACE);
    if (end != std::string::npos)
        result = result.substr(0, end + 1);

    return result;
}

void CommandHandler::invoke(Client* client, const std::string& message) {
    std::stringstream   ss(message);
    std::string         syntax;
    
    while (std::getline(ss, syntax)) {
        syntax = _trim(syntax);

        std::string name = syntax.substr(0, syntax.find(' '));

        try {
            std::vector<std::string>    args;
            std::stringstream           line(syntax.substr(name.length(), syntax.length()));
            std::string                 buf;

            ACommand *cmd = _commands.at(name);

            while (line >> buf)
                args.push_back(buf);

            if (client->get_state() == REGISTERED && cmd->auth_required()) {
                ClientService::reply_message(client, ERR_NOTREGISTERED(client->get_nickname()));
                return;
            }

            cmd->execute(client, args);
        } catch (const std::exception& e) {
            ClientService::reply_message(client, ERR_UNKNOWNCOMMAND(client->get_nickname(), name));
        }
    }
}
