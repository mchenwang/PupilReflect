#pragma once

#include "field.h"
#include "macro.h"

namespace PRefl
{
    template<typename T>
    struct ReflData
    {
        constexpr static bool hasData = false;
        constexpr static bool hasBases = false; // only reflect public bases
    };

    template<typename T> struct ReflObjType {};
    template<typename T> struct ReflObjType<ReflData<T>> { using Type = T; };

    template<typename... Ts>
    requires (Ts::hasData && ...)
    struct ReflDataArray : public TArray<Ts...>
    {
        constexpr ReflDataArray(Ts... datas) noexcept : TArray<Ts...>(datas...) {}
    };

    template<typename T>
    struct ReflOp
    {
        template<typename Type>
        using PureType = std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<Type>>>;

        template<typename TReflObj, typename Func>
        constexpr static void ForEachFields(TReflObj& o, Func&& customer)
        {
            using ReflDataType = ReflData<T>;

            if constexpr (ReflDataType::hasBases)
            {
                ReflDataType::bases.ForEach(
                    [&o, &customer](const auto& e) {
                        ReflOp<ReflObjType<PureType<decltype(e)>>::Type>::ForEachFields(o, std::forward<Func>(customer));
                    }
                );
            }
            
            ReflDataType::fields.ForEach(
                [&o, &customer](const auto& e) {
                    std::forward<Func>(customer)(o, e);
                }
            );
        }

        template<typename TReflObj, typename Func>
        constexpr static void RecursiveForEachFields(TReflObj& o, Func&& customer)
        {
            using ReflDataType = ReflData<T>;
            
            if constexpr (ReflDataType::hasBases)
            {
                ReflDataType::bases.ForEach(
                    [&o, &customer](const auto& e) {
                        ReflOp<ReflObjType<PureType<decltype(e)>>::Type>::RecursiveForEachFields(o, std::forward<Func>(customer));
                    }
                );
            }

            ReflDataType::fields.ForEach(
                [&o, &customer](const auto& e) {
                    std::forward<Func>(customer)(o, e);
                    auto& field = e.GetField(o);
                    using FieldType = PureType<decltype(field)>;
                    if constexpr (ReflData<FieldType>::hasData)
                    {                          
                        ReflOp<FieldType>::RecursiveForEachFields(field, std::forward<Func>(customer));
                    }
                }
            );
        }
    };

    template<typename T>
    struct Refl
    {
        T* pObj;

        Refl(T& obj) : pObj(&obj) {}
        
        template<typename Func>
        constexpr void ForEachFields(Func&& func)
        {
            ReflOp<T>::ForEachFields(*pObj, std::forward<Func>(func));
        }

        template<typename Func>
        constexpr void RecursiveForEachFields(Func&& func)
        {
            ReflOp<T>::RecursiveForEachFields(*pObj, std::forward<Func>(func));
        }
    };
}