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

#ifndef INCLUDED_BB60C_SOURCE_IMPL_H
#define INCLUDED_BB60C_SOURCE_IMPL_H

#include <bb60c/source.h>
#include <bb60c/bb_api.h>

namespace gr {
    namespace bb60c {

        class source_impl : public source
        {
        private:
            int d_handle;

            double d_center;
            double d_ref;
            double d_atten;
            int d_gain;
            int d_decimation;
            double d_bandwidth;
            bool d_purge;
            bool d_bnc;
            unsigned int d_port1;
            unsigned int d_port2;

            gr::thread::mutex d_mutex;
            bool d_param_changed;

            float *d_buffer;
            bool d_len;

        public:
            source_impl(double center,
                        double ref,
                        double atten,
                        int gain,
                        int decimation,
                        double bandwidth,
                        bool purge,
                        bool bnc,
                        int port1,
                        int port2);
            ~source_impl();

            void set_center(double center);
            void set_ref(double ref);
            void set_decimation(int decimation);
            void set_bandwidth(double bandwidth);
            void set_purge(bool purge);

            void configure();

            int work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items);
        };

    } // namespace bb60c
} // namespace gr

#endif /* INCLUDED_BB60C_SOURCE_IMPL_H */

