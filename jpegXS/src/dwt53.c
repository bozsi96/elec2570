/*************************************************************************
** Copyright (c) 2015-2016 intoPIX SA & Fraunhofer IIS                  **
** All rights reserved.                                                 **
**                                                                      **
** intoPIX SA & Fraunhofer IIS hereby grants to ISO/IEC JTC1 SC29 WG1   **
** (JPEG Committee) and each Member of ISO/IEC JTC1 SC29 WG1 (JPEG      **
** Committee) who participate in the Working Group dedicated to the     **
** standardization of JPEG-XS, a non-exclusive, nontransferable,        **
** worldwide, license under "intoPIX SA & Fraunhofer IIS" copyrights    **
** in this software to reproduce, distribute, display, perform and      **
** create derivative works for the sole and exclusive purposes of       **
** creating a test model in the frame of the JPEG-XS compression        **
** standard.                                                            **
**                                                                      **
** Modifications to this code shall be clearly indicated and            **
** identified by the relevant copyright notice(s) of the party          **
** generating these changes and/or derivative works.                    **
**                                                                      **
** Nothing contained in this software shall, except as herein           **
** expressly provided, be construed as conferring by implication,       **
** estoppel or otherwise, any license or right under (i) any existing   **
** or later issuing patent, whether or not the use of information in    **
** this software necessarily employs an invention of any existing or    **
** later issued patent, (ii) any copyright, (iii) any trademark, or     **
** (iv) any other intellectual property right.                          **
**                                                                      **
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND       **
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED        **
** TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A      **
** PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE             **
** COPYRIGHT OWNER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      **
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT     **
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF     **
** USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED      **
** AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT          **
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING       **
** IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF       **
** THE POSSIBILITY OF SUCH DAMAGE.                                      **
**************************************************************************/


#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include "dwt53.h"

//*** Get sizes of the decomposition levels ***
// Dyadic scale of the multi-level decomposition
// - w is the total length
// - ndecomp is the number of decomposition levels
// - lvl_sizes is the output vector of sizes
int dwt53_get_lvl_sizes(int w, int ndecomp, int* lvl_sizes)
{
	int h_lvl, hf_size, size;
	int lf_size = w;

	for (h_lvl = ndecomp; h_lvl >= 0; h_lvl--)
	{
		if (h_lvl > 0)
		{
			hf_size = (lf_size) >> 1;
			lf_size = (lf_size - hf_size);
			size = hf_size;
		}
		else
		{
			size = lf_size;
		}
		lvl_sizes[h_lvl] = size;
	}
	return 0;
}

//*** Discrete Wavelet Transform ***
// - data_in is the pointer to the input vector
// - len is the length of the input vector
// - lf_out is the pointer to the output low-pass component
// - lf_max_len is the maximum length of lf_out
// - lf_len is the output length of lf_out
// - hf_out is the pointer to the output high-pass component
// - hf_max_len is the maximum length of hf_out
// - hf_len is the output length of hf_out
// - inc is the memory increment between two elements of the output vectors (1 for horizontal, width for vertical)
int dwt53(dwt_data_t* data_in, int len, dwt_data_t* lf_out, int lf_max_len, int* lf_len, dwt_data_t* hf_out, int hf_max_len, int* hf_len, int inc)
{
	int i;
	*hf_len = len/2;
	*lf_len = (len+1)/2;
	
	// High pass filter coefficients
	//const float G[] = {-0.5, 1, -0.5};
	//const float Gend[] = {-1,1}; // End boundary
	const int8_t G[] = {-4, 8, -4};
	const int8_t Gend[] = {-8, 8};
	
	// Low pass filter coefficients
	//const float H[] = {-0.125, 0.25, 0.75, 0.25, -0.125};
	//const float H0[] = {0.75, 0.5, -0.25}; // Start boundary
	//const float Hend[] = {-0.125, 0.25, 0.625, 0.25}; // End boundary
	const int8_t H[] = {-1, 2, 6, 2, -1};
	const int8_t H0[] = {6, 4, -2};
	const int8_t Hend[] = {-1, 2, 5, 2};

	if ((*hf_len > hf_max_len) || (*lf_len > lf_max_len))
		return -1;

	// High pass
	for (i=1; i<len-1; i+=2)
		hf_out[(i/2)*inc] = (dwt_data_t) (G[0]*data_in[(i-1)] + G[1]* data_in[i] + G[2]*data_in[(i+1)]) / 8;
	hf_out[((len-1)/2)*inc] = (dwt_data_t) (Gend[0]*data_in[(len-2)] + Gend[1]* data_in[(len-1)]) / 8;
	
	// Low pass
	lf_out[0] = (dwt_data_t) ((H0[0]*data_in[0] + H0[1]* data_in[1] + H0[2]*data_in[2] + 4) / 8 );
	for (i=2; i<=len-4; i+=2){
		lf_out[(i/2)*inc] = (dwt_data_t) ((H[0]*data_in[(i-2)]+ H[1]*data_in[(i-1)]+H[2]*data_in[i] +H[3]*data_in[(i+1)]+H[4]*data_in[(i+2)] + 4) / 8);
	}
	lf_out[((len-2)/2)*inc] = (dwt_data_t) ((Hend[0]*data_in[(len-4)]+Hend[1]*data_in[(len-3)]+Hend[2]*data_in[(len-2)]+Hend[3]*data_in[(len-1)] + 4) / 8);	
	
	return 0;
}


//*** DWT wrapper ***
// Common wrapper for horizontal and vertical DWT
// - data_in is the pointer to the input vector
// - len is the length of the input vector
// - lvls_buffers are the pointers to the successive levels of decomposition
// - tmp_buffer is a temporary buffer to hold input data (at least the length of the input vector = height >> level)
// - inc is the memory increment between two elements of the input and output vectors (1 for horizontal, width for vertical)
int dwt53_wrap(dwt_data_t* data_in, int len, dwt_data_t** lvls_buffers, dwt_data_t* tmp_buffer, int inc)
{
	int hf_len, lf_len;													// Length of the high- and low-pass components of the output vector
	dwt_data_t* data_in_buf = tmp_buffer;				// Buffer with the input data
	
	// Fill in the input data
	if(inc==1){
		memcpy(data_in_buf,data_in,len * sizeof(dwt_data_t));
	}else{
		for(int i = 0; i<len; i++){
			data_in_buf[i] = data_in[i*inc];
		}
	}
	
	// DWT
	dwt53(data_in_buf,len,lvls_buffers[0],(len+1)/2,&lf_len,lvls_buffers[1],len/2, &hf_len, inc);

	return 0;
}



