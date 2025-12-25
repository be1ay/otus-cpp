add_executable(join_server
    src/main.cpp
    src/Server.cpp
    src/Session.cpp
    src/Database.cpp
    )

if(MSVC)
    target_compile_options(join_server PRIVATE /W4)
else()
    target_compile_options(join_server PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(join_server PRIVATE Boost::system)

install(TARGETS join_server
    RUNTIME DESTINATION bin
)

