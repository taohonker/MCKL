//============================================================================
// MCKL/include/mckl/random/birthday_spacings_test.hpp
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

#ifndef MCKL_RANDOM_BIRTHDAY_SPACINGS_TEST_HPP
#define MCKL_RANDOM_BIRTHDAY_SPACINGS_TEST_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/poisson_test.hpp>

namespace mckl
{

/// \brief Birthday spacings test
/// \ingroup RandomTest
///
/// \tparam D Multiplier
/// \tparam T Length of the tuple
template <std::size_t D, std::size_t T>
class BirthdaySpacingsTest : public PoissonTest<BirthdaySpacingsTest<D, T>>
{
    static_assert(D > 1, "**BirthdaySpacingsTest** used with D less than two");

    static_assert(T > 0, "**BirthdaySpacingsTest** used with T equal to zero");

    public:
    BirthdaySpacingsTest(std::size_t n) : n_(n) {}

    template <typename RNGType, typename U01Type>
    std::size_t operator()(RNGType &rng, U01Type &u01)
    {
        using result_type = typename U01Type::result_type;

        const std::size_t k = internal::BufferSize<result_type, T>::value;
        const std::size_t m = n_ / k;
        const std::size_t l = n_ % k;
        Vector<result_type> r(k * T);
        spacings_.resize(n_);
        std::size_t *s = spacings_.data();
        for (std::size_t i = 0; i != m; ++i, s += k)
            generate(rng, u01, k, r.data(), s);
        generate(rng, u01, l, r.data(), s);

        std::size_t d0 = spacings_.front();
        std::sort(spacings_.begin(), spacings_.end());
        for (std::size_t i = 0; i != n_ - 1; ++i)
            spacings_[i] = spacings_[i + 1] - spacings_[i];
        spacings_.back() = K_ - spacings_.back() + d0;
        std::sort(spacings_.begin(), spacings_.end());
        std::size_t e = 0;
        for (std::size_t i = 0; i != n_ - 1; ++i) {
            if (spacings_[i] == spacings_[i + 1])
                ++e;
        }

        return e;
    }

    double mean() const
    {
        double n = static_cast<double>(n_);
        double k = static_cast<double>(K_);

        return std::exp(
            3 * std::log(n) - std::log(k) - 2 * const_ln_2<double>());
    }

    private:
    static constexpr std::size_t K_ = internal::Pow<std::size_t, D, T>::value;

    std::size_t n_;
    Vector<std::size_t> spacings_;

    template <typename RNGType, typename U01Type>
    void generate(RNGType &rng, U01Type &u01, std::size_t n,
        typename U01Type::result_type *r, std::size_t *s)
    {
        rand(rng, u01, n * T, r);
        mul(n * T, static_cast<typename U01Type::result_type>(D), r, r);
        for (std::size_t i = 0; i != n; ++i, r += T)
            s[i] = internal::serial_index<D, T>(r);
    }
}; // class BirthdaySpacingsTest

} // namespace mckl

#endif // MCKL_RANDOM_BIRTHDAY_SPACINGS_TEST_HPP
