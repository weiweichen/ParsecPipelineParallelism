/* @(#) LabQ2Lab convert Lab 32bit packed format to float Lab
	@(#) uses imb_LabQ2Lab
Copyright Kirk Martinez 2/5/1993
Modified: 16/6/93
 * 7/6/93 JC
 *	- adapted for partial v2
 * 16/11/94 JC
 *	- adapted to new im_wrap_oneonebuf() function.
 * 9/2/95 JC
 *	- new im_wrapone function
 * 22/5/95 JC
 *	- changed char to unsigned char for RS/6000 
 * 	- small tidies and speed-ups
 * 4/9/97 JC
 *	- L* = 100.0 now handled correctly
 */

/*

    This file is part of VIPS.
    
    VIPS is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 */

/*

    These files are distributed with VIPS - http://www.vips.ecs.soton.ac.uk

 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /*HAVE_CONFIG_H*/
#include <vips/intl.h>

#include <stdio.h>

#include <vips/vips.h>

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif /*WITH_DMALLOC*/

/* imb_LabQ2Lab: CONVERT n pels from packed 32bit Lab to float values
 * in a buffer
 * ARGS:   PEL *inp       pointer to first byte of Lab32 buffer
 * float *outbuf   destination buffer
 *	int n           number of pels to process
 * (C) K.Martinez 2/5/93
 */
void
imb_LabQ2Lab( PEL *inp, float *outbuf, int n )        
{
	signed char *b;		/* to read input bytes */
	int l;
	int lsbs;               /* for lsbs byte */
	int c;                  /* counter      */
	float *out;

	/* Read input with a signed pointer to get signed ab easily.
	 */
	b = (signed char *) inp;
	out = outbuf;
	for( c = 0; c < n; c++ ) {
		/* Get extra bits.
		 */
		lsbs = ((unsigned char *) b)[3];

		/* Build L.
		 */
		l = ((unsigned char *)b)[0];
		l = (l << 2) | (lsbs >> 6);
		out[0] = (float) l * (100.0 / 1023.0);

		/* Build a.
		 */
		l = (b[1] << 3) | ((lsbs >> 3) & 0x7);
		out[1] = (float) l * 0.125;

		/* And b.
		 */
		l = (b[2] << 3) | (lsbs & 0x7);
		out[2] = (float) l * 0.125;        

		b += 4;
		out += 3;
	}
}

/* unpack a Lab32 ie 10,11,11 Lab image into float image
this is a wrapper around the buffer processing function
(C) K.Martinez 2/5/93
*/

int
im_LabQ2Lab( IMAGE *labim,  IMAGE *outim )
{
	/* check for coded Lab type 
	 */
	if( labim->Coding != IM_CODING_LABQ ) {
		im_errormsg( "im_LabQ2Lab: not a packed Lab image" );
		return( -1 );
	}

	/* set up output image 
	 */
	if( im_cp_desc( outim, labim ) )
		return( -1 );
	outim->Bands = 3;
	outim->Type = IM_TYPE_LAB;
	outim->BandFmt = IM_BANDFMT_FLOAT;
	outim->Bbits = 32;
	outim->Coding = IM_CODING_NONE;

	if( im_wrapone( labim, outim, 
		(im_wrapone_fn) imb_LabQ2Lab, NULL, NULL ) )
		return( -1 );

	return( 0 );
}
