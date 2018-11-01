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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "source_impl.h"

namespace gr {
    namespace bb60c {

        source::sptr
        source::make(double center,
                     double ref,
                     double atten,
                     int gain,
                     int decimation,
                     double bandwidth,
                     bool purge,
                     bool bnc,
                     int port1,
                     int port2)
        {
            return gnuradio::get_initial_sptr(
                new source_impl(center, ref, atten, gain, decimation, bandwidth, purge, bnc, port1, port2)
            );
        }

        void ERROR_CHECK(bbStatus status) {
            if(status != bbNoError) {
                bool isWarning = status < bbNoError ? false : true;
                std::cout << "** " << (isWarning ? "Warning: " : "Error: ") << bbGetErrorString(status) << " **" << "\n";
                if(!isWarning) abort();
            }
        }

        source_impl::source_impl(double center,
                                 double ref,
                                 double atten,
                                 int gain,
                                 int decimation,
                                 double bandwidth,
                                 bool purge,
                                 bool bnc,
                                 int port1,
                                 int port2) :
            gr::sync_block("source",
                           gr::io_signature::make(0,0,0),
                           gr::io_signature::make(1, 1, sizeof(gr_complex))),
            d_handle(-1),
            d_center(center),
            d_ref(ref),
            d_atten(atten),
            d_gain(gain),
            d_decimation(decimation),
            d_bandwidth(bandwidth),
            d_purge(purge),
            d_bnc(bnc),
            d_port1(port1),
            d_port2(port2),
            d_param_changed(true),
            d_buffer(0),
            d_len(0)
        {
            std::cout << "\nAPI Version: " << bbGetAPIVersion() << "\n";

            // Open device
            ERROR_CHECK(bbOpenDevice(&d_handle));

            unsigned int serial;
            ERROR_CHECK(bbGetSerialNumber(d_handle, &serial));
            std::cout << "Serial Number: "<< serial << "\n";
        }

        source_impl::~source_impl()
        {
            bbAbort(d_handle);
            bbCloseDevice(d_handle);

            if(d_buffer) delete [] d_buffer;
        }

        void
        source_impl::set_center(double center) {
            gr::thread::scoped_lock lock(d_mutex);
            d_center = center;
            d_param_changed = true;
        }

        void
        source_impl::set_ref(double ref) {
            gr::thread::scoped_lock lock(d_mutex);
            d_ref = ref;
            d_param_changed = true;
        }

        void
        source_impl::set_decimation(int decimation) {
            gr::thread::scoped_lock lock(d_mutex);
            d_decimation = decimation;
            d_param_changed = true;
        }

        void
        source_impl::set_bandwidth(double bandwidth) {
            gr::thread::scoped_lock lock(d_mutex);
            d_bandwidth = bandwidth;
            d_param_changed = true;
        }

        void
        source_impl::set_purge(bool purge) {
            gr::thread::scoped_lock lock(d_mutex);
            d_purge = purge;
        }

        void
        source_impl::configure() {
            gr::thread::scoped_lock lock(d_mutex);

            // Configure
            ERROR_CHECK(bbConfigureCenterSpan(d_handle, d_center, 20e6)); // Span unused, set valid default
            ERROR_CHECK(bbConfigureLevel(d_handle, d_ref, d_atten));
            ERROR_CHECK(bbConfigureGain(d_handle, d_gain));
            if(d_bnc) ERROR_CHECK(bbConfigureIO(d_handle, d_port1, d_port2));
            ERROR_CHECK(bbConfigureIQ(d_handle, d_decimation, d_bandwidth));

            // Initiate for IQ streaming
            ERROR_CHECK(bbInitiate(d_handle, BB_STREAMING, BB_STREAM_IQ));

            // Get IQ streaming info
            double actualBandwidth;
            int sampleRate;
            ERROR_CHECK(bbQueryStreamInfo(d_handle, 0, &actualBandwidth, &sampleRate));
            std::cout << "\nActual Bandwidth: "<< actualBandwidth << "\n";
            std::cout << "Sample Rate: "<< sampleRate << "\n";
        }

        int
        source_impl::work(int noutput_items,
                          gr_vector_const_void_star &input_items,
                          gr_vector_void_star &output_items)
        {
            std::complex<float> *o = (std::complex<float> *)output_items[0];

            // Initiate new configuration if necessary
            if(d_param_changed) {
                configure();
                d_param_changed = false;
            }

            // Allocate memory if necessary
            if(!d_buffer || noutput_items != d_len) {
                if(d_buffer) delete [] d_buffer;
                d_buffer = new std::complex<float>[noutput_items];
                d_len = noutput_items;
            }

            bbIQPacket pkt;
            pkt.iqData = (float *)d_buffer;
            pkt.iqCount = noutput_items;
            pkt.triggers = 0;
            pkt.triggerCount = 0;
            pkt.purge = d_purge;

            // Get IQ
            ERROR_CHECK(bbGetIQ(d_handle, &pkt));

            // Move data to output array
            for(int i = 0; i < noutput_items; i++) {
                o[i] =  d_buffer[i];
            }

            return noutput_items;
        }

    } /* namespace bb60c */
} /* namespace gr */

