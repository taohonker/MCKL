//============================================================================
// MCKL/example/pf/include/pf_cv.hpp
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

#ifndef MCKL_EXAMPLE_PFCV_HPP
#define MCKL_EXAMPLE_PFCV_HPP

#include "pf.hpp"

template <mckl::MatrixLayout Layout>
using PFCVBase = mckl::StateMatrix<Layout, 4, double>;

template <mckl::MatrixLayout Layout, typename RNGSetType>
class PFCV : public PFCVBase<Layout>
{
    public:
    using rng_set_type = RNGSetType;

    using base = PFCVBase<Layout>;

    template <typename T>
    using base_idx = typename base::template particle_index_type<T>;

    template <typename T>
    class particle_index_type : public base_idx<T>
    {
        public:
        particle_index_type(std::size_t id, mckl::Particle<T> *pptr)
            : base_idx<T>(id, pptr)
        {
        }

        double &pos_x() { return this->at(0); }
        double &pos_y() { return this->at(1); }
        double &vel_x() { return this->at(2); }
        double &vel_y() { return this->at(3); }

        double log_likelihood(std::size_t iter)
        {
            const double scale = 10;
            const double nu = 10;

            double llh_x =
                scale * (pos_x() - this->particle().state().obs_x_[iter]);
            double llh_y =
                scale * (pos_y() - this->particle().state().obs_y_[iter]);

            llh_x = std::log(1 + llh_x * llh_x / nu);
            llh_y = std::log(1 + llh_y * llh_y / nu);

            return -0.5 * (nu + 1) * (llh_x + llh_y);
        }
    }; // class particle_index_type

    PFCV(std::size_t N) : PFCVBase<Layout>(N)
    {
        double x = 0;
        double y = 0;
        std::ifstream data("pf_cv.data");
        while (data >> x >> y) {
            obs_x_.push_back(x);
            obs_y_.push_back(y);
        }
        data.close();
    }

    std::size_t n() const { return obs_x_.size(); }

    private:
    mckl::Vector<double> obs_x_;
    mckl::Vector<double> obs_y_;
}; // class PFCV

template <typename Backend, mckl::MatrixLayout Layout, typename RNGSetType>
class PFCVInit : public mckl::SamplerEvalSMP<PFCV<Layout, RNGSetType>,
                     PFCVInit<Backend, Layout, RNGSetType>, Backend>
{
    public:
    using T = PFCV<Layout, RNGSetType>;

    void eval_each(std::size_t, mckl::ParticleIndex<T> idx)
    {
        const double sd_pos0 = 2;
        const double sd_vel0 = 1;
        mckl::NormalDistribution<double> normal_pos(0, sd_pos0);
        mckl::NormalDistribution<double> normal_vel(0, sd_vel0);

        auto &rng = idx.rng();
        idx.pos_x() = normal_pos(rng);
        idx.pos_y() = normal_pos(rng);
        idx.vel_x() = normal_vel(rng);
        idx.vel_y() = normal_vel(rng);
    }
}; // PFCVInit

template <typename Backend, mckl::MatrixLayout Layout, typename RNGSetType>
class PFCVMove : public mckl::SamplerEvalSMP<PFCV<Layout, RNGSetType>,
                     PFCVMove<Backend, Layout, RNGSetType>, Backend>
{
    public:
    using T = PFCV<Layout, RNGSetType>;

    void eval_each(std::size_t, mckl::ParticleIndex<T> idx)
    {
        const double sd_pos = std::sqrt(0.02);
        const double sd_vel = std::sqrt(0.001);
        const double delta = 0.1;
        mckl::NormalDistribution<double> normal_pos(0, sd_pos);
        mckl::NormalDistribution<double> normal_vel(0, sd_vel);

        auto &rng = idx.rng();
        idx.pos_x() += normal_pos(rng) + delta * idx.vel_x();
        idx.pos_y() += normal_pos(rng) + delta * idx.vel_y();
        idx.vel_x() += normal_vel(rng);
        idx.vel_y() += normal_vel(rng);
    }
}; // class PFCVMove

