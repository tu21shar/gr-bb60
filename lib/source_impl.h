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
      // Nothing to declare in this block.

     public:
     int handle;
     unsigned int sernum;	
     bbStatus status;

     int d_gain;
     float d_fc;
     float d_bw;
     std::string d_mode;
     unsigned int d_DetectorType;	
     unsigned int d_VerticalScale;	
     float d_atten;
     float d_ref;
     float d_rbw,d_vbw,d_sweeptime;
     unsigned int d_rbwtype, d_rejecttype;	
     unsigned int d_window, d_procunit;	
     unsigned int d_sweepSize;
     double d_binSize, d_startFreq;
     float d_framescale;
     int d_framerate,d_rtframewidth,d_rtframeheight;
     std::string d_UseBNC;
     int d_BNC1,d_BNC2;			
     int d_decim;	
     int d_refer;
     float *iq_buffer;
     int d_iq,d_no;
     float *d_mins,*d_maxs;
     float d_span;
     double d_c_bandwidth;
     int d_c_sampleRate;
     int d_consumed;	
	
	

      source_impl(int gain,float fc,float bw,std::string mode,unsigned int DetectorType,unsigned int VerticalScale,float atten,float ref,float rbw,float vbw,float sweeptime,unsigned int rbwtype,unsigned int rejecttype,unsigned int window,unsigned int procunit,float framescale,int framerate,std::string UseBNC,int BNC1,int BNC2,int decim,float span);
      ~source_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace bb60c
} // namespace gr

#endif /* INCLUDED_BB60C_SOURCE_IMPL_H */

