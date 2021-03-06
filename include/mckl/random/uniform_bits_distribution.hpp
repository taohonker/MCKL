//============================================================================
// MCKL/include/mckl/random/uniform_bits_distribution.hpp
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

#ifndef MCKL_RANDOM_UNIFORM_BITS_DISTRIBUTION_HPP
#define MCKL_RANDOM_UNIFORM_BITS_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>

namespace mckl
{

namespace internal
{

template <typename UIntType, typename RNGType>
inline void uniform_bits_distribution_impl(
    RNGType &rng, std::size_t n, UIntType *r, std::false_type)
{
    UniformBitsDistribution<UIntType> ubits;
    for (std::size_t i = 0; i != n; ++i)
        r[i] = ubits(rng);
}

template <typename UIntType, typename RNGType>
inline void uniform_bits_distribution_impl(
    RNGType &rng, std::size_t n, UIntType *r, std::true_type)
{
    static constexpr int rbits = RNGTraits<RNGType>::bits;
    static constexpr int ubits = std::numeric_limits<UIntType>::digits;
    static constexpr std::size_t rate = ubits / rbits;
    const std::size_t m = n * rate;
    rand(rng, m, reinterpret_cast<typename RNGType::result_type *>(r));
}

template <typename UIntType, typename RNGType>
inline void uniform_bits_distribution(RNGType &rng, std::size_t n, UIntType *r)
{
    static constexpr int rbits = RNGTraits<RNGType>::bits;
    static constexpr int ubits = std::numeric_limits<UIntType>::digits;
    static constexpr int tbits =
        std::numeric_limits<typename RNGType::result_type>::digits;
    static constexpr std::size_t ualign = alignof(UIntType);
    static constexpr std::size_t talign = alignof(UIntType);

    uniform_bits_distribution_impl(
        rng, n, r,
        std::integral_constant<
            bool, (RNGType::min() == 0 && RNGTraits<RNGType>::is_full_range &&
                      rbits == tbits && ubits >= tbits && ubits % tbits == 0 &&
                      ualign >= talign && ualign % talign == 0)>());
}

template <typename UIntType, typename RNGType>
inline void uniform_bits_distribution(RNGType &rng, std::size_t n, UIntType *r,
    const typename UniformBitsDistribution<UIntType>::param_type &)
{
    uniform_bits_distribution(rng, n, r);
}

} // namespace mckl::internal

/// \brief Uniform bits distribution
/// \ingroup Distribution
///
/// \details
/// For a given unsigned integer type `UIntType` with \f$W\f$ bits, the output
/// will be unsigned integers on the set \f$\{0,\dots,2^W - 1\}\f$ regardless
/// of the range of the input `RNGType`
template <typename UIntType>
class UniformBitsDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_UINT_TYPE(UniformBits, short)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_0(UniformBits, uniform_bits, UIntType)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0

    public:
    result_type min() const { return std::numeric_limits<result_type>::min(); }

    result_type max() const { return std::numeric_limits<result_type>::max(); }

    void reset() {}

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &)
    {
        return generate(rng,
            std::integral_constant<bool, RNGTraits<RNGType>::is_full_range>());
    }

    template <typename RNGType>
    static UIntType generate(RNGType &rng, std::false_type)
    {
        std::independent_bits_engine<RNGType,
            std::numeric_limits<UIntType>::digits, UIntType>
            eng(std::move(rng));
        UIntType u = eng();
        rng = std::move(eng.base());

        return u;
    }

    template <typename RNGType>
    static UIntType generate(RNGType &rng, std::true_type)
    {
        static constexpr int w = std::numeric_limits<UIntType>::digits;
        static constexpr int r = RNGTraits<RNGType>::bits;

        return generate_patch(rng, std::integral_constant<bool, (w <= r)>());
    }

    template <typename RNGType>
    static UIntType generate_patch(RNGType &rng, std::true_type)
    {
        return static_cast<UIntType>(rng() - RNGType::min());
    }

    template <typename RNGType>
    static UIntType generate_patch(RNGType &rng, std::false_type)
    {
        return patch<0>(rng, std::true_type());
    }

    template <int, typename RNGType>
    static UIntType patch(RNGType &, std::false_type)
    {
        return 0;
    }

    template <int N, typename RNGType>
    static UIntType patch(RNGType &rng, std::true_type)
    {
        static constexpr int w = std::numeric_limits<UIntType>::digits;
        static constexpr int r = RNGTraits<RNGType>::bits;
        static constexpr int p = r * N;
        static constexpr int q = r * N + r;

        UIntType u = static_cast<UIntType>(rng() - RNGType::min())
            << static_cast<UIntType>(p);

        return u + patch<N + 1>(rng, std::integral_constant<bool, (q < w)>());
    }
}; // class UniformBitsDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(UniformBits, UIntType)

} // namespace mckl

#endif // MCKL_RANDOM_UNIFORM_BITS_DISTRIBUTION_HPP
