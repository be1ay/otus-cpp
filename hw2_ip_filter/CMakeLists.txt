# hw2_ip_filter/CMakeLists.txt

add_executable(ip_filter ip_filter.cpp)

if(MSVC)
    target_compile_options(ip_filter PRIVATE /W4)
else()
    target_compile_options(ip_filter PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_link_libraries(ip_filter
        homework
    )

install(TARGETS ip_filter RUNTIME DESTINATION bin)

