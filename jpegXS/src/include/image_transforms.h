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


#ifndef IMAGE_TRANSFORMS_H

#include <stdlib.h>
#include <stdio.h>

#include "tco.h"
#include "dwt53.h"

 

 
int image_shift(image_t* image_in, image_t* image_out, int shift, int rounding);

 
int image_offset(image_t* image_in, image_t* image_out, int offset);

 
int tco_image_clamp(image_t* image_in, image_t* image_out, int low, int high);


 
 
 
 
int tco_image_apply_dwt_hor(image_t* image_in, image_t* image_out, int ndecomp, int kernel, int* tmp_buffer, int tile_width);

 
int tco_image_apply_partial_dwt_hor(image_t* image_in, image_t* image_out, int ndecomp, dwt_data_t* tmp_buffer,
				    int level);

 
int tco_image_apply_dwt_ver(image_t* image_in, image_t* image_out, int kernel);

 
int tco_image_apply_partial_dwt_ver(image_t* image_in, image_t* image_out, dwt_data_t* tmp_buffer, int lvl);

 
int tco_image_apply_idwt_ver(image_t* image_in, image_t* image_out, int kernel);

 
int tco_image_apply_partial_idwt_ver(image_t* image_in, image_t* image_out, int kernel,int lvl);

 
 
 
 
 
int tco_image_apply_idwt_hor(image_t* image_in, image_t* image_out, int ndecomp, int kernel, int tile_width);

 
int tco_image_apply_partial_idwt_hor(image_t* image_in, image_t* image_out, int ndecomp, int kernel, int level);

#endif
