/*	$NetBSD: fat.c,v 1.8 1997/10/17 11:19:53 ws Exp $	*/

/*
 * Copyright (C) 1995, 1996, 1997 Wolfgang Solfrank
 * Copyright (c) 1995 Martin Husemann
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
 *	This product includes software developed by Martin Husemann
 *	and Wolfgang Solfrank.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <sys/cdefs.h>
#ifndef lint
__RCSID("$NetBSD: fat.c,v 1.8 1997/10/17 11:19:53 ws Exp $");
#endif /* not lint */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#include "ext.h"
#include "fsutil.h"

static int checkclnum __P((struct bootblock *, int, cl_t, cl_t *));
static int clustdiffer __P((cl_t, cl_t *, cl_t *, int));

/*
 * Check a cluster number for valid value
 */
static int
checkclnum(boot, fat, cl, next)
	struct bootblock *boot;
	int fat;
	cl_t cl;
	cl_t *next;
{
	if (*next >= (CLUST_RSRVD&boot->ClustMask))
		*next |= ~boot->ClustMask;
	if (*next == CLUST_FREE) {
		boot->NumFree++;
		return FSOK;
	}
	if (*next == CLUST_BAD) {
		boot->NumBad++;
		return FSOK;
	}
	if (*next < CLUST_FIRST
	    || (*next >= boot->NumClusters && *next < CLUST_EOFS)) {
		pwarn("Cluster %u in FAT %d continues with %s cluster number %u\n",
		      cl, fat,
		      *next < CLUST_RSRVD ? "out of range" : "reserved",
		      *next&boot->ClustMask);
		if (ask(0, "Truncate")) {
			*next = CLUST_EOF;
			return FSFATMOD;
		}
		return FSERROR;
	}
	return FSOK;
}

/*
 * Read a FAT and decode it into internal format
 */
int
readfat(fs, boot, no, fp)
	int fs;
	struct bootblock *boot;
	int no;
	struct fatEntry **fp;
{
	struct fatEntry *fat;
	u_char *buffer, *p;
	cl_t cl;
	off_t off;
	int ret = FSOK;

	boot->NumFree = boot->NumBad = 0;
	fat = malloc(sizeof(struct fatEntry) * boot->NumClusters);
	buffer = malloc(boot->FATsecs * boot->BytesPerSec);
	if (fat == NULL || buffer == NULL) {
		perror("No space for FAT");
		if (fat)
			free(fat);
		return FSFATAL;
	}

	memset(fat, 0, sizeof(struct fatEntry) * boot->NumClusters);

	off = boot->ResSectors + no * boot->FATsecs;
	off *= boot->BytesPerSec;

	if (lseek(fs, off, SEEK_SET) != off) {
		perror("Unable to read FAT");
		free(buffer);
		free(fat);
		return FSFATAL;
	}

	if (read(fs, buffer, boot->FATsecs * boot->BytesPerSec)
	    != boot->FATsecs * boot->BytesPerSec) {
		perror("Unable to read FAT");
		free(buffer);
		free(fat);
		return FSFATAL;
	}

	if (buffer[0] != boot->Media
	    || buffer[1] != 0xff || buffer[2] != 0xff
	    || (boot->ClustMask == CLUST16_MASK && buffer[3] != 0xff)
	    || (boot->ClustMask == CLUST32_MASK
		&& ((buffer[3]&0x0f) != 0x0f
		    || buffer[4] != 0xff || buffer[5] != 0xff
		    || buffer[6] != 0xff || (buffer[7]&0x0f) != 0x0f))) {
		char *msg;

		switch (boot->ClustMask) {
		case CLUST32_MASK:
			msg = "FAT starts with odd byte sequence (%02x%02x%02x%02x%02x%02x%02x%02x)\n";
			break;
		case CLUST16_MASK:
			msg = "FAT starts with odd byte sequence (%02x%02x%02x%02x)\n";
			break;
		default:
			msg = "FAT starts with odd byte sequence (%02x%02x%02x)\n";
			break;
		}
		pwarn(msg,
		      buffer[0], buffer[1], buffer[2], buffer[3],
		      buffer[4], buffer[5], buffer[6], buffer[7]);
		if (ask(1, "Correct"))
			ret |= FSFATMOD;
	}
	switch (boot->ClustMask) {
	case CLUST32_MASK:
		p = buffer + 8;
		break;
	case CLUST16_MASK:
		p = buffer + 4;
		break;
	default:
		p = buffer + 3;
		break;
	}
	for (cl = CLUST_FIRST; cl < boot->NumClusters;) {
		switch (boot->ClustMask) {
		case CLUST32_MASK:
			fat[cl].next = p[0] + (p[1] << 8)
				       + (p[2] << 16) + (p[3] << 24);
			fat[cl].next &= boot->ClustMask;
			ret |= checkclnum(boot, no, cl, &fat[cl].next);
			cl++;
			p += 4;
			break;
		case CLUST16_MASK:
			fat[cl].next = p[0] + (p[1] << 8);
			ret |= checkclnum(boot, no, cl, &fat[cl].next);
			cl++;
			p += 2;
			break;
		default:
			fat[cl].next = (p[0] + (p[1] << 8)) & 0x0fff;
			ret |= checkclnum(boot, no, cl, &fat[cl].next);
			cl++;
			if (cl >= boot->NumClusters)
				break;
			fat[cl].next = ((p[1] >> 4) + (p[2] << 4)) & 0x0fff;
			ret |= checkclnum(boot, no, cl, &fat[cl].next);
			cl++;
			p += 3;
			break;
		}
	}

