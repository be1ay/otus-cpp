#pragma once

#include <type_traits>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <list>
#include <utility>
#include <tuple>

template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type
print_ip(T value) {
    constexpr size_t n = sizeof(T);
    for (size_t i = 0; i < n; ++i) {
        size_t shift = (n - 1 - i) * 8;
        uint8_t byte = (value >> shift) & 0xFF;
        std::cout << static_cast<int>(byte);
        if (i != n - 1)
            std::cout << ".";
    }
    std::cout << std::endl;
}

template<typename T>
typename std::enable_if<std::is_same<std::decay_t<T>,std::string>::value>::type
print_ip(T&& value) {
    std::cout <<value<< std::endl;
}


// Проверка наличия begin/end и value_type
template<typename T, typename = void>
struct is_container : std::false_type {};

template<typename T>
struct is_container<T,std::void_t<typename T::value_type,
                                decltype(std::declval<T>().begin()),
                                decltype(std::declval<T>().end())>> : std::true_type {};

//исключить string из контейнеров
template<typename T>
struct is_container_not_string : std::integral_constant<bool, is_container<T>::value &&
                                                              !std::is_same<std::decay_t<T>, std::string>::value>{};

template<typename T>
typename std::enable_if<is_container_not_string<T>::value>::type                             
print_ip(const T& value) {
    for (auto it = value.begin(); it != value.end(); ++it) {
        std::cout << *it;
        if (std::next(it) != value.end())
            std::cout << ".";
    }
    std::cout << std::endl;
}

//TUPLE
template<typename Tuple, std::size_t... Is>
void print_tuple_impl(const Tuple& t, std::index_sequence<Is...>) {
    // Распаковываем индексы и печатаем элементы
    ((std::cout << std::get<Is>(t) << (Is + 1 < sizeof...(Is) ? "." : "")), ...);
}

template<typename... Ts>
void print_ip(const std::tuple<Ts...>& t) {
    // Генерируем индексы 0..N-1
    print_tuple_impl(t, std::index_sequence_for<Ts...>{});
    std::cout << std::endl;
}