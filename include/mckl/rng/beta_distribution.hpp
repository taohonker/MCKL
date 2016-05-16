//============================================================================
// MCKL/include/mckl/rng/beta_distribution.hpp
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

#ifndef MCKL_RNG_BETA_DISTRIBUTION_HPP
#define MCKL_RNG_BETA_DISTRIBUTION_HPP

#include <mckl/rng/internal/common.hpp>
#include <mckl/rng/normal_distribution.hpp>
#include <mckl/rng/u01_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename RealType>
inline bool beta_distribution_check_param(RealType alpha, RealType beta)
{
    return alpha > 0 && beta > 0;
}

enum BetaDistributionAlgorithm {
    BetaDistributionAlgorithmAS,
    BetaDistributionAlgorithm11,
    BetaDistributionAlgorithm1X,
    BetaDistributionAlgorithmX1,
    BetaDistributionAlgorithmC,
    BetaDistributionAlgorithmJ,
    BetaDistributionAlgorithmA1,
    BetaDistributionAlgorithmA2,
    BetaDistributionAlgorithmA3
}; // enum BetaDistributionAlgorithm

template <typename RealType>
class BetaDistributionConstant
{
    public:
    BetaDistributionConstant(RealType alpha = 1, RealType beta = 1)
    {
        reset(alpha, beta);
    }

    void reset(RealType alpha, RealType beta)
    {
        const RealType K = static_cast<RealType>(0.852);
        const RealType C = static_cast<RealType>(-0.956);
        const RealType D = beta + K * alpha * alpha + C;
        if (is_equal(alpha, static_cast<RealType>(0.5)) &&
            is_equal(beta, static_cast<RealType>(0.5)))
            algorithm = BetaDistributionAlgorithmAS;
        else if (is_one(alpha) && is_one(beta))
            algorithm = BetaDistributionAlgorithm11;
        else if (is_one(alpha))
            algorithm = BetaDistributionAlgorithm1X;
        else if (is_one(beta))
            algorithm = BetaDistributionAlgorithmX1;
        else if (alpha > 1 && beta > 1)
            algorithm = BetaDistributionAlgorithmC;
        else if (alpha < 1 && beta < 1 && D <= 0)
            algorithm = BetaDistributionAlgorithmJ;
        else if (alpha < 1 && beta < 1)
            algorithm = BetaDistributionAlgorithmA1;
        else if (alpha < 1 && beta > 1)
            algorithm = BetaDistributionAlgorithmA2;
        else if (alpha > 1 && beta < 1)
            algorithm = BetaDistributionAlgorithmA3;
        else
            algorithm = BetaDistributionAlgorithmC;

        a = b = t = p = 0;
        switch (algorithm) {
            case BetaDistributionAlgorithmAS: break;
            case BetaDistributionAlgorithm11: break;
            case BetaDistributionAlgorithm1X: b = 1 / beta; break;
            case BetaDistributionAlgorithmX1: a = 1 / alpha; break;
            case BetaDistributionAlgorithmC:
                a = alpha + beta;
                b = std::min(alpha, beta);
                if (b > 1)
                    b = std::sqrt((2 * alpha * beta - a) / (a - 2));
                b = 1 / b;
                t = alpha + 1 / b;
                p = a * std::log(a);
                break;
            case BetaDistributionAlgorithmJ:
                a = 1 / alpha;
                b = 1 / beta;
                break;
            case BetaDistributionAlgorithmA1:
                a = 1 / alpha;
                b = 1 / beta;
                t = std::sqrt(alpha * (1 - alpha));
                t /= t + std::sqrt(beta * (1 - beta));
                p = beta * t;
                p /= p + alpha * (1 - t);
                break;
            case BetaDistributionAlgorithmA2:
                a = 1 / alpha;
                b = 1 / beta;
                t = 1 - alpha;
                t /= t + beta;
                p = beta * t;
                p /= p + alpha * std::pow(1 - t, beta);
                break;
            case BetaDistributionAlgorithmA3:
                a = 1 / beta;
                b = 1 / alpha;
                t = 1 - beta;
                t /= t + alpha;
                p = alpha * t;
                p /= p + beta * std::pow(1 - t, alpha);
                break;
        }
    }

