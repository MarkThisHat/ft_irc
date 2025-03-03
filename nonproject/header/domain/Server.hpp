#pragma once

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <set>
# include <map>

# include "interfaces/IVault.hpp"
# include "interfaces/ISocket.hpp"
# include "interfaces/IMultiplexer.hpp"
# include "SignalManager.hpp"
# include "ServerException.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "Constants.hpp"
// # include "CommandHandler.hpp"


class Server {
    private:
        bool                        _running;
        IVault&                     _vault;
        ISocket&                    _socket;
        IMultiplexer&               _multiplexer;
        std::set<Channel *>         _channels;
        


    public:
        Server(IVault& vault, ISocket& socket, IMultiplexer& multiplexer);
        ~Server();

        // Getters
        const IMultiplexer&             get_multiplexer() const;

        const std::set<Channel*>&       get_channels() const;
        Channel*                        get_channel(const std::string& name);

        const std::map<int, Client*>&   get_clients() const;
        Client*                         get_client(const std::string& name);

        // Métodos
        void start();
        void stop();

        Channel*    create_channel(const std::string &name, const std::string &key, Client *client);
};

#endif // SERVER_HPP
