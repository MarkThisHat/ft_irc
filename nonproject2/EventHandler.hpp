#pragma once

#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <string>

class EventHandler {
public:
    // Reads data from a file descriptor and returns it as a string
    static std::string readData(int fd);

    // Handles an event on a file descriptor (e.g., read, error, hang-up) deepshit generated
    //static void handleEvent(int fd, int events);
};

#endif // EVENTHANDLER_HPP