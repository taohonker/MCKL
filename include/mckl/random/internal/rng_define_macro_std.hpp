//============================================================================
// MCKL/include/mckl/random/internal/rng_define_macro_std.hpp
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

MCKL_RNG_DEFINE_MACRO(::std::mt19937, mt19937, mt19937)
MCKL_RNG_DEFINE_MACRO(::std::mt19937_64, mt19937_64, mt19937_64)
MCKL_RNG_DEFINE_MACRO(::std::minstd_rand0, minstd_rand0, minstd_rand0)
MCKL_RNG_DEFINE_MACRO(::std::minstd_rand, minstd_rand, minstd_rand)
MCKL_RNG_DEFINE_MACRO(::std::ranlux24_base, ranlux24_base, ranlux24_base)
MCKL_RNG_DEFINE_MACRO(::std::ranlux48_base, ranlux48_base, ranlux48_base)
MCKL_RNG_DEFINE_MACRO(::std::ranlux24, ranlux24, ranlux24)
MCKL_RNG_DEFINE_MACRO(::std::ranlux48, ranlux48, ranlux48)
MCKL_RNG_DEFINE_MACRO(::std::knuth_b, knuth_b, knuth_b)
