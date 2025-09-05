#define BOOST_TEST_MODULE test_ip

#include "lib.h"
#include "version.h"
#include <iostream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_ip)

BOOST_AUTO_TEST_CASE(test_pasre_ip) {

    std::string sIp = "192.168.1.1";
    IP ip = parse_ip(sIp);
    std::array arrIP = {192,168,1,1};
    BOOST_CHECK(ip.octets == arrIP);
}

BOOST_AUTO_TEST_SUITE_END()