//============================================================================
// MCKL/example/utility/src/utility_hdf5.hpp
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

#include "utility_hdf5.hpp"

int main(int argc, char **argv)
{
    std::size_t nrow = 1000;
    if (argc > 1)
        nrow = static_cast<std::size_t>(std::atoi(argv[1]));

    std::size_t ncol = 1000;
    if (argc > 2)
        ncol = static_cast<std::size_t>(std::atoi(argv[2]));

    std::cout << std::string(100, '=') << std::endl;
    std::cout << std::setw(20) << std::left << "Orignal type" << std::setw(20)
              << std::left << "Load type" << std::setw(20) << std::right
              << std::fixed << "Time (ms) store" << std::setw(20) << std::right
              << std::fixed << "Time (ms) load" << std::setw(20) << std::right
              << "Test" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    MCKL_HDF5_TEST(float, float);
    MCKL_HDF5_TEST(float, double);
    MCKL_HDF5_TEST(float, long double);
    MCKL_HDF5_TEST(double, float);
    MCKL_HDF5_TEST(double, double);
    MCKL_HDF5_TEST(double, long double);
    MCKL_HDF5_TEST(long double, float);
    MCKL_HDF5_TEST(long double, float);
    MCKL_HDF5_TEST(long double, long double);

    return 0;
}
