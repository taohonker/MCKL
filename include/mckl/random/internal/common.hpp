//============================================================================
// MCKL/include/mckl/random/internal/common.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_COMMON_HPP
#define MCKL_RANDOM_INTERNAL_COMMON_HPP

#include <mckl/internal/common.hpp>

#if MCKL_HAS_MKL
#include <mkl_version.h>
#include <mkl_vsl.h>
#endif

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_0(Name, T, t, Type)        \
    public:                                                                   \
    class param_type                                                          \
    {                                                                         \
        static_assert(std::is_##t<T>::value,                                  \
            "**" #Name "Distribution::param_type** used with " #T             \
            " other than " #Type " integer types");                           \
                                                                              \
        public:                                                               \
        using result_type = T;                                                \
        using distribution_type = Name##Distribution<T>;                      \
                                                                              \
        friend bool operator==(const param_type &, const param_type &)        \
        {                                                                     \
            return true;                                                      \
        }                                                                     \
                                                                              \
        friend bool operator!=(const param_type &, const param_type &)        \
        {                                                                     \
            return false;                                                     \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_ostream<CharT, Traits> &operator<<(                 \
            std::basic_ostream<CharT, Traits> &os, const param_type &)        \
        {                                                                     \
            return os;                                                        \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_istream<CharT, Traits> &operator>>(                 \
            std::basic_istream<CharT, Traits> &is, param_type &)              \
        {                                                                     \
            return is;                                                        \
        }                                                                     \
                                                                              \
        private:                                                              \
        friend distribution_type;                                             \
    }; // class param_type

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_1(Name, name, p1, v1)      \
    public:                                                                   \
    class param_type                                                          \
    {                                                                         \
        static_assert(std::is_floating_point<RealType>::value,                \
            "**" #Name "Distribution::param_type** used with RealType other " \
            "than floating point types");                                     \
                                                                              \
        public:                                                               \
        using result_type = RealType;                                         \
        using distribution_type = Name##Distribution<RealType>;               \
                                                                              \
        explicit param_type(result_type p1 = v1) : p1##_(p1)                  \
        {                                                                     \
            ::mckl::runtime_assert(                                           \
                internal::name##_distribution_check_param(p1),                \
                "**" #Name                                                    \
                "Distribution** constructed with invalid arguments");         \
        }                                                                     \
                                                                              \
        result_type p1() const { return p1##_; }                              \
                                                                              \
        friend bool operator==(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            if (!internal::is_equal(param1.p1##_, param2.p1##_))              \
                return false;                                                 \
            return true;                                                      \
        }                                                                     \
                                                                              \
        friend bool operator!=(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            return !(param1 == param2);                                       \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_ostream<CharT, Traits> &operator<<(                 \
            std::basic_ostream<CharT, Traits> &os, const param_type &param)   \
        {                                                                     \
            if (!os)                                                          \
                return os;                                                    \
                                                                              \
            os << param.p1##_;                                                \
                                                                              \
            return os;                                                        \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_istream<CharT, Traits> &operator>>(                 \
            std::basic_istream<CharT, Traits> &is, param_type &param)         \
        {                                                                     \
            if (!is)                                                          \
                return is;                                                    \
                                                                              \
            result_type p1 = 0;                                               \
            is >> std::ws >> p1;                                              \
                                                                              \
            if (is) {                                                         \
                if (internal::name##_distribution_check_param(p1)) {          \
                    param.p1##_ = p1;                                         \
                } else {                                                      \
                    is.setstate(std::ios_base::failbit);                      \
                }                                                             \
            }                                                                 \
                                                                              \
            return is;                                                        \
        }                                                                     \
                                                                              \
        private:                                                              \
        result_type p1##_;                                                    \
                                                                              \
        friend distribution_type;                                             \
    }; // class param_type

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_2(                         \
    Name, name, p1, v1, p2, v2)                                               \
    public:                                                                   \
    class param_type                                                          \
    {                                                                         \
        static_assert(std::is_floating_point<RealType>::value,                \
            "**" #Name "Distribution::param_type** used with RealType other " \
            "than floating point types");                                     \
                                                                              \
        public:                                                               \
        using result_type = RealType;                                         \
        using distribution_type = Name##Distribution<RealType>;               \
                                                                              \
        explicit param_type(result_type p1 = v1, result_type p2 = v2)         \
            : p1##_(p1), p2##_(p2)                                            \
        {                                                                     \
            ::mckl::runtime_assert(                                           \
                internal::name##_distribution_check_param(p1, p2),            \
                "**" #Name                                                    \
                "Distribution** constructed with invalid arguments");         \
        }                                                                     \
                                                                              \
        result_type p1() const { return p1##_; }                              \
        result_type p2() const { return p2##_; }                              \
                                                                              \
        friend bool operator==(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            if (!internal::is_equal(param1.p1##_, param2.p1##_))              \
                return false;                                                 \
            if (!internal::is_equal(param1.p2##_, param2.p2##_))              \
                return false;                                                 \
            return true;                                                      \
        }                                                                     \
                                                                              \
        friend bool operator!=(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            return !(param1 == param2);                                       \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_ostream<CharT, Traits> &operator<<(                 \
            std::basic_ostream<CharT, Traits> &os, const param_type &param)   \
        {                                                                     \
            if (!os)                                                          \
                return os;                                                    \
                                                                              \
            os << param.p1##_ << ' ';                                         \
            os << param.p2##_;                                                \
                                                                              \
            return os;                                                        \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_istream<CharT, Traits> &operator>>(                 \
            std::basic_istream<CharT, Traits> &is, param_type &param)         \
        {                                                                     \
            if (!is)                                                          \
                return is;                                                    \
                                                                              \
            result_type p1 = 0;                                               \
            result_type p2 = 0;                                               \
            is >> std::ws >> p1;                                              \
            is >> std::ws >> p2;                                              \
                                                                              \
            if (is) {                                                         \
                if (internal::name##_distribution_check_param(p1, p2)) {      \
                    param.p1##_ = p1;                                         \
                    param.p2##_ = p2;                                         \
                } else {                                                      \
                    is.setstate(std::ios_base::failbit);                      \
                }                                                             \
            }                                                                 \
                                                                              \
            return is;                                                        \
        }                                                                     \
                                                                              \
        private:                                                              \
        result_type p1##_;                                                    \
        result_type p2##_;                                                    \
                                                                              \
        friend distribution_type;                                             \
    }; // class param_type

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_3(                         \
    Name, name, p1, v1, p2, v2, p3, v3)                                       \
    public:                                                                   \
    class param_type                                                          \
    {                                                                         \
        static_assert(std::is_floating_point<RealType>::value,                \
            "**" #Name "Distribution::param_type** used with RealType other " \
            "than floating point types");                                     \
                                                                              \
        public:                                                               \
        using result_type = RealType;                                         \
        using distribution_type = Name##Distribution<RealType>;               \
                                                                              \
        explicit param_type(                                                  \
            result_type p1 = v1, result_type p2 = v2, result_type p3 = v3)    \
            : p1##_(p1), p2##_(p2), p3##_(p3)                                 \
        {                                                                     \
            ::mckl::runtime_assert(                                           \
                internal::name##_distribution_check_param(p1, p2, p3),        \
                "**" #Name                                                    \
                "Distribution** constructed with invalid arguments");         \
        }                                                                     \
                                                                              \
        result_type p1() const { return p1##_; }                              \
        result_type p2() const { return p2##_; }                              \
        result_type p3() const { return p3##_; }                              \
                                                                              \
        friend bool operator==(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            if (!internal::is_equal(param1.p1##_, param2.p1##_))              \
                return false;                                                 \
            if (!internal::is_equal(param1.p2##_, param2.p2##_))              \
                return false;                                                 \
            if (!internal::is_equal(param1.p3##_, param2.p3##_))              \
                return false;                                                 \
            return true;                                                      \
        }                                                                     \
                                                                              \
        friend bool operator!=(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            return !(param1 == param2);                                       \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_ostream<CharT, Traits> &operator<<(                 \
            std::basic_ostream<CharT, Traits> &os, const param_type &param)   \
        {                                                                     \
            if (!os)                                                          \
                return os;                                                    \
                                                                              \
            os << param.p1##_ << ' ';                                         \
            os << param.p2##_ << ' ';                                         \
            os << param.p3##_;                                                \
                                                                              \
            return os;                                                        \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_istream<CharT, Traits> &operator>>(                 \
            std::basic_istream<CharT, Traits> &is, param_type &param)         \
        {                                                                     \
            if (!is)                                                          \
                return is;                                                    \
                                                                              \
            result_type p1 = 0;                                               \
            result_type p2 = 0;                                               \
            result_type p3 = 0;                                               \
            is >> std::ws >> p1;                                              \
            is >> std::ws >> p2;                                              \
            is >> std::ws >> p3;                                              \
                                                                              \
            if (is) {                                                         \
                if (internal::name##_distribution_check_param(p1, p2, p3)) {  \
                    param.p1##_ = p1;                                         \
                    param.p2##_ = p2;                                         \
                    param.p3##_ = p3;                                         \
                } else {                                                      \
                    is.setstate(std::ios_base::failbit);                      \
                }                                                             \
            }                                                                 \
                                                                              \
            return is;                                                        \
        }                                                                     \
                                                                              \
        private:                                                              \
        result_type p1##_;                                                    \
        result_type p2##_;                                                    \
        result_type p3##_;                                                    \
                                                                              \
        friend distribution_type;                                             \
    }; // class param_type

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_4(                         \
    Name, name, p1, v1, p2, v2, p3, v3, p4, v4)                               \
    public:                                                                   \
    class param_type                                                          \
    {                                                                         \
        static_assert(std::is_floating_point<RealType>::value,                \
            "**" #Name "Distribution::param_type** used with RealType other " \
            "than floating point types");                                     \
                                                                              \
        public:                                                               \
        using result_type = RealType;                                         \
        using distribution_type = Name##Distribution<RealType>;               \
                                                                              \
        explicit param_type(result_type p1 = v1, result_type p2 = v2,         \
            result_type p3 = v3, result_type p4 = v4)                         \
            : p1##_(p1), p2##_(p2), p3##_(p3), p4##_(p4)                      \
        {                                                                     \
            ::mckl::runtime_assert(                                           \
                internal::name##_distribution_check_param(p1, p2, p3, p4),    \
                "**" #Name                                                    \
                "Distribution** constructed with invalid arguments");         \
        }                                                                     \
                                                                              \
        result_type p1() const { return p1##_; }                              \
        result_type p2() const { return p2##_; }                              \
        result_type p3() const { return p3##_; }                              \
        result_type p4() const { return p4##_; }                              \
                                                                              \
        friend bool operator==(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            if (!internal::is_equal(param1.p1##_, param2.p1##_))              \
                return false;                                                 \
            if (!internal::is_equal(param1.p2##_, param2.p2##_))              \
                return false;                                                 \
            if (!internal::is_equal(param1.p3##_, param2.p3##_))              \
                return false;                                                 \
            if (!internal::is_equal(param1.p4##_, param2.p4##_))              \
                return false;                                                 \
            return true;                                                      \
        }                                                                     \
                                                                              \
        friend bool operator!=(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            return !(param1 == param2);                                       \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_ostream<CharT, Traits> &operator<<(                 \
            std::basic_ostream<CharT, Traits> &os, const param_type &param)   \
        {                                                                     \
            if (!os)                                                          \
                return os;                                                    \
                                                                              \
            os << param.p1##_ << ' ';                                         \
            os << param.p2##_ << ' ';                                         \
            os << param.p3##_ << ' ';                                         \
            os << param.p4##_;                                                \
                                                                              \
            return os;                                                        \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_istream<CharT, Traits> &operator>>(                 \
            std::basic_istream<CharT, Traits> &is, param_type &param)         \
        {                                                                     \
            if (!is)                                                          \
                return is;                                                    \
                                                                              \
            result_type p1 = 0;                                               \
            result_type p2 = 0;                                               \
            result_type p3 = 0;                                               \
            result_type p4 = 0;                                               \
            is >> std::ws >> p1;                                              \
            is >> std::ws >> p2;                                              \
            is >> std::ws >> p3;                                              \
            is >> std::ws >> p4;                                              \
                                                                              \
            if (is) {                                                         \
                if (internal::name##_distribution_check_param(                \
                        p1, p2, p3, p4)) {                                    \
                    param.p1##_ = p1;                                         \
                    param.p2##_ = p2;                                         \
                    param.p3##_ = p3;                                         \
                    param.p4##_ = p4;                                         \
                } else {                                                      \
                    is.setstate(std::ios_base::failbit);                      \
                }                                                             \
            }                                                                 \
                                                                              \
            return is;                                                        \
        }                                                                     \
                                                                              \
        private:                                                              \
        result_type p1##_;                                                    \
        result_type p2##_;                                                    \
        result_type p3##_;                                                    \
        result_type p4##_;                                                    \
                                                                              \
        friend distribution_type;                                             \
    }; // class param_type

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_0(Name, T)                \
    public:                                                                   \
    using result_type = T;                                                    \
    using distribution_type = Name##Distribution<T>;                          \
                                                                              \
    Name##Distribution() = default;                                           \
    explicit Name##Distribution(const param_type &) { reset(); }              \
    explicit Name##Distribution(param_type &&) { reset(); }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_1(Name, p1, v1)           \
    public:                                                                   \
    using result_type = RealType;                                             \
    using distribution_type = Name##Distribution<RealType>;                   \
                                                                              \
    explicit Name##Distribution(result_type p1 = v1) : param_(p1)             \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(const param_type &param) : param_(param)      \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(param_type &&param)                           \
        : param_(std::move(param))                                            \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    result_type p1() const { return param_.p1(); }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_2(Name, p1, v1, p2, v2)   \
    public:                                                                   \
    using result_type = RealType;                                             \
    using distribution_type = Name##Distribution<RealType>;                   \
                                                                              \
    explicit Name##Distribution(result_type p1 = v1, result_type p2 = v2)     \
        : param_(p1, p2)                                                      \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(const param_type &param) : param_(param)      \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(param_type &&param)                           \
        : param_(std::move(param))                                            \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    result_type p1() const { return param_.p1(); }                            \
    result_type p2() const { return param_.p2(); }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_3(                        \
    Name, p1, v1, p2, v2, p3, v3)                                             \
    public:                                                                   \
    using result_type = RealType;                                             \
    using distribution_type = Name##Distribution<RealType>;                   \
                                                                              \
    explicit Name##Distribution(                                              \
        result_type p1 = v1, result_type p2 = v2, result_type p3 = v3)        \
        : param_(p1, p2, p3)                                                  \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(const param_type &param) : param_(param)      \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(param_type &&param)                           \
        : param_(std::move(param))                                            \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    result_type p1() const { return param_.p1(); }                            \
    result_type p2() const { return param_.p2(); }                            \
    result_type p3() const { return param_.p3(); }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_4(                        \
    Name, p1, v1, p2, v2, p3, v3, p4, v4)                                     \
    public:                                                                   \
    using result_type = RealType;                                             \
    using distribution_type = Name##Distribution<RealType>;                   \
                                                                              \
    explicit Name##Distribution(result_type p1 = v1, result_type p2 = v2,     \
        result_type p3 = v3, result_type p4 = v4)                             \
        : param_(p1, p2, p3, p4)                                              \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(const param_type &param) : param_(param)      \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(param_type &&param)                           \
        : param_(std::move(param))                                            \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    result_type p1() const { return param_.p1(); }                            \
    result_type p2() const { return param_.p2(); }                            \
    result_type p3() const { return param_.p3(); }                            \
    result_type p4() const { return param_.p4(); }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_OPERATOR(Name, name)                  \
    public:                                                                   \
    const param_type &param() const { return param_; }                        \
                                                                              \
    void param(const param_type &param)                                       \
    {                                                                         \
        param_ = param;                                                       \
        reset();                                                              \
    }                                                                         \
                                                                              \
    void pram(param_type &&param)                                             \
    {                                                                         \
        param_ = std::move(param);                                            \
        reset();                                                              \
    }                                                                         \
                                                                              \
    template <typename RNGType>                                               \
    result_type operator()(RNGType &rng)                                      \
    {                                                                         \
        return operator()(rng, param_);                                       \
    }                                                                         \
                                                                              \
    template <typename RNGType>                                               \
    result_type operator()(RNGType &rng, const param_type &param)             \
    {                                                                         \
        return generate(rng, param);                                          \
    }                                                                         \
                                                                              \
    template <typename RNGType>                                               \
    void operator()(RNGType &rng, std::size_t n, result_type *r)              \
    {                                                                         \
        operator()(rng, n, r, param_);                                        \
    }                                                                         \
                                                                              \
    template <typename RNGType>                                               \
    void operator()(                                                          \
        RNGType &rng, std::size_t n, result_type *r, const param_type &param) \
    {                                                                         \
        if (n < 100) {                                                        \
            for (std::size_t i = 0; i != n; ++i)                              \
                r[i] = operator()(rng, param);                                \
        } else {                                                              \
            name##_distribution(rng, n, r, param);                            \
        }                                                                     \
    }                                                                         \
                                                                              \
    friend bool operator==(                                                   \
        const distribution_type &dist1, const distribution_type &dist2)       \
    {                                                                         \
        return dist1.param_ == dist2.param_ && dist1.is_equal(dist2);         \
    }                                                                         \
                                                                              \
    friend bool operator!=(                                                   \
        const distribution_type &dist1, const distribution_type &dist2)       \
    {                                                                         \
        return !(dist1 == dist2);                                             \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    friend std::basic_ostream<CharT, Traits> &operator<<(                     \
        std::basic_ostream<CharT, Traits> &os, const distribution_type &dist) \
    {                                                                         \
        if (!os)                                                              \
            return os;                                                        \
                                                                              \
        os << dist.param_;                                                    \
        dist.ostream(os);                                                     \
                                                                              \
        return os;                                                            \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    friend std::basic_istream<CharT, Traits> &operator>>(                     \
        std::basic_istream<CharT, Traits> &is, distribution_type &dist)       \
    {                                                                         \
        if (!is)                                                              \
            return is;                                                        \
                                                                              \
        is >> std::ws >> dist.param_;                                         \
        if (is)                                                               \
            dist.istream(is);                                                 \
                                                                              \
        return is;                                                            \
    }                                                                         \
                                                                              \
    private:                                                                  \
    param_type param_;

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0                              \
    private:                                                                  \
    bool is_equal(const distribution_type &) const { return true; }           \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void ostream(std::basic_ostream<CharT, Traits> &) const                   \
    {                                                                         \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void istream(std::basic_istream<CharT, Traits> &)                         \
    {                                                                         \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_1(T1, m1)                      \
    private:                                                                  \
    T1 m1;                                                                    \
                                                                              \
    bool is_equal(const distribution_type &other) const                       \
    {                                                                         \
        if (!internal::is_equal(m1, other.m1))                                \
            return false;                                                     \
        return true;                                                          \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void ostream(std::basic_ostream<CharT, Traits> &os) const                 \
    {                                                                         \
        if (!os)                                                              \
            return;                                                           \
                                                                              \
        os << ' ' << m1;                                                      \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void istream(std::basic_istream<CharT, Traits> &is)                       \
    {                                                                         \
        if (!is)                                                              \
            return;                                                           \
                                                                              \
        T1 tmp1;                                                              \
        is >> std::ws >> tmp1;                                                \
        if (is) {                                                             \
            m1 = std::move(tmp1);                                             \
        }                                                                     \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_2(T1, m1, T2, m2)              \
    private:                                                                  \
    T1 m1;                                                                    \
    T2 m2;                                                                    \
                                                                              \
    bool is_equal(const distribution_type &other) const                       \
    {                                                                         \
        if (!internal::is_equal(m1, other.m1))                                \
            return false;                                                     \
        if (!internal::is_equal(m2, other.m2))                                \
            return false;                                                     \
        return true;                                                          \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void ostream(std::basic_ostream<CharT, Traits> &os) const                 \
    {                                                                         \
        if (!os)                                                              \
            return;                                                           \
                                                                              \
        os << ' ' << m1;                                                      \
        os << ' ' << m2;                                                      \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void istream(std::basic_istream<CharT, Traits> &is)                       \
    {                                                                         \
        if (!is)                                                              \
            return;                                                           \
                                                                              \
        T1 tmp1;                                                              \
        T2 tmp2;                                                              \
        is >> std::ws >> tmp1;                                                \
        is >> std::ws >> tmp2;                                                \
        if (is) {                                                             \
            m1 = std::move(tmp1);                                             \
            m2 = std::move(tmp2);                                             \
        }                                                                     \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_3(T1, m1, T2, m2, T3, m3)      \
    private:                                                                  \
    T1 m1;                                                                    \
    T2 m2;                                                                    \
    T3 m3;                                                                    \
                                                                              \
    bool is_equal(const distribution_type &other) const                       \
    {                                                                         \
        if (!internal::is_equal(m1, other.m1))                                \
            return false;                                                     \
        if (!internal::is_equal(m2, other.m2))                                \
            return false;                                                     \
        if (!internal::is_equal(m3, other.m3))                                \
            return false;                                                     \
        return true;                                                          \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void ostream(std::basic_ostream<CharT, Traits> &os) const                 \
    {                                                                         \
        if (!os)                                                              \
            return;                                                           \
                                                                              \
        os << ' ' << m1;                                                      \
        os << ' ' << m2;                                                      \
        os << ' ' << m3;                                                      \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void istream(std::basic_istream<CharT, Traits> &is)                       \
    {                                                                         \
        if (!is)                                                              \
            return;                                                           \
                                                                              \
        T1 tmp1;                                                              \
        T2 tmp2;                                                              \
        T3 tmp3;                                                              \
        is >> std::ws >> tmp1;                                                \
        is >> std::ws >> tmp2;                                                \
        is >> std::ws >> tmp3;                                                \
        if (is) {                                                             \
            m1 = std::move(tmp1);                                             \
            m2 = std::move(tmp2);                                             \
            m3 = std::move(tmp3);                                             \
        }                                                                     \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_4(                             \
    T1, m1, T2, m2, T3, m3, T4, m4)                                           \
    private:                                                                  \
    T1 m1;                                                                    \
    T2 m2;                                                                    \
    T3 m3;                                                                    \
    T4 m4;                                                                    \
                                                                              \
    bool is_equal(const distribution_type &other) const                       \
    {                                                                         \
        if (!internal::is_equal(m1, other.m1))                                \
            return false;                                                     \
        if (!internal::is_equal(m2, other.m2))                                \
            return false;                                                     \
        if (!internal::is_equal(m3, other.m3))                                \
            return false;                                                     \
        if (!internal::is_equal(m4, other.m4))                                \
            return false;                                                     \
        return true;                                                          \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void ostream(std::basic_ostream<CharT, Traits> &os) const                 \
    {                                                                         \
        if (!os)                                                              \
            return;                                                           \
                                                                              \
        os << ' ' << m1;                                                      \
        os << ' ' << m2;                                                      \
        os << ' ' << m3;                                                      \
        os << ' ' << m4;                                                      \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void istream(std::basic_istream<CharT, Traits> &is)                       \
    {                                                                         \
        if (!is)                                                              \
            return;                                                           \
                                                                              \
        T1 tmp1;                                                              \
        T2 tmp2;                                                              \
        T3 tmp3;                                                              \
        T4 tmp4;                                                              \
        is >> std::ws >> tmp1;                                                \
        is >> std::ws >> tmp2;                                                \
        is >> std::ws >> tmp3;                                                \
        is >> std::ws >> tmp4;                                                \
        if (is) {                                                             \
            m1 = std::move(tmp1);                                             \
            m2 = std::move(tmp2);                                             \
            m3 = std::move(tmp3);                                             \
            m4 = std::move(tmp4);                                             \
        }                                                                     \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_0(Name, name, T, t, Type)             \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_0(Name, T, t, Type)            \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_0(Name, T)                    \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_OPERATOR(Name, name)

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_1(Name, name, p1, v1)                 \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_1(Name, name, p1, v1)          \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_1(Name, p1, v1)               \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_OPERATOR(Name, name)

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_2(Name, name, p1, v1, p2, v2)         \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_2(Name, name, p1, v1, p2, v2)  \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_2(Name, p1, v1, p2, v2)       \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_OPERATOR(Name, name)

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_3(Name, name, p1, v1, p2, v2, p3, v3) \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_3(                             \
        Name, name, p1, v1, p2, v2, p3, v3)                                   \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_3(                            \
        Name, p1, v1, p2, v2, p3, v3)                                         \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_OPERATOR(Name, name)

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_4(                                    \
    Name, name, p1, v1, p2, v2, p3, v3, p4, v4)                               \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_4(                             \
        Name, name, p1, v1, p2, v2, p3, v3, p4, v4)                           \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_3(                            \
        Name, p1, v1, p2, v2, p3, v3, p4, v4)                                 \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_OPERATOR(Name, name)

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_0(name)                          \
    template <typename RealType, typename RNGType>                            \
    inline void name##_distribution(RNGType &rng, std::size_t n, RealType *r) \
    {                                                                         \
        static_assert(std::is_floating_point<RealType>::value,                \
            "**" #name "_distribution** used with RealType other than "       \
            "floating point types");                                          \
                                                                              \
        const std::size_t K = internal::BufferSize<RealType>::value;          \
        const std::size_t M = n / K;                                          \
        const std::size_t L = n % K;                                          \
        for (std::size_t i = 0; i != M; ++i, r += K)                          \
            internal::name##_distribution_impl<K>(rng, K, r);                 \
        internal::name##_distribution_impl<K>(rng, L, r);                     \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_1(name, p1)                      \
    template <typename RealType, typename RNGType>                            \
    inline void name##_distribution(                                          \
        RNGType &rng, std::size_t N, RealType *r, RealType p1)                \
    {                                                                         \
        static_assert(std::is_floating_point<RealType>::value,                \
            "**" #name "_distribution** used with RealType other than "       \
            "floating point types");                                          \
                                                                              \
        const std::size_t K = internal::BufferSize<RealType>::value;          \
        const std::size_t M = N / K;                                          \
        const std::size_t L = N % K;                                          \
        for (std::size_t i = 0; i != M; ++i, r += K)                          \
            internal::name##_distribution_impl<K>(rng, K, r, p1);             \
        internal::name##_distribution_impl<K>(rng, L, r, p1);                 \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_2(name, p1, p2)                  \
    template <typename RealType, typename RNGType>                            \
    inline void name##_distribution(                                          \
        RNGType &rng, std::size_t N, RealType *r, RealType p1, RealType p2)   \
    {                                                                         \
        static_assert(std::is_floating_point<RealType>::value,                \
            "**" #name "_distribution** used with RealType other than "       \
            "floating point types");                                          \
                                                                              \
        const std::size_t K = internal::BufferSize<RealType>::value;          \
        const std::size_t M = N / K;                                          \
        const std::size_t L = N % K;                                          \
        for (std::size_t i = 0; i != M; ++i, r += K)                          \
            internal::name##_distribution_impl<K>(rng, K, r, p1, p2);         \
        internal::name##_distribution_impl<K>(rng, L, r, p1, p2);             \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_3(name, p1, p2, p3)              \
    template <typename RealType, typename RNGType>                            \
    inline void name##_distribution(RNGType &rng, std::size_t N, RealType *r, \
        RealType p1, RealType p2, RealType p3)                                \
    {                                                                         \
        static_assert(std::is_floating_point<RealType>::value,                \
            "**" #name "_distribution** used with RealType other than "       \
            "floating point types");                                          \
                                                                              \
        const std::size_t K = internal::BufferSize<RealType>::value;          \
        const std::size_t M = N / K;                                          \
        const std::size_t L = N % K;                                          \
        for (std::size_t i = 0; i != M; ++i, r += K)                          \
            internal::name##_distribution_impl<K>(rng, K, r, p1, p2, p3);     \
        internal::name##_distribution_impl<K>(rng, L, r, p1, p2, p3);         \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_4(name, p1, p2, p3, p4)          \
    template <typename RealType, typename RNGType>                            \
    inline void name##_distribution(RNGType &rng, std::size_t N, RealType *r, \
        RealType p1, RealType p2, RealType p3, RealType p4)                   \
    {                                                                         \
        static_assert(std::is_floating_point<RealType>::value,                \
            "**" #name "_distribution** used with RealType other than "       \
            "floating point types");                                          \
                                                                              \
        const std::size_t K = internal::BufferSize<RealType>::value;          \
        const std::size_t M = N / K;                                          \
        const std::size_t L = N % K;                                          \
        for (std::size_t i = 0; i != M; ++i, r += K)                          \
            internal::name##_distribution_impl<K>(rng, K, r, p1, p2, p3, p4); \
        internal::name##_distribution_impl<K>(rng, L, r, p1, p2, p3, p4);     \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND_0(Name, name, T)                 \
    template <typename T, typename RNGType>                                   \
    inline void name##_distribution(RNGType &rng, std::size_t N, T *r,        \
        const typename Name##Distribution<T>::param_type &)                   \
    {                                                                         \
        name##_distribution(rng, N, r);                                       \
    }                                                                         \
                                                                              \
    template <typename T, typename RNGType>                                   \
    inline void rand(                                                         \
        RNGType &rng, Name##Distribution<T> &dist, std::size_t N, T *r)       \
    {                                                                         \
        dist(rng, N, r);                                                      \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND_1(Name, name, p1)                \
    template <typename RealType, typename RNGType>                            \
    inline void name##_distribution(RNGType &rng, std::size_t N, RealType *r, \
        const typename Name##Distribution<RealType>::param_type &param)       \
    {                                                                         \
        name##_distribution(rng, N, r, param.p1());                           \
    }                                                                         \
                                                                              \
    template <typename RealType, typename RNGType>                            \
    inline void rand(RNGType &rng, Name##Distribution<RealType> &dist,        \
        std::size_t N, RealType *r)                                           \
    {                                                                         \
        dist(rng, N, r);                                                      \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND_2(Name, name, p1, p2)            \
    template <typename RealType, typename RNGType>                            \
    inline void name##_distribution(RNGType &rng, std::size_t N, RealType *r, \
        const typename Name##Distribution<RealType>::param_type &param)       \
    {                                                                         \
        name##_distribution(rng, N, r, param.p1(), param.p2());               \
    }                                                                         \
                                                                              \
    template <typename RealType, typename RNGType>                            \
    inline void rand(RNGType &rng, Name##Distribution<RealType> &dist,        \
        std::size_t N, RealType *r)                                           \
    {                                                                         \
        dist(rng, N, r);                                                      \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND_3(Name, name, p1, p2, p3)        \
    template <typename RealType, typename RNGType>                            \
    inline void name##_distribution(RNGType &rng, std::size_t N, RealType *r, \
        const typename Name##Distribution<RealType>::param_type &param)       \
    {                                                                         \
        name##_distribution(rng, N, r, param.p1(), param.p2(), param.p3());   \
    }                                                                         \
                                                                              \
    template <typename RealType, typename RNGType>                            \
    inline void rand(RNGType &rng, Name##Distribution<RealType> &dist,        \
        std::size_t N, RealType *r)                                           \
    {                                                                         \
        dist(rng, N, r);                                                      \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND_4(Name, name, p1, p2, p3, p4)    \
    template <typename RealType, typename RNGType>                            \
    inline void name##_distribution(RNGType &rng, std::size_t N, RealType *r, \
        const typename Name##Distribution<RealType>::param_type &param)       \
    {                                                                         \
        name##_distribution(                                                  \
            rng, N, r, param.p1(), param.p2(), param.p3, p4());               \
    }                                                                         \
                                                                              \
    template <typename RealType, typename RNGType>                            \
    inline void rand(RNGType &rng, Name##Distribution<RealType> &dist,        \
        std::size_t N, RealType *r)                                           \
    {                                                                         \
        dist(rng, N, r);                                                      \
    }

namespace mckl
{

namespace internal
{

MCKL_DEFINE_TYPE_DISPATCH_TRAIT(CtrType, ctr_type, NullType)
MCKL_DEFINE_TYPE_DISPATCH_TRAIT(KeyType, key_type, NullType)

} // namespace mckl::internal

/// \brief Traits of RNG engines
/// \ingroup Random
template <typename RNGType>
class RNGTraits
{
    using result_type = typename RNGType::result_type;

    static constexpr typename RNGType::result_type R_ = RNGType::min() == 0 &&
            RNGType::max() == std::numeric_limits<result_type>::max() ?
        0 :
        RNGType::max() - RNGType::min() + const_one<result_type>();

    public:
    /// \brief If the random integers are uniform on the set
    /// \f$\{0,\dots,2^W - 1\}\f$ for some \f$W > 0\f$.
    static constexpr bool is_full_range =
        RNGType::min() == 0 && (R_ & (R_ - 1)) == 0;

    static constexpr int bits = internal::Log2<result_type, R_>::value;
}; // class RNGTraits;

/// \brief Parameter type for closed interval
/// \ingroup Random
class Closed;

/// \brief Parameter type for open interval
/// \ingroup Random
class Open;

/// \brief Generate random integers
/// \ingroup Random
template <typename RNGType>
inline typename RNGType::result_type rand(RNGType &rng)
{
    return rng();
}

/// \brief Generate random integers
/// \ingroup Random
template <typename RNGType>
inline void rand(RNGType &rng, std::size_t n, typename RNGType::result_type *r)
{
    for (std::size_t i = 0; i != n; ++i)
        r[i] = rng();
}

/// \brief Generate random distribution numbers
/// \ingroup Random
template <typename RNGType, typename DistributionType>
inline typename DistributionType::result_type rand(
    RNGType &rng, const DistributionType &distribution)
{
    return distribution(rng);
}

/// \brief Generate random distribution numbers
/// \ingroup Random
template <typename RNGType, typename DistributionType>
inline void rand(RNGType &rng, const DistributionType &distribution,
    std::size_t n, typename DistributionType::result_type *r)
{
    DistributionType dist(distribution);
    for (std::size_t i = 0; i != n; ++i)
        r[i] = dist(rng);
}

template <typename, typename>
class CounterEngine;

template <typename = double>
class BetaDistribution;

template <typename = double>
class CauchyDistribution;

template <typename = double>
class ChiSquaredDistribution;

template <typename = double, std::size_t = Dynamic>
class DirichletDistribution;

template <typename = int>
class DiscreteDistribution;

template <typename = double>
class ExponentialDistribution;

template <typename = double>
class ExtremeValueDistribution;

template <typename = double>
class FisherFDistribution;

template <typename = double>
class GammaDistribution;

template <typename = double>
class LaplaceDistribution;

template <typename = double>
class LevyDistribution;

template <typename = double>
class LogisticDistribution;

template <typename = double>
class LognormalDistribution;

template <typename = double>
class NormalDistribution;

template <typename = double, std::size_t = Dynamic>
class NormalMVDistribution;

template <typename = double>
class ParetoDistribution;

template <typename = double>
class RayleighDistribution;

template <typename = double>
class StudentTDistribution;

template <typename = double>
class U01Distribution;

template <typename = double, typename = Open, typename = Closed>
class U01LRDistribution;

template <typename = unsigned>
class UniformBitsDistribution;

template <typename = double>
class UniformRealDistribution;

template <typename = double>
class WeibullDistribution;

template <typename ResultType, typename Generator>
inline void rand(
    CounterEngine<ResultType, Generator> &, std::size_t, ResultType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, BetaDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, CauchyDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, ChiSquaredDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, DiscreteDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, ExponentialDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, ExtremeValueDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, FisherFDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, GammaDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, LaplaceDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, LevyDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, LogisticDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, LognormalDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, NormalDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, std::size_t Dim, typename RNGType>
inline void rand(
    RNGType &, NormalMVDistribution<RealType, Dim> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, ParetoDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, RayleighDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, StudentTDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, U01Distribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType, typename Lower, typename Upper>
inline void rand(RNGType &, U01LRDistribution<RealType, Lower, Upper> &,
    std::size_t, RealType *);

template <typename UIntType, typename RNGType>
inline void rand(
    RNGType &, UniformBitsDistribution<UIntType> &, std::size_t, UIntType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, UniformRealDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &, WeibullDistribution<RealType> &, std::size_t, RealType *);

template <typename RealType, typename RNGType>
inline void beta_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void cauchy_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void chi_squared_distribution(
    RNGType &, std::size_t, RealType *, RealType);

template <typename RealType, typename RNGType>
inline void exponential_distribution(
    RNGType &, std::size_t, RealType *, RealType);

template <typename RealType, typename RNGType>
inline void extreme_value_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void fisher_f_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void gamma_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void laplace_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void levy_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void logistic_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void lognormal_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void normal_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void normal_mv_distribution(RNGType &, std::size_t, RealType *,
    std::size_t, const RealType *, const RealType *);

template <typename RealType, typename RNGType>
inline void pareto_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void rayleigh_distribution(
    RNGType &, std::size_t, RealType *, RealType);

template <typename RealType, typename RNGType>
inline void student_t_distribution(
    RNGType &, std::size_t, RealType *, RealType);

template <typename RealType, typename RNGType>
inline void u01_distribution(RNGType &, std::size_t, RealType *);

template <typename, typename, typename RealType, typename RNGType>
inline void u01_lr_distribution(RNGType &, std::size_t, RealType *);

template <typename UIntType, typename RNGType>
inline void uniform_bits_distribution(RNGType &, std::size_t, UIntType *);

template <typename RealType, typename RNGType>
inline void uniform_real_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

template <typename RealType, typename RNGType>
inline void weibull_distribution(
    RNGType &, std::size_t, RealType *, RealType, RealType);

#if MCKL_HAS_MKL

class MKLStream;

template <MKL_INT, int>
class MKLEngine;

template <MKL_INT BRNG, int Bits>
inline void rand(MKLEngine<BRNG, Bits> &, std::size_t,
    typename MKLEngine<BRNG, Bits>::result_type *);

template <MKL_INT BRNG, int Bits>
inline void beta_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, float *, float, float);

template <MKL_INT BRNG, int Bits>
inline void beta_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, double *, double, double);

template <MKL_INT BRNG, int Bits>
inline void cauchy_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, float *, float, float);

template <MKL_INT BRNG, int Bits>
inline void cauchy_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, double *, double, double);

template <MKL_INT BRNG, int Bits>
inline void exponential_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, float *, float);

template <MKL_INT BRNG, int Bits>
inline void exponential_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, double *, double);

template <MKL_INT BRNG, int Bits>
inline void extreme_value_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, float *, float, float);

template <MKL_INT BRNG, int Bits>
inline void extreme_value_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, double *, double, double);

template <MKL_INT BRNG, int Bits>
inline void gamma_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, float *, float, float);

template <MKL_INT BRNG, int Bits>
inline void gamma_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, double *, double, double);

template <MKL_INT BRNG, int Bits>
inline void laplace_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, float *, float, float);

template <MKL_INT BRNG, int Bits>
inline void laplace_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, double *, double, double);

template <MKL_INT BRNG, int Bits>
inline void lognormal_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, float *, float, float);

template <MKL_INT BRNG, int Bits>
inline void lognormal_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, double *, double, double);

template <MKL_INT BRNG, int Bits>
inline void normal_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, float *r, float, float);

template <MKL_INT BRNG, int Bits>
inline void normal_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, double *r, double, double);

template <MKL_INT BRNG, int Bits>
inline void normal_mv_distribution(MKLEngine<BRNG, Bits> &, std::size_t,
    float *, std::size_t, const float *, const float *);

template <MKL_INT BRNG, int Bits>
inline void normal_mv_distribution(MKLEngine<BRNG, Bits> &, std::size_t,
    double *, std::size_t, const double *, const double *);

template <MKL_INT BRNG, int Bits>
inline void rayleigh_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, float *, float);

template <MKL_INT BRNG, int Bits>
inline void rayleigh_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, double *, double);

template <MKL_INT BRNG, int Bits>
inline void u01_distribution(MKLEngine<BRNG, Bits> &, std::size_t, float *);

template <MKL_INT BRNG, int Bits>
inline void u01_distribution(MKLEngine<BRNG, Bits> &, std::size_t, double *);

template <MKL_INT BRNG, int Bits>
inline void uniform_real_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, float *, float, float);

template <MKL_INT BRNG, int Bits>
inline void uniform_real_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, double *, double, double);

template <MKL_INT BRNG, int Bits>
inline void weibull_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, float *, float, float);

template <MKL_INT BRNG, int Bits>
inline void weibull_distribution(
    MKLEngine<BRNG, Bits> &, std::size_t, double *, double, double);

#endif // MCKL_HAS_MKL

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_COMMON_HPP
