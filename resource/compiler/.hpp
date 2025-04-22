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

        int FolderInside(std::filesystem::path a, std::filesystem::path b) {
            // neg: <b> is outside <a>
            // zer: <b> is <a>
            // pos: <b> is inside <a>

            try {
                a = std::filesystem::weakly_canonical(a);
                b = std::filesystem::weakly_canonical(b);
            } catch (...) { return -1; }

            if (a == b)
                return 0;
            else if (std::distance(a.begin(), a.end()) > std::distance(b.begin(), b.end()))
                return -1;

            auto aBegin = a.begin(), bBegin = b.begin();
            auto aEnd = a.end(), bEnd = b.end();
            while (aBegin != aEnd && bBegin != bEnd) {
                if (*aBegin != *bBegin)
                    return -1;
                ++aBegin, ++bBegin;
            }

            if (aBegin == aEnd)
                return std::distance(bBegin, bEnd);
            return -1;
        };

    public:
        Compiler(const std::filesystem::path folder = "temp") {
            if (FolderInside(std::filesystem::current_path(), std::filesystem::absolute(folder)) < 0)
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

        void compile(const std::string name, const std::string code, const bool keep = false) {
            const std::filesystem::path fileName = name;
            if (fileName.has_parent_path() || fileName.has_extension())
                throw std::runtime_error("Compiler: invalid file name");

            std::filesystem::path file = std::filesystem::path(dir / fileName), cpp = file;
            cpp.replace_extension(".cpp");

            std::ofstream out(cpp.string());
            if (!out)
                throw std::runtime_error("Compiler: could not open file "+cpp.string());

            out << code;
            out.close();

            int status = system(("g++ -std=c++20 -Wall -g "+cpp.string()+" -o "+file.string()).c_str());
            if (status != 0)
                throw std::runtime_error("Compiler: compilation failed");

            if (keep)
                compiled.insert({ file.string(), 1 });
            else {
                std::remove(cpp.string().c_str());
                compiled.insert({ file.string(), 0 });
            }
        };

        bool has(const std::string name) const {
            const std::filesystem::path fileName = name;
            if (fileName.has_parent_path() || fileName.has_extension())
                throw std::runtime_error("Compiler: invalid file name");
            return compiled.find((dir / fileName).string()) != compiled.end();
        };

        template<typename T>
        std::vector<T> execute(const std::string name, const std::string args = "") const {
            const std::filesystem::path fileName = name;
            if (fileName.has_parent_path() || fileName.has_extension())
                throw std::runtime_error("Compiler: invalid file name");

            std::filesystem::path file = std::filesystem::path(dir / fileName);
            std::filesystem::path cpp = file;
            cpp.replace_extension(".cpp");

            auto it = compiled.find(file.string());
            if (it == compiled.end())
                throw std::runtime_error("Compiler: program not compiled");

            FILE* pipe = popen((file.string()+" "+args).c_str(), "r");
            if (!pipe)
                throw std::runtime_error("Compiler: popen failed");

            char buffer[128];
            std::string result = "";
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
                result += buffer;
            pclose(pipe);

            std::istringstream iss(result);
            std::vector<T> vec;
            T value{};
            while (iss >> value)
                vec.push_back(value);

            return vec;
        };

        bool remove(const std::string name) {
            const std::filesystem::path fileName = name;
            if (fileName.has_parent_path() || fileName.has_extension())
                throw std::runtime_error("Compiler: invalid file name");

            std::filesystem::path file = std::filesystem::path(dir / fileName);
            std::filesystem::path cpp = file;
            cpp.replace_extension(".cpp");

            auto it = compiled.find(file.string());
            if (it != compiled.end()) {
                if (it->second == 1)
                    std::remove(cpp.string().c_str());
                std::remove(file.string().c_str());
                compiled.erase(it);
                return true;
            } else
                return false;
        };

        void clear() {
            for (const auto& [ file, type ] : compiled) {
                std::filesystem::path file = std::filesystem::path(file);
                if (type == 1) {
                    std::filesystem::path cpp = file;
                    cpp.replace_extension(".cpp");
                    std::remove(cpp.string().c_str());
                }
                std::remove(file.string().c_str());
            }
            compiled.clear();
        };

        Compiler& operator=(const Compiler&) = delete;
        Compiler& operator=(Compiler&&) = delete;
};