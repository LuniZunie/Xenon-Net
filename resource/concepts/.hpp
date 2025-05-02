#pragma once

#include <concepts>
#include <vector>

namespace concepts {
    class number {
        public:
            static constexpr short Any = 0;
            static constexpr short Integer = 1;
            static constexpr short Floating = 2;
            static constexpr short Signed = 3;
            static constexpr short Unsigned = 4;
            static constexpr short Short = 5;
            static constexpr short Long = 6;
            static constexpr short LongLong = 7;

            template <typename T>
            static constexpr bool assert(std::vector<short> types) {
                for (const auto& type : types) {
                    switch (type) {
                        case Any:
                            if (!std::is_arithmetic_v<T>) return false;
                            break;
                        case Integer:
                            if (!std::is_integral_v<T>) return false;
                            break;
                        case Floating:
                            if (!std::is_floating_point_v<T>) return false;
                            break;
                        case Signed:
                            if (!std::is_signed_v<T>) return false;
                            break;
                        case Unsigned:
                            if (!std::is_unsigned_v<T>) return false;
                            break;
                        case Short:
                            if (sizeof(T) != sizeof(short)) return false;
                            break;
                        case Long:
                            if (sizeof(T) != sizeof(long)) return false;
                            break;
                        case LongLong:
                            if (sizeof(T) != sizeof(long long)) return false;
                            break;
                        default:
                            throw std::invalid_argument("Invalid type specified in number::assert.");
                    }
                }

                return true;
            }
    };

    class container {
        public:
            template <typename T, typename U = typename T::value_type>
            static constexpr bool assert() { // Check if T is a container (has value_type and begin/end methods) and check that the value_type is the same as T
                if constexpr (!std::is_same_v<typename T::value_type, U>)
                    return false;
                else
                    return requires(T c) {
                        typename T::value_type;
                        { c.begin() } -> std::input_iterator;
                        { c.end() } -> std::input_iterator;
                    };
            }
    };
};