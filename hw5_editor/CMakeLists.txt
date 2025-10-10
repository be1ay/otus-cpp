add_executable(myeditor main.cpp controller.cpp document.cpp)

if(MSVC)
    target_compile_options(myeditor PRIVATE /W4)
else()
    target_compile_options(myeditor PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS myeditor RUNTIME DESTINATION bin)
