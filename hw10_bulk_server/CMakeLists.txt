add_executable(bulk_server
    src/main.cpp
    src/Dispatcher.cpp
    src/CommandCollector.cpp
    src/Server.cpp
    src/Session.cpp
    )

if(MSVC)
    target_compile_options(bulk_server PRIVATE /W4)
else()
    target_compile_options(bulk_server PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(bulk_server PRIVATE Boost::system)

install(TARGETS bulk_server
    RUNTIME DESTINATION bin
)

