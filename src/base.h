#pragma once

#include "type.h"

#include <algorithm>
#include <string>
#include <type_traits>

namespace PRefl
{
    template<size_t N>
    struct StringLiteral
    {
        constexpr static size_t size = N;
        char data[N];

        constexpr StringLiteral(const char (&str)[N]) noexcept
        {
            std::copy_n(str, N, data);
        }
    };
    
    template<StringLiteral Str>
    struct Name
    {
        constexpr static StringLiteral view{ Str };
    };

    template<typename T> concept IsName = requires (T) { T::view; };

    template<typename TName, typename T>
    requires (IsName<TName>)
    struct NamedValue
    {
        using ValueType = T;
        constexpr static StringLiteral name{ TName::view };
        constexpr static bool hasValue = true;
        T value;

        constexpr NamedValue(TName, T v) noexcept : value(v) {}
    };

    template<typename TName>
    requires (IsName<TName>)
    struct NamedValue<TName, void>
    {
        constexpr static StringLiteral name{ TName::view };
        constexpr static bool hasValue = false;
        constexpr NamedValue(TName) noexcept {}
    };

    template<typename TName> NamedValue(TName) -> NamedValue<TName, void>;
    template<typename TName, typename T> NamedValue(TName, T) -> NamedValue<TName, T>;
    
}