#include "SignalManager.hpp"

bool* SignalManager::_running = NULL;

void SignalManager::initialize(bool* running) {
    if (!running) {
      throw std::invalid_argument("Signal Manager invalid usage - argument cannot be NULL");
    }

    _running = running;

    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGQUIT, signalHandler);
}

void SignalManager::signalHandler(int signum) {
    if (_running) {
        *_running = false;
    }
}
