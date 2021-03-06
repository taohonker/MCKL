//============================================================================
// MCKL/include/mckl/random/u01_distribution.hpp
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

#ifndef MCKL_RANDOM_U01_DISTRIBUTION_HPP
#define MCKL_RANDOM_U01_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01.hpp>
#include <mckl/random/uniform_bits_distribution.hpp>

/// \brief Default U01 distribution using fixed point conversion
/// \ingroup Config
#ifndef MCKL_U01_USE_FIXED_POINT
#define MCKL_U01_USE_FIXED_POINT 1
#endif

/// \brief Use 64-bit intermediate random integers for double precison output
/// \ingroup Config
#ifndef MCKL_U01_USE_64BITS_DOUBLE
#define MCKL_U01_USE_64BITS_DOUBLE 0
#endif

#define MCKL_DEFINE_U01_DISTRIBUTION(Name, name)                              \
    template <typename RealType>                                              \
    class Name##Distribution                                                  \
    {                                                                         \
        MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(Name)                \
        MCKL_DEFINE_RANDOM_DISTRIBUTION_0(Name, name, RealType)               \
        MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0                              \
                                                                              \
        public:                                                               \
        result_type min() const { return 0; }                                 \
                                                                              \
        result_type max() const { return 1; }                                 \
                                                                              \
        void reset() {}                                                       \
                                                                              \
        private:                                                              \
        template <typename RNGType>                                           \
        result_type generate(RNGType &rng, const param_type &)                \
        {                                                                     \
            using UIntType =                                                  \
                typename internal::U01UIntType<RNGType, RealType>;            \
                                                                              \
            UniformBitsDistribution<UIntType> ubits;                          \
                                                                              \
            return name<UIntType, result_type>(ubits(rng));                   \
        }                                                                     \
    };

#define MCKL_DEFINE_U01_DISTRIBUTION_IMPL(name)                               \
    template <std::size_t K, typename RealType, typename RNGType>             \
    inline void name##_distribution_impl(                                     \
        RNGType &rng, std::size_t n, RealType *r)                             \
    {                                                                         \
        using UIntType = U01UIntType<RNGType, RealType>;                      \
                                                                              \
        Array<UIntType, K> s;                                                 \
        uniform_bits_distribution(rng, n, s.data());                          \
        name<UIntType, RealType>(n, s.data(), r);                             \
    }

