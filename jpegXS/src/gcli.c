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


#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "gcli.h"

// Bit scan reverse (position of most significant 1)
#define BSR(x) (31 - __builtin_clz(x))
#define GCLI(x) (((x) == 0) ? 0 : (BSR((x))+1))  

//*** Search GCLI in buffer ***
// Data are grouped by 4 and GCLI is extracted for each group
// - in is the pointer to the input vector
// - len is the length of the input vector
// - out is the output vector with GCLI values
// - max_out_len is the maximum length of the output vector
// - out_len is the output length of the output vector
int compute_gcli_buf(const sig_mag_data_t* in, int len, gcli_data_t* out, int max_out_len, int* out_len)
{
	int i,j;
	sig_mag_data_t or_all;
	*out_len = len>>2;
	if (*out_len > max_out_len)
		return -1;

	for (i=0; i < len; i+=4)
	{
		// OR on groups of 4 values
		for (or_all = 0, j=0; j<4; j++)
			or_all |= *in++;
		// GLCI is after the most significant 1 in the computed OR 
    
     //printf("gcli %d\n",GCLI(or_all & (~SIGN_BIT_MASK)));
		*out++ = GCLI(or_all & (~SIGN_BIT_MASK));
	}
	return 0;
}

