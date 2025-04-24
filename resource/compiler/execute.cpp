#include ".hpp"

template <typename T>
std::vector<T> Compiler::execute(const std::string name, const std::string args) const {
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