#ifndef JPEG_XS_ELEC2570_H
#define JPEG_XS_ELEC2570_H

#include "tco.h"

static tco_conf_t jpeg_xs_elec2570_conf = {
	1024		/* bitstream_nbytes */,
	4		/* bpp */,
	7		/* profile */,
	4		/* group_size */, 
	2		/* ndecomp_h */,
	1		/* ndecomp_v */,
	1		/* ndecomp_vh */,
	64		/* col_sz */,
	16		/* slice_height */,
	64		/* budget_report_nbytes */,
	15		/* in_depth */,
	3		/* quant */,
	0		/* gains_choice */,
	{0},
	{0},
}; 


#endif /* JPEG_XS_ELEC2570_H */
