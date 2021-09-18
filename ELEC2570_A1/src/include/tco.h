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

 

#ifndef TCO_H
#define TCO_H

#include <stdint.h>
#include <math.h>

 
typedef int16_t tco_data_in_t;

#define MAX_NDECOMP_H 2
#define MAX_NDECOMP_V 1
#define MAX_FILTER_TYPES 10  
#define MAX_NCOMPS 1


enum {
	VER_KERNEL_53 = 0,
	VER_KERNEL_22 = 1,
	VER_KERNEL_137 = 2,
	VER_KERNEL_NB = 3,
};

enum {
	HOR_KERNEL_53 = 0,
	HOR_KERNEL_22 = 1,
	HOR_KERNEL_137 = 2,
	HOR_KERNEL_NB = 3,
};

enum {
	GAINS_CHOICE_PSNR,
	GAINS_CHOICE_VISUAL,
	GAINS_CHOICE_MANUAL,
};


typedef struct tco_conf_t {
	int32_t		bitstream_nbytes;  
	int32_t		bpp;			   
	uint8_t		profile;
	uint8_t		group_size; 
	uint8_t		ndecomp_h;
	uint8_t		ndecomp_v;
	uint8_t		ndecomp_vh;  
	uint16_t col_sz;
	int16_t		slice_height;	 
	int32_t		budget_report_nbytes;	 	  
	uint8_t		in_depth;  
	uint8_t		quant;  
	int 		gains_choice;
	uint8_t 	lvl_gains[MAX_FILTER_TYPES];
	uint8_t 	lvl_priorities[MAX_FILTER_TYPES];
} tco_conf_t;
 
typedef struct image_t
{
	int magic;  
	int ncomps;  
	int w;  
	int h;  
	int depth;  
	tco_data_in_t *array_ptr;  
} image_t;
 

typedef struct tco_enc_context_t tco_enc_context_t;

 
tco_enc_context_t* tco_enc_init(tco_conf_t* conf, image_t* image_in);

 
int tco_enc_close(tco_enc_context_t* ctx);

 
int tco_enc_image( tco_enc_context_t* ctx, image_t* image_in,
							void* codestream_buf, int codestream_buf_byte_size,
							int* codestream_byte_size);

int tco_conf_validate(tco_conf_t* conf_in, image_t* image, tco_conf_t* conf_out);

#endif
