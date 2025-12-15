#include "processCommands.h"
#include <string>

void processCommands(std::istream& in, CommandCollector& collector) {
    std::string command;
    while (in >> command) {
        if (command == "EOF") break;

        if (command == "{") {
            collector.openBrace();
        }
        else if (command == "}") {
            collector.closeBrace();
        }
        else {
            collector.addCommand(command);
            collector.tryFlushBySize();
        }
    }
    collector.finish();
}