    RealType a;
    RealType b;
    RealType t;
    RealType p;
    BetaDistributionAlgorithm algorithm;
}; // class BetaDistributionConstant

} // namespace internal

/// \brief Beta distribution
/// \ingroup Distribution
template <typename RealType>
class BetaDistribution
{
    MCKL_DEFINE_RNG_DISTRIBUTION_2(Beta, beta, alpha, 1, beta, 1)

    public:
    result_type min() const { return 0; }

    result_type max() const { return 1; }

    void reset() { constant_.reset(alpha(), beta()); }

    private:
    internal::BetaDistributionConstant<RealType> constant_;

    bool is_equal(const distribution_type &other) const
    {
        if (!is_equal(constant_.a, other.constant_.a))
            return false;
        if (!is_equal(constant_.b, other.constant_.b))
            return false;
        if (!is_equal(constant_.t, other.constant_.t))
            return false;
        if (!is_equal(constant_.p, other.constant_.p))
            return false;
        if (!is_equal(constant_.algorithm, other.constant_.algorithm))
            return false;
        return true;
    }

    template <typename CharT, typename Traits>
    void ostream(std::basic_ostream<CharT, Traits> &) const
    {
    }

    template <typename CharT, typename Traits>
    void istream(std::basic_istream<CharT, Traits> &)
    {
        reset();
    }

    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        if (param == param_)
            return generate(rng, param_, constant_);

        internal::BetaDistributionConstant<RealType> constant(
            param.alpha(), param.beta());

