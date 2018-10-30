/* -*- c++ -*- */
/*
 * Copyright (C) 2018 Signal Hound, Inc. <support@signalhound.com>
 *
 * Adapted from code by Tushar Sharma <tu21sharma@gmail.com>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef INCLUDED_BB60C_SOURCE_H
#define INCLUDED_BB60C_SOURCE_H

#include <bb60c/api.h>
#include <gnuradio/sync_block.h>
#include <iostream>
#include <math.h>
#include <string.h>

namespace gr {
  namespace bb60c {

    /*!
     * This block acquires IQ data from the Signal Hound BB60C spectrum analyzer.
     *
     *
     */
    class BB60C_API source : virtual public gr::sync_block
    {
    public:
      typedef boost::shared_ptr<source> sptr;

      static sptr make(double center,
                       double ref,
                       double atten,
                       int gain,
                       int decimation,
                       double bandwidth,
                       bool purge,
                       bool bnc,
                       int port1,
                       int port2);

      virtual void set_center(double center) = 0;
      virtual void set_ref(double ref) = 0;
      virtual void set_decimation(int decimation) = 0;
      virtual void set_bandwidth(double bandwidth) = 0;
      virtual void set_purge(bool purge) = 0;
    };

  } // namespace bb60c
} // namespace gr

#endif /* INCLUDED_BB60C_SOURCE_H */

