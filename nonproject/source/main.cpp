#include <iostream>

#include "Vault.hpp"
#include "Socket.hpp"
#include "Multiplexer.hpp"
#include "Server.hpp"


int main(int ac, char** av) {
    // if (ac != 3)
    //     throw std::runtime_error("Usage: ./ircserv <port> <password>");
    (void) ac;
    (void) av;
    int         port        = 6667;         // av[1]
    std::string password    = "123456";     // av[2]


    // Injeções de dependência
    const std::string   IP_ADDRESS      = "0.0.0.0";
    IVault*             vault           = new Vault(password);
    ISocket*            server_socket;
    try {
        server_socket   = new Socket(IP_ADDRESS, port);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    IMultiplexer*       multiplexer     = new Multiplexer(server_socket->get_fd());

    Server      server(vault, server_socket, multiplexer);

    // Iniciando servidor
    try {
        server.start();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}


/* TESTANDO CONNECTIVIDADE
nc -zv 127.0.0.1 6667
Se o servidor estiver ativo e aceitando conexões, aparece "Ncat: Connected to 127.0.0.1:6667".
Caso contrário, verá uma mensagem de erro, "Ncat: Connection refused."

ou

netstat -tuln | grep 6667
Se o servidor estiver rodando, o comando retornará uma linha mostrando que ele está escutando na porta 6667.
Se não houver saída, o servidor não está rodando ou não está escutando na porta especificada.
*/


/* CONECTANDO AO SERVIDOR
nc 127.0.0.1 6667
Para desconectar, necessário o uso do ctrl+C.
*/