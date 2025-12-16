#include "async.h"
#include <cstring>
#include <thread>
#include <chrono>
#include <iostream>

int main() {
#if 0
    // создаём контекст с bulk_size = 2
    async_handle_t h = connect(2);
    if (!h) {
        std::cerr << "Failed to create async context\n";
        return 1;
    }

    // передаём команды кусками
    const char* data1 = "cmd1\ncmd2\ncmd3\n";
    receive(h, data1, std::strlen(data1));

    // cmd4 часть от bulk переданного ранее
    const char* data2 = "cmd4\ncmd5";
    receive(h, data2, std::strlen(data2));

    // завершение
    disconnect(h);
#endif
    // проверка ввода из задания
    std::cout<<"--------1----------------"<<std::endl;
    async_handle_t h2 = connect(3);
    const char* data3 = "cmd1\ncmd2\ncmd3\n";
    receive(h2, data3, std::strlen(data3));
    const char* data4 = "cmd4\ncmd5\n";
    receive(h2, data4, std::strlen(data4));
    disconnect(h2);
    std::cout<<"--------2----------------"<<std::endl;
    async_handle_t h3 = connect(3);
    const char* data3_1 = "cmd1\ncmd2\n{\n";
    receive(h3, data3_1, std::strlen(data3_1));

    const char* data3_2 = "cmd3\ncmd4\n}\n";
    receive(h3, data3_2, std::strlen(data3_2));

    const char* data3_3 = "{\ncmd5\ncmd6\n{\ncmd7\ncmd8\n}\ncmd9\n}\n";
    receive(h3, data3_3, std::strlen(data3_3));

    //потеря булки раний выход
    const char* data3_4 = "{\ncmd10\ncmd11\n";
    receive(h3, data3_4, std::strlen(data3_4));

    disconnect(h3);

    return 0;
}
