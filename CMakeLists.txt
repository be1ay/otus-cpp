cmake_minimum_required(VERSION 3.16)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(Homeworks VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

# Подключаем подпроекты (каждое ДЗ отдельно)
add_subdirectory(hw2_ip_filter)
add_subdirectory(hw3_allocator)
add_subdirectory(hw4_print_ip)
add_subdirectory(hw5_editor)
add_subdirectory(hw6_matrix)

add_library(homework lib.cpp hw5_editor/controller.cpp
                             hw5_editor/document.cpp)

target_include_directories(homework
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if(WITH_BOOST_TEST)
    enable_testing()
    add_subdirectory(tests)
endif()

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)