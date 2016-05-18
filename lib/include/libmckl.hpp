//============================================================================
// MCKL/lib/src/libmckl.hpp
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

#ifndef MCKL_LIBMCKL_HPP
#define MCKL_LIBMCKL_HPP

#include <mckl/internal/common.h>
#include <mckl/mckl.hpp>

namespace mckl
{

using WeightC = Weight;

using StateMatrixC = StateMatrix<RowMajor, Dynamic, double>;

using ParticleC = Particle<StateMatrixC>;

using ParticleIndexC = ParticleIndex<StateMatrixC>;

using SamplerC = Sampler<StateMatrixC>;

using MonitorC = Monitor<StateMatrixC>;

inline WeightC &cast(const mckl_weight &weight)
{
    return *(reinterpret_cast<WeightC *>(weight.ptr));
}

inline WeightC *cast(const mckl_weight *weight_ptr)
{
    return reinterpret_cast<WeightC *>(weight_ptr->ptr);
}

inline StateMatrixC &cast(const mckl_state_matrix &state_matrix)
{
    return *(reinterpret_cast<StateMatrixC *>(state_matrix.ptr));
}

inline StateMatrixC *cast(const mckl_state_matrix *state_matrix_ptr)
{
    return reinterpret_cast<StateMatrixC *>(state_matrix_ptr->ptr);
}

inline ParticleC &cast(const mckl_particle &particle)
{
    return *(reinterpret_cast<ParticleC *>(particle.ptr));
}

inline ParticleC *cast(const mckl_particle *particle_ptr)
{
    return reinterpret_cast<ParticleC *>(particle_ptr->ptr);
}

inline MonitorC &cast(const mckl_monitor &monitor)
{
    return *(reinterpret_cast<MonitorC *>(monitor.ptr));
}

inline MonitorC *cast(const mckl_monitor *monitor_ptr)
{
    return reinterpret_cast<MonitorC *>(monitor_ptr->ptr);
}

inline SamplerC &cast(const mckl_sampler &sampler)
{
    return *(reinterpret_cast<SamplerC *>(sampler.ptr));
}

inline SamplerC *cast(const mckl_sampler *sampler_ptr)
{
    return reinterpret_cast<SamplerC *>(sampler_ptr->ptr);
}

template <typename>
inline constexpr MCKLRNGType rng_type();

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#ifdef MCKL_RNG_DEFINE_MACRO_NA
#undef MCKL_RNG_DEFINE_MACRO_NA
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    template <>                                                               \
    inline constexpr MCKLRNGType rng_type<RNGType>()                          \
    {                                                                         \
        return MCKL##Name;                                                    \
    }

#include <mckl/random/internal/rng_define_macro.hpp>

} // namespace mckl

#endif // MCKL_LIBMCKL_HPP
