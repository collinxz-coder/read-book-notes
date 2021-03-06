#	$NetBSD: Makefile.inc,v 1.6 1997/07/17 19:47:10 phil Exp $

ECHO	?=	echo

BINDIR	?=	/usr/local/bin
FONTDIR	=	/usr/share/misc/pcvtfonts
LIBDIR	=	/usr/local/lib
LIBMODE	=	644

# for pcvt_ioctl.h

INCDIR	=	/usr/include/machine
INCOWN	=	bin
INCGRP	=	bin
INCMODE	=	444

${INCDIR}/pcvt_ioctl.h: ${.CURDIR}/../../pcvt_ioctl.h
	${INSTALL} ${COPY} -o ${INCOWN} -g ${INCGRP} -m ${INCMODE} \
	${.CURDIR}/../../pcvt_ioctl.h ${INCDIR}

.depend:	${INCDIR}/pcvt_ioctl.h

MAN3EXT	=	3
MAN4EXT	=	4
MAN5EXT	=	5
MAN8EXT	=	8
MANDIR	=	/usr/local/man

MAN = ${MAN1} ${MAN3} ${MAN4} ${MAN5} ${MAN8}

.if	empty(MAN) && defined(PROG)
MAN+=	${PROG}.1
.endif
