/*
 * Written by J.T. Conklin, Apr 10, 1995
 * Public domain.
 */

#include <ieeefp.h>

fp_rnd
fpgetround()
{
	int x;

	__asm__("st %%fsr,%0" : "=m" (*&x));
	return (x >> 30) & 0x03;
}
