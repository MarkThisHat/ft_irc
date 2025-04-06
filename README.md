# ft_irc - École 42 Project

This project is an IRC server implementation in C++, inspired by **RFC 1459** and modern IRC servers. The goal is to create a lightweight, event-driven server capable of handling multiple clients, supporting IRC protocol features, and managing real-time communication across channels.

## Features

- Compliant with the **IRC protocol (RFC 1459)**.
- Handles multiple clients concurrently using **multiplexing** (epoll).
- Channel creation and management.
- Client nickname and user state management.
- Event-driven architecture for scalability.
- Supports basic IRC commands such as:
    - [x] `PASS <password>`: Entra com a senha do Server
    - [x] `NICK <nickname>`: Define seu nickname de usuário
    - [x] `USER <username> <hostname> <servername> :<realname>`: Define seu nome de usuário e seu nome real
    - [x] `JOIN <channel> <key>`: Entra em um canal
    - [x] `PART <channels> :<message>`: Sai de um canal
    - [x] `PRIVMSG <msgtarget> :<message>`: Envia uma mensagem particular a um usuário
    - [x] `QUIT :<message>`: Desconecta-se do servidor
    - [x] `TOPIC <channel>` ou `TOPIC <channel> :<new_topic>`: Lê ou redefine o tópico de um canal
    - [x] `INVITE <nickname> <channel>`: Envia um invite para um canal a um usuário
    - [x] `KICK <channel> <client> :<message>`: Expulsa um usuário de um canal
    - [x] `MODE <nickname> <flags>` ou `MODE <channel> <flags> <args>`: Altera modos de um usuário ou canal

        | Modos | Descrição |
        |:---:|:--- |
        | i | Definir/remover o canal somente para convidados (Invite-only). |
        | t | Definir/remover as restrições do comando TOPIC para operadores de canal. |
        | k | Definir/remover a chave (senha) do canal. |
        | o | Dar/tirar privilégios de operador de canal. |
        | l | Definir/remover o limite de usuários do canal. |


### Command to test split up stuff `(echo -n "NI"; sleep 1; echo -n "CK "; sleep 1; echo " rulyo") | nc 127.0.0.1 6667`



## Project Structure

- **source/\*\***: Directory for cpp files.
- **header/\*\***: Directory for hpp files.
