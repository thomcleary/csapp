	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 15, 0	sdk_version 15, 2
	.globl	_cread                          ## -- Begin function cread
	.p2align	4, 0x90
_cread:                                 ## @cread
	.cfi_startproc
## %bb.0:
	testq	%rdi, %rdi
	je	LBB0_1
## %bb.2:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	(%rdi), %rax
	popq	%rbp
	retq
LBB0_1:
	xorl	%eax, %eax
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_cread_alt                      ## -- Begin function cread_alt
	.p2align	4, 0x90
_cread_alt:                             ## @cread_alt
	.cfi_startproc
## %bb.0:
	testq	%rdi, %rdi
	je	LBB1_1
## %bb.2:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	(%rdi), %rcx
	popq	%rbp
	jmp	LBB1_3
LBB1_1:
	xorl	%ecx, %ecx
LBB1_3:
	xorl	%eax, %eax
	testq	%rcx, %rcx
	cmovgq	%rcx, %rax
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
.subsections_via_symbols
