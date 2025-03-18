# Bomblab

## Symbol Table

```shell
$ objdump -t bomb
```

## Disassemble

```shell
$ objdump -d bomb > disassembly.asm
$ objdump -s -j .rodata bomb >> disassembly.asm
$ objdump -d -j .data bomb >> disassembly.asm
```

## Strings

```shell
strings bomb
```

## Phase 1

```shell
root in csapp/labs/bomblab on  main via C v13.3.0-gcc took 32s
🐳 ❯ gdb bomb
GNU gdb (Ubuntu 15.0.50.20240403-0ubuntu1) 15.0.50.20240403-git
Copyright (C) 2024 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from bomb...
(gdb) break phase_1
Breakpoint 1 at 0x400ee0
(gdb) run
Starting program: /csapp/labs/bomblab/bomb
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!
Pls dont blow up

Breakpoint 1, 0x0000000000400ee0 in phase_1 ()
(gdb) x/s $edi
0x603780 <input_strings>:       "Pls dont blow up"
(gdb) disas /r
Dump of assembler code for function phase_1:
=> 0x0000000000400ee0 <+0>:     48 83 ec 08             sub    $0x8,%rsp
   0x0000000000400ee4 <+4>:     be 00 24 40 00          mov    $0x402400,%esi
   0x0000000000400ee9 <+9>:     e8 4a 04 00 00          call   0x401338 <strings_not_equal>
   0x0000000000400eee <+14>:    85 c0                   test   %eax,%eax
   0x0000000000400ef0 <+16>:    74 05                   je     0x400ef7 <phase_1+23>
   0x0000000000400ef2 <+18>:    e8 43 05 00 00          call   0x40143a <explode_bomb>
   0x0000000000400ef7 <+23>:    48 83 c4 08             add    $0x8,%rsp
   0x0000000000400efb <+27>:    c3                      ret
End of assembler dump.
(gdb) stepi
0x0000000000400ee4 in phase_1 ()
(gdb) stepi
0x0000000000400ee9 in phase_1 ()
(gdb) x/s $rsi
0x402400:       "Border relations with Canada have never been better."
(gdb) set $edi = $esi
(gdb) x/s $edi
0x402400:       "Border relations with Canada have never been better."
(gdb) continue
Continuing.
Phase 1 defused. How about the next one?
```

## Phase 2

```asm
# %rdi = char *input
0000000000400efc <phase_2>:
  400efc:	55                   	push   %rbp                         # push %rbp (base pointer)
  400efd:	53                   	push   %rbx                         # push %rbx (callee saved)
  400efe:	48 83 ec 28          	sub    $0x28,%rsp                   # Create stack frame of 40 bytes
  400f02:	48 89 e6             	mov    %rsp,%rsi                    # rsi = rsp (%rsi = int numbers[6])
  400f05:	e8 52 05 00 00       	call   40145c <read_six_numbers>    # call read_six_numbers(input, &numbers)
  400f0a:	83 3c 24 01          	cmpl   $0x1,(%rsp)                  # compare (numbers[0] - 1)
  400f0e:	74 20                	je     400f30 <phase_2+0x34>        # if (numbers[0] - 1 == 0): goto 400f30
  400f10:	e8 25 05 00 00       	call   40143a <explode_bomb>        # else: explode_bomb()
  400f15:	eb 19                	jmp    400f30 <phase_2+0x34>
  400f17:	8b 43 fc             	mov    -0x4(%rbx),%eax              # rax = numbers[curr_num - 1] (previous number)
  400f1a:	01 c0                	add    %eax,%eax                    # rax += rax
  400f1c:	39 03                	cmp    %eax,(%rbx)                  # compare (numbers[curr_num] - numbers[curr_num - 1])
  400f1e:	74 05                	je     400f25 <phase_2+0x29>        # if compare == 0: goto 400f25
  400f20:	e8 15 05 00 00       	call   40143a <explode_bomb>        # else: explode_bomb()
  400f25:	48 83 c3 04          	add    $0x4,%rbx                    # curr_num = curr_num + 1
  400f29:	48 39 eb             	cmp    %rbp,%rbx                    # compare (curr_num - numbers.length)
  400f2c:	75 e9                	jne    400f17 <phase_2+0x1b>        # if (curr_num < numbers[numbers.length]): goto 400f17
  400f2e:	eb 0c                	jmp    400f3c <phase_2+0x40>        # else: goto 400f3c
  400f30:	48 8d 5c 24 04       	lea    0x4(%rsp),%rbx               # rbx = numbers[1]
  400f35:	48 8d 6c 24 18       	lea    0x18(%rsp),%rbp              # rbp = numbers[numbers.length] (out of bounds)
  400f3a:	eb db                	jmp    400f17 <phase_2+0x1b>        # goto 400f17 (loop?)
  400f3c:	48 83 c4 28          	add    $0x28,%rsp                   # restore stack
  400f40:	5b                   	pop    %rbx                         # restore callee saved register rbx
  400f41:	5d                   	pop    %rbp                         # restore callee saved register rbp
  400f42:	c3                   	ret                                 # return
```

