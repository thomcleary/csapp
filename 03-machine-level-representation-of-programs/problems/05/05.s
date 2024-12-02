	.file	"05.c"
	.text
	.globl	decode1
	.type	decode1, @function
decode1:
.LFB12:
	.cfi_startproc
	endbr64
	movq	(%rdi), %r8
	movq	(%rsi), %rcx
	movq	(%rdx), %rax
	movq	%r8, (%rsi)
	movq	%rcx, (%rdx)
	movq	%rax, (%rdi)
	ret
	.cfi_endproc
.LFE12:
	.size	decode1, .-decode1
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	endbr64
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	movq	$1, (%rsp)
	movq	$2, 8(%rsp)
	movq	$3, 16(%rsp)
	leaq	16(%rsp), %rdx
	leaq	8(%rsp), %rsi
	movq	%rsp, %rdi
	call	decode1
	movq	24(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L5
	movl	$0, %eax
	addq	$40, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L5:
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
