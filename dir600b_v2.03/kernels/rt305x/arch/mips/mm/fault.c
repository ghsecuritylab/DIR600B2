/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995 - 2000 by Ralf Baechle
 */
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/smp_lock.h>
#include <linux/vt_kern.h>		/* For unblank_screen() */
#include <linux/module.h>

#include <asm/branch.h>
#include <asm/mmu_context.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/ptrace.h>
#include <asm/highmem.h>		/* For VMALLOC_END */
#ifdef ELBOX_KERNEL_PANIC_FREEZE
/*
 * Kide@2008/07/04:
 * Save coredump to file as process running in user space access illegal memory.
 */
void do_coredump_to_file(struct pt_regs *regs, unsigned long write, unsigned long address)
{
	struct task_struct *tsk = current;
	struct file *pfile = NULL;
	char fname[65], coremsg[256];

	sprintf(fname, "/tmp/%s_coredump", tsk->comm);
	pfile = filp_open(fname, O_CREAT | O_NOFOLLOW | FMODE_WRITE, 0600);
	if (IS_ERR(pfile))
		return;
	memset(coremsg, 0, sizeof(coremsg));
	sprintf(coremsg, "%s(pid:%d) illegal %s 0x%08lx (epc=0x%08lx, ra=0x%08lx)\n",
		tsk->comm, tsk->pid, write ? "access to" : "access from",
		address, (unsigned long)regs->cp0_epc, (unsigned long)regs->regs[31]);

	if (pfile->f_op && pfile->f_op->write != NULL)
	{
		mm_segment_t oldfs;
		int	i = 0, ret = 0;
		long	*sp;
		
		pfile->f_pos = 0;
		oldfs = get_fs(); set_fs(KERNEL_DS);
		ret = pfile->f_op->write(pfile, coremsg, strlen(coremsg), &pfile->f_pos);
		pfile->f_pos += ret;
		
		/* dump stack */
		ret = pfile->f_op->write(pfile, "Stack:\n", strlen("Stack:\n"), &pfile->f_pos);
		pfile->f_pos += ret;

		sp = (long *)regs->regs[29];
		memset(coremsg, 0, sizeof(coremsg));
		while ((long) sp & (PAGE_SIZE - 1))
		{
			long	stackdata;
			
			if (i && ((i % 8) == 0))
			{
				coremsg[72] = '\n';
				ret = pfile->f_op->write(pfile, coremsg, 73, &pfile->f_pos);
				pfile->f_pos += ret;
				memset(coremsg, 0, sizeof(coremsg));
			}
			if (i > 40)
				break;
			
			if (__get_user(stackdata, sp++))
				break;
			
			sprintf(&coremsg[(i%8)*9], " %08lx", stackdata);
			i++;
		} // end of while loop
		pfile->f_op->write(pfile, " ...\n", strlen(" ...\n"), &pfile->f_pos);
		set_fs(oldfs);
	}
	filp_close(pfile, NULL);
}
#endif
/*
 * This routine handles page faults.  It determines the address,
 * and the problem, and then passes it off to one of the appropriate
 * routines.
 */