```C
void phase_2(char *input, int numbers[6]) {
    for (int i = 1; i < 6; i++) {
        int current = numbers[i];
        int previous = numbers[i - 1];

        if (previous + previous != current) {
            explode_bomb();
        }
    }
}
```

## Phase 3

```asm
# void phase_3(char *input)
# input in %rdi
0000000000400f43 <phase_3>:
  400f43:	48 83 ec 18          	sub    $0x18,%rsp                       # Add 24 bytes to stack frame
  400f47:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx                   # rcx = rsp+12 (int y;)
  400f4c:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx                   # rdx = rsp+8  (int x;)
  400f51:	be cf 25 40 00       	mov    $0x4025cf,%esi                   # rsi = address of "%d %d" (char *format)
  400f56:	b8 00 00 00 00       	mov    $0x0,%eax                        # rax = 0
  400f5b:	e8 90 fc ff ff       	call   400bf0 <__isoc99_sscanf@plt>     # sscanf(input, format, &x, &y);
  400f60:	83 f8 01             	cmp    $0x1,%eax                        # compare (rax - 1)
  400f63:	7f 05                	jg     400f6a <phase_3+0x27>            # if rax > 1: goto 400f6a
  400f65:	e8 d0 04 00 00       	call   40143a <explode_bomb>            # else: explode_bomb()
  400f6a:	83 7c 24 08 07       	cmpl   $0x7,0x8(%rsp)                   # compare (x - 7)
  400f6f:	77 3c                	ja     400fad <phase_3+0x6a>            # if (unsigned)x > 7: goto 400fad
  400f71:	8b 44 24 08          	mov    0x8(%rsp),%eax                   # else: rax = x
  400f75:	ff 24 c5 70 24 40 00 	jmp    *0x402470(,%rax,8)               # goto jump_table[x * 8]
  400f7c:	b8 cf 00 00 00       	mov    $0xcf,%eax                       # case 0: rax = 207
  400f81:	eb 3b                	jmp    400fbe <phase_3+0x7b>            # goto 400fbe
  400f83:	b8 c3 02 00 00       	mov    $0x2c3,%eax
  400f88:	eb 34                	jmp    400fbe <phase_3+0x7b>
  400f8a:	b8 00 01 00 00       	mov    $0x100,%eax
  400f8f:	eb 2d                	jmp    400fbe <phase_3+0x7b>
  400f91:	b8 85 01 00 00       	mov    $0x185,%eax
  400f96:	eb 26                	jmp    400fbe <phase_3+0x7b>
  400f98:	b8 ce 00 00 00       	mov    $0xce,%eax
  400f9d:	eb 1f                	jmp    400fbe <phase_3+0x7b>
  400f9f:	b8 aa 02 00 00       	mov    $0x2aa,%eax
  400fa4:	eb 18                	jmp    400fbe <phase_3+0x7b>
  400fa6:	b8 47 01 00 00       	mov    $0x147,%eax
  400fab:	eb 11                	jmp    400fbe <phase_3+0x7b>
  400fad:	e8 88 04 00 00       	call   40143a <explode_bomb>            # explode_bomb()
  400fb2:	b8 00 00 00 00       	mov    $0x0,%eax
  400fb7:	eb 05                	jmp    400fbe <phase_3+0x7b>
  400fb9:	b8 37 01 00 00       	mov    $0x137,%eax
  400fbe:	3b 44 24 0c          	cmp    0xc(%rsp),%eax                   # compare (rax - y)
  400fc2:	74 05                	je     400fc9 <phase_3+0x86>            # if rax == y: goto 400fc9
  400fc4:	e8 71 04 00 00       	call   40143a <explode_bomb>            # else: explode_bomb()
  400fc9:	48 83 c4 18          	add    $0x18,%rsp                       # restore stack
  400fcd:	c3                   	ret                                     # return
  ...
Contents of section .rodata:
   ...
   402470 7c0f4000 00000000 b90f4000 00000000  |.@.......@.....
   402480 830f4000 00000000 8a0f4000 00000000  ..@.......@.....
   402490 910f4000 00000000 980f4000 00000000  ..@.......@.....
   4024a0 9f0f4000 00000000 a60f4000 00000000  ..@.......@.....
```

