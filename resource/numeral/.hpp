#pragma once

#include <concepts>

namespace numeral {
    template <typename T>
    concept is = std::is_arithmetic_v<T>;
    template <typename T>
    concept is_signed = std::is_arithmetic_v<T> && std::is_signed_v<T>;
    template <typename T>
    concept is_unsigned = std::is_arithmetic_v<T> && std::is_unsigned_v<T>;

    namespace integer {
        template <typename T>
        concept is = std::is_integral_v<T>;
        template <typename T>
        concept is_signed = std::is_integral_v<T> && std::is_signed_v<T>;
        template <typename T>
        concept is_unsigned = std::is_integral_v<T> && std::is_unsigned_v<T>;
    };

    namespace floating {
        template <typename T>
        concept is = std::is_floating_point_v<T>;
        template <typename T>
        concept is_signed = std::is_floating_point_v<T> && std::is_signed_v<T>;
        template <typename T>
        concept is_unsigned = std::is_floating_point_v<T> && std::is_unsigned_v<T>;
    };
};