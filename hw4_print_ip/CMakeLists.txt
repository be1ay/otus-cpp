# hw_print_ip/CMakeLists.txt

add_executable(print_ip print_ip.cpp)

if(MSVC)
    target_compile_options(print_ip PRIVATE /W4)
else()
    target_compile_options(print_ip PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS print_ip RUNTIME DESTINATION bin)
