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

#ifndef BITPACKING_H
#define BITPACKING_H
#include <stdint.h>

typedef enum unary_alphabet_e
{
	UNARY_ALPHABET_0 = 0,
	UNARY_ALPHABET_4_CLIPPED = 1,
	UNARY_ALPHABET_FULL = 2,
	UNARY_ALPHABET_NB
} unary_alphabet_t;


 
#define FIRST_ALPHABET UNARY_ALPHABET_0
#define SECOND_ALPHABET UNARY_ALPHABET_4_CLIPPED

 
#define MAX_UNARY 15
#define MAX_UNARY_CLIPPED 13

#if (SECOND_ALPHABET == UNARY_ALPHABET_4_CLIPPED)
#define MAX_UNARY_SECONDARY MAX_UNARY_CLIPPED
#else
#define MAX_UNARY_SECONDARY MAX_UNARY
#endif

typedef uint64_t packer_data_t;

typedef struct bit_packer_t bit_packer_t;


bit_packer_t* bitpacker_open(void);
int bitpacker_set_buffer(bit_packer_t* packer, void* ptr, int max_size);
packer_data_t* bitpacker_get_buffer(bit_packer_t* packer);
int bitpacker_write(bit_packer_t* packer, packer_data_t val, unsigned int nbits);
int bitpacker_write_unary_signed(bit_packer_t* packer, int8_t val, unary_alphabet_t alphabet);
int bitpacker_write_unary_unsigned(bit_packer_t* packer, int8_t val);

int bitpacker_align(bit_packer_t* packer);
int bitpacker_get_len(bit_packer_t* packer);
int bitpacker_add_padding(bit_packer_t* packer, int nbits);
int bitpacker_flush(bit_packer_t* packer);
int bitpacker_reset(bit_packer_t* packer);
int bitpacker_append(bit_packer_t* packer, void* ptr_from, int nbits);
void bitpacker_close(bit_packer_t* packer);


#endif
