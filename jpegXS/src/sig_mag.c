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

#include "sig_mag.h"

#if defined(_WIN32) && !defined(__MINGW32__)
#define INLINE _inline
#else
#define INLINE inline
#endif

//*** Buffer from 2's complement to sign/magnitude representation ***
// - data_in is the pointer to the input vector
// - in_len is the length of the input vector
// - data_out is the pointer to the output vector
// - max_out_len is the maximum length of the output vector
int to_sig_mag_buf(const sig_mag_data_in_t* data_in, int in_len, sig_mag_data_t* data_out, int max_out_len)
{
	int i=0;
	if (max_out_len < in_len)
		return -1;
	for (i=0;i<in_len;i++){
    *data_out = (*data_in >= 0) ? *data_in : (-*data_in) | SIGN_BIT_MASK;
    data_out++;
    data_in++;
  }
	return 0;
}
