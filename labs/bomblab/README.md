# Bomblab

## Symbol Table

```shell
$ objdump -t bomb
```

## Disassemble

```shell
$ objdump -d bomb
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

TODO

## Phase 4

TODO

## Phase 5

TODO

## Phase 6

TODO

## Secret Phase

TODO
