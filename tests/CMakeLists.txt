# tests/CMakeLists.txt
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# Список всех тестов
set(TEST_SOURCES
    test_version.cpp
    test_exact_version.cpp
    test_ip.cpp
    test_myallocator.cpp
    test_print_ip.cpp
)
file(GLOB HW_DIRS LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/hw*")

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)

    add_executable(${test_name} ${test_src})

    set_target_properties(${test_name} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    )

    target_include_directories(${test_name}
        PRIVATE
            ${Boost_INCLUDE_DIR}
            ${CMAKE_BINARY_DIR}
            ${CMAKE_SOURCE_DIR}
            ${HW_DIRS}
    )

    target_link_libraries(${test_name}
        ${Boost_LIBRARIES}
        homework
    )

    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
