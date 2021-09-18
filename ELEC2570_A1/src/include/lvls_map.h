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


#ifndef LVLS_MAP_H
#define LVLS_MAP_H

 

#include "tco.h"

 
 
#define MAX_LVLS_PER_COMP ((MAX_NDECOMP_V+1) * (MAX_NDECOMP_H+1))
#define MAX_LVLS (MAX_LVLS_PER_COMP)



 
#define MAX_BANDLINES (MAX_LVLS + 3 * ((1 << (MAX_NDECOMP_V - 1)) - 1))


 
#define MAX_SUBPKTS MAX_BANDLINES


 
typedef enum {
	LL_Band = 0,  
	HL_Band = 1,  
	LH_Band = 2,
	HH_Band = 3
} band_t;

typedef struct lvls_map_t
{
	int n_comps;  
	int n_lvls_v;  
	int n_lvls_h[MAX_NDECOMP_V+1];

	int level_count;     
	int position_count;  
	 
	int lvls_order_map[MAX_BANDLINES];  

	 
	int positions_subpkt[MAX_BANDLINES]; 
	int n_subpkt;

	 
	int v_from_lvl[MAX_LVLS];
	int h_from_lvl[MAX_LVLS];
	 
	int band_widths[MAX_LVLS];
	int band_heights[MAX_LVLS];
	int lvls_size_max;  	

	 
	int col_sz;
	 
	int num_cols;
} lvls_map_t;


 
int fill_lvls_map(tco_conf_t* conf, image_t* im, lvls_map_t* map);
 
tco_data_in_t* lvlmap_get_ptr_by_column(const lvls_map_t* map, const image_t* image, int lvl, int prec_idx, int prec_height);

 
extern int band_to_level(const lvls_map_t *map,int res_level,int band_type);

 
extern band_t first_band_of_map(const lvls_map_t *map,int reslvl);

 
extern band_t last_band_of_map(const lvls_map_t *map,int reslvl);

 
extern int num_cols(const lvls_map_t *map);

 
extern int width_of_level(const lvls_map_t *map,int lvl);

 
int map_get_N_filtertypes(int ndecomp_h, int ndecomp_v);
 
int map_get_NL(int ncomps, int ndecomp_h, int ndecomp_v);



#endif
