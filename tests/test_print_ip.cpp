#define BOOST_TEST_MODULE MyAllocatorTest
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <iostream>
#include "print_ip.h"

BOOST_AUTO_TEST_CASE(PrintInt8Test)
{
    std::ostringstream oss;
    std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf()); // перенаправляем cout

    print_ip(int8_t{-1}); // должно вывести 255

    std::cout.rdbuf(old_buf); // возвращаем cout обратно

    BOOST_CHECK_EQUAL(oss.str(), "255\n");
}

BOOST_AUTO_TEST_CASE(PrintInt16Test)
{
    std::ostringstream oss;
    std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf()); 

    print_ip( int16_t{0} );

    std::cout.rdbuf(old_buf);

    BOOST_CHECK_EQUAL(oss.str(), "0.0\n");
}

BOOST_AUTO_TEST_CASE(PrintInt32Test)
{
    std::ostringstream oss;
    std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf()); 

    print_ip( int32_t{2130706433} );

    std::cout.rdbuf(old_buf);

    BOOST_CHECK_EQUAL(oss.str(), "127.0.0.1\n");
}

BOOST_AUTO_TEST_CASE(PrintInt64Test)
{
    std::ostringstream oss;
    std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf()); 

    print_ip( int64_t{8875824491850138409} );

    std::cout.rdbuf(old_buf);

    BOOST_CHECK_EQUAL(oss.str(), "123.45.67.89.101.112.131.41\n");
}

BOOST_AUTO_TEST_CASE(PrintStringTest)
{
    std::ostringstream oss;
    std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf()); 

    print_ip( std::string{"Hello, World!"} );

    std::cout.rdbuf(old_buf);

    BOOST_CHECK_EQUAL(oss.str(), "Hello, World!\n");
}

BOOST_AUTO_TEST_CASE(PrintVectorTest)
{
    std::ostringstream oss;
    std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf()); 

    print_ip( std::vector<int>{100, 200, 300, 400} );

    std::cout.rdbuf(old_buf);

    BOOST_CHECK_EQUAL(oss.str(), "100.200.300.400\n");
}

BOOST_AUTO_TEST_CASE(PrintListTest)
{
    std::ostringstream oss;
    std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf()); 

    print_ip( std::list<short>{400, 300, 200, 100} );

    std::cout.rdbuf(old_buf);

    BOOST_CHECK_EQUAL(oss.str(), "400.300.200.100\n");
}

BOOST_AUTO_TEST_CASE(PrintTupleTest)
{
    std::ostringstream oss;
    std::streambuf* old_buf = std::cout.rdbuf(oss.rdbuf()); 

    print_ip( std::make_tuple(123, 456, 789, 0) );

    std::cout.rdbuf(old_buf);

    BOOST_CHECK_EQUAL(oss.str(), "123.456.789.0\n");
}