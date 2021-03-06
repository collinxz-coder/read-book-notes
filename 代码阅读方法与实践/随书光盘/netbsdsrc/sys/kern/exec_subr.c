/*	$NetBSD: exec_subr.c,v 1.10 1996/09/30 23:18:45 cgd Exp $	*/

/*
 * Copyright (c) 1993, 1994, 1996 Christopher G. Demetriou
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
 *      This product includes software developed by Christopher G. Demetriou.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
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

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/malloc.h>
#include <sys/vnode.h>
#include <sys/filedesc.h>
#include <sys/exec.h>
#include <sys/mman.h>

#include <vm/vm.h>

/*
 * XXX cgd 960926: this module should collect simple statistics
 * (calls, extends, kills).
 */

#ifdef DEBUG
/*
 * new_vmcmd():
 *	create a new vmcmd structure and fill in its fields based
 *	on function call arguments.  make sure objects ref'd by
 *	the vmcmd are 'held'.
 *
 * If not debugging, this is a macro, so it's expanded inline.
 */

void
new_vmcmd(evsp, proc, len, addr, vp, offset, prot)
	struct	exec_vmcmd_set *evsp;
	int	(*proc) __P((struct proc * p, struct exec_vmcmd *));
	u_long	len;
	u_long	addr;
	struct	vnode *vp;
	u_long	offset;
	u_int	prot;
{
	struct exec_vmcmd    *vcp;

	if (evsp->evs_used >= evsp->evs_cnt)
		vmcmdset_extend(evsp);
	vcp = &evsp->evs_cmds[evsp->evs_used++];
	vcp->ev_proc = proc;
	vcp->ev_len = len;
	vcp->ev_addr = addr;
	if ((vcp->ev_vp = vp) != NULL)
		vref(vp);
	vcp->ev_offset = offset;
	vcp->ev_prot = prot;
}
#endif /* DEBUG */

void
vmcmdset_extend(evsp)
	struct	exec_vmcmd_set *evsp;
{
	struct exec_vmcmd *nvcp;
	u_int ocnt;

#ifdef DIAGNOSTIC
	if (evsp->evs_used < evsp->evs_cnt)
		panic("vmcmdset_extend: not necessary");
#endif

	/* figure out number of entries in new set */
	ocnt = evsp->evs_cnt;
	evsp->evs_cnt += ocnt ? ocnt : EXEC_DEFAULT_VMCMD_SETSIZE;

	/* allocate it */
	MALLOC(nvcp, struct exec_vmcmd *, 
	    (evsp->evs_cnt * sizeof(struct exec_vmcmd)), M_EXEC, M_WAITOK);

	/* free the old struct, if there was one, and record the new one */
	if (ocnt) {
		bcopy(evsp->evs_cmds, nvcp, (ocnt * sizeof(struct exec_vmcmd)));
		FREE(evsp->evs_cmds, M_EXEC);
	}
	evsp->evs_cmds = nvcp;
}

void
kill_vmcmds(evsp)
	struct	exec_vmcmd_set *evsp;
{
	struct exec_vmcmd *vcp;
	int i;

	if (evsp->evs_cnt == 0)
		return;

	for (i = 0; i < evsp->evs_used; i++) {
		vcp = &evsp->evs_cmds[i];
		if (vcp->ev_vp != NULLVP)
			vrele(vcp->ev_vp);
	}
	evsp->evs_used = evsp->evs_cnt = 0;
	FREE(evsp->evs_cmds, M_EXEC);
}

/*
 * vmcmd_map_pagedvn():
 *	handle vmcmd which specifies that a vnode should be mmap'd.
 *	appropriate for handling demand-paged text and data segments.
 */

int
vmcmd_map_pagedvn(p, cmd)
	struct proc *p;
	struct exec_vmcmd *cmd;
{
	/*
	 * note that if you're going to map part of an process as being
	 * paged from a vnode, that vnode had damn well better be marked as
	 * VTEXT.  that's handled in the routine which sets up the vmcmd to
	 * call this routine.
	 */
	return vm_mmap(&p->p_vmspace->vm_map, &cmd->ev_addr, cmd->ev_len,
	    cmd->ev_prot, VM_PROT_ALL, MAP_FIXED|MAP_COPY, (caddr_t)cmd->ev_vp,
	    cmd->ev_offset);
}

/*
 * vmcmd_map_readvn():
 *	handle vmcmd which specifies that a vnode should be read from.
 *	appropriate for non-demand-paged text/data segments, i.e. impure
 *	objects (a la OMAGIC and NMAGIC).
 */
int
vmcmd_map_readvn(p, cmd)
	struct proc *p;
	struct exec_vmcmd *cmd;
{
	int error;

	error = vm_allocate(&p->p_vmspace->vm_map, &cmd->ev_addr,
	    cmd->ev_len, 0);
	if (error)
		return error;

	error = vn_rdwr(UIO_READ, cmd->ev_vp, (caddr_t)cmd->ev_addr,
	    cmd->ev_len, cmd->ev_offset, UIO_USERSPACE, IO_UNIT,
	    p->p_ucred, (int *)0, p);
	if (error)
		return error;

	return vm_map_protect(&p->p_vmspace->vm_map, trunc_page(cmd->ev_addr),
	    round_page(cmd->ev_addr + cmd->ev_len), cmd->ev_prot, FALSE);
}

/*
 * vmcmd_map_zero():
 *	handle vmcmd which specifies a zero-filled address space region.  The
 *	address range must be first allocated, then protected appropriately.
 */

int
vmcmd_map_zero(p, cmd)
	struct proc *p;
	struct exec_vmcmd *cmd;
{
	int error;

	error = vm_allocate(&p->p_vmspace->vm_map, &cmd->ev_addr,
	    cmd->ev_len, 0);
	if (error)
		return error;

	return vm_map_protect(&p->p_vmspace->vm_map, trunc_page(cmd->ev_addr),
	    round_page(cmd->ev_addr + cmd->ev_len), cmd->ev_prot, FALSE);
}
