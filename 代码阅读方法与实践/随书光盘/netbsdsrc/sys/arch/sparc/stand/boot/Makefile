#	$NetBSD: Makefile,v 1.5.8.3 1997/11/15 18:52:12 mellon Exp $

PROG=		boot
PROGSOURCE=	boot.c net.c netif_sun.c conf.c
NEWVERSWHAT=	"Secondary Boot"

INCLUDE_LIBZ=	yes
SAMISCMAKEFLAGS= SA_USE_CREAD=yes

.include "../Makefile.buildboot"

RELOC=	0x300000

${PROG}: ${OBJS} ${LIBSA} ${LIBZ} ${LIBKERN}
	sh ${.CURDIR}/../common/newvers.sh ${.CURDIR}/version ${NEWVERSWHAT}
	${COMPILE.c} vers.c
	${LD} -o ${.TARGET} -N -T ${RELOC} -e start ${OBJS} \
	    vers.o ${LIBSA} ${LIBZ} ${LIBKERN}
	@size ${.TARGET}

afterinstall:
	# conjure up a magic header that is accepted by all Sun PROMS;
	# see installboot.c for details.
	printf '\01\03\01\07\060\200\0\07' | \
		dd of=${DESTDIR}/${BINDIR}/boot.net bs=32 conv=sync
	dd if=${DESTDIR}/${BINDIR}/boot of=${DESTDIR}/${BINDIR}/boot.net \
		bs=32 skip=1 seek=1
	chmod ${BINMODE} ${DESTDIR}/${BINDIR}/boot.net
	chown ${BINOWN}.${BINGRP} ${DESTDIR}/${BINDIR}/boot.net

.include <bsd.prog.mk>