	free(buffer);
	*fp = fat;
	return ret;
}

/*
 * Get type of reserved cluster
 */
char *
rsrvdcltype(cl)
	cl_t cl;
{
	if (cl < CLUST_BAD)
		return "reserved";
	if (cl > CLUST_BAD)
		return "as EOF";
	return "bad";
}

static int
clustdiffer(cl, cp1, cp2, fatnum)
	cl_t cl;
	cl_t *cp1;
	cl_t *cp2;
	int fatnum;
{
	if (*cp1 >= CLUST_RSRVD) {
		if (*cp2 >= CLUST_RSRVD) {
			if ((*cp1 < CLUST_BAD && *cp2 < CLUST_BAD)
			    || (*cp1 > CLUST_BAD && *cp2 > CLUST_BAD)) {
				pwarn("Cluster %u is marked %s with different indicators, ",
				      cl, rsrvdcltype(*cp1));
				if (ask(1, "fix")) {
					*cp2 = *cp1;
					return FSFATMOD;
				}
				return FSFATAL;
			}
			pwarn("Cluster %u is marked %s in FAT 0, %s in FAT %d\n",
			      cl, rsrvdcltype(*cp1), rsrvdcltype(*cp2), fatnum);
			if (ask(0, "use FAT 0's entry")) {
				*cp2 = *cp1;
				return FSFATMOD;
			}
			if (ask(0, "use FAT %d's entry", fatnum)) {
				*cp1 = *cp2;
				return FSFATMOD;
			}
			return FSFATAL;
		}
		pwarn("Cluster %u is marked %s in FAT 0, but continues with cluster %u in FAT %d\n",
		      cl, rsrvdcltype(*cp1), *cp2, fatnum);
		if (ask(0, "Use continuation from FAT %d", fatnum)) {
			*cp1 = *cp2;
			return FSFATMOD;
		}
		if (ask(0, "Use mark from FAT 0")) {
			*cp2 = *cp1;
			return FSFATMOD;
		}
		return FSFATAL;
	}
	if (*cp2 >= CLUST_RSRVD) {
		pwarn("Cluster %u continues with cluster %u in FAT 0, but is marked %s in FAT %d\n",
		      cl, *cp1, rsrvdcltype(*cp2), fatnum);
		if (ask(0, "Use continuation from FAT 0")) {
			*cp2 = *cp1;
			return FSFATMOD;
		}
		if (ask(0, "Use mark from FAT %d", fatnum)) {
			*cp1 = *cp2;
			return FSFATMOD;
		}
		return FSERROR;
	}
	pwarn("Cluster %u continues with cluster %u in FAT 0, but with cluster %u in FAT %d\n",
	      cl, *cp1, *cp2, fatnum);
	if (ask(0, "Use continuation from FAT 0")) {
		*cp2 = *cp1;
		return FSFATMOD;
	}
	if (ask(0, "Use continuation from FAT %d", fatnum)) {
		*cp1 = *cp2;
		return FSFATMOD;
	}
	return FSERROR;
}

/*
 * Compare two FAT copies in memory. Resolve any conflicts and merge them
 * into the first one.
 */
