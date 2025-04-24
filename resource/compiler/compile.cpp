#include ".hpp"

void Compiler::compile(const std::string name, const std::string code, const bool keep) {
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

    int status = system(("g++ -std=c++20 -Wall -g -lm "+cpp.string()+" -o "+file.string()).c_str());
    if (status != 0)
        throw std::runtime_error("Compiler: compilation failed");

    if (keep)
        compiled.insert({ file.string(), 1 });
    else {
        std::remove(cpp.string().c_str());
        compiled.insert({ file.string(), 0 });
    }
};