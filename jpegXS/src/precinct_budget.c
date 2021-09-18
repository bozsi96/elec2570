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

#include "packing.h"
#include "precinct_budget.h"

#include <stdio.h>

#define ALIGN_TO_BITS(value, nbits) ((value + nbits - 1) & (~(nbits - 1)))

int init_budget_info(precinct_budget_info_t *pbi, int n_subpkts)
{
	int subpkt;
	 
	for (subpkt = 0; subpkt < n_subpkts; subpkt++)
	{
		pbi->subpkt_size_gcli[subpkt] = 0;
		pbi->subpkt_size_data[subpkt] = 0;
		pbi->subpkt_size_sign[subpkt] = 0;
	}
	return 0;
}


 
int precinct_get_budget(	precinct_t* precinct,
								precinct_budget_table_t* pbt,
								int* gtli_table,
								precinct_budget_info_t *out)
{
	int position;
	int subpkt = 0;

	init_budget_info(out, precinct_nb_subpkts_of(precinct));

	 
	for (position = 0; position < line_count_of(precinct); position++)
	{
		subpkt = precinct_subpkt_of(precinct, position);
		out->subpkt_size_gcli[subpkt] += pbt_get_gcli_bgt_of(pbt)[position];
		out->subpkt_size_data[subpkt] += pbt_get_data_bgt_of(pbt)[position];
		out->subpkt_size_sign[subpkt] += pbt_get_sign_bgt_of(pbt)[position];

	}

	 
	for (subpkt = 0; subpkt < precinct_nb_subpkts_of(precinct); subpkt++)
	{
		out->subpkt_alignbits_gcli[subpkt] = ALIGN_TO_BITS(out->subpkt_size_gcli[subpkt], 8) - out->subpkt_size_gcli[subpkt];
		out->subpkt_alignbits_data[subpkt] = ALIGN_TO_BITS(out->subpkt_size_data[subpkt], 8) - out->subpkt_size_data[subpkt];
		out->subpkt_alignbits_sign[subpkt] = ALIGN_TO_BITS(out->subpkt_size_sign[subpkt], 8) - out->subpkt_size_sign[subpkt];

		out->subpkt_size_gcli[subpkt] += out->subpkt_alignbits_gcli[subpkt];
		out->subpkt_size_data[subpkt] += out->subpkt_alignbits_data[subpkt];
		out->subpkt_size_sign[subpkt] += out->subpkt_alignbits_sign[subpkt];
	}

	 
	out->prec_header_size = ALIGN_TO_BITS(PREC_HDR_PREC_SIZE + PREC_HDR_SCENARIO_SIZE + PREC_HDR_REFINEMENT_SIZE + bands_count_of(precinct)*2, PREC_HDR_ALIGNMENT);
	out->precinct_bits = out->prec_header_size;
	for (subpkt = 0; subpkt < precinct_nb_subpkts_of(precinct); subpkt++)
	{
		out->pkt_header_size[subpkt] = PKT_HDR_DATA_SIZE_SHORT + PKT_HDR_GCLI_SIZE_SHORT;
		out->pkt_header_size[subpkt]+=PKT_HDR_SIGN_SIZE_SHORT;
		
		out->pkt_header_size[subpkt] = ALIGN_TO_BITS(out->pkt_header_size[subpkt], PKT_HDR_ALIGNMENT);
		out->precinct_bits += out->pkt_header_size[subpkt];

		 
		out->precinct_bits += out->subpkt_size_gcli[subpkt];
		out->precinct_bits += out->subpkt_size_data[subpkt];
		out->precinct_bits += out->subpkt_size_sign[subpkt];
	}


	return out->precinct_bits;
}


precinct_budget_table_t* pbt_open(int position_count)
{
	precinct_budget_table_t* pbt = malloc(sizeof(precinct_budget_table_t));
	if (pbt)
	{
		pbt->position_count = position_count;
	}
	return pbt;
}

void pbt_close(precinct_budget_table_t* pbt)
{
	if (pbt)
	{
		free(pbt);
	}
}


