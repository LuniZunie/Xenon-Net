#include ".hpp"

bool Compiler::has(const std::string name) const {
    const std::filesystem::path fileName = name;
    if (fileName.has_parent_path() || fileName.has_extension())
        throw std::runtime_error("Compiler: invalid file name");
    return compiled.find((dir / fileName).string()) != compiled.end();
};