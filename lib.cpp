#include <sstream>
#include "lib.h"
#include "version.h"

int version() {
	return PROJECT_VERSION_PATCH;
}

IP parse_ip(const std::string& s){
    IP ip{};
    char dot;
    std::istringstream iss(s);
    iss >> ip.octets[0] >> dot >> ip.octets[1] >> dot >> ip.octets[2] >> dot >> ip.octets[3];
    return ip;
}