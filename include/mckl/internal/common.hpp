//============================================================================
// MCKL/include/mckl/internal/common.hpp
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

#ifndef MCKL_INTERNAL_COMMON_HPP
#define MCKL_INTERNAL_COMMON_HPP

#include <mckl/internal/basic.hpp>
#include <mckl/math.hpp>
#include <mckl/utility/aligned_memory.hpp>

namespace mckl
{

namespace internal
{

template <typename T, std::size_t N>
using StaticVector =
    typename std::conditional<N == Dynamic, Vector<T>, std::array<T, N>>::type;

class StirlingMatrix2
{
    public:
    StirlingMatrix2(std::size_t n, std::size_t m)
        : ncol_(m + 1), data_((n + 1) * (m + 1))
    {
        std::fill(data_.begin(), data_.end(), 0);
        get(0, 0) = 1;
        for (std::size_t j = 1; j <= m; ++j) {
            get(j, j) = 1;
            for (std::size_t i = j + 1; i <= n; ++i)
                get(i, j) = j * get(i - 1, j) + get(i - 1, j - 1);
        }
    }

    double operator()(std::size_t i, std::size_t j) const
    {
        return data_[i * ncol_ + j];
    }

    private:
    std::size_t ncol_;
    Vector<double> data_;

    double &get(std::size_t i, std::size_t j) { return data_[i * ncol_ + j]; }
}; // class StirlingMatrix

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_INTERNAL_COMMON_HPP
