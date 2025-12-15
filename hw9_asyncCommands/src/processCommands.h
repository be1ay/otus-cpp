#pragma once
#include <istream>
#include "CommandCollector.h"

void processCommands(std::istream& in, CommandCollector& collector);
