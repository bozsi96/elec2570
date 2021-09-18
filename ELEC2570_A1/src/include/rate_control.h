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


#ifndef SRC_RATE_CONTROL_H
#define SRC_RATE_CONTROL_H

#include "tco.h"
#include "precinct.h"
#include "precinct_budget.h"


#define RATE_CONTROL_MAX_NUM_PRECINCTS_IN_WINDOW 2


typedef struct rate_control_t rate_control_t;

 
typedef struct rc_results_t {
	int scenario;
	int refinement;
	int gcli_method;
	
	 
	precinct_budget_info_t pbinfo;
	 
	int precinct_total_bits;
	 
	int padding_bits;

	int* gtli_table_data;
	int* gtli_table_gcli;
	
	int rc_error;
} rc_results_t;


 
rate_control_t* rate_control_open(  tco_conf_t* conf, 
									lvls_map_t* map, 
									int image_height);

 
void rate_control_init(rate_control_t* rc, int rate_bytes, int report_bytes);

 
int rate_control_push( rate_control_t* rc, precinct_t* precinct );

 
precinct_t* rate_control( rate_control_t* rc, rc_results_t* rc_results, precinct_t* precinct );


void rate_control_close( rate_control_t* rc );


 
void rate_control_compute_column_rate(int ncols,
									  int image_rate_bytes, 
									  int image_report_bytes, 
									  int* nbytes_column,
									  int* report_column);

#endif
