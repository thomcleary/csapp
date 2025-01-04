	.file	"main.c"
	.text
	.globl	fun
	.type	fun, @function
fun:
.LFB12:
	.cfi_startproc
	endbr64
	movl	$0, %eax
	jmp	.L2
.L3:
	addq	(%rdi), %rax
	movq	8(%rdi), %rdi
.L2:
	testq	%rdi, %rdi
	jne	.L3
	ret
	.cfi_endproc
.LFE12:
	.size	fun, .-fun
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"fun(a): %ld\n"
.LC1:
	.string	"main.c"
.LC2:
	.string	"result == a.v + b.v"
	.text
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$48, %rsp
	.cfi_def_cfa_offset 64
	movq	%fs:40, %rax
	movq	%rax, 40(%rsp)
	xorl	%eax, %eax
	movq	$37, (%rsp)
	movq	$0, 8(%rsp)
	movq	$42, 16(%rsp)
	movq	$0, 24(%rsp)
	leaq	16(%rsp), %rax
	movq	%rax, 8(%rsp)
	movq	%rsp, %rdi
	call	fun
	movq	%rax, %rbx
	movq	%rax, %rdx
	leaq	.LC0(%rip), %rsi
	movl	$2, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movq	16(%rsp), %rax
	addq	(%rsp), %rax
	cmpq	%rbx, %rax
	jne	.L8
	movq	40(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L9
	movl	$0, %eax
	addq	$48, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L8:
	.cfi_restore_state
	leaq	__PRETTY_FUNCTION__.0(%rip), %rcx
	movl	$45, %edx
	leaq	.LC1(%rip), %rsi
	leaq	.LC2(%rip), %rdi
	call	__assert_fail@PLT
.L9:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.section	.rodata
	.type	__PRETTY_FUNCTION__.0, @object
	.size	__PRETTY_FUNCTION__.0, 5
__PRETTY_FUNCTION__.0:
	.string	"main"
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