asmlinkage void do_page_fault(struct pt_regs *regs, unsigned long write,
			      unsigned long address)
{
	struct vm_area_struct * vma = NULL;
	struct task_struct *tsk = current;
	struct mm_struct *mm = tsk->mm;
	const int field = sizeof(unsigned long) * 2;
	siginfo_t info;

#if 0
	printk("Cpu%d[%s:%d:%0*lx:%ld:%0*lx]\n", raw_smp_processor_id(),
	       current->comm, current->pid, field, address, write,
	       field, regs->cp0_epc);
#endif

	info.si_code = SEGV_MAPERR;

	/*
	 * We fault-in kernel-space virtual memory on-demand. The
	 * 'reference' page table is init_mm.pgd.
	 *
	 * NOTE! We MUST NOT take any locks for this case. We may
	 * be in an interrupt or a critical region, and should
	 * only copy the information from the master page table,
	 * nothing more.
	 */
	if (unlikely(address >= VMALLOC_START && address <= VMALLOC_END))
		goto vmalloc_fault;
#ifdef MODULE_START
	if (unlikely(address >= MODULE_START && address < MODULE_END))
		goto vmalloc_fault;
#endif

	/*
	 * If we're in an interrupt or have no user
	 * context, we must not take the fault..
	 */
	if (in_atomic() || !mm)
		goto bad_area_nosemaphore;

	down_read(&mm->mmap_sem);
	vma = find_vma(mm, address);
	if (!vma)
		goto bad_area;
	if (vma->vm_start <= address)
		goto good_area;
	if (!(vma->vm_flags & VM_GROWSDOWN))
		goto bad_area;
	if (expand_stack(vma, address))
		goto bad_area;
/*
 * Ok, we have a good vm_area for this memory access, so
 * we can handle it..
 */
good_area:
	info.si_code = SEGV_ACCERR;

	if (write) {
		if (!(vma->vm_flags & VM_WRITE))
			goto bad_area;
	} else {
		if (!(vma->vm_flags & (VM_READ | VM_WRITE | VM_EXEC)))
			goto bad_area;
	}

survive:
	/*
	 * If for any reason at all we couldn't handle the fault,
	 * make sure we exit gracefully rather than endlessly redo
	 * the fault.
	 */
	switch (handle_mm_fault(mm, vma, address, write)) {
	case VM_FAULT_MINOR:
		tsk->min_flt++;
		break;
	case VM_FAULT_MAJOR:
		tsk->maj_flt++;
		break;
	case VM_FAULT_SIGBUS:
		goto do_sigbus;
	case VM_FAULT_OOM:
		goto out_of_memory;
	default:
		BUG();
	}

	up_read(&mm->mmap_sem);
	return;

/*
 * Something tried to access memory that isn't in our memory map..
 * Fix it, but check if it's kernel or user first..
 */
bad_area:
	up_read(&mm->mmap_sem);

bad_area_nosemaphore:
	/* User mode accesses just cause a SIGSEGV */
	if (user_mode(regs)) {
		tsk->thread.cp0_badvaddr = address;
		tsk->thread.error_code = write;
#if 0
		printk("do_page_fault() #2: sending SIGSEGV to %s for "
		       "invalid %s\n%0*lx (epc == %0*lx, ra == %0*lx)\n",
		       tsk->comm,
		       write ? "write access to" : "read access from",
		       field, address,
		       field, (unsigned long) regs->cp0_epc,
		       field, (unsigned long) regs->regs[31]);
#endif
#ifdef ELBOX_KERNEL_PANIC_FREEZE
		do_coredump_to_file(regs, write, address);
#endif
		info.si_signo = SIGSEGV;
		info.si_errno = 0;
		/* info.si_code has been set above */
		info.si_addr = (void __user *) address;
		force_sig_info(SIGSEGV, &info, tsk);
		return;
	}

no_context:
	/* Are we prepared to handle this kernel fault?  */
	if (fixup_exception(regs)) {
		current->thread.cp0_baduaddr = address;
		return;
	}

	/*
	 * Oops. The kernel tried to access some bad page. We'll have to
	 * terminate things with extreme prejudice.
	 */
	bust_spinlocks(1);

	printk(KERN_ALERT "CPU %d Unable to handle kernel paging request at "
	       "virtual address %0*lx, epc == %0*lx, ra == %0*lx\n",
	       raw_smp_processor_id(), field, address, field, regs->cp0_epc,
	       field,  regs->regs[31]);
	die("Oops", regs);

/*
 * We ran out of memory, or some other thing happened to us that made
 * us unable to handle the page fault gracefully.
 */
out_of_memory:
	up_read(&mm->mmap_sem);
	if (is_init(tsk)) {
		yield();
		down_read(&mm->mmap_sem);
		goto survive;
	}
	printk("VM: killing process %s\n", tsk->comm);
	if (user_mode(regs))
	{
#ifdef ELBOX_KERNEL_PANIC_FREEZE
		do_coredump_to_file(regs, write, address);
#endif
		do_exit(SIGKILL);
	}
	goto no_context;

do_sigbus:
	up_read(&mm->mmap_sem);

	/* Kernel mode? Handle exceptions or die */
	if (!user_mode(regs))
		goto no_context;
	else
	/*
	 * Send a sigbus, regardless of whether we were in kernel
	 * or user mode.
	 */
#if 0
		printk("do_page_fault() #3: sending SIGBUS to %s for "
		       "invalid %s\n%0*lx (epc == %0*lx, ra == %0*lx)\n",
		       tsk->comm,
		       write ? "write access to" : "read access from",
		       field, address,
		       field, (unsigned long) regs->cp0_epc,
		       field, (unsigned long) regs->regs[31]);
#endif
	tsk->thread.cp0_badvaddr = address;
	info.si_signo = SIGBUS;
	info.si_errno = 0;
	info.si_code = BUS_ADRERR;
	info.si_addr = (void __user *) address;
	force_sig_info(SIGBUS, &info, tsk);

#ifdef ELBOX_KERNEL_PANIC_FREEZE
	do_coredump_to_file(regs, write, address);
#endif

	return;
vmalloc_fault:
	{
		/*
		 * Synchronize this task's top level page-table
		 * with the 'reference' page table.
		 *
		 * Do _not_ use "tsk" here. We might be inside
		 * an interrupt in the middle of a task switch..
		 */
		int offset = __pgd_offset(address);
		pgd_t *pgd, *pgd_k;
		pud_t *pud, *pud_k;
		pmd_t *pmd, *pmd_k;
		pte_t *pte_k;

		pgd = (pgd_t *) pgd_current[raw_smp_processor_id()] + offset;
		pgd_k = init_mm.pgd + offset;

		if (!pgd_present(*pgd_k))
			goto no_context;
		set_pgd(pgd, *pgd_k);

		pud = pud_offset(pgd, address);
		pud_k = pud_offset(pgd_k, address);
		if (!pud_present(*pud_k))
			goto no_context;

		pmd = pmd_offset(pud, address);
		pmd_k = pmd_offset(pud_k, address);
		if (!pmd_present(*pmd_k))
			goto no_context;
		set_pmd(pmd, *pmd_k);

		pte_k = pte_offset_kernel(pmd_k, address);
		if (!pte_present(*pte_k))
			goto no_context;
		return;
	}
}
