add_executable(bulk main.cpp)

if(MSVC)
    target_compile_options(bulk PRIVATE /W4)
else()
    target_compile_options(bulk PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS bulk RUNTIME DESTINATION bin)