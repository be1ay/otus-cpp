cmake_minimum_required(VERSION 3.16)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(print_ip VERSION ${PROJECT_VESRION})

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

configure_file(version.h.in version.h)


add_library(homework lib.cpp)

target_include_directories(homework
    PRIVATE "${CMAKE_BINARY_DIR}"
)

set_target_properties(homework PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(WITH_BOOST_TEST)
    enable_testing()
    add_subdirectory(tests)
endif()


if (MSVC)
    target_compile_options(homework PRIVATE
        /W4
    )
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(homework PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()
#----------IP_FILTER
add_executable(ip_filter ip_filter.cpp)
set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_link_libraries(ip_filter PRIVATE
    homework
)
if (MSVC)
    target_compile_options(ip_filter PRIVATE
        /W4
    )
else ()
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

#---------/IP_FILTER
#---------- ALLOCATOR----------------

    add_executable(my_allocator MyAllocator.cpp MyAllocator.h)

    set_target_properties(my_allocator PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    if (MSVC)
        target_compile_options(my_allocator PRIVATE /W4)
    else()
        target_compile_options(my_allocator PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()

    install(TARGETS my_allocator RUNTIME DESTINATION bin)

#----------/ALLOCATOR----------------

#----------PRINT_IP------------------
add_executable(print_ip print_ip.cpp)
#target_link_libraries(print_ip PRIVATE homework)

install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
#----------/PRINT_IP--------------------

#install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
