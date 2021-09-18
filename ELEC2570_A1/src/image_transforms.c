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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "image_transforms.h"
#include "dwt53.h"

 

//*** Image precision increase/decrease ***
// Data bits are shifted left or right
// - image_in is the image structure with the input image
// - image_out is the image structure with the output image
// - shift is the number of bits to be shifted (negative for right shift)
// - rounding is used for rounding correction
int image_shift(image_t* image_in, image_t* image_out, int shift, int rounding)
{
	int i; 
	int len = image_in->w * image_in->h;
	int16_t* data_in = image_in->array_ptr;
	int16_t* data_out = image_out->array_ptr;
	
	// Shift to the left
	if (shift > 0) 
	{ 
			
		for (i=0; i<len; i++) 
		{ 
			data_out[i] = data_in[i] << shift; 
			if (data_out[i] && rounding) 
				data_out[i] |= (1<<(shift-1)); 
		} 
	} 
	// Shift to the right
	else if (shift < 0)
	{ 
			
		for (i=0; i<len; i++) 
		{ 
			if (rounding) 
				data_out[i] =  (data_in[i] + (1 << (-shift-1))) >> (-shift); 
			else 
				data_out[i] = data_in[i] >> (-shift); 
		} 
	} 
	// No shift
	else   
	{ 
		for (i=0; i<len; i++) 
			data_out[i] = data_in[i]; 
	} 
	return 0;
}

//*** Image DC offset ***
// - image_in is the image structure with the input image
// - image_out is the image structure with the output image
// - offset is the number added to each pixel
int image_offset(image_t* image_in, image_t* image_out, int offset)
{
	int i;
	int16_t* data_in = image_in->array_ptr;
	int len = image_in->w * image_in->h;
	int16_t* data_out = image_out->array_ptr;
	
	for (i=0; i<len; i++)
		*data_out++ = *data_in++ + offset;
	return 0;
}

//*** Image horizontal Discrete Wavelet Transform ***
// Performs the DWT in the horizontal direction on all or a part of an image
// - image_in is the image structure with the input image
// - image_out is the image structure with the output image, partly replaced by the DWT values
// - ndecomp is the number of decompositions to be performed --> set to 1
// - tmp_buffer is a temporary buffer to hold input data (at least the length of the input vector = width >> level)
// - level is the number of previously performed decompositions in both directions
int tco_image_apply_partial_dwt_hor(image_t* image_in, image_t* image_out, int ndecomp, dwt_data_t* tmp_buffer,
				    int level)
{
	int lvl_sizes[MAX_NDECOMP_H + 1];																		// Size of each decomposition level (dyadic scale in multi-decomposition)
	const int len       = image_in->w;																	// Total length, before multiple decompositions
	const int width     = (image_in->w + (1 << level) - 1) >> level;		// Width of the partial image on which DWT is applied
	const int height    = (image_in->h + (1 << level) - 1) >> level;		// Height of the partial image on which DWT is applied
	dwt_data_t* data_in = image_in->array_ptr;													// Pointer to the beginning of the vector 
	dwt_data_t* data_out[MAX_NDECOMP_H + 1];														// Pointer to each output decomposition level
	
	// Get sizes of each decomposition level (dyadic scale)
	memset(lvl_sizes , 0, MAX_NDECOMP_H * sizeof(int));
	dwt53_get_lvl_sizes(width, ndecomp, lvl_sizes);
	
	// Compute base pointers of each output level
	data_out[0] = image_out->array_ptr;
	for (int i=1; i<=MAX_NDECOMP_H; i++)
		data_out[i] = data_out[i-1] + lvl_sizes[i-1];

	// Perform DWT on each line (horizontal direction)
	for (int line = 0; line < height; line++)
	{
		// Performs the DWT --> only 1 decomposition
		dwt53_wrap(data_in, width, data_out, tmp_buffer, 1);

		// Copy the rest of the line
		if (width < len)
			memcpy(data_out[0] + width,data_in + width,(len - width) * sizeof(dwt_data_t));
		
		// Switch pointers to the next line
		data_in += len;
		for (int i=0; i<=ndecomp; i++)
			data_out[i] += len;
	}
	 
	// Copy remaining lines
	if (height < image_in->h)
		memcpy(data_out[0],data_in,(image_in->h - height) * len * sizeof(dwt_data_t));
	return 0;
}

//*** Image vertical Discrete Wavelet Transform ***
// Performs the DWT in the vertical direction on all or a part of an image
// - image_in is the image structure with the input image
// - image_out is the image structure with the output image, partly replaced by the DWT values
// - ndecomp is the number of decompositions to be performed --> set to 1
// - tmp_buffer is a temporary buffer to hold input data (at least the length of the input vector = height >> level)
// - level is the number of previously performed decompositions in both directions
int tco_image_apply_partial_dwt_ver(image_t* image_in, image_t* image_out, dwt_data_t* tmp_buffer,int level)
{
	int lvl_sizes[MAX_NDECOMP_V + 1];																		// Size of each decomposition level (dyadic scale in multi-decomposition)
	const int len       = image_in->w;																	// Total length, before multiple decompositions
	const int width     = (image_in->w + (1 << level) - 1) >> level;		// Width of the partial image on which DWT is applied
	const int height    = (image_in->h + (1 << level) - 1) >> level;		// Height of the partial image on which DWT is applied
	dwt_data_t* data_in = image_in->array_ptr;													// Pointer to the beginning of the vector 
	dwt_data_t* data_out[MAX_NDECOMP_V + 1];														// Pointer to each output decomposition level
	
	// Get sizes of each decomposition level (dyadic scale)
	memset(lvl_sizes , 0, MAX_NDECOMP_V * sizeof(int));
	dwt53_get_lvl_sizes(height, 1, lvl_sizes);
	
	// Compute base pointers of each output level
	data_out[0] = image_out->array_ptr;
	for (int i=1; i<=MAX_NDECOMP_V; i++)
		data_out[i] = data_out[i-1] + lvl_sizes[i-1]*len;

	// Perform DWT on each column (vertical direction)
	for (int col = 0; col < width; col++)
	{
		// Performs the DWT --> only 1 decomposition
		dwt53_wrap(data_in, height, data_out, tmp_buffer, len);

		// Switch pointers to the next line
		data_in += 1;
		for (int i=0; i<=1; i++)
			data_out[i] += 1;
	}
	 
	// Copy remaining columns
	if (width < image_in->w){
		for(int i = 0; i<len; i++){
			memcpy(data_out[0],data_in,(image_in->w - width) * sizeof(dwt_data_t));
			data_out[0] += image_in->w;
		}
	}
	
	// Copy remaining lines
	if (height < image_in->h){
		data_out[0] -= width;
		memcpy(data_out[0],data_in,(image_in->h - height) * len * sizeof(dwt_data_t));
	}
	return 0;
}
