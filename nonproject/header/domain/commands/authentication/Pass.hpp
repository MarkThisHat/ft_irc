#ifndef PASS_HPP
# define PASS_HPP

# include "ACommand.hpp"
# include "Macros.hpp"
# include "ClientService.hpp"


class Pass : public ACommand {
    private:
        bool            _has_valid_parameters(Client* client, const std::vector<std::string>& args);
        bool            _is_already_registered(Client* client);
        bool            _is_password_valid(const std::string& password);

    public:
        Pass(Server& server);
        ~Pass();

        void execute(Client* client, std::vector<std::string> args);
};

#endif // PASS_HPP