```C
void phase_3(char *input) {
    int x, y, z;
    sscanf(input, "%d %d", &x, &y);

    switch (x) {
        case 0:
            z = 207;
            break
        case 1:
            ...
        case 2:
            ...
        case 3:
            ...
        case 4:
            ...
        case 5:
            ...
        case 6:
            ...
        case 7:
            ...
        default:
            explode_bomb();
    }

    if (y != z) {
        explode_bomb();
    }
}
```

## Phase 4

```asm
# int func4(int x, int y, int z)
0000000000400fce <func4>:
  400fce:	48 83 ec 08          	sub    $0x8,%rsp                        # Allocate stack frame (8 bytes)
  400fd2:	89 d0                	mov    %edx,%eax                        # rax = z
  400fd4:	29 f0                	sub    %esi,%eax                        # rax = z - y
  400fd6:	89 c1                	mov    %eax,%ecx                        # rcx = rax
  400fd8:	c1 e9 1f             	shr    $0x1f,%ecx                       # rcx = (z - y) >> 0x1f = (z - y) >> 31
  400fdb:	01 c8                	add    %ecx,%eax                        # rax = (z - y) + (sign of z-y)
  400fdd:	d1 f8                	sar    $1,%eax                          # rax = rax >> 1 (arithmetic shift)
  400fdf:	8d 0c 30             	lea    (%rax,%rsi,1),%ecx               # rcx = rax + y
  400fe2:	39 f9                	cmp    %edi,%ecx                        # compare (x - rcx)
  400fe4:	7e 0c                	jle    400ff2 <func4+0x24>              # if (x <= rcx): goto 400ff2
  400fe6:	8d 51 ff             	lea    -0x1(%rcx),%edx                  # z = rcx - 1
  400fe9:	e8 e0 ff ff ff       	call   400fce <func4>                   # func4(x, y, z)
  400fee:	01 c0                	add    %eax,%eax                        # rax += rax
  400ff0:	eb 15                	jmp    401007 <func4+0x39>              # goto 401007 (return)
  400ff2:	b8 00 00 00 00       	mov    $0x0,%eax                        # rax = 0
  400ff7:	39 f9                	cmp    %edi,%ecx                        # compare (rcx - x)
  400ff9:	7d 0c                	jge    401007 <func4+0x39>              # if (rcx >= x): goto 401007 (return) - want to hit here so rax == 0
  400ffb:	8d 71 01             	lea    0x1(%rcx),%esi                   # y = rcx + 1
  400ffe:	e8 cb ff ff ff       	call   400fce <func4>                   # func4(x, y, z)
  401003:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax            # rax = rax + rax + 1
  401007:	48 83 c4 08          	add    $0x8,%rsp                        # deallocate stack frame
  40100b:	c3                   	ret                                     # return

000000000040100c <phase_4>:
  40100c:	48 83 ec 18          	sub    $0x18,%rsp                       # Allocate stack frame (24 bytes)
  401010:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx                   # y = rsp+12 (int *)
  401015:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx                   # x = rsp+8 (int *)
  40101a:	be cf 25 40 00       	mov    $0x4025cf,%esi                   # char *format = "%d %d"
  40101f:	b8 00 00 00 00       	mov    $0x0,%eax                        # rax = 0
  401024:	e8 c7 fb ff ff       	call   400bf0 <__isoc99_sscanf@plt>     # sscanf(input, format, x, y)
  401029:	83 f8 02             	cmp    $0x2,%eax                        # compare (rax - 2)
  40102c:	75 07                	jne    401035 <phase_4+0x29>            # if (rax != 2): goto 401035
  40102e:	83 7c 24 08 0e       	cmpl   $0xe,0x8(%rsp)                   # else: compare (x - 14)
  401033:	76 05                	jbe    40103a <phase_4+0x2e>            #   if (x <= 14): goto 40103a
  401035:	e8 00 04 00 00       	call   40143a <explode_bomb>            # explode_bomb()
  40103a:	ba 0e 00 00 00       	mov    $0xe,%edx                        # rdx = 14;
  40103f:	be 00 00 00 00       	mov    $0x0,%esi                        # rsi = 0
  401044:	8b 7c 24 08          	mov    0x8(%rsp),%edi                   # rdi = x;
  401048:	e8 81 ff ff ff       	call   400fce <func4>                   # func4(rdi, rsi, rdx)
  40104d:	85 c0                	test   %eax,%eax                        # test (rax & rax)
  40104f:	75 07                	jne    401058 <phase_4+0x4c>            # if (rax != 0): goto 401058
  401051:	83 7c 24 0c 00       	cmpl   $0x0,0xc(%rsp)                   # compare (y - 0)
  401056:	74 05                	je     40105d <phase_4+0x51>            # if y == 0: goto 40105d
  401058:	e8 dd 03 00 00       	call   40143a <explode_bomb>            # explode_bomb()
  40105d:	48 83 c4 18          	add    $0x18,%rsp                       # deallocate stack frame
  401061:	c3                   	ret                                     # return
```

