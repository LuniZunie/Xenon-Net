#include ".hpp"

int Compiler::folder_inside(std::filesystem::path a, std::filesystem::path b) {
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