#include ".hpp"

bool Compiler::erase(const std::string name) {
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