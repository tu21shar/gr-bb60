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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "source_impl.h"
#include <stdlib.h>
#include <fstream>
#include <stdio.h>



namespace gr {
    namespace bb60c {

        source::sptr
        source::make(double center,
                     double ref,
                     double atten,
                     int gain,
                     int decimation,
                     double bandwidth,
                     std::string useBNC,
                     int port1,
                     int port2)
        {
            return gnuradio::get_initial_sptr(
                new source_impl(center, ref, atten, gain, decimation, bandwidth, useBNC, port1, port2)
            );
        }

        void ERROR_CHECK(bbStatus status) {
            if(status != bbNoError) {
                std::cout << "Error: " << bbGetErrorString(status) << "\n";
                abort();
            }
        }

        /*
         * The private constructor
         */
        source_impl::source_impl(double center,
                                 double ref,
                                 double atten,
                                 int gain,
                                 int decimation,
                                 double bandwidth,
                                 std::string useBNC,
                                 int port1,
                                 int port2) :
            gr::sync_block("source",
                           gr::io_signature::make(0,0,0),
                           gr::io_signature::make(1, 1, sizeof(gr_complex))),
            handle(-1),
            d_center(center),
            d_ref(ref),
            d_atten(atten),
            d_gain(gain),
            d_decimation(decimation),
            d_bandwidth(bandwidth),
            d_useBNC(useBNC == "Yes" ? true : false),
            d_port1(port1),
            d_port2(port2)
        {
            std::cout << "\nAPI Version: " << bbGetAPIVersion() << "\n";

            // Open device
            ERROR_CHECK(bbOpenDevice(&handle));

            unsigned int serial;
            ERROR_CHECK(bbGetSerialNumber(handle, &serial));
            std::cout << "Serial Number: "<< serial << "\n";

            // Configure
            ERROR_CHECK(bbConfigureCenterSpan(handle, d_center, 20e6)); // Span unused, set valid default
            ERROR_CHECK(bbConfigureLevel(handle, d_ref, d_atten));
            ERROR_CHECK(bbConfigureGain(handle, d_gain));
            if(d_useBNC) ERROR_CHECK(bbConfigureIO(handle, d_port1, d_port2));
            ERROR_CHECK(bbConfigureIQ(handle, d_decimation, d_bandwidth));

            // Initiate for IQ streaming
            ERROR_CHECK(bbInitiate(handle, BB_STREAMING, BB_STREAM_IQ));
            std::cout << "\nBB60C initiated for IQ streaming...\n";

            // Get IQ streaming info
            double actualBandwidth;
            int sampleRate;
            ERROR_CHECK(bbQueryStreamInfo(handle, 0, &actualBandwidth, &sampleRate));
            std::cout << "\nActual Bandwidth: "<< actualBandwidth << "\n";
            std::cout << "Sample Rate: "<< sampleRate << "\n";
        }

        /*
         * Our virtual destructor.
         */
        source_impl::~source_impl()
        {
            bbAbort(handle);
            bbCloseDevice(handle);
        }

        int source_impl::work(int noutput_items,
                              gr_vector_const_void_star &input_items,
                              gr_vector_void_star &output_items)
        {
            float *o = (float*)output_items[0];

            // Allocate memory
            const int BLOCK_SIZE = noutput_items / 2;
            float *buffer = new float[BLOCK_SIZE * 2];

            bbIQPacket pkt;
            pkt.iqData = buffer;
            pkt.iqCount = BLOCK_SIZE;
            pkt.triggers = 0;
            pkt.triggerCount = 0;
            pkt.purge = false;

            // Get IQ
            ERROR_CHECK(bbGetIQ(handle, &pkt));

            // Move data to output array
            for(int i = 0; i < std::min(pkt.dataRemaining, noutput_items); i++) {
                o[i] =  buffer[i];
            }

            delete [] buffer;

            return noutput_items;
        }

    } /* namespace bb60c */
} /* namespace gr */

