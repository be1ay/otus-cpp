# Пути к include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Собираем объектную библиотеку
add_library(async SHARED
    src/async.cpp
    src/Dispatcher.cpp
    src/CommandCollector.cpp
    src/processCommands.cpp
)

# Собираем исполняемый файл демо
add_executable(async_demo
    src/async_demo.cpp
    src/Dispatcher.cpp
    src/CommandCollector.cpp
    src/processCommands.cpp
)

# Линкуем demo с нашей библиотекой
target_link_libraries(async_demo PRIVATE async)

if(MSVC)
    target_compile_options(async_demo PRIVATE /W4)
else()
    target_compile_options(async_demo PRIVATE -Wall -Wextra -pedantic -Werror)
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

