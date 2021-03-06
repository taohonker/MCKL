//============================================================================
// MCKL/example/random/include/random_test.cpp
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

#ifndef MCKL_EXAMPLE_RANDOM_TEST_HPP
#define MCKL_EXAMPLE_RANDOM_TEST_HPP

#include <mckl/random/test.hpp>
#include <mckl/random/u01_distribution.hpp>
#include "random_common.hpp"

inline std::string random_test_pass(double pass)
{
    std::stringstream ss;
    if (pass < 50)
        ss << '*';
    ss << pass << '%';

    return ss.str();
}

template <typename RNGType, typename U01Type, typename TestType>
inline void random_test(std::size_t M, int nwid, int swid, int twid,
    RNGType &rng, U01Type &u01, const TestType &test, const std::string &name)
{
    std::array<double, 4> pass;
    pass.fill(0);
    mckl::StopWatch watch;
    watch.start();
    TestType t(test);
    for (std::size_t i = 0; i != M; ++i) {
        typename TestType::result_type s = t(rng, u01);
        std::get<0>(pass) += t.pass(1e-4, s) ? 1 : 0;
        std::get<1>(pass) += t.pass(1e-3, s) ? 1 : 0;
        std::get<2>(pass) += t.pass(1e-2, s) ? 1 : 0;
        std::get<3>(pass) += t.pass(1e-1, s) ? 1 : 0;
    }
    watch.stop();
    mckl::mul(4, 100.0 / M, pass.data(), pass.data());
    std::cout << std::setw(nwid) << std::left << name;
    std::cout << std::setw(swid) << std::right
              << random_test_pass(std::get<0>(pass));
    std::cout << std::setw(swid) << std::right
              << random_test_pass(std::get<1>(pass));
    std::cout << std::setw(swid) << std::right
              << random_test_pass(std::get<2>(pass));
    std::cout << std::setw(swid) << std::right
              << random_test_pass(std::get<3>(pass));
    std::cout << std::setw(twid) << std::right << watch.seconds();
    std::cout << std::endl;
}

template <typename RNGType, typename TestType>
inline void random_test(std::size_t M, int nwid, int swid, int twid,
    RNGType &rng, const TestType &test, const std::string &name)
{
    std::uniform_real_distribution<double> u01std(0, 1);
    mckl::U01CCDistribution<double> u01cc;
    mckl::U01CODistribution<double> u01co;
    mckl::U01OCDistribution<double> u01oc;
    mckl::U01OODistribution<double> u01oo;

    std::size_t lwid = static_cast<std::size_t>(nwid + swid * 4 + twid);
    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << name << std::endl;
    std::cout << std::string(lwid, '-') << std::endl;
    std::cout << std::setw(nwid) << std::left << "Level";
    std::cout << std::setw(swid) << std::right << "0.01%";
    std::cout << std::setw(swid) << std::right << "0.1%";
    std::cout << std::setw(swid) << std::right << "1%";
    std::cout << std::setw(swid) << std::right << "10%";
    std::cout << std::setw(twid) << std::right << "Time (s)";
    std::cout << std::endl;
    std::cout << std::string(lwid, '-') << std::endl;
    random_test(M, nwid, swid, twid, rng, u01std, test, "STD");
    random_test(M, nwid, swid, twid, rng, u01cc, test, "U01CC");
    random_test(M, nwid, swid, twid, rng, u01co, test, "U01CO");
    random_test(M, nwid, swid, twid, rng, u01oc, test, "U01OC");
    random_test(M, nwid, swid, twid, rng, u01oo, test, "U01OO");
    std::cout << std::string(lwid, '-') << std::endl;
}