### Notes

- requires input of 2 numbers 'x y'

- x must be less than or equal to 14

```asm
40102e:	83 7c 24 08 0e       	cmpl   $0xe,0x8(%rsp)                   # else: compare (x - 14)
401033:	76 05                	jbe    40103a <phase_4+0x2e>            #   if (x <= 14): goto 40103a
401035:	e8 00 04 00 00       	call   40143a <explode_bomb>            # explode_bomb()
```

- y must be 0

```asm
401056:	74 05                	je     40105d <phase_4+0x51>            # if y == 0: goto 40105d
401058:	e8 dd 03 00 00       	call   40143a <explode_bomb>            # explode_bomb()
```

## Phase 5

```asm
0000000000401062 <phase_5>:
  401062:	53                   	push   %rbx                             # save callee saved register
  401063:	48 83 ec 20          	sub    $0x20,%rsp                       # allocate stack frame (32 bytes)
  401067:	48 89 fb             	mov    %rdi,%rbx                        # rbx = input (char *)
  40106a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax                    # rax = *(fs+28) (thread local storage)
  401071:	00 00
  401073:	48 89 44 24 18       	mov    %rax,0x18(%rsp)                  # rsp+24 = rax
  401078:	31 c0                	xor    %eax,%eax                        # rax = 0
  40107a:	e8 9c 02 00 00       	call   40131b <string_length>           # string_length(input)
  40107f:	83 f8 06             	cmp    $0x6,%eax                        # compare (rax - 6)
  401082:	74 4e                	je     4010d2 <phase_5+0x70>            # if (rax == 6): goto 4010d2
  401084:	e8 b1 03 00 00       	call   40143a <explode_bomb>            # else: explode_bomb()
  401089:	eb 47                	jmp    4010d2 <phase_5+0x70>
  40108b:	0f b6 0c 03          	movzbl (%rbx,%rax,1),%ecx               # rcx = input[0]
  40108f:	88 0c 24             	mov    %cl,(%rsp)                       # *rsp = input[0]
  401092:	48 8b 14 24          	mov    (%rsp),%rdx                      # rdx = input[0]
  401096:	83 e2 0f             	and    $0xf,%edx                        # rdx = input[0] & 15 (bottom 4 bits of input[0]
  401099:	0f b6 92 b0 24 40 00 	movzbl 0x4024b0(%rdx),%edx              # rdx = 0x4024b0 + rdx (cipher_str[rdx])
  4010a0:	88 54 04 10          	mov    %dl,0x10(%rsp,%rax,1)            # *(rsp+16 + rax) = cipher_str[rdx] (lower byte of rdx)
  4010a4:	48 83 c0 01          	add    $0x1,%rax                        # rax += 1
  4010a8:	48 83 f8 06          	cmp    $0x6,%rax                        # (rax - 6)
  4010ac:	75 dd                	jne    40108b <phase_5+0x29>            # if (rax != 6): goto 40108b
  4010ae:	c6 44 24 16 00       	movb   $0x0,0x16(%rsp)                  # else: *(rsp+22) = 0 (add '\0' to end of string)
  4010b3:	be 5e 24 40 00       	mov    $0x40245e,%esi                   # rsi = 0x40245e (secret_str)
  4010b8:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi                  # rdi = rsp+16 (generated_str)
  4010bd:	e8 76 02 00 00       	call   401338 <strings_not_equal>       # strings_not_equal(generated_str, secret_str)
  4010c2:	85 c0                	test   %eax,%eax                        # test (rax & rax)
  4010c4:	74 13                	je     4010d9 <phase_5+0x77>            # if (strings_not_equal(generated_str, secret_str) == 0): goto 4010d9
  4010c6:	e8 6f 03 00 00       	call   40143a <explode_bomb>            # else: explode_bomb()
  4010cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  4010d0:	eb 07                	jmp    4010d9 <phase_5+0x77>
  4010d2:	b8 00 00 00 00       	mov    $0x0,%eax                        # rax = 0
  4010d7:	eb b2                	jmp    40108b <phase_5+0x29>            # goto 40108b
  4010d9:	48 8b 44 24 18       	mov    0x18(%rsp),%rax                  # rax = rsp+24
  4010de:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax                    #
  4010e5:	00 00                                                         #
  4010e7:	74 05                	je     4010ee <phase_5+0x8c>            #
  4010e9:	e8 42 fa ff ff       	call   400b30 <__stack_chk_fail@plt>    #
  4010ee:	48 83 c4 20          	add    $0x20,%rsp                       # deallocate stack frame
  4010f2:	5b                   	pop    %rbx                             # restore registers
  4010f3:	c3                   	ret                                     # return
```

