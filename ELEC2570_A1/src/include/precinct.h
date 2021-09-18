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

#ifndef SRC_PRECINCT_H
#define SRC_PRECINCT_H

#include <stdlib.h>
#include "lvls_map.h"
#include "gcli.h"
#include "tco.h"

#if defined(_WIN32) && !defined(__MINGW32__)
#define INLINE _inline
#else
#define INLINE inline
#endif

 

struct precinct_struct {
	 
	gcli_data_t* gcli_buf[MAX_LVLS]; 
  int gcli_size[MAX_LVLS];
	 
	sig_mag_data_t* sig_mag_data_buf[MAX_LVLS];
  int sig_mag_size[MAX_LVLS];

	 
	const lvls_map_t *geometry;
	 
	int group_size;
	 
	int idx;
};

typedef struct precinct_struct precinct_t;

 
extern precinct_t *precinct_open_column(const lvls_map_t *geometry);
extern precinct_t *precinct_open(const lvls_map_t *geometry,int group_size);

 
extern void precinct_close(precinct_t *prec);

 
extern void copy_gclis(precinct_t *dest,const precinct_t *src);

 
extern void fill_precinct(precinct_t *prec,const image_t *img,int prec_idx);


 
extern void update_image(const precinct_t *prec, image_t *target, int prec_idx);

 
extern int component_count_of(const precinct_t *prec);

 
extern band_t first_band_of(const precinct_t *prec,int res_lvl);

 
extern band_t last_band_of(const precinct_t *prec,int res_lvl);

 
extern int band_index_of(const precinct_t *prec,int component,int res_level,band_t band_type);

 
extern int bands_count_of(const precinct_t *prec);

 
extern int line_count_of(const precinct_t *prec);

 
extern sig_mag_data_t *precinct_line_of(const precinct_t *prec,int band_index);

 
extern size_t precinct_width_of(const precinct_t *prec,int band_index);

 
extern gcli_data_t *precinct_gcli_of(const precinct_t *prec,int band_index);
 
extern size_t precinct_gcli_width_of(const precinct_t *prec,int band_index);
extern size_t precinct_max_gcli_width_of(const precinct_t *prec);


extern int precinct_get_gcli_group_size(const precinct_t *prec);

 
extern void update_gclis(precinct_t *prec);




 
int precinct_idx_of(const precinct_t *prec);

 
int precinct_set_idx_of(precinct_t *prec, int idx);

 
void copy_data(precinct_t *dest, const precinct_t *src);

 
int precinct_copy(precinct_t *dest, const precinct_t *src);


 
int precinct_is_first_of_slice(const precinct_t *prec, int slice_height);

 
int precinct_is_last_of_slice(const precinct_t *prec, int slice_height, int im_height);

 
int precinct_is_last_of_image(const precinct_t *prec, int im_height);


 
int precinct_spacial_lines_of(const precinct_t *prec, int im_height);

int precinct_data_checksum_of(precinct_t* prec);

 
void dump_precinct_gclis(precinct_t* precinct, precinct_t* precinct_top, int* gtli, int* gtli_top);



 
static INLINE int precinct_lvl_of(const precinct_t* precinct, int position)
{
	return precinct->geometry->lvls_order_map[position];
}

static INLINE int precinct_subpkt_of(const precinct_t* precinct, int position)
{
	return precinct->geometry->positions_subpkt[position];
}

static INLINE int precinct_nb_subpkts_of(const precinct_t* precinct)
{
	return precinct->geometry->n_subpkt;
}

 
static INLINE int resolution_levels_of(const precinct_t *prec)
{
	return prec->geometry->n_lvls_h[0];
}

 
static INLINE int vertical_levels_of(const precinct_t *prec)
{
	return prec->geometry->n_lvls_v;
}

 
static INLINE int precinct_height_of(const precinct_t *prec,int band_index)
{
	int height = 1;

	return height;
}

#endif
