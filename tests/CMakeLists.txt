# tests/CMakeLists.txt
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# Список всех тестов
set(TEST_SOURCES
    test_version.cpp
    test_exact_version.cpp
    test_ip.cpp
)

find_package(OpenSSL REQUIRED)
#target_link_libraries(test_md5 PRIVATE OpenSSL::SSL OpenSSL::Crypto)


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
    )

    target_link_libraries(${test_name}
        ${Boost_LIBRARIES}
        helloworld
    )

    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
