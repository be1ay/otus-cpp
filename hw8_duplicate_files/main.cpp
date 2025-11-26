#include <cassert>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/unordered_map.hpp>
#include <boost/program_options.hpp>

#include "utils.hpp"

namespace po = boost::program_options;
using namespace hw8;

int main(int argc, char* argv[])
{
        // Определение опций
        po::options_description desc("Опции утилиты");
        desc.add_options()
            ("help,h", "Вывести справку")
            ("scan-dir,s", po::value<std::vector<std::string>>()->multitoken(),
             "Директории для сканирования (может быть несколько)")
            ("exclude-dir,e", po::value<std::vector<std::string>>()->multitoken(),
             "Директории для исключения из сканирования (может быть несколько)")
            ("level,l", po::value<int>()->default_value(0),
             "Уровень сканирования (0 - только указанная директория, -1 = полный рекурсивный)")
            ("min-size,m", po::value<size_t>()->default_value(1),
             "Минимальный размер файла (по умолчанию >1 байта)")
            ("mask,k", po::value<std::vector<std::string>>()->multitoken(),
             "Маски имён файлов для сравнения (регистронезависимые)")
            ("block-size,b", po::value<size_t>()->default_value(1024),
             "Размер блока чтения файлов")
            ("hash-algo,a", po::value<std::string>()->default_value("crc32"),
             "Алгоритм хэширования (crc32, md5)");

        // Парсинг
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        // Справка
        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        // Получение значений

        std::vector<std::string> scandirs;
        if (vm.count("scan-dir")) {
            scandirs = vm["scan-dir"].as<std::vector<std::string>>();
            std::cout << "Директории для сканирования:\n";
            for (auto& d : scandirs) std::cout << "  " << d << "\n";
        }

        std::vector<std::string> exdirs;
        if (vm.count("exclude-dir")) {
            exdirs = vm["exclude-dir"].as<std::vector<std::string>>();
            std::cout << "Исключаемые директории:\n";
            for (auto& d : exdirs) std::cout << "  " << d << "\n";
        }

        std::vector<std::string> masks;
        if (vm.count("mask")) {
            masks = vm["mask"].as<std::vector<std::string>>();
            std::cout << "Маски файлов:\n";
            for (auto& m : masks) std::cout << "  " << m << "\n";
        }

        int level = vm["level"].as<int>();
        std::cout << "Уровень сканирования: " << level << "\n";

        size_t minSize = vm["min-size"].as<size_t>();
        std::cout << "Минимальный размер файла: " << minSize << " байт\n";

        size_t blockSize = vm["block-size"].as<size_t>();
        std::cout << "Размер блока чтения: " << blockSize << " байт\n";

        std::string algo = vm["hash-algo"].as<std::string>();
        std::cout << "Алгоритм хэширования: " << algo << "\n";

        // Проверка допустимых алгоритмов
        if (algo != "crc32" && algo != "md5") {
            std::cerr << "Ошибка: неизвестный алгоритм " << algo << "\n";
            return 1;
        }

    boost::unordered_map<std::size_t, std::vector<FileInfo>> size_groups;

    // Канонизируем исключённые директории
    std::vector<fs::path> expaths;
    for (const auto& ed : exdirs) {
        try { expaths.push_back(fs::canonical(ed)); }
        catch (...) { expaths.push_back(fs::path(ed)); }
    }

    for (const auto& root_str : scandirs) {
        fs::path root(root_str);
        if (!fs::exists(root) || !fs::is_directory(root)) {
            std::cerr << "Пропускаю " << root_str << " (нет или не каталог)\n";
            continue;
        }

        fs::path root_can;
        try { root_can = fs::canonical(root); }
        catch (...) { root_can = root; }

        for (fs::recursive_directory_iterator it(root), end; it != end; ++it) {
            const fs::path p = it->path();

            // Проверка исключённых директорий
            bool excluded = false;
            fs::path p_can;
            try { p_can = fs::canonical(p); }
            catch (...) { p_can = p; }

            for (const auto& ex : expaths) {
                if (p_can == ex || (!ex.empty() && p_can.string().find(ex.string() + fs::path::preferred_separator) == 0)) {
                    excluded = true;
                    break;
                }
            }
            // Проверка исключённых директорий
            if (excluded && fs::is_directory(p)){
                it.disable_recursion_pending(); // не заходим внутрь
            }

            // Ограничение глубины
            if (level == 0 && fs::is_directory(p)){
                it.disable_recursion_pending(); // не заходим никуда
            }
            else if (level > 0 && it.depth() >= level && fs::is_directory(p)){
                it.disable_recursion_pending(); // ограничиваем глубину
            }
            // level == -1 → полный рекурсивный обход

            // Фильтрация файлов
            if (fs::is_regular_file(p)) {
                if (level == 0) {
                    fs::path parent_can;
                    try { parent_can = fs::canonical(p.parent_path()); }
                    catch (...) { parent_can = p.parent_path(); }
                    if (parent_can != root_can) continue; // только корень
                }

                std::size_t sz = 0;
                try { sz = fs::file_size(p); }
                catch (...) { continue; }

                if (sz >= minSize && match_mask(p.filename().string(), masks)) {
                    size_groups[sz].push_back(FileInfo{p,std::vector<std::string>()});
                }
            }
        }
    }

    // вывод результата
    for (const auto& [size, files] : size_groups) {
        std::cout << "Размер: " << size << " байт\n";
        for (const auto& f : files) {
            std::cout << "  " << f.path << "\n";
        }
    }
    std::cout<<"-------------------------------------------" <<std::endl;

    // Для union-find нужны две карты: ранги и родители
    boost::unordered_map<fs::path, int> rank_map;
    boost::unordered_map<fs::path, fs::path> parent_map;

    // Создаём структуру disjoint_sets
    boost::disjoint_sets<
        boost::associative_property_map<boost::unordered_map<fs::path,int>>,
        boost::associative_property_map<boost::unordered_map<fs::path,fs::path>>
    > ds(
        boost::make_assoc_property_map(rank_map),
        boost::make_assoc_property_map(parent_map)
    );

    std::vector<std::set<fs::path>> duplicates;

    // 1. Инициализация множеств только для кандидатов
    for (auto& [size, files] : size_groups) {
        if (files.size() < 2) continue; // уникальные размеры отбрасываем
        for (auto& f : files) {
            ds.make_set(f.path);
        }
    }

    // 2. Сравнение файлов внутри каждой группы
    for (auto& [size, files] : size_groups) {
        if (files.size() < 2) continue;
        for (size_t i = 0; i < files.size(); ++i) {
            for (size_t j = i + 1; j < files.size(); ++j) {
                if (compare_files(files[i], files[j], blockSize, algo)) {
                    ds.union_set(files[i].path, files[j].path);
                }
            }
        }
    }

    // 3. Сбор групп дубликатов
    boost::unordered_map<fs::path, std::set<fs::path>> groups;
    for (auto& [size, files] : size_groups) {
        if (files.size() < 2) continue;
        for (auto& f : files) {
            fs::path root = ds.find_set(f.path);
            groups[root].insert(f.path);
        }
    }

    // 4. Вывод
    for (auto& [root, group] : groups) {
        for (auto& f : group) {
            std::cout << f.string() << "\n";
        }
        std::cout << "\n";
    }
    return 0;
}