namespace mckl
{

namespace internal
{

#if MCKL_U01_USE_64BITS_DOUBLE

template <typename RNGType, typename RealType>
using U01UIntType =
    typename std::conditional<(RNGTraits<RNGType>::bits >= 64 ||
                                  std::is_same<RealType, long double>::value ||
                                  std::is_same<RealType, double>::value),
        std::uint64_t, std::uint32_t>::type;

#else // MCKL_U01_USE_64BITS_DOUBLE

template <typename RNGType, typename RealType>
using U01UIntType =
    typename std::conditional<(RNGTraits<RNGType>::bits >= 64 ||
                                  std::is_same<RealType, long double>::value),
        std::uint64_t, std::uint32_t>::type;

#endif // MCKL_U01_USE_64BITS_DOUBLE

MCKL_DEFINE_U01_DISTRIBUTION_IMPL(u01_cc)
MCKL_DEFINE_U01_DISTRIBUTION_IMPL(u01_co)
MCKL_DEFINE_U01_DISTRIBUTION_IMPL(u01_oc)
MCKL_DEFINE_U01_DISTRIBUTION_IMPL(u01_oo)

#if MCKL_U01_USE_FIXED_POINT

template <std::size_t K, typename RealType, typename RNGType>
inline void u01_distribution_impl(RNGType &rng, std::size_t n, RealType *r)
{
    u01_co_distribution_impl<K>(rng, n, r);
}

#else // MCKL_U01_USE_FIXED_POINT

template <std::size_t, typename RealType, typename UIntType>
inline RealType u01_distribution_impl(const UIntType *, std::false_type)
{
    return 0;
}

template <std::size_t N, typename RealType, typename UIntType>
inline RealType u01_distribution_impl(const UIntType *u, std::true_type)
{
    static constexpr int W = std::numeric_limits<UIntType>::digits;
    static constexpr int M = std::numeric_limits<RealType>::digits;
    static constexpr int P = (M + W - 1) / W;
    static constexpr int Q = 1 > P ? 1 : P;

    return static_cast<RealType>(u[N]) *
        U01Pow2Inv<RealType, (Q - N) * W>::value +
        u01_distribution_impl<N + 1, RealType>(
               u, std::integral_constant<bool, N + 1 < Q>());
}

template <std::size_t K, typename RealType, typename RNGType>
inline void u01_distribution_impl(RNGType &rng, std::size_t n, RealType *r)
{
    using UIntType = U01UIntType<RNGType, RealType>;

    static constexpr int W = std::numeric_limits<UIntType>::digits;
    static constexpr int M = std::numeric_limits<RealType>::digits;
    static constexpr int P = (M + W - 1) / W;
    static constexpr int Q = 1 > P ? 1 : P;

    Array<UIntType, K * Q> s;
    uniform_bits_distribution(rng, n * Q, s.data());
    const UIntType *u = s.data();
    for (std::size_t i = 0; i != n; ++i, u += Q)
        r[i] = u01_distribution_impl<0, RealType>(u, std::true_type());
}

#endif // MCKL_U01_USE_FIXED_POINT

MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_0(U01, u01, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_0(U01CC, u01_cc, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_0(U01CO, u01_co, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_0(U01OC, u01_oc, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_0(U01OO, u01_oo, RealType)

} // namespace mckl::internal

/// \brief Standard uniform distribution on [0, 1]
/// \ingroup Distribution
MCKL_DEFINE_U01_DISTRIBUTION(U01CC, u01_cc)

/// \brief Standard uniform distribution on [0, 1)
/// \ingroup Distribution
MCKL_DEFINE_U01_DISTRIBUTION(U01CO, u01_co)

/// \brief Standard uniform distribution on (0, 1]
/// \ingroup Distribution
MCKL_DEFINE_U01_DISTRIBUTION(U01OC, u01_oc)

/// \brief Standard uniform distribution on (0, 1)
/// \ingroup Distribution
MCKL_DEFINE_U01_DISTRIBUTION(U01OO, u01_oo)

#if MCKL_U01_USE_FIXED_POINT

template <typename RealType>
class U01Distribution : public U01CODistribution<RealType>
{
}; // class U01Distribution

#else // MCKL_U01_USE_FIXED_POINT

/// \brief Standard uniform distribution on [0, 1)
/// \ingroup Distribution
template <typename RealType>
class U01Distribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(U01)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_0(U01, u01, RealType)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0

    public:
    result_type min() const { return 0; }
    result_type max() const { return 1; }

    void reset() {}

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &)
    {
        return generate<0>(rng, std::true_type());
    }

    template <std::size_t, typename RNGType>
    result_type generate(RNGType &, std::false_type)
    {
        return 0;
    }

    template <std::size_t N, typename RNGType>
    result_type generate(RNGType &rng, std::true_type)
    {
        using UIntType = internal::U01UIntType<RNGType, RealType>;

        UniformBitsDistribution<UIntType> ubits;

        static constexpr int W = std::numeric_limits<UIntType>::digits;
        static constexpr int M = std::numeric_limits<RealType>::digits;
        static constexpr int P = (W + M - 1) / W;
        static constexpr int Q = 1 > P ? 1 : P;

        return static_cast<RealType>(ubits(rng)) *
            internal::U01Pow2Inv<RealType, (Q - N) * W>::value +
            generate<N + 1>(rng, std::integral_constant<bool, N + 1 < Q>());
    }
}; // class U01Distribution

#endif // MCKL_U01_USE_FIXED_POINT

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(U01, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(U01CC, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(U01CO, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(U01OC, RealType)
MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(U01OO, RealType)

} // namespace mckl

#endif // MCKL_RANDOM_U01_DISTRIBUTION_HPP