        return generate(rng, param, constant);
    }

    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param,
        const internal::BetaDistributionConstant<RealType> &constant)
    {
        result_type r = 0;
        switch (constant.algorithm) {
            case internal::BetaDistributionAlgorithmAS:
                r = generate_as(rng, param, constant);
                break;
            case internal::BetaDistributionAlgorithm11:
                r = generate_11(rng, param, constant);
                break;
            case internal::BetaDistributionAlgorithm1X:
                r = generate_1x(rng, param, constant);
                break;
            case internal::BetaDistributionAlgorithmX1:
                r = generate_x1(rng, param, constant);
                break;
            case internal::BetaDistributionAlgorithmC:
                r = generate_c(rng, param, constant);
                break;
            case internal::BetaDistributionAlgorithmJ:
                r = generate_j(rng, param, constant);
                break;
            case internal::BetaDistributionAlgorithmA1:
                r = generate_a1(rng, param, constant);
                break;
            case internal::BetaDistributionAlgorithmA2:
                r = generate_a2(rng, param, constant);
                break;
            case internal::BetaDistributionAlgorithmA3:
                r = generate_a3(rng, param, constant);
                break;
        }

        return r;
    }

    template <typename RNGType>
    result_type generate_as(RNGType &rng, const param_type &,
        const internal::BetaDistributionConstant<RealType> &)
    {
        U01OODistribution<RealType> u01;
        result_type u = u01(rng);
        u = std::sin(
            -const_pi_by2<result_type>() + const_pi<result_type>() * u);

        return static_cast<result_type>(0.5) +
            static_cast<result_type>(0.5) * u;
    }

    template <typename RNGType>
    result_type generate_11(RNGType &rng, const param_type &,
        const internal::BetaDistributionConstant<RealType> &)
    {
        U01OODistribution<RealType> u01;

        return u01(rng);
    }

    template <typename RNGType>
    result_type generate_1x(RNGType &rng, const param_type &,
        const internal::BetaDistributionConstant<RealType> &constant)
    {
        U01OODistribution<RealType> u01;

        return 1 - std::exp(constant.b * std::log(u01(rng)));
    }

    template <typename RNGType>
    result_type generate_x1(RNGType &rng, const param_type &,
        const internal::BetaDistributionConstant<RealType> &constant)
    {
        U01OODistribution<RealType> u01;

        return std::exp(constant.a * std::log(u01(rng)));
    }

    template <typename RNGType>
    result_type generate_c(RNGType &rng, const param_type &param,
        const internal::BetaDistributionConstant<RealType> &constant)
    {
        U01OODistribution<RealType> u01;
        const result_type ln_4 = 2 * const_ln_2<result_type>();
        result_type x = 0;
        result_type y = 0;
        result_type left = 0;
        result_type right = 0;
        do {
            result_type u1 = u01(rng);
            result_type u2 = u01(rng);
            result_type v = constant.b * std::log(u1 / (1 - u1));
            x = param.alpha() * std::exp(v);
            y = param.beta() + x;
            left = (constant.p - constant.a * std::log(y)) +
                (constant.t * v - ln_4);
            right = std::log(u1 * u1 * u2);
        } while (left < right);

        return x / y;
    }

    template <typename RNGType>
    result_type generate_j(RNGType &rng, const param_type &,
        const internal::BetaDistributionConstant<RealType> &constant)
    {
        U01OODistribution<RealType> u01;
        result_type x = 0;
        result_type y = 0;
        do {
            x = std::pow(u01(rng), constant.a);
            y = std::pow(u01(rng), constant.b);
        } while (x + y > 1);

        return x / (x + y);
    }

    template <typename RNGType>
    result_type generate_a1(RNGType &rng, const param_type &param,
        const internal::BetaDistributionConstant<RealType> &constant)
    {
        U01OODistribution<RealType> u01;
        while (true) {
            result_type u = u01(rng);
            result_type e = -std::log(u01(rng));
            result_type x = 0;
            result_type v = 0;
            if (u < constant.p) {
                x = constant.t * std::pow(u / constant.p, constant.a);
                v = (1 - param.beta()) * std::log((1 - x) / (1 - constant.t));
            } else {
                x = 1 -
                    (1 - constant.t) *
                        std::pow((1 - u) / (1 - constant.p), constant.b);
                v = (1 - param.alpha()) * std::log(x / constant.t);
            }
            if (v < e)
                return x;
        }
    }

    template <typename RNGType>
    result_type generate_a2(RNGType &rng, const param_type &param,
        const internal::BetaDistributionConstant<RealType> &constant)
    {
        U01OODistribution<RealType> u01;
        while (true) {
            result_type u = u01(rng);
            result_type e = -std::log(u01(rng));
            result_type x = 0;
            result_type v = 0;
            if (u < constant.p) {
                x = constant.t * std::pow(u / constant.p, constant.a);
                v = (1 - param.beta()) * std::log(1 - x);
            } else {
                x = 1 -
                    (1 - constant.t) *
                        std::pow((1 - u) / (1 - constant.p), constant.b);
                v = (1 - param.alpha()) * std::log(x / constant.t);
            }
            if (v < e)
                return x;
        }
    }

    template <typename RNGType>
    result_type generate_a3(RNGType &rng, const param_type &param,
        const internal::BetaDistributionConstant<RealType> &constant)
    {
        U01OODistribution<RealType> u01;
        while (true) {
            result_type u = u01(rng);
            result_type e = -std::log(u01(rng));
            result_type x = 0;
            result_type v = 0;
            if (u < constant.p) {
                x = constant.t * std::pow(u / constant.p, constant.a);
                v = (1 - param.alpha()) * std::log(1 - x);
            } else {
                x = 1 -
                    (1 - constant.t) *
                        std::pow((1 - u) / (1 - constant.p), constant.b);
                v = (1 - param.beta()) * std::log(x / constant.t);
            }
            if (v < e)
                return 1 - x;
        }
    }
}; // class BetaDistribution

