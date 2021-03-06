/*	$NetBSD: if_le.c,v 1.14 1997/03/19 16:24:39 gwr Exp $	*/

/*-
 * Copyright (c) 1995 Charles M. Hannum.  All rights reserved.
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Ralph Campbell and Rick Macklem.
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
 *
 *	@(#)if_le.c	8.2 (Berkeley) 11/16/93
 */

#include "bpfilter.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/mbuf.h>
#include <sys/syslog.h>
#include <sys/socket.h>
#include <sys/device.h>

#include <net/if.h>
#include <net/if_ether.h>
#include <net/if_media.h>

#ifdef INET
#include <netinet/in.h>
#include <netinet/if_inarp.h>
#endif

#include <vm/vm.h>

#include <machine/autoconf.h>
#include <machine/cpu.h>
#include <machine/pmap.h>

#include <mvme68k/dev/pccreg.h>
#include <mvme68k/dev/pccvar.h>

#include <dev/ic/am7990reg.h>
#include <dev/ic/am7990var.h>

#include <mvme68k/dev/if_lereg.h>
#include <mvme68k/dev/if_levar.h>

int 	le_pcc_match __P((struct device *, struct cfdata  *, void *));
void	le_pcc_attach __P((struct device *, struct device *, void *));

struct cfattach le_pcc_ca = {
	sizeof(struct le_softc), le_pcc_match, le_pcc_attach
};

hide void le_pcc_wrcsr __P((struct am7990_softc *, u_int16_t, u_int16_t));
hide u_int16_t le_pcc_rdcsr __P((struct am7990_softc *, u_int16_t));

void *ledatabuf; /* XXXCDC hack from pmap bootstrap */

hide void
le_pcc_wrcsr(sc, port, val)
	struct am7990_softc *sc;
	u_int16_t port, val;
{
	register struct lereg1 *ler1 = ((struct le_softc *)sc)->sc_r1;

	ler1->ler1_rap = port;
	ler1->ler1_rdp = val;
}

hide u_int16_t
le_pcc_rdcsr(sc, port)
	struct am7990_softc *sc;
	u_int16_t port;
{
	register struct lereg1 *ler1 = ((struct le_softc *)sc)->sc_r1;
	u_int16_t val;

	ler1->ler1_rap = port;
	val = ler1->ler1_rdp;
	return (val);
}

int
le_pcc_match(parent, cf, aux)
	struct device *parent;
	struct cfdata *cf;
	void *aux;
{
	struct pcc_attach_args *pa = aux;

	if (strcmp(pa->pa_name, le_cd.cd_name))
		return (0);

	pa->pa_ipl = cf->pcccf_ipl;
	return (1);
}

void
le_pcc_attach(parent, self, aux)
	struct device *parent, *self;
	void *aux;
{
	struct le_softc *lesc = (void *)self;
	struct am7990_softc *sc = &lesc->sc_am7990;
	struct pcc_attach_args *pa = aux;

	/* XXX the following declarations should be elsewhere */
	extern void myetheraddr __P((u_char *));

	/* Map control registers. */
	lesc->sc_r1 = (struct lereg1 *)PCC_VADDR(pa->pa_offset);

	sc->sc_mem = ledatabuf;		/* XXX */
	sc->sc_conf3 = LE_C3_BSWP;
	sc->sc_addr = (u_long)sc->sc_mem;
	sc->sc_memsize = 4 * NBPG;	/* XXX */

	myetheraddr(sc->sc_enaddr);

	sc->sc_copytodesc = am7990_copytobuf_contig;
	sc->sc_copyfromdesc = am7990_copyfrombuf_contig;
	sc->sc_copytobuf = am7990_copytobuf_contig;
	sc->sc_copyfrombuf = am7990_copyfrombuf_contig;
	sc->sc_zerobuf = am7990_zerobuf_contig;

	sc->sc_rdcsr = le_pcc_rdcsr;
	sc->sc_wrcsr = le_pcc_wrcsr;
	sc->sc_hwinit = NULL;

	am7990_config(sc);

	/* Are we the boot device? */
	if (PCC_PADDR(pa->pa_offset) == bootaddr) 
		booted_device = self;

	pccintr_establish(PCCV_LE, am7990_intr, pa->pa_ipl, sc);
	sys_pcc->le_int = pa->pa_ipl | PCC_IENABLE;
}
