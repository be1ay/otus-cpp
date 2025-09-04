#define BOOST_TEST_MODULE test_exact_version
#include <boost/test/unit_test.hpp>
#include "lib.h"
#include "version.h"

BOOST_AUTO_TEST_SUITE(test_exact_version)

BOOST_AUTO_TEST_CASE(test_patch_matches_cmake) {
    BOOST_CHECK_EQUAL(version(), PROJECT_VERSION_PATCH);
}

BOOST_AUTO_TEST_SUITE_END()

