# Пути к include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Собираем объектную библиотеку
add_library(async SHARED
    src/async.cpp
    src/Dispatcher.cpp
    src/CommandCollector.cpp
    src/processCommands.cpp
)

#  Исполняемый демо ручной режим
add_executable(async_demo
    src/async_demo.cpp
    src/Dispatcher.cpp
    src/CommandCollector.cpp
    src/processCommands.cpp
)

# Исполняемый демо asyc lib режим
add_executable(async_demo_lib
    src/async_demo_lib.cpp
)

# Линкуем demo с нашей библиотекой
target_link_libraries(async_demo PRIVATE async)

# Линкуем demo с нашей библиотекой
target_link_libraries(async_demo_lib PRIVATE async)

if(MSVC)
    target_compile_options(async_demo PRIVATE /W4)
    target_compile_options(async_demo_lib PRIVATE /W4)
else()
    target_compile_options(async_demo PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(async_demo_lib PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS async
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/
    DESTINATION include
)

install(TARGETS async_demo
    RUNTIME DESTINATION bin
)

install(TARGETS async_demo_lib
    RUNTIME DESTINATION bin
)
