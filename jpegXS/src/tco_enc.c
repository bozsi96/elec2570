/*************************************************************************
** Copyright (c) 2015-2016 intoPIX SA & Fraunhofer IIS                  **
** Copyright 2017 Canon Inc.                                            **
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
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <time.h>

#include "xs_markers.h"
#include "tco_conf.h"
#include "precinct.h"
#include "image_transforms.h"
#include "dwt53.h"
#include "bitpacking.h"
#include "gcli.h"
#include "sig_mag.h"
#include "budget.h"
#include "packing.h"
#include "lvls_map.h"
#include "rate_control.h"

#define BSR(x) (31 - __builtin_clz(x))
#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#define MIN(a,b) (((a)<(b)) ? (a) : (b))

struct tco_enc_context_t
{
	int ncomps;
	int w;
	int h;
	int depth;
	tco_conf_t conf;
	lvls_map_t map; 
	precinct_t  *precinct;

	dwt_data_t* dwt53_tmp_data;

	bit_packer_t* bitstream;
	int bitstream_len;

	rate_control_t* rc;
};

//*** Context initialization ***
// - conf contains configuration parameters (cfr tco.h)
// - image_in is the image structure with the input image
tco_enc_context_t* tco_enc_init(tco_conf_t* conf, image_t* image)
{
	tco_enc_context_t* ctx;
	 
	if (tco_conf_validate(conf, image, conf)) 												// Check validity of configuration parameters
		return NULL;
	 
	ctx = (tco_enc_context_t*) malloc(sizeof(tco_enc_context_t));			// Allocate space for context structure

	if (ctx)
	{
		memset(ctx, 0, sizeof(tco_enc_context_t));											// Initialize to zero
		 
		ctx->ncomps = image->ncomps;																		// Number of components in image (1 for black and white, more for colours)
		ctx->w = image->w;																							// Image width
		ctx->h = image->h;																							// Image height
		ctx->depth = image->depth;																			// Bit depth, 8 bits as input format is in bytes

		fill_lvls_map(conf, image, &ctx->map);													// Create map of DWT decomposition levels 


		ctx->rc = rate_control_open(conf, &(ctx->map), image->h);		// Initialize rate allocation structure

		 
		ctx->conf = *conf;																							// Set configuration

		ctx->dwt53_tmp_data = (dwt_data_t*) malloc( MAX(ctx->w, ctx->h) * sizeof(dwt_data_t));	// Allocate data buffer used for DWT
		 
		ctx->precinct = precinct_open_column(&(ctx->map));									// Initialize precinct structure

		ctx->bitstream = bitpacker_open();															// Initialize output bitstream structure
		ctx->bitstream_len = 0;																					// Output length
	}
	return ctx;
}

//*** Image data transformation ***
// - ctx contains global variables (as defined above)
// - image_in is the image structure with the input image
// Note: output is written back in image_in structure
int tco_enc_transforms( tco_enc_context_t* ctx, image_t* image_in )
{
	tco_conf_t* conf = &(ctx->conf);			// Configuration parameters

	image_shift(image_in, image_in, (conf->in_depth - ctx->depth), 0); 		// Increase data range
	 
	image_offset(image_in, image_in, -(1<<(conf->in_depth-1)));					// Center data range around zero

	// Perform DWT in both directions (vertical and horizontal) for each vertical decomposition
	int vlvl;
	for(vlvl = 0;vlvl < conf->ndecomp_v;vlvl++)
	{
		tco_image_apply_partial_dwt_ver(image_in, image_in, ctx->dwt53_tmp_data,vlvl);
		
		tco_image_apply_partial_dwt_hor(image_in, image_in, 1, ctx->dwt53_tmp_data, vlvl);
	}

	// Perform additional horizontal DWT
	if (ctx->map.n_lvls_h[0] - 1 > vlvl)
	{
		tco_image_apply_partial_dwt_hor(image_in, image_in, ctx->map.n_lvls_h[0] - 1 - vlvl, ctx->dwt53_tmp_data, vlvl);
	}

	image_shift(image_in, image_in,-ctx->conf.quant, 0);									// Reduce data range (initial global quantization)

	return 0;
}

//*** Main TICO encoding function ***
// - ctx contains global variables (as defined above)
// - image_in is the image structure with the input image
// - codestream_buf is the output buffer for the bitstream
// - codestream_buf_byte_size is the allocated size of the buffer ( = maximum length of the bitstream)
// - codestream_byte_size is the actual output length of the bitstream
int tco_enc_image( tco_enc_context_t* ctx, image_t* image_in, void* codestream_buf, int codestream_buf_byte_size, int* codestream_byte_size)
{
	tco_conf_t* conf = &(ctx->conf); 		// Configuration parameters
	rc_results_t rc_results;						// Rate control output structure (cfr. rate_control.h)
	int header_len = 0;									// Length of bitstream header
	int slice_idx = 0;									// Current slice index
	int rate_control_nbytes;						// Number of bytes to be filled by rate control
	
	bitpacker_set_buffer(ctx->bitstream , codestream_buf,codestream_buf_byte_size);		// Initialize bitstream packer (structure used to append data to bitstream)
	 
	header_len = xs_write_head(ctx->bitstream, image_in, conf);												// Write header to bitstream
	 
	// Number of bytes that can be used by the rate control (= total number of bytes - header bytes - final marker bytes - slices header bytes)
	rate_control_nbytes = conf->bitstream_nbytes - (header_len/8) - XS_MARKER_NBYTES - (XS_SLICE_HEADER_NBYTES * ((image_in->h+conf->slice_height-1)/conf->slice_height));

	rate_control_init(ctx->rc, rate_control_nbytes, conf->budget_report_nbytes);			// Initialize rate control
	
	tco_enc_transforms(ctx, image_in);																								// Perform data transforms (mainly DWT)
	
	// Compress data using entropy coding
	for (int line_idx=0; line_idx < image_in->h; line_idx+=(1 << (ctx->map.n_lvls_v-1))) 					// 2 lines/precinct for 1 vertical decomposition
	{
		fill_precinct(ctx->precinct,image_in, line_idx >> (ctx->map.n_lvls_v - 1));		// Fill data in precinct structure (with sign/magnitude conversion)
    
		update_gclis(ctx->precinct);			// Search Greatest Coded Line Index (GCLI)

		rate_control(ctx->rc, &rc_results, ctx->precinct);		// Perform rate allocation ( = find best GTLI)

		if (precinct_is_first_of_slice(ctx->precinct, conf->slice_height)) 								// Write slice header to bitstream at the beginning of each slice
			{
			xs_write_slice_header(ctx->bitstream, slice_idx++);
		}
    
		pack_precinct(ctx->bitstream, ctx->precinct, &rc_results); 						// Compress precinct and write to bitstream

	}

	xs_write_tail(ctx->bitstream);																														// Write end of the bitstream

	 
	*codestream_byte_size = ((bitpacker_get_len(ctx->bitstream) + 7) >> 3);										// Compute final bitstream size
	// Correct endianness of last bytes
	bitpacker_flush(ctx->bitstream);
	
	return 0;
}
