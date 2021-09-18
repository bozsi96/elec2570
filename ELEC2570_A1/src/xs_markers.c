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
#include <stdlib.h>

#include "lvls_map.h"
#include "tco_conf.h"
#include "bitpacking.h"
#include "xs_markers.h"



int xs_write_head(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf)
{
	int nbits = 0;

	nbits += bitpacker_write(bitstream, XS_MARKER_SOC, XS_MARKER_NBITS);
	nbits += xs_write_capabilities_marker(bitstream, im, conf);
	
	nbits += xs_write_picture_header(bitstream, im, conf);
	nbits += xs_write_component_table(bitstream, im, conf);
	nbits += xs_write_weights_table(bitstream, im, conf);
	nbits += xs_write_extension_marker(bitstream, im, conf);
	return nbits;
}


int xs_write_tail(bit_packer_t* bitstream)
{
	return bitpacker_write(bitstream, XS_MARKER_EOC, XS_MARKER_NBITS);
}


int xs_write_capabilities_marker(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf)
{
	int nbits = 0;
	int lcap;
	nbits += bitpacker_write(bitstream, XS_MARKER_CAP, XS_MARKER_NBITS);
	lcap = 2;
	nbits += bitpacker_write(bitstream, lcap, XS_MARKER_NBITS);
	return nbits;
}


int xs_write_picture_header(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf)
{
	int nbits=0;
	int lpih=26;
	int precinct_height=(1<<(conf->ndecomp_v));

	nbits += bitpacker_write(bitstream, XS_MARKER_PIH, XS_MARKER_NBITS);
	nbits += bitpacker_write(bitstream, lpih, XS_MARKER_NBITS);
	nbits += bitpacker_write(bitstream, conf->bitstream_nbytes, 32);
	nbits += bitpacker_write(bitstream,   0, 16);
	nbits += bitpacker_write(bitstream,   0, 16);
	nbits += bitpacker_write(bitstream, im->w, 16);
	nbits += bitpacker_write(bitstream, im->h, 16);
	 
	nbits += bitpacker_write(bitstream, 0, 16);
	nbits += bitpacker_write(bitstream, (conf->slice_height)/(precinct_height), 16);
	nbits += bitpacker_write(bitstream, im->ncomps, 8);  
	nbits += bitpacker_write(bitstream, conf->group_size, 8);  
	nbits += bitpacker_write(bitstream, 8, 8);  
	nbits += bitpacker_write(bitstream, conf->in_depth, 8);  
	nbits += bitpacker_write(bitstream, conf->quant, 4);  
	nbits += bitpacker_write(bitstream, 4, 4); 
	nbits += bitpacker_write(bitstream, 0, 1); 
	nbits += bitpacker_write(bitstream, 0, 3); 
	nbits += bitpacker_write(bitstream, 0, 4); 
	nbits += bitpacker_write(bitstream, conf->ndecomp_h, 4); 
	nbits += bitpacker_write(bitstream, conf->ndecomp_v, 4); 
	nbits += bitpacker_write(bitstream, 0, 4); 
	nbits += bitpacker_write(bitstream, 1, 2); 
	nbits += bitpacker_write(bitstream, 0, 2); 
	return nbits;
}


int xs_write_component_table(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf)
{
	int nbits=0;
	int lcdt= 2 * im->ncomps + 2;
	int comp;

	nbits += bitpacker_write(bitstream, XS_MARKER_CDT, XS_MARKER_NBITS);
	nbits += bitpacker_write(bitstream, lcdt, XS_MARKER_NBITS);

	for (comp = 0; comp < im->ncomps; comp++)
	{
		nbits += bitpacker_write(bitstream, im->depth, 8);
		 
		if (im->w != im->w)
			nbits += bitpacker_write(bitstream, 2, 4);
		else
			nbits += bitpacker_write(bitstream, 1, 4);
		
		 
		nbits += bitpacker_write(bitstream, 1, 4);
	}
	return nbits;
}

int xs_write_weights_table(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf)
{
	int nbits = 0;
	int lvl, comp;
	int Nl = map_get_NL(im->ncomps, conf->ndecomp_h, conf->ndecomp_v);
	int Nb = map_get_N_filtertypes(conf->ndecomp_h, conf->ndecomp_v);
	int idx = 0;

	nbits += bitpacker_write(bitstream, XS_MARKER_WGT, XS_MARKER_NBITS);
	nbits += bitpacker_write(bitstream, 2*Nl + 2, XS_MARKER_NBITS);
	for (lvl = 0; lvl < Nb; lvl++)
	{
		for (comp = 0; comp < im->ncomps; comp++)
		{
			idx = Nb*comp + lvl;  
			nbits += bitpacker_write(bitstream, conf->lvl_gains[idx], 8);
			nbits += bitpacker_write(bitstream, conf->lvl_priorities[idx], 8);
		
		}
	}
	return nbits;
}

int xs_write_extension_marker(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf)
{
	 
	return 0;
}

int xs_write_slice_header(bit_packer_t* bitstream, int slice_idx)
{
	int nbits = 0;
	nbits += bitpacker_write(bitstream, XS_MARKER_SLH, XS_MARKER_NBITS);
	nbits += bitpacker_write(bitstream, 4, XS_MARKER_NBITS);
	nbits += bitpacker_write(bitstream, slice_idx, 16);
	/* assert(nbits == XS_SLICE_HEADER_NBYTES*8); */ {}
	return nbits;
}
