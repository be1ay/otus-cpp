#define BOOST_TEST_MODULE MyAllocatorTest
#include <boost/test/included/unit_test.hpp>
#include "MyAllocator.h"

BOOST_AUTO_TEST_CASE(basic_allocation)
{
    FixedPoolAllocator<int, 4> alloc;
    int* p = alloc.allocate(1);
    *p = 42;
    BOOST_CHECK_EQUAL(*p, 42);
}

BOOST_AUTO_TEST_CASE(multiple_allocations) {
    FixedPoolAllocator<int, 4> alloc;
    int* p1 = alloc.allocate(2);
    int* p2 = alloc.allocate(2);
    BOOST_CHECK_NE(p1, p2); // разные адреса
}

BOOST_AUTO_TEST_CASE(overflow_throws) {
    FixedPoolAllocator<int, 2> alloc;
    alloc.allocate(2); // заполняем пул
    BOOST_CHECK_THROW(alloc.allocate(1), std::bad_alloc);
}

BOOST_AUTO_TEST_CASE(shared_state_between_copies) {
    FixedPoolAllocator<int, 4> alloc1;
    auto alloc2 = alloc1; // копия
    alloc1.allocate(2);
    BOOST_CHECK_THROW(alloc2.allocate(3), std::bad_alloc); // общий пул
}

BOOST_AUTO_TEST_CASE(works_with_vector) {
    std::vector<int, FixedPoolAllocator<int, 4>> v; // проверка с stl контейнером
    v.push_back(1);
    v.push_back(2);
    BOOST_CHECK_EQUAL(v.size(), 2);
}