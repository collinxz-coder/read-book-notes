/*	$NetBSD: pmap.h,v 1.2 1997/10/16 01:55:31 sakamoto Exp $	*/

/*-
 * Copyright (C) 1995, 1996 Wolfgang Solfrank.
 * Copyright (C) 1995, 1996 TooLs GmbH.
 * All rights reserved.
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
 *	This product includes software developed by TooLs GmbH.
 * 4. The name of TooLs GmbH may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY TOOLS GMBH ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TOOLS GMBH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef	_MACHINE_PMAP_H_
#define	_MACHINE_PMAP_H_

#include <machine/pte.h>

/*
 * Segment registers
 */
#ifndef	_LOCORE
typedef u_int sr_t;
#endif	/* _LOCORE */
#define	SR_TYPE		0x80000000
#define	SR_SUKEY	0x40000000
#define	SR_PRKEY	0x20000000
#define	SR_VSID		0x00ffffff

#ifndef _LOCORE
/*
 * Pmap stuff
 */
struct pmap {
	sr_t pm_sr[16];		/* segments used in this pmap */
	int pm_refs;		/* ref count */
};

typedef	struct pmap *pmap_t;

#ifdef	_KERNEL
extern struct pmap kernel_pmap_;
#define	pmap_kernel()	(&kernel_pmap_)

#define pmap_clear_modify(pa)		(ptemodify((pa), PTE_CHG, 0))
#define	pmap_clear_reference(pa)	(ptemodify((pa), PTE_REF, 0))
#define	pmap_is_modified(pa)		(ptebits((pa), PTE_CHG))
#define	pmap_is_referenced(pa)		(ptebits((pa), PTE_REF))
#define	pmap_change_wiring(pm, va, wired)

#define	pmap_phys_address(x)		(x)

void pmap_bootstrap __P((u_int kernelstart, u_int kernelend, u_int size));

#define	vtophys(va)	((int)(pmap_extract(pmap_kernel(), (vm_offset_t)va)))

#endif	/* _KERNEL */
#endif	/* _LOCORE */

#endif	/* _MACHINE_PMAP_H_ */
