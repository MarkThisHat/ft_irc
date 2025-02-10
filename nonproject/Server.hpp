#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Socket.hpp"
# include "ServerException.hpp"
# include "Multiplexer.hpp"

# define MAX_EVENTS 20


class Multiplexer;

class Server : private Socket
{
    private:
        bool                    _running;
        const std::string       _password;

    public:
        Server(const std::string& host, int port, const std::string& pass);
        ~Server();
        
        bool    is_running() const;
        int     get_server_fd() const;
        const std::string get_password() const;
        

        void    turn_on(Multiplexer multiplexer);
        void    turn_off(Multiplexer multiplexer);
};

#endif // SERVER_HPP
