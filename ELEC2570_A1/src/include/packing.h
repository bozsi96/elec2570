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


#ifndef PACKING_H
#define PACKING_H

#include "sig_mag.h"
#include "gcli.h" 
#include "bitpacking.h"
#include "precinct.h"
#include "rate_control.h"


 
#define PREC_HDR_PREC_SIZE 24
#define PREC_HDR_SCENARIO_SIZE 8
#define PREC_HDR_REFINEMENT_SIZE 8
 
#define PREC_HDR_ALIGNMENT 8

 
#define PKT_HDR_DATA_SIZE_SHORT 15
#define PKT_HDR_DATA_SIZE_LONG 20
#define PKT_HDR_GCLI_SIZE_SHORT 13
#define PKT_HDR_GCLI_SIZE_LONG 20
#define PKT_HDR_SIGN_SIZE_SHORT 11
#define PKT_HDR_SIGN_SIZE_LONG 15
#define PKT_HDR_ALIGNMENT 8
 
int pkt_hdr_is_short(int image_width, int ncomps, int n_decomp_v);

#define SUBPKT_ALIGNMENT 8

 
int pack_precinct(bit_packer_t* bitstream, precinct_t* precinct, rc_results_t* ra_result);


int pack_gclis(bit_packer_t* bitstream, precinct_t* precinct, rc_results_t* ra_result, int position);
int pack_data(bit_packer_t* bitstream, sig_mag_data_t* buf, int buf_len, gcli_data_t* gclis, int gtli);
int pack_sign(bit_packer_t* bitstream, sig_mag_data_t* buf, int buf_len, gcli_data_t* gclis, int gtli);


#endif
