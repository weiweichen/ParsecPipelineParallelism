/* @(#) Extract the real part of a complex image. Output is float or double.
 * @(#)
 * @(#) int im_c2real( in, out )
 * @(#) IMAGE *in, *out;
 * @(#)
 * @(#) All functions return 0 on success and -1 on error
 * @(#)
 *
 * Copyright: 1990, N. Dessipris.
 *
 * Author: Nicos Dessipris
 * Written on: 12/02/1990
 * Modified on : 09/05/1990
 * 15/6/93 JC
 *	- stupid stupid includes and externs fixed
 *	- I have been editing for 1 1/2 hours and I'm still drowning in
 *	  rubbish extetrnshh
 * 13/12/94 JC
 *	- modernised
 * 21/12/94 JC
 *	- im_c2amph() adapted to make im_c2real() and im_c2imag()
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
#include <stdlib.h>
#include <math.h>

#include <vips/vips.h>

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif /*WITH_DMALLOC*/

#define loop(TYPE) \
{\
	TYPE *p = (TYPE *) in;\
	TYPE *q = (TYPE *) out;\
	int x;\
	\
	for( x = 0; x < n; x++ ) {\
		double re = *p;\
		\
		p += 2;\
		*q++ = re;\
	}\
}

/* c2real buffer processor.
 */
static void
buffer_c2real( void *in, void *out, int w, IMAGE *im )
{
	int n = w * im->Bands;

	switch( im->BandFmt ) {
		case IM_BANDFMT_DPCOMPLEX:      loop(double); break; 
		case IM_BANDFMT_COMPLEX:        loop(float); break;
		default:
			error_exit( "buffer_c2real: internal error" );	
	}
}

int 
im_c2real( IMAGE *in, IMAGE *out )
{
	if( in->Coding != IM_CODING_NONE || !im_iscomplex( in ) ) {
		im_errormsg( "im_c2real: input should be uncoded complex" );
		return( -1 );
	}
        if( im_cp_desc( out, in ) )
                return( -1 );

	/* Output will be float or double.
	 */
	if( in->BandFmt == IM_BANDFMT_DPCOMPLEX ) {
		out->BandFmt = IM_BANDFMT_DOUBLE;
		out->Bbits = IM_BBITS_DOUBLE;
	}
	else {
		out->BandFmt = IM_BANDFMT_FLOAT;
		out->Bbits = IM_BBITS_FLOAT;
	}

        /* Do the processing.
         */
        if( im_wrapone( in, out,
                (im_wrapone_fn) buffer_c2real, in, NULL ) )
                return( -1 );

	return( 0 );
}
