#	$NetBSD: Makefile,v 1.42.2.1 1997/10/27 18:09:59 thorpej Exp $

.include <bsd.own.mk>			# for configuration variables.

# NOTE THAT etc *DOES NOT* BELONG IN THE LIST BELOW

SUBDIR+= lib include bin libexec sbin usr.bin usr.sbin share

.if exists(games)
SUBDIR+= games
.endif

SUBDIR+= gnu

.if (${MACHINE_ARCH} == "arm32")
SUBDIR+= shark-tools
.endif

SUBDIR+= sys

.if exists(domestic) && !defined(EXPORTABLE_SYSTEM)
SUBDIR+= domestic
.endif

.if exists(regress)
.ifmake !(install)
SUBDIR+= regress
.endif

regression-tests:
	@echo Running regression tests...
	@(cd ${.CURDIR}/regress && ${MAKE} regress)
.endif

beforeinstall:
.ifndef DESTDIR
	(cd ${.CURDIR}/etc && ${MAKE} DESTDIR=/ distrib-dirs)
.else
	(cd ${.CURDIR}/etc && ${MAKE} distrib-dirs)
.endif

afterinstall:
.ifndef NOMAN
	(cd ${.CURDIR}/share/man && ${MAKE} makedb)
.if (${MACHINE_ARCH} == "arm32")
	(cd ${.CURDIR}/shark-tools && ${MAKE} makedb)
.endif
.endif

build: beforeinstall
	(cd ${.CURDIR}/share/mk && ${MAKE} install)
	${MAKE} includes
.if !defined(UPDATE)
	${MAKE} cleandir
.endif
	(cd ${.CURDIR}/lib/csu && ${MAKE} depend && ${MAKE} && ${MAKE} install)
	(cd ${.CURDIR}/lib && ${MAKE} depend && ${MAKE} && ${MAKE} install)
	(cd ${.CURDIR}/gnu/lib && ${MAKE} depend && ${MAKE} && ${MAKE} install)
.if exists(domestic) && !defined(EXPORTABLE_SYSTEM)
	(cd ${.CURDIR}/domestic/lib/ && ${MAKE} depend && ${MAKE} && \
	    ${MAKE} install)
.endif
	${MAKE} depend && ${MAKE} && ${MAKE} install

.if (${MACHINE_ARCH} == "arm32")
.ifndef DESTDIR
shark-distribution:
	@echo setenv DESTDIR before doing that!
	@false
.else
shark-distribution:
	cd ${.CURDIR}/etc && ${MAKE} distribution
	cd ${.CURDIR}/shark-tools/etc && ${MAKE} distribution
	cd ${.CURDIR}/shark-tools/kernel && ${MAKE} distribution
.endif  # DESTDIR check
.endif	# MACHINE_ARCH check

.include <bsd.subdir.mk>
