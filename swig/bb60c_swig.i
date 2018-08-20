/* -*- c++ -*- */

#define BB60C_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "bb60c_swig_doc.i"

%{
#include "bb60c/source.h"
%}


%include "bb60c/source.h"
GR_SWIG_BLOCK_MAGIC2(bb60c, source);
