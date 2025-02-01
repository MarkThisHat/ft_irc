#include "EventHandler.hpp"
#include <vector>
#include <unistd.h>
#include <stdexcept>
#include <iostream>

std::string EventHandler::readData(int fd) {
    const int BUFFER_SIZE = 1024;
    std::vector<char> buffer(BUFFER_SIZE);
    ssize_t bytes_read = read(fd, &buffer[0], BUFFER_SIZE - 1);

    if (bytes_read < 0)
        throw std::runtime_error("Error reading from socket");

    if (bytes_read == 0) 
        return "";

    buffer[bytes_read] = '\0';
    return std::string(&buffer[0]);
}
