#include "SignalManager.hpp"


bool* SignalManager::_running = NULL;

void SignalManager::initialize(bool* running) {
    if (!running)
      throw std::invalid_argument("Signal Manager invalid usage - argument cannot be NULL");
    
    _running = running;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = _signal_handler;
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);   // Ctrl+C
    sigaction(SIGTERM, &sa, NULL);  // Termination request
    sigaction(SIGQUIT, &sa, NULL);  // Ctrl+\ (Quit)
    sigaction(SIGHUP, &sa, NULL);   // Terminal disconnect
    sigaction(SIGTSTP, &sa, NULL);   // Terminal disconnect
}

void SignalManager::_signal_handler(int signum) {
    (void)signum;
    if (_running)
        *_running = false;
}