### Notes

- input must be 6 characters long

```asm
401078:	31 c0                	xor    %eax,%eax                        # rax = 0
40107a:	e8 9c 02 00 00       	call   40131b <string_length>           # string_length(input)
40107f:	83 f8 06             	cmp    $0x6,%eax                        # compare (rax - 6)
401082:	74 4e                	je     4010d2 <phase_5+0x70>            # if (rax == 6): goto 4010d2
401084:	e8 b1 03 00 00       	call   40143a <explode_bomb>            # else: explode_bomb()
```

- `char[] cipher` at 0x4024b0

```shell
(gdb) x 0x4024b0
0x4024b0 <array.3449>:  "maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?"
```

- `char[] secret` at 0x40245e

```shell
(gdb) x/s 0x40245e
0x40245e:       "flyers"
```

### Cipher

| Char | Offset |
| ---- | ------ |
| m    | 0      |
| a    | 1      |
| d    | 2      |
| u    | 3      |
| i    | 4      |
| e    | 5      |
| r    | 6      |
| s    | 7      |
| n    | 8      |
| f    | 9      |
| o    | 10     |
| t    | 11     |
| v    | 12     |
| b    | 13     |
| y    | 14     |
| l    | 15     |

- `flyers` == `[9][15][14][5][6][7]`
- `0x9`,`0xf`,`0xe`,`0x5`,`0x6`,`0x7`
- Each input character's last 4 bits must match the above
- `i`,`o`,`n`,`e`,`f`,`g`
- `0x69`,`0x6f`,`0x6e`,`0x65`,`0x66`,`0x67`

## Phase 6

