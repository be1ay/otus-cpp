# hw_allocator/CMakeLists.txt

# main.cpp как отдельный исполняемый файл
add_executable(my_allocator MyAllocator.cpp)
target_include_directories(my_allocator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} 
)


if(MSVC)
    target_compile_options(my_allocator PRIVATE /W4)
else()
    target_compile_options(my_allocator PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS my_allocator RUNTIME DESTINATION bin)
