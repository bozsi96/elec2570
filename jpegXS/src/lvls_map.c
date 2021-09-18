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



#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "lvls_map.h"
#include "dwt53.h"

#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#define MIN(a,b) (((a)<(b)) ? (a) : (b))
 
int map_get_N_filtertypes(int ndecomp_h, int ndecomp_v)
{
	
	return 2*MIN(ndecomp_v, ndecomp_h) + MAX(ndecomp_h, ndecomp_v) + 1;
}
 
int map_get_NL(int ncomps, int ndecomp_h, int ndecomp_v)
{
	return ncomps * map_get_N_filtertypes(ndecomp_h, ndecomp_v);
}


 
int band_to_level(const lvls_map_t *map,int res_level,int band_type)
{
	int one_band_levels =  map->n_lvls_h[0] - map->n_lvls_v;
	 
	if (res_level <= one_band_levels)
	{
		 
		/* assert(!(band_type == LL_Band && res_level)); */ {}
		/* assert(!(res_level == 0 && band_type != LL_Band)); */ {}

		return res_level;
	}
	else
	{
		 
		/* assert(band_type > LL_Band); */ {}
		/* assert(band_type <= HH_Band); */ {}

		return one_band_levels + 1 +(res_level - one_band_levels - 1) * 3 + band_type - HL_Band;
	}
}

 
int height_of_band(const lvls_map_t *map,int reslvl)
{
	int one_band_levels =  map->n_lvls_h[0] - map->n_lvls_v;

	if (reslvl <= one_band_levels)
	{
		return 1;
	}
	else
	{
		return 1 << (reslvl - one_band_levels - 1);
	}
}

 
band_t first_band_of_map(const lvls_map_t *map,int reslvl)
{
	if (reslvl == 0)
		return LL_Band;
	else
		return HL_Band;
}

 
band_t last_band_of_map(const lvls_map_t *map,int reslvl)
{
	int one_band_levels =  map->n_lvls_h[0] - map->n_lvls_v;

	if (reslvl == 0)
	{
		return LL_Band;
	}
	else if (reslvl <= one_band_levels)
	{
		return HL_Band;
	}
	else
		return HH_Band;
}

 
int resolutions_of_map(const lvls_map_t *map)
{
	return map->n_lvls_h[0];
}

 
static int to_hlvl(const lvls_map_t* map,int res,int bandtype)
{
	switch(bandtype)
	{
	case LL_Band:
	case LH_Band:
		return 0;
	case HL_Band:
	case HH_Band:
		return res;
	}
	/* assert(!"invalid band type"); */ {}
	return 0;
}

int num_cols(const lvls_map_t *map)
{
	return map->num_cols;
}
 
int width_of_level(const lvls_map_t *map,int lvl)
{
	int hlvl  = map->h_from_lvl[lvl];
	int width = 0;

	if (hlvl == 0)
	{
		 
		int _lvl;
		for (_lvl = 0; _lvl < map->h_from_lvl[lvl+1]; _lvl++)
			width += map->band_widths[_lvl];
	}
	else
	{
		width = map->band_widths[hlvl];
	}
	return width;
}

static int to_vlvl(const lvls_map_t *map,int res,int bandtype)
{
	int one_band_levels =  map->n_lvls_h[0] - map->n_lvls_v;

	switch(bandtype)
	{
	case LL_Band:
	case HL_Band:
		return 0;
	case LH_Band:
	case HH_Band:
		return res - one_band_levels;
	}
	/* assert(!"invalid band type"); */ {}
	return 0;
}

int fill_lvls_map(tco_conf_t* conf, image_t* im, lvls_map_t* map)
{
	int subpkt = 0;
	int idx = 0, lvl = 0;
	int v_lvl;
	 

	map->n_lvls_v = conf->ndecomp_v + 1;
	map->level_count = 0;
	map->position_count = 0;
	for (v_lvl=0; v_lvl<map->n_lvls_v;v_lvl++)
	{
		map->n_lvls_h[v_lvl] = ((v_lvl==0) || (conf->ndecomp_vh == 0)) ? conf->ndecomp_h + 1 : conf->ndecomp_vh + 1;
		map->level_count += map->n_lvls_h[v_lvl];
		if (v_lvl==0)
			map->position_count = conf->ndecomp_h + 1;
		else
			map->position_count = (map->position_count - 1) + 3 * (1<<(v_lvl-1));
	}

	map->col_sz = conf->col_sz;
	map->num_cols = 1;

	for(int res = 0; res < map->n_lvls_h[0]; res++)
	{
		int first = first_band_of_map(map,res);
		int last  = last_band_of_map(map,res);
		int band;
		for(band = first; band <= last; band++)
		{
			int lvl = band_to_level(map,res,band);
			map->h_from_lvl[lvl]     = to_hlvl(map,res,band);
			map->v_from_lvl[lvl]     = to_vlvl(map,res,band);
		}
	}
	
	int res;
	int lvl_height;
	int res_lvl0;
	int subpkt0_lvls;
	 
	idx = 0;
	res = 0;
	subpkt = 0;
	subpkt0_lvls = conf->ndecomp_h - conf->ndecomp_v + 1;
	for (lvl=0; lvl < subpkt0_lvls; lvl++)
	{
		map->lvls_order_map[idx] = lvl;
		map->positions_subpkt[idx] = subpkt;
		idx++;
		res++;
	}
		
			 
	subpkt = 1;
	lvl_height = 1;
	for (res_lvl0 = subpkt0_lvls; res_lvl0 < map->level_count; res_lvl0 += 3)  
	{
		for (lvl = res_lvl0; lvl < res_lvl0+3; lvl++)  
		{
			
			map->lvls_order_map[idx] = lvl;
			map->positions_subpkt[idx] = subpkt;
			idx++;
			 
			subpkt++;
		}
				 
		res++;
		lvl_height = lvl_height * 2;
	}
 
	memset(map->band_heights,0,sizeof(map->band_heights));
	memset(map->band_widths ,0,sizeof(map->band_widths));

	if (map->n_lvls_v > 1)
		dwt53_get_lvl_sizes(im->h,map->n_lvls_v - 1   ,map->band_heights);
	dwt53_get_lvl_sizes(im->w,map->n_lvls_h[0] - 1,map->band_widths);

	 
	for (lvl = 0; lvl <map->n_lvls_h[0]; lvl++)
		if (map->band_widths[lvl] > map->lvls_size_max)
			map->lvls_size_max = map->band_widths[lvl];

	map->n_subpkt = subpkt;

	return 0;
}

tco_data_in_t* lvlmap_get_ptr_by_column(const lvls_map_t* map, const image_t* image, int lvl, int prec_idx, int prec_height)
{
	tco_data_in_t* ptr = NULL;
	int v_lvl, h_lvl;
	int xpos = 0;
	int ypos = 0;

	v_lvl = map->v_from_lvl[lvl];
	h_lvl = map->h_from_lvl[lvl];

	for (int i = 0; i < v_lvl; i++)
		ypos += map->band_heights[i];

	ypos += prec_idx * prec_height;


	if (ypos >= image->h)
		return NULL;


	ptr = image->array_ptr;
	ptr += ypos * image->w;

	for (int i = 0; i < h_lvl; i++)
		xpos += map->band_widths[i];



	return ptr + xpos;
}
