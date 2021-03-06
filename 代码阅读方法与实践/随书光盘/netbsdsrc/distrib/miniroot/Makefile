#	$NetBSD: Makefile,v 1.16 1997/09/22 21:33:31 scottr Exp $

.include <bsd.own.mk>

REV!=sh ${BSDSRCDIR}/sys/conf/osrelease.sh -s

CBIN=		instbin
ARCHDIR=	${.CURDIR}/../${MACHINE}/miniroot

.if exists(${ARCHDIR}/Makefile.inc)
.include "${ARCHDIR}/Makefile.inc"
.endif

VND?=		vnd0c
VND_DEV=	/dev/${VND}
VND_RDEV=	/dev/r${VND}
VND_GEOM?=
IMAGE?=		miniroot-${REV}.fs
IMAGESIZE?=	12288 # 6MB in 512 byte blocks
NEWFSOPTS?=
MOUNT_POINT?=	/mnt
BOOTINSTALL?=	true

.ifdef DISKTYPE
DISKTYPEARG=	-t ${DISKTYPE}
.endif

LISTS=		${.CURDIR}/list ${ARCHDIR}/list
CRUNCHCONF=	${CBIN}.conf
MTREE=		${.CURDIR}/mtree.conf

.ifndef DESTDIR
all:
	@echo setenv DESTDIR before makeing a miniroot!
	@false
.else

all: ${CBIN}
	dd if=/dev/zero of=${IMAGE} count=${IMAGESIZE}
	vnconfig ${DISKTYPEARG} -v -c ${VND_DEV} ${IMAGE} ${VND_GEOM}
	-newfs -m 0 -o space ${NEWFSOPTS} ${VND_RDEV} ${DISKTYPE}
	${BOOTINSTALL}
	mount ${VND_DEV} ${MOUNT_POINT}
	mtree -def ${MTREE} -p ${MOUNT_POINT}/ -u
	REV=${REV} CURDIR=${.CURDIR} OBJDIR=${.OBJDIR} ARCHDIR=${ARCHDIR} \
	    TARGDIR=${MOUNT_POINT} VND_RDEV=${VND_RDEV} \
	    sh ${.CURDIR}/runlist.sh ${LISTS}
	@echo ""
	@df -i ${MOUNT_POINT}
	@echo ""
	umount ${MOUNT_POINT}
	vnconfig -u ${VND_DEV}
	cat /*bin/* > /dev/null

unconfig:
	-umount -f ${MOUNT_POINT}
	-vnconfig -u ${VND_DEV}
	-/bin/rm -f ${IMAGE}

${CBIN}.conf: ${LISTS}
	awk -f ${.CURDIR}/makeconf.awk CBIN=${CBIN} ${LISTS} > ${CBIN}.conf

${CBIN}.mk ${CBIN}.cache ${CBIN}.c: ${CRUNCHCONF}
	crunchgen -D ${.CURDIR}/../.. -L ${DESTDIR}/usr/lib ${CRUNCHCONF}

${CBIN}: ${CBIN}.mk ${CBIN}.cache ${CBIN}.c
	make -f ${CBIN}.mk all

clean cleandir:
	/bin/rm -f core ${IMAGE} ${CBIN} ${CBIN}.mk ${CBIN}.cache \
	${CBIN}.conf *.o *.lo *.c

.endif	# DESTDIR check
.include <bsd.obj.mk>
.include <bsd.subdir.mk>
