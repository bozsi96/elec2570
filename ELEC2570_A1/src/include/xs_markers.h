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

#ifndef XS_MARKERS_H
#define XS_MARKERS_H

#include "tco.h"
#include "tco_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitpacking.h"

 
#define XS_MARKER_NBYTES 2
#define XS_MARKER_NBITS 16

 
#define XS_SLICE_HEADER_NBYTES ((2*XS_MARKER_NBYTES)+2)

#define XS_MARKER_SOC 0xff10
#define XS_MARKER_EOC 0xff11
#define XS_MARKER_PIH 0xff12
#define XS_MARKER_CDT 0xff13
#define XS_MARKER_WGT 0xff14
#define XS_MARKER_COM 0xff15
#define XS_MARKER_SLH 0xff20
#define XS_MARKER_CAP 0xff50


 
int xs_write_head(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf);

 
int xs_write_capabilities_marker(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf);
int xs_write_picture_header(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf);
int xs_write_component_table(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf);
int xs_write_weights_table(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf);
int xs_write_extension_marker(bit_packer_t* bitstream, image_t* im, tco_conf_t *conf);

 
int xs_write_tail(bit_packer_t* bitstream);

 
int xs_write_slice_header(bit_packer_t* bitstream, int slice_idx);

#endif
