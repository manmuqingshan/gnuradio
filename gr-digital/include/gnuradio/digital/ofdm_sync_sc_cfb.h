/* -*- c++ -*- */
/*
 * Copyright 2013 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */


#ifndef INCLUDED_DIGITAL_OFDM_SYNC_SC_CFB_H
#define INCLUDED_DIGITAL_OFDM_SYNC_SC_CFB_H

#include <gnuradio/digital/api.h>
#include <gnuradio/hier_block2.h>

namespace gr {
namespace digital {

/*!
 * \brief Schmidl & Cox synchronisation for OFDM
 * \ingroup ofdm_blk
 * \ingroup synchronizers_blk
 *
 * \details
 * Input: complex samples.
 * Output 0: Fine frequency offset, scaled by the OFDM symbol duration.
 *           This is \f$\hat{\varphi}\f$ in [1]. The normalized frequency
 *           offset is then 2.0*output0/fft_len.
 * Output 1: Beginning of the first OFDM symbol after the first (doubled) OFDM
 *           symbol. The beginning is marked with a 1 (it's 0 everywhere else).
 *
 * The evaluation of the coarse frequency offset is *not* done in this block.
 * Also, the initial equalizer taps are not calculated here.
 *
 * Note that we use a different normalization factor in the timing metric than
 * the authors do in their original work[1]. If the timing metric (8) is
 * \f[
 * M(d) = \frac{|P(d)|^2}{(R(d))^2},
 * \f]
 * we calculate the normalization as
 * \f[
 * R(d) = \frac{1}{2} \sum_{k=0}^{N-1} |r_{k+d}|^2,
 * \f]
 * i.e., we estimate the energy from *both* half-symbols. This avoids spurious
 * detections at the end of a burst, when the energy level suddenly drops.
 *
 * [1] Schmidl, T.M. and Cox, D.C., "Robust frequency and timing
 * synchronization for OFDM", Communications, IEEE Transactions on, 1997.
 */
class DIGITAL_API ofdm_sync_sc_cfb : virtual public hier_block2
{
public:
    typedef std::shared_ptr<ofdm_sync_sc_cfb> sptr;

    /*!
     * \param fft_len FFT length
     * \param cp_len Length of the guard interval (cyclic prefix) in samples
     * \param use_even_carriers If true, this indicates that carriers in the
     *                          sync preamble are occupied such that the even
     *                          carriers are used (0, 2, 4, ...). If you use all
     *                          carriers, that would include the DC carrier,
     *                          so be careful. If false, it indicates the odd
     *                          carriers are used (1, 3, 5, ...).
     * \param threshold detection threshold. Default is 0.9.
     */
    static sptr
    make(int fft_len, int cp_len, bool use_even_carriers = false, float threshold = 0.9);

    virtual void set_threshold(float threshold) = 0;
    virtual float threshold() const = 0;
};

} // namespace digital
} // namespace gr

#endif /* INCLUDED_DIGITAL_OFDM_SYNC_SC_CFB_H */
