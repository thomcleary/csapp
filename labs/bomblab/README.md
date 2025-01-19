# Bomblab

## Symbol Table

```shell
$ objdump -t bomb
```

## Disassemble

```shell
$ objdump -d bomb > disassembly.asm
$ objdump -s -j .rodata bomb >> disassembly.asm
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
  400efe:	48 83 ec 28          	sub    $0x28,%rsp                   # Create stack frame of 28 bytes
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
  400f43:	48 83 ec 18          	sub    $0x18,%rsp                       # Add 18 bytes to stack frame
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
  40100c:	48 83 ec 18          	sub    $0x18,%rsp                       # Allocate stack frame (18 bytes)
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

### NOTES

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

TODO

## Phase 6

TODO

## Secret Phase

TODO