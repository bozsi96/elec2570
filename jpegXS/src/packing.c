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



#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <inttypes.h>

#include "budget.h"
#include "packing.h"
#include "lvls_map.h"
#include "sb_weighting.h"
#include "tco.h"

#define MAX(a,b) (((a)>(b)) ? (a) : (b))

//*** Data packer ***
// - bitstream is the structure with the output bitstream
// - buf is the input buffer with sign values
// - buf_len is the length of the input buffer
// - gclis is the buffer of GCLI values
// - gtli is the GTLI of the precinct
int pack_data(bit_packer_t* bitstream, sig_mag_data_t* buf, int buf_len, gcli_data_t* gclis, int gtli)
{
	int i=0,idx=0,group=0,bp=0,bit_len=0;
	int start_bp = 0;

	for (group = 0; group<((buf_len+3)>>2); group++)
	{
		if (gclis[group] > gtli)
		{
			start_bp = gclis[group]-1;
			// Write bit between GTLI and GCLI
			for (bp=start_bp; bp>=gtli; bp--) {
				for (i=0; i<4; i++) {
					bit_len += bitpacker_write(bitstream,(buf[idx+i]>>bp),1);
				}
			}
		}
		idx+=4;
	}
	return bit_len;
}

//*** Sign packer ***
// - bitstream is the structure with the output bitstream
// - buf is the input buffer with sign values
// - buf_len is the length of the input buffer
// - gclis is the buffer of GCLI values
// - gtli is the GTLI of the precinct
int pack_sign(bit_packer_t* bitstream, sig_mag_data_t* buf, int buf_len, gcli_data_t* gclis, int gtli)
{
	int idx=0,group=0,bit_len=0;
	for (idx = 0; idx < buf_len; idx++)
	{
		group = idx >>2;
		// Write sign (1 bit) if data is different from zero
		if ((gclis[group] > gtli) && ((buf[idx] & ~SIGN_BIT_MASK) >> gtli))
			bit_len += bitpacker_write(bitstream, (buf[idx]>>SIGN_BIT_POSITION),1);
	}
	return bit_len;
}

//*** GLCI packer ***
// - ctx is the structure containing configuration information
// - bitstream is the structure with the output bitstream
// - precinct is the precinct to be packed
// - ra_result is the structure with the result of the rate allocation
// - position is the position of the precinct
int pack_gclis(bit_packer_t* bitstream, precinct_t* precinct, rc_results_t* ra_result, int position)
{
	int gtli = ra_result->gtli_table_gcli[position];	
	int values_len;
	gcli_data_t *values_to_code; 	 

	values_to_code = precinct_gcli_of(precinct,position);
	values_len = precinct_gcli_width_of(precinct,position);
	
  int n=1,bit_len = 0;
	int i;
  for (i=0; (i < values_len) && (n>0); i++)
	{
    gcli_data_t residual = MAX(0, values_to_code[i] - gtli);
		n = bitpacker_write_unary_unsigned(bitstream, residual);
		bit_len += n;
	}
	if (n <= 0)
		return 1;

	
	return 0;
}

//*** Precinct packer ***
// - bitstream is the structure with the output bitstream
// - precinct is the precinct to be packed
// - ra_result is the structure with the result of the rate allocation
int pack_precinct(bit_packer_t* bitstream, precinct_t* precinct, rc_results_t* ra_result)
{
	int len_before = bitpacker_get_len(bitstream);
	int lvl, idx_start, idx_stop, idx, gtli;
	int subpkt;
	int position;
	int header_len;


	// Pack precinct header
	
	// Compute header size
	header_len = PREC_HDR_PREC_SIZE + PREC_HDR_SCENARIO_SIZE + PREC_HDR_REFINEMENT_SIZE;
	header_len += (2 * bands_count_of(precinct));
	header_len = (header_len + 7) & ~0x7;
	 
	// Write precinct size
	bitpacker_write(bitstream, ((ra_result->precinct_total_bits-header_len)>>3), PREC_HDR_PREC_SIZE);
	// Write scenario
	bitpacker_write(bitstream, ra_result->scenario, PREC_HDR_SCENARIO_SIZE);
	// Write refinement
	bitpacker_write(bitstream, ra_result->refinement, PREC_HDR_REFINEMENT_SIZE);

	// Write methods
	for (position = 0; position < line_count_of(precinct); position++)
	{
		lvl = precinct_lvl_of(precinct, position);
		bitpacker_write(bitstream, ra_result->gcli_method, 2);						// Method is fixed
	}
	bitpacker_align(bitstream);

	// Write GCLI, sign and data for each line
	subpkt=0;
	for (idx_start=idx_stop=0; idx_stop < line_count_of(precinct); idx_stop++)
	{
		if ((idx_stop != (line_count_of(precinct) - 1)) &&
			 (precinct_subpkt_of(precinct, idx_stop) ==
			  precinct_subpkt_of(precinct, idx_stop+1)))
			continue;

		// Write length
		bitpacker_write(bitstream, 0, 1);  
		bitpacker_write(bitstream, ra_result->pbinfo.subpkt_size_data[subpkt] >> 3,PKT_HDR_DATA_SIZE_SHORT);
		bitpacker_write(bitstream, ra_result->pbinfo.subpkt_size_gcli[subpkt] >> 3,PKT_HDR_GCLI_SIZE_SHORT);
		bitpacker_write(bitstream, ra_result->pbinfo.subpkt_size_sign[subpkt] >> 3,PKT_HDR_SIGN_SIZE_SHORT);
		bitpacker_align(bitstream);
		
		// Pack GCLI
		for (idx=idx_start; idx<=idx_stop; idx++)
		{
			pack_gclis(bitstream, precinct, ra_result, idx);
		}
		bitpacker_align(bitstream);
		
		// Pack data
		for (idx=idx_start; idx<=idx_stop; idx++)
		{
			lvl = precinct_lvl_of(precinct, idx);
			gtli = ra_result->gtli_table_data[lvl];
			pack_data(bitstream,
						  precinct_line_of(precinct,lvl),
						  (int)precinct_width_of(precinct,lvl),
						  precinct_gcli_of(precinct,lvl),gtli);
		}
		bitpacker_align(bitstream);
		
		// Pack sign
		for (idx=idx_start; idx<=idx_stop; idx++)
		{
			lvl = precinct_lvl_of(precinct, idx);
			gtli = ra_result->gtli_table_data[lvl];

			pack_sign(bitstream,
							precinct_line_of(precinct,lvl),
							(int)precinct_width_of(precinct,lvl),
							precinct_gcli_of(precinct,lvl),gtli);
		}
		bitpacker_align(bitstream);  
		
		 
		idx_start = idx_stop + 1;
		subpkt++;
	}

	// Add zero padding
	if (ra_result->pbinfo.precinct_bits < ra_result->precinct_total_bits)
	{
		bitpacker_add_padding(bitstream,ra_result->precinct_total_bits - ra_result->pbinfo.precinct_bits);
	}
	return 0;
}