int
comparefat(boot, first, second, fatnum)
	struct bootblock *boot;
	struct fatEntry *first;
	struct fatEntry *second;
	int fatnum;
{
	cl_t cl;
	int ret = FSOK;

	for (cl = CLUST_FIRST; cl < boot->NumClusters; cl++)
		if (first[cl].next != second[cl].next)
			ret |= clustdiffer(cl, &first[cl].next, &second[cl].next, fatnum);
	return ret;
}

void
clearchain(boot, fat, head)
	struct bootblock *boot;
	struct fatEntry *fat;
	cl_t head;
{
	cl_t p, q;

	for (p = head; p >= CLUST_FIRST && p < boot->NumClusters; p = q) {
		if (fat[p].head != head)
			break;
		q = fat[p].next;
		fat[p].next = fat[p].head = CLUST_FREE;
		fat[p].length = 0;
	}
}

/*
 * Check a complete FAT in-memory for crosslinks
 */
int
checkfat(boot, fat)
	struct bootblock *boot;
	struct fatEntry *fat;
{
	cl_t head, p, h;
	u_int len;
	int ret = 0;
	int conf;

	/*
	 * pass 1: figure out the cluster chains.
	 */
	for (head = CLUST_FIRST; head < boot->NumClusters; head++) {
		/* find next untravelled chain */
		if (fat[head].head != 0		/* cluster already belongs to some chain */
		    || fat[head].next == CLUST_FREE
		    || fat[head].next == CLUST_BAD)
			continue;		/* skip it. */

		/* follow the chain and mark all clusters on the way */
		for (len = 0, p = head;
		     p >= CLUST_FIRST && p < boot->NumClusters;
		     p = fat[p].next) {
			fat[p].head = head;
			len++;
		}

		/* the head record gets the length */
		fat[head].length = fat[head].next == CLUST_FREE ? 0 : len;
	}

	/*
	 * pass 2: check for crosslinked chains (we couldn't do this in pass 1 because
	 * we didn't know the real start of the chain then - would have treated partial
	 * chains as interlinked with their main chain)
	 */
	for (head = CLUST_FIRST; head < boot->NumClusters; head++) {
		/* find next untravelled chain */
		if (fat[head].head != head)
			continue;

		/* follow the chain to its end (hopefully) */
		for (p = head;
		     fat[p].next >= CLUST_FIRST && fat[p].next < boot->NumClusters;
		     p = fat[p].next)
			if (fat[fat[p].next].head != head)
				break;
		if (fat[p].next >= CLUST_EOFS)
			continue;

		if (fat[p].next == 0) {
			pwarn("Cluster chain starting at %u ends with free cluster\n", head);
			if (ask(0, "Clear chain starting at %u", head)) {
				clearchain(boot, fat, head);
				ret |= FSFATMOD;
			} else
				ret |= FSERROR;
			continue;
		}
		if (fat[p].next >= CLUST_RSRVD) {
			pwarn("Cluster chain starting at %u ends with cluster marked %s\n",
			      head, rsrvdcltype(fat[p].next));
			if (ask(0, "Clear chain starting at %u", head)) {
				clearchain(boot, fat, head);
				ret |= FSFATMOD;
			} else
				ret |= FSERROR;
			continue;
		}
		if (fat[p].next < CLUST_FIRST || fat[p].next >= boot->NumClusters) {
			pwarn("Cluster chain starting at %u ends with cluster out of range (%u)\n",
			      head, fat[p].next);
			if (ask(0, "Clear chain starting at %u", head)) {
				clearchain(boot, fat, head);
				ret |= FSFATMOD;
			} else
				ret |= FSERROR;
		}
		pwarn("Cluster chains starting at %u and %u are linked at cluster %u\n",
		      head, fat[p].head, p);
		conf = FSERROR;
		if (ask(0, "Clear chain starting at %u", head)) {
			clearchain(boot, fat, head);
			conf = FSFATMOD;
		}
		if (ask(0, "Clear chain starting at %u", h = fat[p].head)) {
			if (conf == FSERROR) {
				/*
				 * Transfer the common chain to the one not cleared above.
				 */
				for (; p >= CLUST_FIRST && p < boot->NumClusters;
				     p = fat[p].next) {
					if (h != fat[p].head) {
						/*
						 * Have to reexamine this chain.
						 */
						head--;
						break;
					}
					fat[p].head = head;
				}
			}
			clearchain(boot, fat, h);
			conf |= FSFATMOD;
		}
		ret |= conf;
	}

	return ret;
}

/*
 * Write out FATs encoding them from the internal format
 */
