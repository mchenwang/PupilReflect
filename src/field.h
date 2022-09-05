#pragma once

#include "base.h"
#include "attribute.h"

namespace PRefl
{
    namespace Detail
    {
        template<typename T>
        struct StaticFieldTraits;
        
        template<typename Object, typename T>
        struct StaticFieldTraits<T Object::*> { constexpr static bool value = false; };

        template<typename T>
        struct StaticFieldTraits<T*> { constexpr static bool value = true; };
    }

    template<typename TName, typename T, typename TAttrs>
    requires (TAttrs::type == EType::AttrArray && !std::is_void_v<T>)
    struct Field : public NamedValue<TName, T>
    {
        constexpr static EType type = EType::Field;
        constexpr static bool hasAttributes = TAttrs::size > 0;
        constexpr static bool isStatic = Detail::StaticFieldTraits<T>::value;
        TAttrs attributes;

        constexpr Field(TName name, T value) noexcept
            : NamedValue<TName, T>(name, value) {}

        constexpr Field(TName name, T value, TAttrs attrs) noexcept
            : NamedValue<TName, T>(name, value), attributes(attrs) {}

        template<typename TObj>
        constexpr auto& GetField(TObj& o) const
        {
            if constexpr (isStatic) return *(this->value);
            else if constexpr (std::is_pointer_v<TObj>)
                return o->*(this->value);
            else return o.*(this->value);
        }
    };

    template<typename TName, typename T, typename TAttrs>
    Field(TName, T, TAttrs) -> Field<TName, T, TAttrs>;
    template<typename TName, typename T>
    Field(TName, T) -> Field<TName, T, AttrArray<>>;

    template<typename T>
    concept IsField = (T::type == EType::Field);

    template<typename... TField>
    requires (IsField<TField> && ...)
    struct FieldArray : public TArray<TField...>
    {
        constexpr static EType type = EType::FieldArray;
        constexpr FieldArray(TField... fields) noexcept : TArray<TField...>(fields...) {}
    };
}