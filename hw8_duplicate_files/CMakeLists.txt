add_executable(bayan main.cpp utils.cpp)

if(MSVC)
    target_compile_options(bayan PRIVATE /W4)
else()
    target_compile_options(bayan PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

find_package(Boost REQUIRED COMPONENTS filesystem system program_options)
target_link_libraries(bayan PRIVATE Boost::filesystem Boost::system Boost::program_options)



    
install(TARGETS bayan RUNTIME DESTINATION bin)