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
#include <math.h>
#include "tco.h"

#include "elec2570_config.h"
#include "tco.h"
#include "input/image_64_test.h"
#include "tco_conf.h"
 
void elec2570_image_init(image_t* ptr);
 
uint8_t* bitstream_buf = NULL; 			// Buffer for output bitstream
int bitstream_size = 0; 						// Size of output bitstream

////////  MAIN  /////////
int main()
{
	image_t image; 										// Structure containing the image with additional information
	tco_conf_t conf; 									// Structure containing global configuration parameters

	int bitstream_buf_max_size; 			// Maximum size for output buffer
	tco_enc_context_t *ctx = NULL; 		// Structure containing global variables of the encoding ( = context)
	
	conf = jpeg_xs_elec2570_conf; 		// Set configuration
	conf.bitstream_nbytes = IMAGE_TARGET_LENGTH; 		// Set target output bitstream size: compression ratio of 0.5
	
	elec2570_image_init(&image); 									// Initialize image structure
	
	ctx = tco_enc_init(&conf, &image);						// Initialize context
	
	bitstream_buf_max_size = (conf.bitstream_nbytes + 7) & (~0x7);   // Maximum output buffer size, rounded to upper byte
	bitstream_buf = malloc(bitstream_buf_max_size);									 // Allocate space for output bitstream
	
	tco_enc_image(ctx, &image, bitstream_buf, bitstream_buf_max_size, &bitstream_size);  // Actual encoding task
	
	// End of encoding task
}

//*** Initialize image structure (image_t defined in tco.h) ***
// - ptr is the pointer to the structure
// Note: Input image data defined in input.h
void elec2570_image_init(image_t* ptr){	
	tco_data_in_t* array_ptr;
	int h = IMAGE_H; 
	int w = IMAGE_W;
	array_ptr = (tco_data_in_t*) malloc(w*h*sizeof(tco_data_in_t));
	ptr->magic = 0x74636F69; 	// Unused
	ptr->h = h;								// Image height
	ptr->w = w;								// Image width
	ptr->ncomps = 1;					// Number of components in image (1 for black and white, more for colours)
	ptr->depth = 8;						// Bit depth, 8 bits as input format is in bytes
	for(int i = 0; i<w*h; i++){		// Fill in data array
		array_ptr[i] = input_image[i];
	}
	ptr->array_ptr = array_ptr;		// Pointer to the data array
	
}
