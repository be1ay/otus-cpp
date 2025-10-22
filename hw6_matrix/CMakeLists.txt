add_executable(mymatrix main.cpp)

if(MSVC)
    target_compile_options(mymatrix PRIVATE /W4)
else()
    target_compile_options(mymatrix PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS mymatrix RUNTIME DESTINATION bin)