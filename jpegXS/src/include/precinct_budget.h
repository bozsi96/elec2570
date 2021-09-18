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


#ifndef PRECINCT_BUDGET_H
#define PRECINCT_BUDGET_H

#include <stdint.h>
#include "precinct.h"

 
typedef struct precinct_budget_info_t
{
	 
	int precinct_bits;

	 
	 
	uint32_t prec_header_size;

	 
	uint32_t pkt_header_size[MAX_BANDLINES];  

	 
	uint32_t subpkt_size_gcli[MAX_SUBPKTS];
	uint32_t subpkt_size_data[MAX_SUBPKTS];
	uint32_t subpkt_size_sign[MAX_SUBPKTS];

	uint32_t subpkt_alignbits_gcli[MAX_SUBPKTS];
	uint32_t subpkt_alignbits_data[MAX_SUBPKTS];
	uint32_t subpkt_alignbits_sign[MAX_SUBPKTS];

} precinct_budget_info_t;


struct precinct_budget_table_t {
	uint32_t gcli_budget_table[MAX_LVLS];
	uint32_t data_budget_table[MAX_LVLS];
	uint32_t sign_budget_table[MAX_LVLS];
	int position_count;
};

 
typedef struct precinct_budget_table_t precinct_budget_table_t;

precinct_budget_table_t* pbt_open(int position_count);


static INLINE uint32_t* pbt_get_gcli_bgt_of(precinct_budget_table_t* pbt)
{
	return pbt->gcli_budget_table;
}

static INLINE uint32_t* pbt_get_data_bgt_of(precinct_budget_table_t* pbt)
{
	return pbt->data_budget_table;
}

static INLINE uint32_t* pbt_get_sign_bgt_of(precinct_budget_table_t* pbt)
{
	return pbt->sign_budget_table;
}

static INLINE int pbt_get_position_count(precinct_budget_table_t* pbt)
{
	return pbt->position_count;
}

void pbt_close(precinct_budget_table_t* pbt);

 
int precinct_get_budget(precinct_t* precinct,
						precinct_budget_table_t* pbt,
						int* gtli_table,
						precinct_budget_info_t *result);


#endif