int
writefat(fs, boot, fat)
	int fs;
	struct bootblock *boot;
	struct fatEntry *fat;
{
	u_char *buffer, *p;
	cl_t cl;
	int i;
	u_int32_t fatsz;
	off_t off;
	int ret = FSOK;

	buffer = malloc(fatsz = boot->FATsecs * boot->BytesPerSec);
	if (buffer == NULL) {
		perror("No space for FAT");
		return FSFATAL;
	}
	memset(buffer, 0, fatsz);
	boot->NumFree = 0;
	p = buffer;
	*p++ = (u_char)boot->Media;
	*p++ = 0xff;
	*p++ = 0xff;
	switch (boot->ClustMask) {
	case CLUST16_MASK:
		*p++ = 0xff;
		break;
	case CLUST32_MASK:
		*p++ = 0x0f;
		*p++ = 0xff;
		*p++ = 0xff;
		*p++ = 0xff;
		*p++ = 0x0f;
		break;
	}
	for (cl = CLUST_FIRST; cl < boot->NumClusters; cl++) {
		switch (boot->ClustMask) {
		case CLUST32_MASK:
			if (fat[cl].next == CLUST_FREE)
				boot->NumFree++;
			*p++ = (u_char)fat[cl].next;
			*p++ = (u_char)(fat[cl].next >> 8);
			*p++ = (u_char)(fat[cl].next >> 16);
			*p &= 0xf0;
			*p++ |= (fat[cl].next >> 24)&0x0f;
			break;
		case CLUST16_MASK:
			if (fat[cl].next == CLUST_FREE)
				boot->NumFree++;
			*p++ = (u_char)fat[cl].next;
			*p++ = (u_char)(fat[cl].next >> 8);
			break;
		default:
			if (fat[cl].next == CLUST_FREE)
				boot->NumFree++;
			if (cl + 1 < boot->NumClusters
			    && fat[cl + 1].next == CLUST_FREE)
				boot->NumFree++;
			*p++ = (u_char)fat[cl].next;
			*p++ = (u_char)((fat[cl].next >> 8) & 0xf)
			       |(u_char)(fat[cl+1].next << 4);
			*p++ = (u_char)(fat[++cl].next >> 4);
			break;
		}
	}
	for (i = 0; i < boot->FATs; i++) {
		off = boot->ResSectors + i * boot->FATsecs;
		off *= boot->BytesPerSec;
		if (lseek(fs, off, SEEK_SET) != off
		    || write(fs, buffer, fatsz) != fatsz) {
			perror("Unable to write FAT");
			ret = FSFATAL; /* Return immediately?		XXX */
		}
	}
	free(buffer);
	return ret;
}

/*
 * Check a complete in-memory FAT for lost cluster chains
 */
int
checklost(dosfs, boot, fat)
	int dosfs;
	struct bootblock *boot;
	struct fatEntry *fat;
{
	cl_t head;
	int mod = FSOK;
	int ret;
	
	for (head = CLUST_FIRST; head < boot->NumClusters; head++) {
		/* find next untravelled chain */
		if (fat[head].head != head
		    || fat[head].next == CLUST_FREE
		    || (fat[head].next >= CLUST_RSRVD
			&& fat[head].next < CLUST_EOFS)
		    || (fat[head].flags & FAT_USED))
			continue;

		pwarn("Lost cluster chain at cluster %u\n%d Cluster(s) lost\n",
		      head, fat[head].length);
		mod |= ret = reconnect(dosfs, boot, fat, head);
		if (mod & FSFATAL)
			break;
		if (ret == FSERROR && ask(0, "Clear")) {
			clearchain(boot, fat, head);
			mod |= FSFATMOD;
		}
	}
	finishlf();

	if (boot->FSInfo) {
		ret = 0;
		if (boot->FSFree != boot->NumFree) {
			pwarn("Free space in FSInfo block (%d) not correct (%d)\n",
			      boot->FSFree, boot->NumFree);
			if (ask(1, "fix")) {
				boot->FSFree = boot->NumFree;
				ret = 1;
			}
		}
		if (boot->NumFree && fat[boot->FSNext].next != CLUST_FREE) {
			pwarn("Next free cluster in FSInfo block (%u) not free\n",
			      boot->FSNext);
			if (ask(1, "fix"))
				for (head = CLUST_FIRST; head < boot->NumClusters; head++)
					if (fat[head].next == CLUST_FREE) {
						boot->FSNext = head;
						ret = 1;
						break;
					}
		}
		if (ret)
			mod |= writefsinfo(dosfs, boot);
	}

	return mod;
}
