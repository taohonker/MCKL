//============================================================================
// MCKL/include/mckl/rng/student_t_distribution.hpp
//----------------------------------------------------------------------------
//                         MCKL: Monte Carlo Kernel Library
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

#ifndef MCKL_RNG_STUDENT_T_DISTRIBUTION_HPP
#define MCKL_RNG_STUDENT_T_DISTRIBUTION_HPP

#include <mckl/rng/internal/common.hpp>
#include <mckl/rng/chi_squared_distribution.hpp>
#include <mckl/rng/normal_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename RealType>
inline bool student_t_distribution_check_param(RealType n)
{
    return n > 0;
}

} // namespace mckl::internal

/// \brief Student-t distribution
/// \ingroup Distribution
template <typename RealType>
class StudentTDistribution
{
    MCKL_DEFINE_RNG_DISTRIBUTION_1(StudentT, student_t, n, 1)
    MCKL_DEFINE_RNG_DISTRIBUTION_MEMBER_2(ChiSquaredDistribution<RealType>,
        chi_squared_, NormalDistribution<RealType>, normal_)

    public:
    result_type min() const
    {
        return std::numeric_limits<result_type>::lowest();
    }

    result_type max() const { return std::numeric_limits<result_type>::max(); }

    void reset()
    {
        chi_squared_ = ChiSquaredDistribution<RealType>(n());
        normal_ = NormalDistribution<RealType>(0, 1);
    }

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        result_type z = normal_(rng);
        result_type u = const_inf<result_type>();
        if (internal::is_equal(param.n(), param_.n())) {
            while (!std::isfinite(u))
                u = n() / chi_squared_(rng);
        } else {
            ChiSquaredDistribution<RealType> chi_squared(param.n());
            while (!std::isfinite(u))
                u = param.n() / chi_squared(rng);
        }

        return z * std::sqrt(u);
    }
}; // class StudentTDistribution

namespace internal
{

template <std::size_t K, typename RealType, typename RNGType>
inline void student_t_distribution_impl(
    RNGType &rng, std::size_t n, RealType *r, RealType df)
{
    Array<RealType, K> s;
    chi_squared_distribution(rng, n, r, df);
    mul(n, 1 / df, r, r);
    sqrt(n, r, r);
    normal_distribution(
        rng, n, s.data(), const_zero<RealType>(), const_one<RealType>());
    div(n, s.data(), r, r);

    StudentTDistribution<RealType> dist(df);
    for (std::size_t i = 0; i != n; ++i)
        if (!std::isfinite(r[i]))
            r[i] = dist(rng);
}

} // namespace mckl::internal

/// \brief Generating student-t random variates
/// \ingroup Distribution
MCKL_DEFINE_RNG_DISTRIBUTION_IMPL_1(student_t, n)
MCKL_DEFINE_RNG_DISTRIBUTION_RAND_1(StudentT, student_t, n)

} // namespace mckl

#endif // MCKL_RNG_STUDENT_T_DISTRIBUTION_HPP
