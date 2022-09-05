#pragma once

#include "base.h"
#include "array.h"

namespace PRefl
{
    template<typename TName, typename T>
    struct Attribute : public NamedValue<TName, T>
    {
        constexpr static EType type = EType::Attr;
        constexpr Attribute(TName name, T value) noexcept
            : NamedValue<TName, T>(name, value) {}
    };

    template<typename TName>
    struct Attribute<TName, void> : public NamedValue<TName, void>
    {
        constexpr static EType type = EType::Attr;
        constexpr Attribute(TName name) noexcept
            : NamedValue<TName, void>(name) {}
    };

    template<typename TName> Attribute(TName) -> Attribute<TName, void>;
    template<typename TName, typename T> Attribute(TName, T) -> Attribute<TName, T>;

    template<typename T> concept IsAttribute = (T::type == EType::Attr);

    template<typename... TAttr>
    requires (IsAttribute<TAttr> && ...)
    struct AttrArray : public TArray<TAttr...>
    {
        constexpr static EType type = EType::AttrArray;
        constexpr AttrArray(TAttr... attrs) noexcept : TArray<TAttr...>(attrs...) {}
    };
}