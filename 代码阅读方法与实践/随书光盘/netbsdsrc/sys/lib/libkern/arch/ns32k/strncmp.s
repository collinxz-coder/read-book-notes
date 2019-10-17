/* 
 * Written by Matthias Pfaller, 1996
 * Public domain.
 */

#include <machine/asm.h>

#if defined(LIBC_SCCS)
	RCSID("$NetBSD: strncmp.S,v 1.1 1996/11/07 07:36:29 matthias Exp $")
#endif

/*
 * int
 * strncmp(const char *s1, const char *s2, size_t len)
 */
ENTRY(strncmp)
	enter	[r4],0
	movd	B_ARG2,r0
	cmpqd	0,r0
	beq	1f
	movd	B_ARG0,r1
	movd	B_ARG1,r2
	movqd	0,r4
	cmpsb	u
	bfc	0f
	cmpb	0(r1),0(r2)
0:	movqd	0,r0
	beq	1f
	movqd	-1,r0
	blo	1f
	movqd	1,r0
1:	exit	[r4]
	ret	0
