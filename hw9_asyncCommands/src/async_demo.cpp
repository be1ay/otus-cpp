#include <iostream>
#include "Dispatcher.h"
#include "CommandCollector.h"
#include "processCommands.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " N\n";
        return 1;
    }
    int N = std::stoi(argv[1]);
    if (N <= 0) {
        std::cerr << "N must be positive\n";
        return 1;
    }

    Dispatcher dispatcher;

    // start dispatcher with notifier (log thread will call notifier.notify)
    dispatcher.start();

    // create collector
    CommandCollector collector(N, dispatcher);

    std::cout << "Enter commands (use EOF or 'EOF' to finish):\n";
    processCommands(std::cin, collector);

    // stop dispatcher and join threads
    dispatcher.stop();

    return 0;
}
