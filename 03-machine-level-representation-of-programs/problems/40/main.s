	.file	"main.c"
	.text
	.globl	fix_set_diag_void
	.type	fix_set_diag_void, @function
fix_set_diag_void:
.LFB12:
	.cfi_startproc
	endbr64
	movq	%rdi, %rax
	leaq	1088(%rdi), %rdx
.L2:
	movl	%esi, (%rax)
	addq	$68, %rax
	cmpq	%rdx, %rax
	jne	.L2
	ret
	.cfi_endproc
.LFE12:
	.size	fix_set_diag_void, .-fix_set_diag_void
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d "
.LC1:
	.string	"\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	subq	$1040, %rsp
	.cfi_def_cfa_offset 1088
	movq	%fs:40, %rax
	movq	%rax, 1032(%rsp)
	xorl	%eax, %eax
	movq	%rsp, %rdi
	movl	$128, %ecx
	rep stosq
	movq	%rsp, %rdi
	movl	$1, %esi
	call	fix_set_diag_void
	leaq	64(%rsp), %rbp
	leaq	1088(%rsp), %r14
	leaq	.LC0(%rip), %r12
	leaq	.LC1(%rip), %r13
.L5:
	leaq	-64(%rbp), %rbx
.L6:
	movl	(%rbx), %edx
	movq	%r12, %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	addq	$4, %rbx
	cmpq	%rbp, %rbx
	jne	.L6
	movq	%r13, %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	addq	$64, %rbp
	cmpq	%r14, %rbp
	jne	.L5
	movq	1032(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L11
	movl	$0, %eax
	addq	$1040, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L11:
	.cfi_restore_state
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
