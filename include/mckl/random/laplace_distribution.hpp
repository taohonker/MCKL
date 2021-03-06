//============================================================================
// MCKL/include/mckl/random/laplace_distribution.hpp
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

#ifndef MCKL_RANDOM_LAPLACE_DISTRIBUTION_HPP
#define MCKL_RANDOM_LAPLACE_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename RealType>
inline bool laplace_distribution_check_param(RealType, RealType b)
{
    return b > 0;
}

template <std::size_t K, typename RealType, typename RNGType>
inline void laplace_distribution_impl(
    RNGType &rng, std::size_t n, RealType *r, RealType a, RealType b)
{
    Array<RealType, K> s;
    u01_oo_distribution(rng, n, r);
    sub(n, r, static_cast<RealType>(0.5), r);
    for (std::size_t i = 0; i != n; ++i) {
        if (r[i] > 0) {
            r[i] = 1 - 2 * r[i];
            s[i] = -b;
        } else {
            r[i] = 1 + 2 * r[i];
            s[i] = b;
        }
    }
    log(n, r, r);
    fma(n, s.data(), r, a, r);
}

MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_2(
    Laplace, laplace, RealType, RealType, a, RealType, b)

} // namespace mckl::internal

/// \brief Laplace distribution
/// \ingroup Distribution
template <typename RealType>
class LaplaceDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(Laplace)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_2(
        Laplace, laplace, RealType, result_type, a, 0, result_type, b, 1)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0

    public:
    result_type min() const
    {
        return std::numeric_limits<result_type>::lowest();
    }

    result_type max() const { return std::numeric_limits<result_type>::max(); }

    void reset() {}

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        U01OODistribution<RealType> u01;
        result_type u = u01(rng) - static_cast<result_type>(0.5);

        return u > 0 ? param.a() - param.b() * std::log(1 - 2 * u) :
                       param.a() + param.b() * std::log(1 + 2 * u);
    }
}; // class LaplaceDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(Laplace, RealType)

} // namespace mckl

#endif // MCKL_RANDOM_LAPLACE_DISTRIBUTION_HPP
