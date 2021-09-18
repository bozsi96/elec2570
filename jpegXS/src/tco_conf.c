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
#include <string.h>

#include <math.h>
#include <stddef.h>
#include "tco_conf.h"
#include "gcli.h"  
#include "tco.h"
#include "lvls_map.h"

#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#define MIN(a,b) (((a)<(b)) ? (a) : (b))

typedef struct default_weights_t {
	int kernel_v;
	int kernel_h;
	int ndecomp_v;
	int ndecomp_h;
	int opt;
	int rct;
	uint8_t	gains[1][MAX_FILTER_TYPES];
	uint8_t priorities[1][MAX_FILTER_TYPES];
} default_weights_t;

default_weights_t default_weights=
{
VER_KERNEL_53, HOR_KERNEL_53,  1,  2, 0, 0,
{{1,0,0,0,0}},
{{11,0,8,5,13}}
};

int tco_conf_validate(tco_conf_t* conf_in, image_t* image, tco_conf_t* conf)
{
	int coded_depth;
	tco_conf_t dummy;

	if (!conf)
		conf = &dummy;

	 
	*conf = *conf_in;

	if (conf->bitstream_nbytes == -1) 
	{
		return -1;
	}

	conf->bpp = (conf->bitstream_nbytes * 8) / (image->w*image->h);
	
	

	 
	if (conf->in_depth == 0)
	{
		 
		conf->in_depth = image->depth;
	}

	coded_depth = conf->in_depth - conf->quant;
	if (coded_depth > TCO_MAX_DEPTH)
	{
		if (conf->quant == 0)
		{
			conf->quant = coded_depth - TCO_MAX_DEPTH;
		}
		else
		{
			return -1;
		}
	}
	 
	if ((conf->gains_choice == GAINS_CHOICE_PSNR) ||
		(conf->gains_choice == GAINS_CHOICE_VISUAL))
	{
		int lvl;
		int nlvls = map_get_NL(image->ncomps, conf->ndecomp_h, conf->ndecomp_v);
		int nlvls_per_comp = map_get_N_filtertypes(conf->ndecomp_h, conf->ndecomp_v);
		struct default_weights_t* def = &default_weights;
		if (def)
		{
			for (lvl = 0; lvl < nlvls; lvl++)
			{
				int comp = lvl / nlvls_per_comp;
				int filtertype = lvl % nlvls_per_comp;
				conf->lvl_gains[lvl] = def->gains[comp][filtertype];
				conf->lvl_priorities[lvl] = def->priorities[comp][filtertype];
			}
		}
		else
		{
			return -1;
		}
	}
	
	return 0;
}