```asm
00000000004010f4 <phase_6>:
  4010f4:	41 56                	push   %r14                         # save callee-saved registers
  4010f6:	41 55                	push   %r13                         #
  4010f8:	41 54                	push   %r12                         #
  4010fa:	55                   	push   %rbp                         #
  4010fb:	53                   	push   %rbx                         #
  4010fc:	48 83 ec 50          	sub    $0x50,%rsp                   # allocate stack frame (80 bytes)
  401100:	49 89 e5             	mov    %rsp,%r13                    # r13 = rsp
  401103:	48 89 e6             	mov    %rsp,%rsi                    # rsi = rsp (numbers[6])
  401106:	e8 51 03 00 00       	call   40145c <read_six_numbers>    # read_six_numbers(rdi, rsi)
  40110b:	49 89 e6             	mov    %rsp,%r14                    # r14 = rsp
  40110e:	41 bc 00 00 00 00    	mov    $0x0,%r12d                   # r12 = 0
  401114:	4c 89 ed             	mov    %r13,%rbp                    # rbp = r13
  401117:	41 8b 45 00          	mov    0x0(%r13),%eax               # rax = r13
  40111b:	83 e8 01             	sub    $0x1,%eax                    # rax -= 1
  40111e:	83 f8 05             	cmp    $0x5,%eax                    # compare (rax - 5)
  401121:	76 05                	jbe    401128 <phase_6+0x34>        # if rax <= 5: goto 401128 (if first number <= 6)
  401123:	e8 12 03 00 00       	call   40143a <explode_bomb>        # else: explode_bomb()
  401128:	41 83 c4 01          	add    $0x1,%r12d                   # r12 += 1
  40112c:	41 83 fc 06          	cmp    $0x6,%r12d                   # compare (r12 - 6)
  401130:	74 21                	je     401153 <phase_6+0x5f>        # if r2 == 6: goto 401153
  401132:	44 89 e3             	mov    %r12d,%ebx                   # rbx = r12
  401135:	48 63 c3             	movslq %ebx,%rax                    # rax = rax << rbx
  401138:	8b 04 84             	mov    (%rsp,%rax,4),%eax           # rax = rsp + 4*rax
  40113b:	39 45 00             	cmp    %eax,0x0(%rbp)               # compare (*rbp - rax)
  40113e:	75 05                	jne    401145 <phase_6+0x51>        # if *rbp - rax != 0: goto 401145
  401140:	e8 f5 02 00 00       	call   40143a <explode_bomb>        # else: explode_bomb()
  401145:	83 c3 01             	add    $0x1,%ebx                    # rbx += 1
  401148:	83 fb 05             	cmp    $0x5,%ebx                    # compare (rbx - 5)
  40114b:	7e e8                	jle    401135 <phase_6+0x41>        # if rbx - 5 <= 0: goto 401135
  40114d:	49 83 c5 04          	add    $0x4,%r13                    # r13 += 4
  401151:	eb c1                	jmp    401114 <phase_6+0x20>        # goto 401114
  401153:	48 8d 74 24 18       	lea    0x18(%rsp),%rsi              # rsi = rsp+24 (numbers[5 + 1] out of bounds)
  401158:	4c 89 f0             	mov    %r14,%rax                    # rax = r14
  40115b:	b9 07 00 00 00       	mov    $0x7,%ecx                    # rcx = 7
  401160:	89 ca                	mov    %ecx,%edx                    # rdx = rcx
  401162:	2b 10                	sub    (%rax),%edx                  # rdx -= *rax
  401164:	89 10                	mov    %edx,(%rax)                  # *rax = rdx
  401166:	48 83 c0 04          	add    $0x4,%rax                    # rax += 4 (move to next int?)
  40116a:	48 39 f0             	cmp    %rsi,%rax                    # compare (rax - rsi)
  40116d:	75 f1                	jne    401160 <phase_6+0x6c>        # if (rax - rsi != 0): goto 401160 (map each int in numbers 7-numbers[i])
  40116f:	be 00 00 00 00       	mov    $0x0,%esi                    # rsi = 0
  401174:	eb 21                	jmp    401197 <phase_6+0xa3>        # goto 401197
  401176:	48 8b 52 08          	mov    0x8(%rdx),%rdx               # rdx = (rdx+8) (address of next node)
  40117a:	83 c0 01             	add    $0x1,%eax                    # rax += 1 (where was this set before?, is it the last element of numbers?)
  40117d:	39 c8                	cmp    %ecx,%eax                    # compare (rax - rcx) (was rcx set as the first element of numbers?)
  40117f:	75 f5                	jne    401176 <phase_6+0x82>        # if rax - rcx != 0: goto 401176 (while rax != rcx) { rax++ }
  401181:	eb 05                	jmp    401188 <phase_6+0x94>        # else: goto 401188
  401183:	ba d0 32 60 00       	mov    $0x6032d0,%edx               #
  401188:	48 89 54 74 20       	mov    %rdx,0x20(%rsp,%rsi,2)       # *(rsp+32 + 2*rsi) = rdx
  40118d:	48 83 c6 04          	add    $0x4,%rsi                    # rsi += 4
  401191:	48 83 fe 18          	cmp    $0x18,%rsi                   # compare (rsi - 24)
  401195:	74 14                	je     4011ab <phase_6+0xb7>        # if (rsi - 24 == 0): goto 4011ab
  401197:	8b 0c 34             	mov    (%rsp,%rsi,1),%ecx           # rcx = *(rsp+rsi)
  40119a:	83 f9 01             	cmp    $0x1,%ecx                    # compare (rcx - 1)
  40119d:	7e e4                	jle    401183 <phase_6+0x8f>        # if (rcx <= 1): goto 401183
  40119f:	b8 01 00 00 00       	mov    $0x1,%eax                    # else: rax = 1
  4011a4:	ba d0 32 60 00       	mov    $0x6032d0,%edx               # rdx = 0x6032d0
  4011a9:	eb cb                	jmp    401176 <phase_6+0x82>        # goto: 401176
  4011ab:	48 8b 5c 24 20       	mov    0x20(%rsp),%rbx              # rbx = *(rsp+32)
  4011b0:	48 8d 44 24 28       	lea    0x28(%rsp),%rax              # rax = *(rsp+40)
  4011b5:	48 8d 74 24 50       	lea    0x50(%rsp),%rsi              # rsi = *(rsp+80)
  4011ba:	48 89 d9             	mov    %rbx,%rcx                    # rcx = rbx
  4011bd:	48 8b 10             	mov    (%rax),%rdx                  # rdx = *rax
  4011c0:	48 89 51 08          	mov    %rdx,0x8(%rcx)               # *(rcx+8) = rdx
  4011c4:	48 83 c0 08          	add    $0x8,%rax                    # rax += 8
  4011c8:	48 39 f0             	cmp    %rsi,%rax                    # compare (rax - rsi)
  4011cb:	74 05                	je     4011d2 <phase_6+0xde>        # if (rax - rsi == 0): goto 4011d2
  4011cd:	48 89 d1             	mov    %rdx,%rcx                    # else: ...
  4011d0:	eb eb                	jmp    4011bd <phase_6+0xc9>
  4011d2:	48 c7 42 08 00 00 00 	movq   $0x0,0x8(%rdx)               # *(rdx+8) = 0 (set node1.next = NULL)
  4011d9:	00                                                        #
  4011da:	bd 05 00 00 00       	mov    $0x5,%ebp                    # rbp = 5
  4011df:	48 8b 43 08          	mov    0x8(%rbx),%rax               # rax = *(rbx+8) (rax = node6.next)
  4011e3:	8b 00                	mov    (%rax),%eax                  # rax = *rax (rax = node6.next->value)
  4011e5:	39 03                	cmp    %eax,(%rbx)                  # compare (*rbx - rax) (node6.value - node6.next->value)
  4011e7:	7d 05                	jge    4011ee <phase_6+0xfa>        # if *rbx - rax >= 0: goto 4011ee
  4011e9:	e8 4c 02 00 00       	call   40143a <explode_bomb>        # else: explode_bomb()
  4011ee:	48 8b 5b 08          	mov    0x8(%rbx),%rbx               # rbx = *(rbx+8)
  4011f2:	83 ed 01             	sub    $0x1,%ebp                    # rbp -= 1
  4011f5:	75 e8                	jne    4011df <phase_6+0xeb>        # if *rbx - rax != 0: goto 4011df
  4011f7:	48 83 c4 50          	add    $0x50,%rsp                   # else: deallocate stack frame
  4011fb:	5b                   	pop    %rbx                         # restore callee-saved registers
  4011fc:	5d                   	pop    %rbp                         #
  4011fd:	41 5c                	pop    %r12                         #
  4011ff:	41 5d                	pop    %r13                         #
  401201:	41 5e                	pop    %r14                         #
  401203:	c3                   	ret                                 # return
```

