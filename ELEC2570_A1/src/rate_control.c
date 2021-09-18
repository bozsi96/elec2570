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


#include "rate_control.h"
#include "precinct.h"
#include "budget.h"

#include "sb_weighting.h"
#include "xs_markers.h"

#include <stdio.h>

#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#define MIN(a,b) (((a)<(b)) ? (a) : (b))


struct rate_control_t
{
	precinct_budget_table_t* pbt;
	tco_conf_t conf;
	int image_height;
	 
	int nibbles_image;
	int nibbles_consumed;

	int *gtli_table;

	precinct_budget_info_t* pbinfo;
};


rate_control_t* rate_control_open( tco_conf_t* conf, lvls_map_t* map, int image_height)
{
	rate_control_t* rc = (rate_control_t*) malloc(sizeof(rate_control_t));
	if (rc)
	{
		rc->conf = *conf;
		rc->pbt = pbt_open(map->position_count);
		
		
		rc->gtli_table = (int*) malloc(map->level_count * sizeof(int));
		rc->pbinfo = (precinct_budget_info_t*) malloc(sizeof(precinct_budget_info_t));

		rc->image_height = image_height;

	}
	return rc;
}

void rate_control_init(rate_control_t* rc, int image_rate_bytes, int report_nbytes)
{
	if (rc)
	{
		rc->nibbles_consumed = 0;
		rc->nibbles_image = image_rate_bytes * 2;
	}
}
 
precinct_t* rate_control(rate_control_t* rc, rc_results_t* rc_results, precinct_t* precinct )
{
	int scenario, refinement, gcli_method;
	int padding_nibbles;
	int budget_minimum;
	int precinct_nibbles;

  // For the needs of the project, rate control has been largely simplified.
  // Rate allocation parameters are thus set manually:
	scenario=9;
	refinement=13;
  gcli_method=0;
    
  // Compute GTLI values
  compute_gtli_tables(scenario, 
						refinement, 
						bands_count_of(precinct), 
						rc->conf.lvl_gains, 
						rc->conf.lvl_priorities, 
						rc->gtli_table);
  
  // Compute budgets
  // GTLI
	fill_gcli_budget_table(precinct,0, rc->pbt, rc->gtli_table); // Compute resulting packed length
  
  // Data
	fill_data_budget_table(precinct,
						   rc->pbt,
               rc->gtli_table);

// Total
  precinct_get_budget( precinct,
						 rc->pbt,
						 rc->gtli_table,
						 rc->pbinfo);

	precinct_nibbles = (rc->pbinfo[0].precinct_bits)>>2;
  
	rc->nibbles_consumed += precinct_nibbles;
	 
  // Compute padding 
	padding_nibbles = 0;
	if (precinct_is_last_of_image(precinct, rc->image_height))
    padding_nibbles = rc->nibbles_image - rc->nibbles_consumed;  
	else if (rc->nibbles_consumed < budget_minimum)
    padding_nibbles = 0;
	rc->nibbles_consumed += padding_nibbles;
  
  // Store results
	rc_results->scenario = scenario;
	rc_results->refinement = refinement;
	rc_results->gcli_method = gcli_method;
	rc_results->gtli_table_data = rc->gtli_table;
	rc_results->gtli_table_gcli = rc->gtli_table;
	rc_results->pbinfo = rc->pbinfo[0];
	rc_results->padding_bits = (padding_nibbles << 2);
	rc_results->precinct_total_bits = rc_results->pbinfo.precinct_bits + rc_results->padding_bits;
	rc_results->rc_error = 0;

	return precinct;
}