inline void random_test(std::size_t N, std::size_t M, int, char **)
{
    const int nwid = 10;
    const int swid = 10;
    const int twid = 15;

    mckl::RNG rng;

    random_test(M, nwid, swid, twid, rng, mckl::BirthdaySpacingsTest<2, 40>(N),
        "BirthdaySpacings (n = 10^5, d = 2, t = 40)");
    random_test(M, nwid, swid, twid, rng, mckl::BirthdaySpacingsTest<2, 42>(N),
        "BirthdaySpacings (n = 10^5, d = 2, t = 42)");
    random_test(M, nwid, swid, twid, rng, mckl::BirthdaySpacingsTest<2, 44>(N),
        "BirthdaySpacings (n = 10^5, d = 2, t = 44)");
    random_test(M, nwid, swid, twid, rng, mckl::BirthdaySpacingsTest<2, 46>(N),
        "BirthdaySpacings (n = 10^5, d = 2, t = 46)");

    random_test(M, nwid, swid, twid, rng, mckl::CollisionTest<2, 20>(N),
        "Collision (n = 10^5, d = 2, t = 20)");
    random_test(M, nwid, swid, twid, rng, mckl::CollisionTest<2, 30>(N),
        "Collision (n = 10^5, d = 2, t = 30)");
    random_test(M, nwid, swid, twid, rng, mckl::CollisionTest<2, 40>(N),
        "Collision (n = 10^5, d = 2, t = 40)");
    random_test(M, nwid, swid, twid, rng, mckl::CollisionTest<2, 50>(N),
        "Collision (n = 10^5, d = 2, t = 50)");

    random_test(M, nwid, swid, twid, rng, mckl::CouponCollectorTest<2>(N),
        "CouponCollector (n = 10^5, d = 2^1)");
    random_test(M, nwid, swid, twid, rng, mckl::CouponCollectorTest<4>(N),
        "CouponCollector (n = 10^5, d = 2^2)");
    random_test(M, nwid, swid, twid, rng, mckl::CouponCollectorTest<8>(N),
        "CouponCollector (n = 10^5, d = 2^3)");
    random_test(M, nwid, swid, twid, rng, mckl::CouponCollectorTest<16>(N),
        "CouponCollector (n = 10^5, d = 2^4)");

    random_test(M, nwid, swid, twid, rng, mckl::GapTest<>(N, 0, 0.5),
        "Gap (n = 10^5, alpha = 0, beta = 2^-1)");
    random_test(M, nwid, swid, twid, rng, mckl::GapTest<>(N, 0, 0.25),
        "Gap (n = 10^5, alpha = 0, beta = 2^-2)");
    random_test(M, nwid, swid, twid, rng, mckl::GapTest<>(N, 0, 0.125),
        "Gap (n = 10^5, alpha = 0, beta = 2^-3)");
    random_test(M, nwid, swid, twid, rng, mckl::GapTest<>(N, 0, 0.0625),
        "Gap (n = 10^5, alpha = 0, beta = 2^-4)");

    random_test(M, nwid, swid, twid, rng, mckl::MaximumOfTTest<1024, 8>(N),
        "MaximumOfT (n = 10^5, d = 2^10, t = 3)");
    random_test(M, nwid, swid, twid, rng, mckl::MaximumOfTTest<2048, 16>(N),
        "MaximumOfT (n = 10^5, d = 2^11, t = 4)");
    random_test(M, nwid, swid, twid, rng, mckl::MaximumOfTTest<4096, 32>(N),
        "MaximumOfT (n = 10^5, d = 2^12, t = 5)");
    random_test(M, nwid, swid, twid, rng, mckl::MaximumOfTTest<8192, 64>(N),
        "MaximumOfT (n = 10^5, d = 2^13, t = 6)");

    random_test(M, nwid, swid, twid, rng, mckl::PermutationTest<3>(N),
        "Permutation (n = 10^5, t = 3)");
    random_test(M, nwid, swid, twid, rng, mckl::PermutationTest<5>(N),
        "Permutation (n = 10^5, t = 5)");
    random_test(M, nwid, swid, twid, rng, mckl::PermutationTest<7>(N),
        "Permutation (n = 10^5, t = 7)");
    random_test(M, nwid, swid, twid, rng, mckl::PermutationTest<11>(N),
        "Permutation (n = 10^5, t = 11)");

    random_test(M, nwid, swid, twid, rng, mckl::PokerTest<16, 16>(N),
        "Poker (n = 10^5, d = 2^3, t = 2^3)");
    random_test(M, nwid, swid, twid, rng, mckl::PokerTest<8, 16>(N),
        "Poker (n = 10^5, d = 2^3, t = 2^4)");
    random_test(M, nwid, swid, twid, rng, mckl::PokerTest<16, 8>(N),
        "Poker (n = 10^5, d = 2^4, t = 2^3)");
    random_test(M, nwid, swid, twid, rng, mckl::PokerTest<16, 16>(N),
        "Poker (n = 10^5, d = 2^4, t = 2^4)");

    random_test(M, nwid, swid, twid, rng, mckl::RunTest<false>(N),
        "Run (n = 10^5, Up = false)");
    random_test(M, nwid, swid, twid, rng, mckl::RunTest<true>(N),
        "Run (n = 10^5, Up = true)");
    random_test(M, nwid, swid, twid, rng, mckl::RunTest<false>(N),
        "RunIndep (n = 10^5, Up = false)");
    random_test(M, nwid, swid, twid, rng, mckl::RunTest<true>(N),
        "RunIndep (n = 10^5, Up = true)");

    random_test(M, nwid, swid, twid, rng, mckl::SerialTest<64, 2>(N),
        "Serial (n = 10^5, d = 2^6, t = 2)");
    random_test(M, nwid, swid, twid, rng, mckl::SerialTest<16, 3>(N),
        "Serial (n = 10^5, d = 2^4, t = 3)");
    random_test(M, nwid, swid, twid, rng, mckl::SerialOverTest<64, 2>(N),
        "SerialOver (n = 10^5, d = 2^6, t = 2, overlap = true)");
    random_test(M, nwid, swid, twid, rng, mckl::SerialOverTest<16, 3>(N),
        "SerialOver (n = 10^5, d = 2^4, t = 3)");
}

#endif // MCKL_EXAMPLE_RANDOM_TEST_HPP