template <typename Backend, typename RNGSetType>
class PFCVMove<Backend, mckl::ColMajor, RNGSetType>
    : public mckl::SamplerEvalSMP<PFCV<mckl::ColMajor, RNGSetType>,
          PFCVMove<Backend, mckl::ColMajor, RNGSetType>, Backend>
{
    public:
    using T = PFCV<mckl::ColMajor, RNGSetType>;

    void operator()(std::size_t iter, mckl::Particle<T> &particle)
    {
        this->run(iter, particle, 512);
    }

    void eval_range(std::size_t, const mckl::ParticleRange<T> &range)
    {
        const double sd_pos = std::sqrt(0.02);
        const double sd_vel = std::sqrt(0.001);
        const double delta = 0.1;
        mckl::NormalDistribution<double> normal_pos(0, sd_pos);
        mckl::NormalDistribution<double> normal_vel(0, sd_vel);

        double *const pos_x =
            range.particle().state().col_data(0) + range.first();
        double *const pos_y =
            range.particle().state().col_data(1) + range.first();
        double *const vel_x =
            range.particle().state().col_data(2) + range.first();
        double *const vel_y =
            range.particle().state().col_data(3) + range.first();
        double *const w = w_.data() + range.first();
        double *const v = v_.data() + range.first();

        const std::size_t n = range.size();
        auto &rng = range.begin().rng();
        normal_pos(rng, n, w);
        normal_pos(rng, n, v);
        mckl::add(n, w, pos_x, pos_x);
        mckl::add(n, v, pos_y, pos_y);
        mckl::fma(n, delta, vel_x, pos_x, pos_x);
        mckl::fma(n, delta, vel_y, pos_y, pos_y);
        normal_vel(rng, n, w);
        normal_vel(rng, n, v);
        mckl::add(n, w, vel_x, vel_x);
        mckl::add(n, v, vel_y, vel_y);
    }

    void eval_first(std::size_t, mckl::Particle<T> &particle)
    {
        w_.resize(particle.size());
        v_.resize(particle.size());
    }

    private:
    mckl::Vector<double> w_;
    mckl::Vector<double> v_;
}; // class PFCVMove

template <typename Backend, mckl::MatrixLayout Layout, typename RNGSetType>
class PFCVWeight : public mckl::SamplerEvalSMP<PFCV<Layout, RNGSetType>,
                       PFCVWeight<Backend, Layout, RNGSetType>, Backend>
{
    public:
    using T = PFCV<Layout, RNGSetType>;

    void eval_each(std::size_t iter, mckl::ParticleIndex<T> idx)
    {
        w_[idx.i()] = idx.log_likelihood(iter);
    }

    void eval_first(std::size_t, mckl::Particle<T> &particle)
    {
        w_.resize(particle.size());
    }

    void eval_last(std::size_t, mckl::Particle<T> &particle)
    {
        particle.weight().add_log(w_.data());
    }

    private:
    mckl::Vector<double> w_;
}; // class PFCVWeight

template <typename Backend, mckl::MatrixLayout Layout, typename RNGSetType>
class PFCVEval : public mckl::MonitorEvalSMP<PFCV<Layout, RNGSetType>,
                     PFCVEval<Backend, Layout, RNGSetType>, Backend>
{
    public:
    using T = PFCV<Layout, RNGSetType>;

    void eval_each(
        std::size_t, std::size_t, mckl::ParticleIndex<T> idx, double *res)
    {
        res[0] = idx.pos_x();
        res[1] = idx.pos_y();
    }
}; // class PFCVEval

template <typename Backend, mckl::ResampleScheme Scheme,
    mckl::MatrixLayout Layout, typename RNGSetType>
