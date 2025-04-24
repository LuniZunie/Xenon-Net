#include <iostream>
#include <unordered_map>

#include "resource/range/.hpp"

int main() {
    auto range = Range(0.0, 9.0, 0.5, true, false);
    for (auto item : range) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    std::system("pause"); // Wait for user input before closing the console window
}