#ifndef INVITE_HPP
# define INVITE_HPP

# include "ACommand.hpp"


class Invite : public ACommand {
    private:
        

    public:
        Invite(Server& server);
        ~Invite();

        void execute(Client* client, std::vector<std::string> args);
};

#endif // INVITE_HPP
