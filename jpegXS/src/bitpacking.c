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
#include <string.h>
#include <stdint.h>

#include <stdlib.h>

#include "bitpacking.h"

#define MIN(a,b) (((a)<(b)) ? (a) : (b))

// Inverse bytes order
packer_data_t ipx_htobe(packer_data_t in)
{
	int i = 0;
	union {
		packer_data_t integer;
		uint8_t  bytes[sizeof(packer_data_t)];
} a, b;
	a.integer = in;
	for (i = 0; i < sizeof(packer_data_t); i++)
		b.bytes[i] = a.bytes[sizeof(packer_data_t)-1-i];
	return b.integer;
}

// Number of bits in the data format of the packer
#define MAXB (sizeof(packer_data_t)*8)
 
#define PACK_WORD(a) ipx_htobe(a)

struct bit_packer_t
{
	packer_data_t* ptr;
	packer_data_t* ptr_cur;
	packer_data_t* ptr_max;
	int bit_offset;
	int max_size;
	int flushed;
};

bit_packer_t* bitpacker_open()
{
	bit_packer_t* packer = (bit_packer_t*) malloc(sizeof(bit_packer_t));
	packer->ptr = NULL;
	packer->ptr_cur = NULL;

	return packer;
}

int bitpacker_set_buffer(bit_packer_t* packer, void* ptr, int max_size)
{
	packer->ptr = (packer_data_t*) ptr;
	packer->max_size = max_size;
	packer->ptr_max = packer->ptr + (max_size/sizeof(packer_data_t)) - 1;
	bitpacker_reset(packer);
	return 0;
}

packer_data_t* bitpacker_get_buffer(bit_packer_t* packer)
{
	return packer->ptr;
}

int bitpacker_reset(bit_packer_t* packer)
{
	memset(packer->ptr, 0, packer->max_size);
	packer->ptr_cur = packer->ptr;
	packer->bit_offset = 0;
	packer->flushed = 0;
	return 0;
}

//*** Bit packer ***
// Write given number of bits in output buffer for bitstream
// - packer is the structure used to pack bits in the bitstream
// - val are the bits to be pushed in the bitstream
// - nbits is the number of bits to push
int bitpacker_write(bit_packer_t* packer, packer_data_t val, unsigned int nbits)
{
	// Bits are aggregated in data with a format packer_data_t (cfr. bitpacking.h)
	unsigned int available0 = (MAXB-packer->bit_offset);		// Number of bits available in the current data
	int len0 = (available0>=nbits) ? nbits : available0;		// Number of bits inserted in current data
	int len1 = nbits - len0;																// Number of bits inserted in next data

	// Remove bits above nbits
	if (nbits != MAXB)
		val = val & ((((packer_data_t)1U)<<nbits) - ((packer_data_t)1U));
	
	// Push bits in current data
	if (len0)
	{
		*(packer->ptr_cur) |= (val>>len1) << (available0-len0);
		packer->bit_offset += len0;
	}
	// Fetch next data and push bits in it
	if (len1)
	{
		// Bytes in the current data need to be reversed
		*packer->ptr_cur = PACK_WORD(*packer->ptr_cur);
		// Go to next data and reset counter
		packer->ptr_cur++;
		packer->bit_offset = 0;
		*(packer->ptr_cur) |= (val<<(MAXB-len1));
		packer->bit_offset += len1;
	}
	return nbits;
}

//*** Unary encoding ***
// - packer is the structure with the output bitstream
// - val is the value to be encoded
int bitpacker_write_unary_unsigned(bit_packer_t* packer, int8_t val)
{
	// Unary encoding: e.g. 1 -> 10; 2 -> 110; 3 -> 1110; 
	// Compute unary value and write to bitstream
	return bitpacker_write(packer, (((packer_data_t)1U)<<(val+((packer_data_t)1U)))-((packer_data_t)2U), val+1);
}

 
int bitpacker_flush(bit_packer_t* packer)
{
	if (packer->ptr_cur)
	{
		if ((!packer->flushed) && (packer->bit_offset > 0)) {
			*packer->ptr_cur = PACK_WORD(*packer->ptr_cur);
		}
		packer->flushed = 1;
	}
	return 0;
}

 
int bitpacker_align(bit_packer_t* packer)
{
	int n = 0;
	int nbits = packer->bit_offset & 0x7;
	if (nbits)
		n = bitpacker_write(packer, 0, (8 - nbits));
	return n;
}

int bitpacker_get_len(bit_packer_t* packer)
{
	int n;
	n = (int)((packer->ptr_cur - packer->ptr) * MAXB);
	if (packer->bit_offset > 0)
		n += packer->bit_offset;
	return n;
}

int bitpacker_add_padding(bit_packer_t* packer, int nbits)
{
	int i;
	for (i=0; i<nbits; i+=MAXB)
	{
		int burst = MIN(MAXB,nbits-i);
		if (bitpacker_write(packer, 0, burst) < 0)
			return -1;
	}
	return nbits;
}