### Notes:

- `401153` to `40116d` does `numbers.map(num => 7 - num)`

- `*($0x6032d0)` points to first node in linked list of nodes (see disassembly.asm .data section)
- `(gdb) break *0x4011a9`
- `(gbd) x/12gx $rdx` to show the linked list

```C
typedef struct node {
    long value;
    node *next;
} node;
```

- Up until `4011cb`, code is pushing the linked list next pointers onto the stack
- and then reversing the connections by the order they are put on the stack?
- input of `1 2 3 4 6` just reverses the list... maybe specifying a different order will order the list differently?

```shell
(gdb) x/12gx 0x6032d0
0x6032d0 <node1>:       0x000000010000014c      0x0000000000000000
0x6032e0 <node2>:       0x00000002000000a8      0x00000000006032d0
0x6032f0 <node3>:       0x000000030000039c      0x00000000006032e0
0x603300 <node4>:       0x00000004000002b3      0x00000000006032f0
0x603310 <node5>:       0x00000005000001dd      0x0000000000603300
0x603320 <node6>:       0x00000006000001bb      0x0000000000603310
```

- Can see the ordering is `6->1->5->2->4->3`

- Final phase decides if bomb explodes
- `rbx` pointing at new first node
- `rdx` pointing at new last node

```asm
4011d2:	48 c7 42 08 00 00 00 	movq   $0x0,0x8(%rdx)               # last->next = NULL
4011d9:	00                                                        #
4011da:	bd 05 00 00 00       	mov    $0x5,%ebp                    # rbp = 5
4011df:	48 8b 43 08          	mov    0x8(%rbx),%rax               # rax = curr->next
4011e3:	8b 00                	mov    (%rax),%eax                  # rax = *(curr->next)
4011e5:	39 03                	cmp    %eax,(%rbx)                  # compare (curr - next)
4011e7:	7d 05                	jge    4011ee <phase_6+0xfa>        # if curr >= next: goto 4011ee
4011e9:	e8 4c 02 00 00       	call   40143a <explode_bomb>        # else: explode
4011ee:	48 8b 5b 08          	mov    0x8(%rbx),%rbx               # curr = curr->next
4011f2:	83 ed 01             	sub    $0x1,%ebp                    # rbp-- (sub changes eflags/status flags like ZF)
4011f5:	75 e8                	jne    4011df <phase_6+0xeb>        # if (rbp != 0): goto 4011df
```

- Each parent node value must be greater than its child, so our input should sort the linked list in descending order
- `3->4->5->6->1->2`
- The code at the start maps the 6 input numbers to `7 - x`
- To find the input to generate the above ordering, just do `7-x` for each node number
- `4 3 2 1 6 5`

### `phase_6` Explained

- Read 6 numbers from input
- First number must be <= 5 (to safely address nodes in linked list, as there are 6 nodes?)
- Validate there are no duplicate numbers in the input
  - Validate numbers[0] appears only once in numbers
  - For i = 1..5: validate numbers[i] appears only once in numbers
- `numbers.map(n => 7 - n)` (this determines the final ordering of the linked list)
  - `4 3 2 1 6 5` -> `3 4 5 6 1 2`
- Push the address of nodeX on the stack in the order determined by previous mapping
  - &node3, &node4, &node5, &node6, &node1, &node2
- For each node on the stack: switch its `next` pointer to the next node on the stack
- For the last node on the stack, set `next` to `0x0` (NULL)
- For each node, starting at the first on the stack, compare its value with the value of the next node
  - If curr.value < next.value: explode
- Defuse
