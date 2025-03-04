/*
* Copyright (c) 1990 Regents of the University of California.
* All rights reserved.
*
* This code is derived from software contributed to Berkeley by
* Chris Torek.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. All advertising materials mentioning features or use of this software
*    must display the following acknowledgement:
*	This product includes software developed by the University of
*	California, Berkeley and its contributors.
* 4. Neither the name of the University nor the names of its contributors
*    may be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

/* Includes */
#include <orangeos.h>
#include <stdlib.h>

/* Integer division operation */
div_t div(int num, int denom)
{
	div_t r;

	r.quot = num / denom;
	r.rem = num % denom;
	/*
	* The ANSI standard says that |r.quot| <= |n/d|, where
	* n/d is to be computed in infinite precision.  In other
	* words, we should always truncate the quotient towards
	* 0, never -infinity or +infinity.
	*
	* Machine division and remainer may work either way when
	* one or both of n or d is negative.  If only one is
	* negative and r.quot has been truncated towards -inf,
	* r.rem will have the same sign as denom and the opposite
	* sign of num; if both are negative and r.quot has been
	* truncated towards -inf, r.rem will be positive (will
	* have the opposite sign of num).  These are considered
	* `wrong'.
	*
	* If both are num and denom are positive, r will always
	* be positive.
	*
	* This all boils down to:
	*	if num >= 0, but r.rem < 0, we got the wrong answer.
	* In that case, to get the right answer, add 1 to r.quot and
	* subtract denom from r.rem.
	*      if num < 0, but r.rem > 0, we also have the wrong answer.
	* In this case, to get the right answer, subtract 1 from r.quot and
	* add denom to r.rem.
	*/
	if (num >= 0 && r.rem < 0) {
		++r.quot;
		r.rem -= denom;
	}
	else if (num < 0 && r.rem > 0) {
		--r.quot;
		r.rem += denom;
	}
	return (r);
}
