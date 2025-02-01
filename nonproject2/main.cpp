#include <iostream>
#include "Server.hpp"
#include "ServerService.hpp"
#include "Socket.hpp"
#include "Poller.hpp"
// #include "Multiplexer.hpp"



// COM MULTIPLEXER
int main() {
    // Criar servidor e socket
    Server server;
    Socket socket;
    ServerService serverService(server, socket);

    // Configurar o servidor e começar a escutar
    serverService.start("127.0.0.1", 6667);
    
    // Criar e configurar o Multiplexer
    Poller poller;
    // poller.start();

    // Adicionar o socket do servidor ao poller
    poller.addSocket(socket.getSocket(), EPOLLIN);  // POLLIN significa que estamos interessados em leitura
    
    // Simular o loop do servidor
    while (true) {
        // Verifica e lida com os eventos
        poller.pollSockets();
    }

    // Parar o servidor
    serverService.stop();
    // poller.stop();
    
    return 0;
}



// // SEM MULTIPLEXER
// int main() {
//     // Cria uma instância de servidor e socket
//     Server server;
//     Socket socket;

//     // Cria uma instância do gerenciador de servidor, passando o servidor e o socket
//     ServerService serverManager(server, socket);

//     // Inicia o servidor na porta 6667 e no host "127.0.0.1"
//     serverManager.start("127.0.0.1", 6667);

//     // Aqui você pode adicionar lógica para interagir com o servidor, por exemplo, aceitar conexões, etc.
//     std::cout << "Servidor iniciado. Pressione ENTER para parar o servidor..." << std::endl;
//     std::cin.get();  // Espera o usuário pressionar ENTER

//     // Para o servidor
//     serverManager.stop();

//     return 0;
// }

/* TESTANDO CONNECTIVIDADE
nc -zv 127.0.0.1 6667
Se o servidor estiver ativo e aceitando conexões, aparece "Ncat: Connected to 127.0.0.1:6667".
Caso contrário, verá uma mensagem de erro, "Ncat: Connection refused."

ou

netstat -tuln | grep 6667
Se o servidor estiver rodando, o comando retornará uma linha mostrando que ele está escutando na porta 6667.
Se não houver saída, o servidor não está rodando ou não está escutando na porta especificada.
*/