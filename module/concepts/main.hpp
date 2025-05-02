#pragma once

#include <concepts>
#include <type_traits>
#include <vector>

namespace concepts {
    class number {
        private:
        using enum_type = const unsigned short;

        template <typename T, enum_type type>
        static constexpr bool check() {
            if constexpr (type == Integer)
                return std::is_integral_v<T>;
            else if constexpr (type == Floating)
                return std::is_floating_point_v<T>;
            else if constexpr (type == Signed)
                return std::is_signed_v<T>;
            else if constexpr (type == Unsigned)
                return std::is_unsigned_v<T>;
            else if constexpr (type == Short)
                return sizeof(short) == sizeof(T);
            else if constexpr (type == Long)
                return sizeof(long) == sizeof(T);
            else if constexpr (type == LongLong)
                return sizeof(long long) == sizeof(T);
            else
                return false;
        }

        public:
        static constexpr enum_type Integer = 0;
        static constexpr enum_type Floating = 1;
        static constexpr enum_type Signed = 2;
        static constexpr enum_type Unsigned = 3;
        static constexpr enum_type Short = 4;
        static constexpr enum_type Long = 5;
        static constexpr enum_type LongLong = 6;

        template <typename T, enum_type... types>
        static constexpr bool assert() {
            if constexpr (std::is_arithmetic_v<T>)
                return ((check<T, types>()) && ...);
            return false;
        }
    };

    class container {
        public:
        template <typename C, typename T = typename C::value_type>
        static constexpr bool assert() {
            return requires(C c) {
                typename T;
                { c.begin() } -> std::input_iterator;
                { c.end() } -> std::input_iterator;
            };
        };
    };
};