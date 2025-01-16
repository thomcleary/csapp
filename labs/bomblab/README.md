# Bomblab

## Symbol Table

```shell
$ objdump -t bomb
```

## Dissasemble

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