inline void pf_cv_run(std::size_t N, int nwid, int twid)
{
    using T = PFCV<Layout, RNGSetType>;

    mckl::Seed::instance().set(101);
    mckl::Sampler<T> sampler(N);
    sampler.resample_method(Scheme, 0.5);
    sampler.eval(PFCVInit<Backend, Layout, RNGSetType>(), mckl::SamplerInit);
    sampler.eval(PFCVMove<Backend, Layout, RNGSetType>(), mckl::SamplerMove);
    sampler.eval(PFCVWeight<Backend, Layout, RNGSetType>(),
        mckl::SamplerInit | mckl::SamplerMove);
    sampler.monitor(
        "pos", mckl::Monitor<T>(2, PFCVEval<Backend, Layout, RNGSetType>()));
    sampler.monitor("pos").name(0) = "pos.x";
    sampler.monitor("pos").name(1) = "pos.y";
    sampler.initialize();

    const std::size_t n = sampler.particle().state().n();
    const std::string smp(pf_backend_name<Backend>());
    const std::string res(pf_scheme_name<Scheme>());
    const std::string rc(pf_layout_name<Layout>());
    const std::string rs(pf_rng_set_name<RNGSetType>());

    std::string basename = "pf_cv." + smp + "." + res + "." + rc + "." + rs;

#if MCKL_HAS_HDF5
    std::string h5file = basename + ".h5";
    mckl::hdf5store(h5file);
    mckl::hdf5store(h5file, "Particle", true);
    mckl::hdf5store(sampler.particle(), h5file, "Particle/Iter.0", true);
#endif

    mckl::StopWatch watch;
    for (std::size_t i = 1; i < n; ++i) {
        watch.start();
        sampler.iterate();
        watch.stop();
#if MCKL_HAS_HDF5
        mckl::hdf5store(sampler.particle(), h5file,
            "Particle/Iter." + std::to_string(i), true);
#endif
    }

#if MCKL_HAS_HDF5
    mckl::hdf5store(sampler, h5file, "Sampler", true);
    mckl::hdf5store(sampler.monitor("pos"), h5file, "Monitor", true);
#endif
    std::ofstream txt(basename + ".txt");
    txt << sampler << std::endl;
    txt.close();

    mckl::Vector<double> rx(n);
    mckl::Vector<double> ry(n);
    sampler.monitor("pos").read_record(0, rx.data());
    sampler.monitor("pos").read_record(1, ry.data());

    mckl::Vector<double> tx;
    mckl::Vector<double> ty;
    double x = 0;
    double y = 0;
    std::ifstream truth("pf_cv.truth");
    while (truth >> x >> y) {
        tx.push_back(x);
        ty.push_back(y);
    }
    mckl::sub(n, tx.data(), rx.data(), rx.data());
    mckl::sub(n, ty.data(), ry.data(), ry.data());
    mckl::sqr(n, rx.data(), rx.data());
    mckl::sqr(n, ry.data(), ry.data());
    mckl::add(n, rx.data(), ry.data(), rx.data());
    mckl::sqrt(n, rx.data(), rx.data());
    double error = std::accumulate(rx.begin(), rx.end(), 0.0) / n;
    double time = watch.seconds();

    std::cout << std::setw(nwid) << std::left << N;
    std::cout << std::setw(twid) << std::left << smp;
    std::cout << std::setw(twid + 5) << std::left << res;
    std::cout << std::setw(twid) << std::left << rc;
    std::cout << std::setw(twid) << std::left << rs;
    std::cout << std::setw(twid) << std::right << std::fixed << error;
    std::cout << std::setw(twid) << std::right << std::fixed << time;
    std::cout << std::endl;
}

template <typename Backend, mckl::ResampleScheme Scheme,
    mckl::MatrixLayout Layout>
inline void pf_cv_run(std::size_t N, int nwid, int twid)
{
    pf_cv_run<Backend, Scheme, Layout, mckl::RNGSetVector<>>(N, nwid, twid);
#if MCKL_HAS_TBB
    pf_cv_run<Backend, Scheme, Layout, mckl::RNGSetTBB<>>(N, nwid, twid);
#endif
}

template <typename Backend, mckl::ResampleScheme Scheme>
inline void pf_cv_run(std::size_t N, int nwid, int twid)
{
    pf_cv_run<Backend, Scheme, mckl::RowMajor>(N, nwid, twid);
    pf_cv_run<Backend, Scheme, mckl::ColMajor>(N, nwid, twid);
}

template <typename Backend>
inline void pf_cv_run(std::size_t N, int nwid, int twid)
{
    pf_cv_run<Backend, mckl::Multinomial>(N, nwid, twid);
    pf_cv_run<Backend, mckl::Residual>(N, nwid, twid);
    pf_cv_run<Backend, mckl::ResidualStratified>(N, nwid, twid);
    pf_cv_run<Backend, mckl::ResidualSystematic>(N, nwid, twid);
    pf_cv_run<Backend, mckl::Stratified>(N, nwid, twid);
    pf_cv_run<Backend, mckl::Systematic>(N, nwid, twid);
}

inline void pf_cv_run(std::size_t N, int nwid, int twid)
{
    pf_cv_run<mckl::BackendSEQ>(N, nwid, twid);
    pf_cv_run<mckl::BackendSTD>(N, nwid, twid);
#if MCKL_HAS_OMP
    pf_cv_run<mckl::BackendOMP>(N, nwid, twid);
#endif
#if MCKL_HAS_TBB
    pf_cv_run<mckl::BackendTBB>(N, nwid, twid);
#endif
}

inline void pf_cv(std::size_t N)
{
    const int nwid = 10;
    const int twid = 15;
    const std::size_t lwid = nwid + twid * 6 + 5;

    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << std::setw(nwid) << std::left << "N";
    std::cout << std::setw(twid) << std::left << "Backend";
    std::cout << std::setw(twid + 5) << std::left << "ResampleScheme";
    std::cout << std::setw(twid) << std::left << "MatrixLayout";
    std::cout << std::setw(twid) << std::left << "rng_set_type";
    std::cout << std::setw(twid) << std::right << "Error";
    std::cout << std::setw(twid) << std::right << "Time (s)";
    std::cout << std::endl;
    std::cout << std::string(lwid, '-') << std::endl;
    pf_cv_run(N, nwid, twid);
    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_PFCV_HPP
