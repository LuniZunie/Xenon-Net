#include ".hpp"

void Compiler::clear() {
    for (const auto& [ path, type ] : compiled) {
        std::filesystem::path file = std::filesystem::path(path);
        if (type == 1) {
            std::filesystem::path cpp = file;
            cpp.replace_extension(".cpp");
            std::remove(cpp.string().c_str());
        }
        std::remove(file.string().c_str());
    }
    compiled.clear();
};