namespace internal
{

template <std::size_t, typename RealType, typename RNGType>
inline std::size_t beta_distribution_impl_as(RNGType &rng, std::size_t n,
    RealType *r, RealType, RealType,
    const BetaDistributionConstant<RealType> &)
{
    u01_oo_distribution(rng, n, r);
    fma(n, const_pi<RealType>(), r, -const_pi_by2<RealType>(), r);
    sin(n, r, r);
    fma(n, static_cast<RealType>(0.5), r, static_cast<RealType>(0.5), r);

    return n;
}

template <std::size_t, typename RealType, typename RNGType>
inline std::size_t beta_distribution_impl_11(RNGType &rng, std::size_t n,
    RealType *r, RealType, RealType,
    const BetaDistributionConstant<RealType> &)
{
    u01_oo_distribution(rng, n, r);

    return n;
}

template <std::size_t, typename RealType, typename RNGType>
inline std::size_t beta_distribution_impl_1x(RNGType &rng, std::size_t n,
    RealType *r, RealType, RealType,
    const BetaDistributionConstant<RealType> &constant)
{
    u01_oo_distribution(rng, n, r);
    log(n, r, r);
    mul(n, constant.b, r, r);
    exp(n, r, r);
    sub(n, const_one<RealType>(), r, r);

    return n;
}

template <std::size_t, typename RealType, typename RNGType>
inline std::size_t beta_distribution_impl_x1(RNGType &rng, std::size_t n,
    RealType *r, RealType, RealType,
    const BetaDistributionConstant<RealType> &constant)
{
    u01_oo_distribution(rng, n, r);
    log(n, r, r);
    mul(n, constant.a, r, r);
    exp(n, r, r);

    return n;
}

template <std::size_t K, typename RealType, typename RNGType>
inline std::size_t beta_distribution_impl_c(RNGType &rng, std::size_t n,
    RealType *r, RealType alpha, RealType beta,
    const BetaDistributionConstant<RealType> &constant)
{
    const RealType a = constant.a;
    const RealType b = constant.b;
    const RealType t = constant.t;
    const RealType p = constant.p;
    const RealType ln_4 = 2 * const_ln_2<RealType>();
    Array<RealType, K * 5> s;
    RealType *const u1 = s.data();
    RealType *const u2 = s.data() + n;
    RealType *const v = s.data() + n * 2;
    RealType *const x = s.data() + n * 3;
    RealType *const y = s.data() + n * 4;
    u01_oo_distribution(rng, n * 2, s.data());
    sub(n, const_one<RealType>(), u1, v);
    div(n, u1, v, v);
    log(n, v, v);
    mul(n, b, v, v);
    exp(n, v, x);
    mul(n, alpha, x, x);
    add(n, beta, x, y);
    div(n, x, y, x);
    sqr(n, u1, u1);
    mul(n, u1, u2, u2);
    log(n, u2, u2);
    log(n, y, u1);
    fma(n, -a, u1, p, u1);
    fma(n, t, v, -ln_4, v);
    add(n, v, u1, u1);

    std::size_t m = 0;
    for (std::size_t i = 0; i != n; ++i)
        if (u1[i] > u2[i])
            r[m++] = x[i];

    return m;
}

template <std::size_t K, typename RealType, typename RNGType>
inline std::size_t beta_distribution_impl_j(RNGType &rng, std::size_t n,
    RealType *r, RealType, RealType,
    const BetaDistributionConstant<RealType> &constant)
{
    const RealType a = constant.a;
    const RealType b = constant.b;
    Array<RealType, K * 3> s;
    RealType *const x = s.data();
    RealType *const y = s.data() + n;
    RealType *const u = s.data() + n * 2;
    u01_oo_distribution(rng, n * 2, s.data());
    pow(n, x, a, x);
    pow(n, y, b, y);
    add(n, x, y, u);
    div(n, x, u, x);

    std::size_t m = 0;
    for (std::size_t i = 0; i != n; ++i)
        if (u[i] < 1)
            r[m++] = x[i];

    return m;
}

template <std::size_t, typename RealType, typename RNGType>
inline std::size_t beta_distribution_impl_a1(RNGType &, std::size_t,
    RealType *, RealType, RealType, const BetaDistributionConstant<RealType> &)
{
    return 0;
}

template <std::size_t, typename RealType, typename RNGType>
inline std::size_t beta_distribution_impl_a2(RNGType &, std::size_t,
    RealType *, RealType, RealType, const BetaDistributionConstant<RealType> &)
{
    return 0;
}

template <std::size_t, typename RealType, typename RNGType>
inline std::size_t beta_distribution_impl_a3(RNGType &, std::size_t,
    RealType *, RealType, RealType, const BetaDistributionConstant<RealType> &)
{
    return 0;
}

template <std::size_t K, typename RealType, typename RNGType>
inline std::size_t beta_distribution_impl(RNGType &rng, std::size_t n,
    RealType *r, RealType alpha, RealType beta,
    const BetaDistributionConstant<RealType> &constant)
{
    switch (constant.algorithm) {
        case BetaDistributionAlgorithmAS:
            return beta_distribution_impl_as<K>(
                rng, n, r, alpha, beta, constant);
        case BetaDistributionAlgorithm11:
            return beta_distribution_impl_11<K>(
                rng, n, r, alpha, beta, constant);
        case BetaDistributionAlgorithm1X:
            return beta_distribution_impl_1x<K>(
                rng, n, r, alpha, beta, constant);
        case BetaDistributionAlgorithmX1:
            return beta_distribution_impl_x1<K>(
                rng, n, r, alpha, beta, constant);
        case BetaDistributionAlgorithmC:
            return beta_distribution_impl_c<K>(
                rng, n, r, alpha, beta, constant);
        case BetaDistributionAlgorithmJ:
            return beta_distribution_impl_j<K>(
                rng, n, r, alpha, beta, constant);
        case BetaDistributionAlgorithmA1:
            return beta_distribution_impl_a1<K>(
                rng, n, r, alpha, beta, constant);
        case BetaDistributionAlgorithmA2:
            return beta_distribution_impl_a2<K>(
                rng, n, r, alpha, beta, constant);
        case BetaDistributionAlgorithmA3:
            return beta_distribution_impl_a3<K>(
                rng, n, r, alpha, beta, constant);
    }
    return 0;
}

} // namespace mckl::internal

/// \brief Generating Beta random variates
/// \ingroup Distribution
template <typename RealType, typename RNGType>
inline void beta_distribution(
    RNGType &rng, std::size_t n, RealType *r, RealType alpha, RealType beta)
{
    static_assert(std::is_floating_point<RealType>::value,
        "**beta_distribution** used with RealType other than floating point "
        "types");

    const std::size_t k = internal::BufferSize<RealType>::value;
    const internal::BetaDistributionConstant<RealType> constant(alpha, beta);
    while (n > k) {
        std::size_t m = internal::beta_distribution_impl<k>(
            rng, k, r, alpha, beta, constant);
        if (m == 0)
            break;
        n -= m;
        r += m;
    }
    std::size_t m =
        internal::beta_distribution_impl<k>(rng, n, r, alpha, beta, constant);
    n -= m;
    r += m;
    if (n > 0) {
        BetaDistribution<RealType> dist(alpha, beta);
        for (std::size_t i = 0; i != n; ++i)
            r[i] = dist(rng);
    }
}

MCKL_DEFINE_RNG_DISTRIBUTION_RAND_2(Beta, beta, alpha, beta)

} // namespace mckl

#endif // MCKL_RNG_BETA_DISTRIBUTION_HPP
