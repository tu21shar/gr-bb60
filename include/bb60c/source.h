/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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
     * \brief <+description of block+>
     * \ingroup bb60c
     *
     */
    class BB60C_API source : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<source> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of bb60c::source.
       *
       * To avoid accidental use of raw pointers, bb60c::source's
       * constructor is in a private implementation
       * class. bb60c::source::make is the public interface for
       * creating new instances.
       */
      static sptr make(int gain,float fc,float bw,std::string mode,unsigned int DetectorType,unsigned int VerticalScale,float atten,float ref,float rbw,float vbw,float sweeptime,unsigned int rbwtype,unsigned int rejecttype,unsigned int window,unsigned int procunit, float framescale,int framerate,std::string UseBNC,int BNC1,int BNC2,int decim,float span);
    };

  } // namespace bb60c
} // namespace gr

#endif /* INCLUDED_BB60C_SOURCE_H */

