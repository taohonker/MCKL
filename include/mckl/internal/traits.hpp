//============================================================================
// MCKL/include/mckl/internal/traits.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2016, Yan Zhou
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#ifndef MCKL_INTERNAL_TRAITS_HPP
#define MCKL_INTERNAL_TRAITS_HPP

#include <mckl/internal/config.h>
#include <mckl/internal/defines.hpp>
#include <mckl/internal/forward.hpp>
#include <type_traits>

#define MCKL_DEFINE_TYPE_DISPATCH_TRAIT(Outer, Inner, Default)                \
    template <typename T>                                                     \
    class Outer##Trait;                                                       \
                                                                              \
    namespace internal                                                        \
    {                                                                         \
                                                                              \
    template <typename T>                                                     \
    class Has##Outer##Impl                                                    \
    {                                                                         \
        class char2                                                           \
        {                                                                     \
            char c1;                                                          \
            char c2;                                                          \
        };                                                                    \
                                                                              \
        template <typename U>                                                 \
        static char test(typename U::Inner *);                                \
                                                                              \
        template <typename U>                                                 \
        static char2 test(...);                                               \
                                                                              \
        public:                                                               \
        static constexpr bool value =                                         \
            sizeof(test<T>(nullptr)) == sizeof(char);                         \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    class Has##Outer                                                          \
        : public std::integral_constant<bool, Has##Outer##Impl<T>::value>     \
    {                                                                         \
    };                                                                        \
                                                                              \
    template <typename T, bool>                                               \
    class Outer##Dispatch;                                                    \
                                                                              \
    template <typename T>                                                     \
    class Outer##Dispatch<T, false>                                           \
    {                                                                         \
        public:                                                               \
        using type = Default;                                                 \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    class Outer##Dispatch<T, true>                                            \
    {                                                                         \
        public:                                                               \
        using type = typename T::Inner;                                       \
    };                                                                        \
                                                                              \
    } /* namespace interanl */                                                \
                                                                              \
    template <typename T>                                                     \
    class Outer##Trait                                                        \
    {                                                                         \
        public:                                                               \
        static constexpr bool value = internal::Has##Outer<T>::value;         \
        using type = typename internal::Outer##Dispatch<T, value>::type;      \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    using Outer = typename Outer##Trait<T>::type;

#define MCKL_DEFINE_TYPE_TEMPLATE_DISPATCH_TRAIT(Outer, Inner, Default)       \
    template <typename T>                                                     \
    class Outer##Trait;                                                       \
                                                                              \
    namespace internal                                                        \
    {                                                                         \
                                                                              \
    template <typename T>                                                     \
    class Has##Outer##Impl                                                    \
    {                                                                         \
        class char2                                                           \
        {                                                                     \
            char c1;                                                          \
            char c2;                                                          \
        };                                                                    \
                                                                              \
        template <typename U>                                                 \
        static char test(typename U::template Inner<T> *);                    \
                                                                              \
        template <typename U>                                                 \
        static char2 test(...);                                               \
                                                                              \
        public:                                                               \
        static constexpr bool value =                                         \
            sizeof(test<T>(nullptr)) == sizeof(char);                         \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    class Has##Outer                                                          \
        : public std::integral_constant<bool, Has##Outer##Impl<T>::value>     \
    {                                                                         \
    };                                                                        \
                                                                              \
    template <typename T, bool>                                               \
    class Outer##Dispatch;                                                    \
                                                                              \
    template <typename T>                                                     \
    class Outer##Dispatch<T, false>                                           \
    {                                                                         \
        public:                                                               \
        using type = Default<T>;                                              \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    class Outer##Dispatch<T, true>                                            \
    {                                                                         \
        public:                                                               \
        using type = typename T::template Inner<T>;                           \
    };                                                                        \
    }                                                                         \
                                                                              \
    template <typename T>                                                     \
    class Outer##Trait                                                        \
    {                                                                         \
        public:                                                               \
        static constexpr bool value = internal::Has##Outer<T>::value;         \
        using type = typename internal::Outer##Dispatch<T, value>::type;      \
    };                                                                        \
                                                                              \
    template <typename T>                                                     \
    using Outer = typename Outer##Trait<T>::type;

namespace mckl
{

/// \brief Particle::size_type etc., traits
/// \ingroup Traits
MCKL_DEFINE_TYPE_DISPATCH_TRAIT(SizeType, size_type, std::size_t)

namespace internal
{

template <typename T, typename T1, typename... Types>
class is_one_of : public std::integral_constant<bool,
                      is_one_of<T, T1>::value || is_one_of<T, Types...>::value>
{
}; // class is_one_of

template <typename T, typename T1>
class is_one_of<T, T1>
    : public std::integral_constant<bool, std::is_same<T, T1>::value>
{
}; // class is_one_of

template <typename T, typename T1, typename... Types>
class is_seed_seq
    : public std::integral_constant<bool,
          is_seed_seq<T, T1>::value && is_seed_seq<T, Types...>::value>
{
}; // class is_seed_seq

template <typename T, typename T1>
class is_seed_seq<T, T1>
    : public std::integral_constant<bool, !std::is_convertible<T, T1>::value>
{
}; // class is_seed_seq

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_INTERNAL_TRAITS_HPP
