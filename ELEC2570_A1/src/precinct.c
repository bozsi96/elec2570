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

#include "precinct.h"
#include "sig_mag.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 
precinct_t *precinct_open_column(const lvls_map_t *geometry)
{
	precinct_t *prec = malloc(sizeof(precinct_t));
	if (prec)
	{
		prec->geometry        = geometry;
		prec->group_size      = 4;
		prec->idx = 0;
		int idx;
		for (idx = 0; idx < geometry->position_count; idx++)
		{
			int size;
			int size_gcli;
			int lvl   = geometry->lvls_order_map[idx];
    
			size = width_of_level(geometry, lvl);
    
			size_gcli = size>>2;
			
      prec->gcli_buf[idx] = (gcli_data_t*) malloc(size_gcli*sizeof(gcli_data_t));
      prec->gcli_size[idx] = size_gcli;
       
      prec->sig_mag_data_buf[idx] = (sig_mag_data_t*) malloc(size*sizeof(sig_mag_data_t));
      prec->sig_mag_size[idx] = size;
		}
    
		return prec;
		
	}
	return NULL;
}


void fill_precinct(precinct_t *prec,const image_t *image,int prec_idx)
{
	int lvl;
	const tco_data_in_t* ptr;
	int height = 1;
	 
	prec->idx = prec_idx;
	
	for (lvl = 0; lvl < prec->geometry->level_count; lvl++)
	{
		ptr = lvlmap_get_ptr_by_column(prec->geometry, image, lvl, prec_idx, height);
			
		sig_mag_data_t *dst = precinct_line_of(prec,lvl);
		int len      = (int)precinct_width_of(prec,lvl);
		to_sig_mag_buf(ptr,len,dst,len);
	}
}

int bands_count_of(const precinct_t *prec)
{
	return prec->geometry->level_count;
}

 
int line_count_of(const precinct_t *prec)
{
	return prec->geometry->position_count;
}

 
sig_mag_data_t *precinct_line_of(const precinct_t *prec,int band_index)
{
	
	return prec->sig_mag_data_buf[band_index];
}

 
size_t precinct_width_of(const precinct_t *prec,int band_index)
{	
	return prec->sig_mag_size[band_index];
}

 
gcli_data_t *precinct_gcli_of(const precinct_t *prec,int band_index)
{
	return prec->gcli_buf[band_index];
}

 
size_t precinct_gcli_width_of(const precinct_t *prec,int band_index)
{
	return prec->gcli_size[band_index];
}

 
void update_gclis(precinct_t *prec)
{
	int band;

	for(band = 0;band < bands_count_of(prec);band++)
	{
		const sig_mag_data_t *in = precinct_line_of(prec,band);
		gcli_data_t *dst         = precinct_gcli_of(prec,band);
		int reclen,width         = (int)precinct_width_of(prec,band);
		int gcli_count           = (int)precinct_gcli_width_of(prec,band);
		compute_gcli_buf(in,width,dst,gcli_count,&reclen);
	}
}

 
int precinct_is_first_of_slice(const precinct_t *prec, int slice_height)
{
	int precheight = 1 << (prec->geometry->n_lvls_v - 1);
	if (slice_height > 0)
	{
		return (((prec->idx*precheight) % slice_height) == 0);
	}
	else
	{
		return (prec->idx == 0);
	}
}

int precinct_is_last_of_image(const precinct_t *prec, int im_height)
{
	int ret;
	int precheight = 1 << (prec->geometry->n_lvls_v - 1);
	ret = ((im_height + precheight - 1) >> (prec->geometry->n_lvls_v - 1)) == (prec->idx+1);
	 
	return ret;
}
