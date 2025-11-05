#include "CommandCollector.h"
#include "ConsolePrinter.h"
#include "FileLogger.h"

int main(int argc, char** argv) {
    // проверка аргументов
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " N\n";
        return 1;
    }
    int N = std::stoi(argv[1]);
    if (N <= 0) {
        std::cerr << "N must be positive\n";
        return 1;
    }

    BulkNotifier notifier;
    auto console = std::make_shared<ConsolePrinter>();
    auto filelog = std::make_shared<FileLogger>();

    // подписчики — НЕ зависят друг от друга и от collector
    notifier.addObserver(console);
    notifier.addObserver(filelog);

    CommandCollector collector(N, notifier);

    processCommands(std::cin,collector);
    
    return 0;
}