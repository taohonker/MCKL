//============================================================================
// MCKL/include/mckl/internal/compiler.h
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

#ifndef MCKL_INTERNAL_COMPILER_H
#define MCKL_INTERNAL_COMPILER_H

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

#if defined(__OPENCL_VERSION__)
#define MCKL_OPENCL
#include <mckl/internal/compiler/opencl.h>
#elif defined(__INTEL_COMPILER)
#define MCKL_INTEL
#include <mckl/internal/compiler/intel.h>
#elif defined(__clang__)
#define MCKL_CLANG
#include <mckl/internal/compiler/clang.h>
#elif defined(__GNUC__)
#define MCKL_GCC
#include <mckl/internal/compiler/gcc.h>
#elif defined(_MSC_VER)
#define MCKL_MSVC
#include <mckl/internal/compiler/msvc.h>
#endif

#ifndef MCKL_OPENCL
#ifdef __cplusplus
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#else
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#endif
#endif

#ifndef UINT64_C
#error __STDC_CONSTANT_MACROS not defined before #include<stdint.h>
#endif

#ifndef MCKL_HAS_X86
#if defined(i386) || defined(__i386) || defined(__i386__) ||                  \
    defined(_M_IX86) || defined(_X86_) || defined(__x86_64) ||                \
    defined(__x86_64__) || defined(__amd64) || defined(__amd64__) ||          \
    defined(_M_AMD64) || defined(_M_X64)
#define MCKL_HAS_X86 1
#else
#define MCKL_HAS_X86 0
#endif
#endif

#ifndef MCKL_HAS_X86_64
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64) ||           \
    defined(__amd64__) || defined(_M_AMD64) || defined(_M_X64)
#define MCKL_HAS_X86_64 1
#else
#define MCKL_HAS_X86_64 0
#endif
#endif

#ifndef MCKL_HAS_AESNI
#define MCKL_HAS_AESNI 0
#endif

#ifndef MCKL_HAS_RDRAND
#define MCKL_HAS_RDRAND 0
#endif

#ifndef MCKL_HAS_INT128
#define MCKL_HAS_INT128 0
#endif

#endif // MCKL_INTERNAL_COMPILER_HPP
