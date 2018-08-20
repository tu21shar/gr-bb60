/* -*- c++ -*- */
/* 
 * Copyright 2018 <tu21sharma@gmail.com>.
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
    source::make(int gain,float fc,float bw,std::string mode,unsigned int DetectorType,unsigned int VerticalScale,float atten,float ref,float rbw,float vbw,float sweeptime,unsigned int rbwtype,unsigned int rejecttype,unsigned int window,unsigned int procunit,float framescale,int framerate,std::string UseBNC,int BNC1,int BNC2,int decim,float span)
    {
      return gnuradio::get_initial_sptr
        (new source_impl(gain,fc,bw,mode,DetectorType,VerticalScale,atten,ref,  rbw,  vbw,  sweeptime,  rbwtype, rejecttype,window,procunit, framescale,framerate,UseBNC, BNC1,BNC2,decim,span));
    }

    /*
     * The private constructor
     */
    source_impl::source_impl(int gain,float fc,float bw,std::string mode,unsigned int DetectorType,unsigned int VerticalScale,float atten,float ref,float rbw,float vbw,float sweeptime,unsigned int rbwtype,unsigned int rejecttype,unsigned int window,unsigned int procunit,float framescale,int framerate,std::string UseBNC,int BNC1,int BNC2,int decim, float span)
      : gr::sync_block("source",
              gr::io_signature::make(0,0,0),
	      gr::io_signature::make(1, 1, sizeof(gr_complex))),

	d_gain(gain),
	d_fc(fc),
	d_bw(bw),
	d_mode(mode),
	d_DetectorType(DetectorType),
	d_VerticalScale(VerticalScale),
	d_atten(atten),
	d_ref(ref),
	d_rbw(rbw),
	d_vbw(vbw),
	d_sweeptime(sweeptime),
	d_rbwtype(rbwtype),
	d_rejecttype(rejecttype),
	d_window(window),
	d_procunit(procunit),
	d_framescale(framescale),
	d_framerate(framerate),
	d_UseBNC(UseBNC),
	d_BNC1(BNC1),
	d_BNC2(BNC2),
	d_decim(decim),
	d_refer(0),
	d_no(0),
	d_span(span),
	d_consumed(0)

    {
        handle=-1;
	std::cout<<"\nAPI Version: "<<bbGetAPIVersion()<<"\n";
  	status = bbOpenDevice(&handle);
  	if (status==0) std::cout<<"Device opened\n";
  	else {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n"; abort();}

  	bbGetSerialNumber(handle, &sernum);
  	if (status==0) std::cout<<"SerialNum: "<<(int)sernum<<"\n";
  	else {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n"; abort();}

	
        if(d_mode=="Sweep")
	{
		status=bbConfigureAcquisition(handle,d_DetectorType, d_VerticalScale);  
		if(status==0) std::cout<<"Device Detector and Scale Configured\n";
		else  {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n"; abort();}

 	        status=bbConfigureCenterSpan(handle, d_fc, d_span);
		if(status==0) std::cout<<"Device freq and BW set successfully\n";
		else  {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n"; abort();}

		status=bbConfigureLevel(handle, d_ref, d_atten);
		if(status==0) std::cout<<"Device atten anf ref set successfully\n";
		else  {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n"; abort();}
 
 		status=bbConfigureGain(handle, d_gain);
		if (status==0) std::cout<<"Gain Set at "<<d_gain<<"\n";	
  		else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}


  		status=bbConfigureSweepCoupling(handle, d_rbw, d_vbw, d_sweeptime,d_rbwtype, d_rejecttype); 
		if ( (rbw != 10.10e6) && (rbw != 5.050e6) && (rbw != 2.525e6) && (rbw != 1.262e6) && (rbw != 631.2e3) 
    		    && (rbw != 315.6e3) && (rbw != 157.1e3) && (rbw != 78.90e3) && (rbw != 39.45e3) && (rbw != 19.72e3)  
		    && (rbw != 9.863e3) && (rbw != 4.931e3) && (rbw != 2.465e3) && (rbw != 1.232e3) && (rbw != 616.45) 
		    && (rbw != 308.22)  && (rbw != 154.11)  && (rbw != 77.05)   && (rbw != 38.52)   && (rbw != 19.26) 
		    && (rbw != 9.63)    && (rbw != 4.81)    && (rbw != 2.40)    && (rbw != 1.204)   && (rbw != 0.602)   && (rbw != 0.301))
  		{
  			double min_bin_sz = rbw / 3.2;
  			double min_fft = 80.0e6 / min_bin_sz;
  			int order = (int)ceil(log2(min_fft));
  			double fft_size= pow(2,order);
			std::cout<<"FFT Size: "<<fft_size<<"\n";
  		}	
	       if(status==0) std::cout<<"Device sweep settings successful\n";
  	       else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}
	
	       status=bbConfigureWindow(handle, 1);
  	       if(status==0) std::cout<<"Device Window set successfully\n";
  	       else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}

	       status=bbConfigureProcUnits(handle, 0); 
	       if(status==0) std::cout<<"Device Processing units set successfully\n";
  	       else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}

  	       status=bbInitiate(handle, BB_SWEEPING, 0);
	       if(status==0) std::cout<<"Device initiated in sweep mode\n";
  	       else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}

	       status= bbQueryTraceInfo(handle, &d_sweepSize, &d_binSize, &d_startFreq);
	       if(status==0) std::cout<<"Device sweep query successful\n";
  	       else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}


	       d_mins = new float[d_sweepSize];
	       d_maxs = new float[d_sweepSize];
	}

        if(d_mode=="RealTime")
	{
		status=bbConfigureAcquisition(handle,d_DetectorType, d_VerticalScale);  
		if(status==0) std::cout<<"Device Detector and Scale Configured\n";
		else  {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n"; abort();}

 	        status=bbConfigureCenterSpan(handle, d_fc, d_span);
		if(status==0) std::cout<<"Device freq and BW set successfully\n";
		else  {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n"; abort();}

		status=bbConfigureLevel(handle, d_ref, d_atten);
		if(status==0) std::cout<<"Device atten anf ref set successfully\n";
		else  {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n"; abort();}
 
 		status=bbConfigureGain(handle, d_gain);
		if (status==0) std::cout<<"Gain Set at "<<d_gain<<"\n";	
  		else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}


  		status=bbConfigureSweepCoupling(handle, d_rbw, d_vbw, d_sweeptime,d_rbwtype, d_rejecttype); 
		if ( (rbw != 10.10e6) && (rbw != 5.050e6) && (rbw != 2.525e6) && (rbw != 1.262e6) && (rbw != 631.2e3) 
    		    && (rbw != 315.6e3) && (rbw != 157.1e3) && (rbw != 78.90e3) && (rbw != 39.45e3) && (rbw != 19.72e3)  
		    && (rbw != 9.863e3) && (rbw != 4.931e3) && (rbw != 2.465e3) && (rbw != 1.232e3) && (rbw != 616.45) 
		    && (rbw != 308.22)  && (rbw != 154.11)  && (rbw != 77.05)   && (rbw != 38.52)   && (rbw != 19.26) 
		    && (rbw != 9.63)    && (rbw != 4.81)    && (rbw != 2.40)    && (rbw != 1.204)   && (rbw != 0.602)   && (rbw != 0.301))
  		{
  			double min_bin_sz = rbw / 3.2;
  			double min_fft = 80.0e6 / min_bin_sz;
  			int order = (int)ceil(log2(min_fft));
  			double fft_size= pow(2,order);
			std::cout<<"FFT Size: "<<fft_size<<"\n";
  		}	
	       if(status==0) std::cout<<"Device sweep settings successful\n";
  	       else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}
		
	       status = bbConfigureRealTime(handle, d_framescale, d_framerate);
	       if(status==0) std::cout<<"Device RT config set successfully\n";
  	       else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}

	       status=bbInitiate(handle, BB_REAL_TIME, 0);
	       if(status==0) std::cout<<"Device initiated in RT mode\n";
  	       else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}

	       status=bbQueryTraceInfo(handle, &d_sweepSize, &d_binSize, &d_startFreq);
	       if(status==0) std::cout<<"Device trace info queried \n";
  	       else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}

	       status=bbQueryRealTimeInfo(handle, &d_rtframewidth, &d_rtframeheight);
	       if(status==0) std::cout<<"Device  RT trace info queried \n";
  	       else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}

	       
               float *sweep = new float[d_sweepSize];
	       float *frame = new float[d_rtframewidth * d_rtframeheight];
		// Retrieve 1 real-time sweep/frame
	       bbFetchRealTimeFrame(handle, sweep, frame);
		// Finished/close device
	       bbCloseDevice(handle);
	       	

	}

        if(d_mode=="IQ")
	{

		


 	        status=bbConfigureCenterSpan(handle, d_fc, 20e6);
		if(status==0) std::cout<<"Device freq and BW set successfully\n";
		else  {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n"; abort();}

		status=bbConfigureLevel(handle, d_ref, d_atten);
		if(status==0) std::cout<<"Device atten and ref set successfully\n";
		else  {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n"; abort();}
 
 		status=bbConfigureGain(handle, d_gain);
		if (status==0) std::cout<<"Gain Set at "<<d_gain<<"\n";	
  		else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}

		if(d_UseBNC=="Yes")
		{		
  			status= bbConfigureIO(handle, d_BNC1, d_BNC2);
  			if(status==0) std::cout<<"Device IO set successfully\n";
	  		else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  abort();}
		}

		//bbConfigureIQ(int device, int downsampleFactor, double bandwidth);
  		status=bbConfigureIQ(handle, d_decim, d_bw);
		if (status==0) std::cout<<"Device config in IQ mode successfully\n";
  		else {std::cout<<"Error: "<<bbGetErrorString(status)<<"\n";  if (status<0) abort();}

		status=bbInitiate(handle, BB_STREAMING, BB_STREAM_IQ);
		if(status==0) std::cout<<"Device initiated in IQ mode successfully\n";
		else  {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n";  if (status<0) abort();}
		
		// Allocate an array large enough for the I/Q stream configured
		
		status=bbQueryStreamInfo(handle, 0, &d_c_bandwidth, &d_c_sampleRate);
		if(status==0) std::cout<<"Device stream info queried successfully\n";
		else  {std::cout<<"Error: "<< bbGetErrorString(status)<<"\n"; abort();}
		



	}

    }

    /*
     * Our virtual destructor.
     */
    source_impl::~source_impl()
    {
    }

    int
    source_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {

        float *o =(float*) output_items[0];	      
	
	if(d_mode=="Sweep")
	{

	       // Get one sweep
	       if(d_refer==0)
	       {	
	       		bbFetchTrace_32f(handle, d_sweepSize, d_mins, d_maxs);
	       }			
		
  	       for (int a=0;a<noutput_items;a++)
	       {
			if((a+d_no) > ((d_sweepSize)-1)) break;
			
			o[a]=(d_maxs[a+d_no]+d_mins[a+d_no])/2;
	       }
	       d_no=d_no+noutput_items;											
	       d_iq= (d_sweepSize)-d_no;	
	       if (d_iq>0) d_refer =1;
	       else d_refer=0;		
	       		


	}

	if (d_mode=="IQ")
	{	

		const int BLOCK_SIZE = (noutput_items/2);

    		float *buffer = new float[BLOCK_SIZE*2];

    		bbIQPacket pkt;
    		pkt.iqData = buffer;
    		pkt.iqCount = BLOCK_SIZE;
    		pkt.triggers = 0;
    		pkt.triggerCount = 0;

    		pkt.purge = false;



    		status = bbGetIQ(handle, &pkt);
		


		if (noutput_items <= pkt.dataRemaining) 
		{
			for (int i = 0; i < noutput_items; i++)
		        	o[i] =  buffer[i];
		} 
		else 
		{
			for (int i = 0; i < pkt.dataRemaining; i++)
		        	o[i] =  buffer[i];
		}

    		delete [] buffer;



	}

      	return noutput_items;
    }







  } /* namespace bb60c */
} /* namespace gr */

