#pragma once

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class Compiler {
    private:
        std::filesystem::path dir;
        std::unordered_map<std::string, int> compiled;

        int folder_inside(std::filesystem::path a, std::filesystem::path b);

    public:
        Compiler(const std::filesystem::path folder = "temp") {
            if (folder_inside(std::filesystem::current_path(), std::filesystem::absolute(folder)) < 0)
                throw std::invalid_argument("Compiler: folder is outside current path.");

            dir = std::filesystem::absolute(folder);
            if (std::filesystem::exists(dir) && !std::filesystem::is_empty(dir))
                throw std::invalid_argument("Compiler: folder is not empty.");
            std::filesystem::create_directories(dir);
        };
        Compiler(const Compiler&) = delete;
        Compiler(const Compiler&&) = delete;

        ~Compiler() {
            clear();
            std::filesystem::remove_all(dir);
        };

        void compile(const std::string name, const std::string code, const bool keep = false);

        bool has(const std::string name) const;

        template<typename T>
        std::vector<T> execute(const std::string name, const std::string args = "") const;

        bool erase(const std::string name);

        void clear();

        Compiler& operator=(const Compiler&) = delete;
        Compiler& operator=(Compiler&&) = delete;
};