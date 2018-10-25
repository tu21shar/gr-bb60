/* -*- c++ -*- */
/*
 * Copyright 2018 <tu21sharma@gmail.com>.
 * @author 2018 by Tushar Sharma <tu21sharma@gmail.com>
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
            int handle;

            double d_center;
            double d_ref;
            double d_atten;
            int d_gain;
            int d_decimation;
            double d_bandwidth;
            bool d_useBNC;
            unsigned int d_port1;
            unsigned int d_port2;

        public:
            source_impl(double center,
                        double ref,
                        double atten,
                        int gain,
                        int decimation,
                        double bandwidth,
                        std::string useBNC,
                        int port1,
                        int port2);
            ~source_impl();

            int work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items);
        };

    } // namespace bb60c
} // namespace gr

#endif /* INCLUDED_BB60C_SOURCE_IMPL_H */

