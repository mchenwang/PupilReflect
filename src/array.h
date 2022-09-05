#pragma once

#include <tuple>

namespace PRefl
{
    namespace Detail
    {
        template<size_t N, size_t... Ns>
        struct IndexSequence
        {
            static constexpr size_t front = N;
            static constexpr auto next = std::index_sequence<Ns...>{};
            
            constexpr IndexSequence(std::index_sequence<N, Ns...>) noexcept {}
        };

        // template <size_t Index>
        // struct TupleVisitor
        // {
        //     template <typename T, typename Func>
        //     static void Visit(T&& tup, size_t idx, Func&& func)
        //     {
        //         if (idx == Index - 1) std::forward<Func>(func)(std::get<Index - 1>(tup));
        //         else TupleVisitor<Index - 1>::visit(tup, idx, std::forward<Func>(func));
        //     }
        // };

        // template <>
        // struct TupleVisitor<10>
        // {
        //     template <typename T, typename Func>
        //     static void Visit(T&& tup, size_t idx, Func&& func)
        //     {
        //         if (idx == 9) std::forward<Func>(func)(std::get<9>(tup));
        //         else TupleVisitor<9>::visit(tup, idx, std::forward<Func>(func));
        //     }
        // };

        // template <typename Func, typename... Ts>
        // void VisitAt(std::tuple<Ts...>&& tup, size_t idx, Func&& func)
        // {
        //     TupleVisitor<sizeof...(Ts)>::Visit(tup, idx, std::forward<Func>(func));
        // }
    }

    template<typename... Ts>
    struct TArray
    {
        constexpr static size_t size = sizeof...(Ts);
        std::tuple<Ts...> elements;

        constexpr TArray(Ts... element) noexcept : elements(element...) {}

        template<size_t index>
        requires (index < size)
        constexpr auto Get() const
        {
            return std::get<index>(elements);
        }

        template<typename T>
        constexpr auto Push(T newElement) const
        {
            return std::apply(
                [&newElement](auto&... e) {
                    return TArray<Ts..., T>{ e..., newElement};
                }, elements
            );
        }

        template<typename Func>
        constexpr void ForEach(Func&& func) const
        {
            std::apply(
                [&](auto&... e) {
                    (func(e), ...);
                }, elements
            );
        }

        template<typename T, typename CmpFunc, std::size_t... Ns>
        constexpr int FindIf(T target, CmpFunc&& func, Detail::IndexSequence<Ns...> seq) const {
            constexpr size_t index = seq.front;
            if constexpr (sizeof...(Ns) > 1)
                return (std::forward<CmpFunc>(func)(target, Get<index>())) ?
                    index : FindIf(target, std::forward<CmpFunc>(func), Detail::IndexSequence{ seq.next });
            else
                return (std::forward<CmpFunc>(func)(target, Get<index>())) ?
                    index : -1;
        }

        // @return the index of target element and returns -1 when it does not exist
        template<typename T, typename CmpFunc>
        constexpr int FindIf(T target, CmpFunc&& func) const
        {
            return FindIf(target, std::forward<CmpFunc>(func), Detail::IndexSequence{ std::make_index_sequence<size>{} });
        }
    };
}