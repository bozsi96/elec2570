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

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "budget.h"
#include "gcli.h"
#include "sig_mag.h"

#include "bitpacking.h"


#define BSR(x) (31 - __builtin_clz(x))
#define GCLI(x) (((x) == 0) ? 0 : (BSR((x))+1))  
#define MAX(a,b) (((a)>(b)) ? (a) : (b))

int fill_data_budget_table(precinct_t* prec,
							precinct_budget_table_t* pbt,
							int* gtli_table)
{
	int position, gtli;
	for (position = 0; position < line_count_of(prec); position++)
	{
		int lvl = precinct_lvl_of(prec, position);
    gtli = gtli_table[lvl];
    
		budget_get_data_budget(precinct_gcli_of(prec,lvl),
						(int)precinct_gcli_width_of(prec,lvl),
						&(pbt_get_data_bgt_of(pbt)[position]),
						MAX_GCLI + 1, gtli);

		 
		budget_get_sign_budget(precinct_line_of(prec,lvl),
							   		(int)precinct_width_of(prec,lvl),
									&(pbt_get_sign_bgt_of(pbt)[position]),
									MAX_GCLI+1,gtli);
	}
	return 0;
}


int budget_get_data_budget(const gcli_data_t* gclis, int gclis_len, uint32_t* budget, int table_size, int gtli)
{
	int i;

	*budget=0;
	
	for (i=0; i<gclis_len; i++)
	{
		int n_bitplanes = (gclis[i] - gtli);

		if (n_bitplanes > 0)
      *budget += (n_bitplanes <<2);

	}
	return 0;
}

int budget_get_sign_budget(const sig_mag_data_t* datas_buf,	int data_len,
							uint32_t* budget, int table_size, int gtli)
{
	int i,quant_val;
	 
	*budget = 0;

	for (i=0; i < data_len; i++)
	{
		quant_val = (datas_buf[i] & ~SIGN_BIT_MASK) >> gtli; 
		if (quant_val != 0)
			*budget += 1;  
	}
	return 0;
}

uint32_t budget_getunary_unsigned(const gcli_data_t* pred_buf, int len)
{
	int i = 0, budget = 0;
	for (i=0; i<len; i++)
		budget += (pred_buf[i] + 1);
	return budget;
}


int fill_gcli_budget_table(const precinct_t* prec,
										int gcli_method, 
										precinct_budget_table_t* pbt,
                    int *gtli_table)
{
	int i,position, gtli;


	for (position = 0; position < line_count_of(prec); position++)
	{
    gtli = gtli_table[position];
		
    uint32_t bgt_gcli = 0;
		gcli_data_t* gcli_buffer = precinct_gcli_of(prec,position);
		int gcli_buffer_length = precinct_gcli_width_of(prec,position);

    for(i=0; i<gcli_buffer_length; i++){
      gcli_data_t residual = MAX(0, gcli_buffer[i] - gtli);
      
      bgt_gcli += residual+1; // unary coding
    }
      
    pbt_get_gcli_bgt_of(pbt)[position] = bgt_gcli;
	}
	return 0;
}
