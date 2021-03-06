/*	$NetBSD: ldd.c,v 1.1 1996/12/16 19:59:56 cgd Exp $	*/

/*
 * Copyright 1996 John D. Polstra.
 * Copyright 1996 Matt Thomas <matt@3am-software.com>
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
 *      This product includes software developed by John Polstra.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <dirent.h>

#include "debug.h"
#include "rtld.h"

/*
 * Data declarations.
 */
const char *_rtld_error_message;	/* Message for dlopen(), or NULL */
bool _rtld_trust;		/* False for setuid and setgid programs */
Obj_Entry *_rtld_objlist;	/* Head of linked list of shared objects */
Obj_Entry **_rtld_objtail = &_rtld_objlist;
				/* Link field of last object in list */
Obj_Entry *_rtld_objmain;	/* The main program shared object */

Search_Path *_rtld_paths;

static void print_needed(Obj_Entry *);


/*
 * Main entry point for dynamic linking.  The argument is the stack
 * pointer.  The stack is expected to be laid out as described in the
 * SVR4 ABI specification, Intel 386 Processor Supplement.  Specifically,
 * the stack pointer points to a word containing ARGC.  Following that
 * in the stack is a null-terminated sequence of pointers to argument
 * strings.  Then comes a null-terminated sequence of pointers to
 * environment strings.  Finally, there is a sequence of "auxiliary
 * vector" entries.
 *
 * This function returns the entry point for the main program in %eax,
 * and the dynamic linker's exit procedure in %edx.  We accomplish this
 * by declaring the return value to have the 64-bit type "long long".
 * Such values are returned with their most-significant 32 bits in %edx,
 * and their least-significant 32 bits in %eax.
 */
int
main(
    int argc,
    char **argv)
{
#ifdef DEBUG
    debug = 1;
#endif
    _rtld_add_paths(&_rtld_paths, RTLD_DEFAULT_LIBRARY_PATH);


    _rtld_trust = geteuid() == getuid() && getegid() == getgid();

    if (_rtld_trust) {
	_rtld_add_paths(&_rtld_paths, getenv("LD_LIBRARY_PATH"));
    }

    for (argc--, argv++; argc != 0; argc--, argv++) {
	int fd = open(*argv, O_RDONLY);
	if (fd == -1) {
	    warn("%s", *argv);
	    continue;
	}
	_rtld_objmain = _rtld_map_object(*argv, fd);
	close(fd);
	if (_rtld_objmain == NULL) {
	    warnx("%s", _rtld_error_message);
	    continue;
	}

	_rtld_objmain->path = *argv;
	_rtld_objmain->mainprog = true;
	_rtld_digest_dynamic(_rtld_objmain);

	/* Link the main program into the list of objects. */
	*_rtld_objtail = _rtld_objmain;
	_rtld_objtail = &_rtld_objmain->next;
	++_rtld_objmain->refcount;

	(void) _rtld_load_needed_objects(_rtld_objmain);

	printf("%s:\n", _rtld_objmain->path);
	print_needed(_rtld_objmain);

	while (_rtld_objlist != NULL) {
	    Obj_Entry *obj = _rtld_objlist;
	    _rtld_objlist = obj->next;
	    while (obj->rpaths != NULL) {
		const Search_Path *rpath = obj->rpaths;
		obj->rpaths = rpath->sp_next;
		free((void *) rpath->sp_path);
		free((void *) rpath);
	    }
	    while (obj->needed != NULL) {
		const Needed_Entry *needed = obj->needed;
		obj->needed = needed->next;
		free((void *) needed);
	    }
	    (void) munmap(obj->relocbase, obj->mapsize);
	    free(obj->path);
	    free(obj);
	}

	_rtld_objmain = NULL;
	_rtld_objtail = &_rtld_objlist;
    }
    return 0;
}

/*
 * Error reporting function.  Use it like printf.  If formats the message
 * into a buffer, and sets things up so that the next call to dlerror()
 * will return the message.
 */
void
_rtld_error(
    const char *fmt, ...)
{
    static char buf[512];
    va_list ap;
    va_start(ap, fmt);
    xvsnprintf(buf, sizeof buf, fmt, ap);
    _rtld_error_message = buf;
    va_end(ap);
}

static void
print_needed(
    Obj_Entry *obj)
{
    const Needed_Entry *needed;

    for (needed = obj->needed; needed != NULL; needed = needed->next) {
	char libnamebuf[200];
	const char *libname = obj->strtab + needed->name, *cp;
	if (strncmp(libname, "lib", 3) == 0
		&& (cp = strstr(libname, ".so")) != NULL) {
	    strcpy(libnamebuf, "-l");
	    memcpy(&libnamebuf[2], libname + 3, cp - (libname + 3));
	    strcpy(&libnamebuf[cp - (libname + 3) + 2], cp + 3);
	    libname = libnamebuf;
	}
	
	if (needed->obj != NULL) {
	    print_needed(needed->obj);
	    if (!needed->obj->printed) {
		printf("\t %s => %s\n", libname, needed->obj->path);
		needed->obj->printed = 1;
	    }
	} else {
	    printf("\t %s => not found\n", libname);
	}
    }
}
