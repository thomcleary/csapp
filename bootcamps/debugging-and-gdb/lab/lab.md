# Debugging & GDB

## Part 1

Follow the instructions to step through the `phase1.c` code

```shell
$ gcc -o phase1 -g -std=c99 phase1.c
$ gdb ./phase1
```

### `(gdb) break main`

```shell
(gdb) break main
Breakpoint 1 at 0x13be: file phase1.c, line 55.
```

### `(gbd) break unscramble`

```shell
(gdb) break unscramble
Breakpoint 2 at 0x1318: file phase1.c, line 41.
```

### `(gbd) break reverse`

```shell
(gdb) break reverse
Breakpoint 3 at 0x1284: file phase1.c, line 31.
```

### `(gbd) break toggleCase`

```shell
(gdb) break toggleCase
Breakpoint 4 at 0x11f7: file phase1.c, line 20.
```

### `(gbd) info break`

```shell
(gdb) info break
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x00000000000013be in main at phase1.c:55
2       breakpoint     keep y   0x0000000000001318 in unscramble at phase1.c:41
3       breakpoint     keep y   0x0000000000001284 in reverse at phase1.c:31
4       breakpoint     keep y   0x00000000000011f7 in toggleCase at phase1.c:20
```

### `(gbd) run`

```shell
(gdb) run
Starting program: /csapp/bootcamps/debugging-and-gdb/lab/phase1
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Breakpoint 1, main () at phase1.c:55
55	    word_t *secret_msg = (word_t *) calloc(1, sizeof(word_t));
```

### `(gbd) next` (do this twice)

```shell
(gdb) next
56	    secret_msg->str = (char *) calloc(length, sizeof(char));
(gdb) next
57	    strncpy(secret_msg->str, "!312 Av XPHy QBBt", length);;
```

### `(gbd) print *(word_t *)secret_msg`

```shell
(gdb) print *(word_t *)secret_msg
$1 = {str = 0x5555555592c0 "", len = 0}
```

### `(gbd) next`

```shell
(gdb) next
58	    secret_msg->len = length;
```

### `(gbd) print *(word_t *)secret_msg`

```shell
(gdb) print *(word_t *)secret_msg
$1 = {str = 0x5555555592c0 "!312 Av XPHy QBBt", len = 0}
```

### `(gbd) continue`

```shell
(gdb) continue
Continuing.

Breakpoint 2, unscramble (msg=0x5555555592a0) at phase1.c:41
41	    char chng = 13;
```

### `(gbd) step`

```shell
(gdb) step
42	    for (int i = 0; i < msg->len; i++) {
```

### `(gbd) next` (do this twice)

```shell
(gdb) next
43	        ltr = msg->str[i];
(gdb) next
44	        if (isAlpha(ltr)) {
```

### `(gbd) print ltr`

```shell
(gdb) print ltr
$3 = 33 '!'
```

### `(gbd) print isAlpha(ltr)`

```shell
(gdb) print isAlpha(ltr)
$4 = false
```

### `(gbd) watch ltr`

```shell
(gdb) watch ltr
Hardware watchpoint 5: ltr
```

### `(gbd) continue`

```shell
(gdb) continue
Continuing.

Hardware watchpoint 5: ltr

Old value = 33 '!'
New value = 51 '3'
unscramble (msg=0x5555555592a0) at phase1.c:44
44	        if (isAlpha(ltr)) {
```

### `(gbd) step`

```shell
(gdb) step
isAlpha (ltr=51 '3') at phase1.c:14
14	    bool soln = (('a' <= ltr && ltr <= 'z') || ('A' <= ltr && ltr <= 'Z'));
```

### `(gbd) backtrace`

```shell
(gdb) backtrace
#0  isAlpha (ltr=51 '3') at phase1.c:14
#1  0x0000555555555345 in unscramble (msg=0x5555555592a0) at phase1.c:44
#2  0x0000555555555429 in main () at phase1.c:60
```

## Part 2

You are given `phase2.c`, a file that contains buggy code! First try compiling and running the file.

```shell
$ gcc -std=c99 -o phase2 -g phase2.c
$ ./phase2
```

### What did you observe?

```shell
root in bootcamps/debugging-and-gdb/lab on  main via C v13.3.0-gcc
🐳 ❯ ./phase2
fish: Job 1, './phase2' terminated by signal SIGFPE (Floating point exception)
```

The next part of the lab focuses on finding out where the crash
happened using `GDB`

```shell
(gdb) run
```

### What does `GDB` tell you about the error?

```shell
🐳 ❯ gdb phase2
...
(gdb) run
Starting program: /csapp/bootcamps/debugging-and-gdb/lab/phase2
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Program received signal SIGFPE, Arithmetic exception.
0x0000555555555399 in calc_avg (S=0x5555555592a0) at phase2.c:51
51	  return total_sum / S->top->len;
```

Try backtracking to obtain further information

```shell
(gdb) bt
```

```shell
(gdb) bt
#0  0x0000555555555399 in calc_avg (S=0x5555555592a0) at phase2.c:51
#1  0x0000555555555420 in main () at phase2.c:60
```

This should reveal information where the error is coming from. Your next step is to use
`GDB` commands to further investigate why it is happening.

_Hints:_ Utilise `gdb` commands discussed in phase1 such as breakpoint, watchpoints to specific
variables and printing values. This is a great time to showcase your learnings from this session.
It might be useful to call function such as `stack_print` by:

```shell
$ print (void) stack_print(S)
```

### Phase 2 commands

```shell
(gdb) break calc_avg
Breakpoint 1 at 0x555555555353: file phase2.c, line 47.
(gdb) run
Starting program: /csapp/bootcamps/debugging-and-gdb/lab/phase2
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Breakpoint 1, calc_avg (S=0x5555555592a0) at phase2.c:47
47	  int total_sum = 0;
(gdb) print (void) stack_print(S)
TOP:
b 13 |
$1 = void
(gdb) next
48	  for (Node* l = S->top; l != S->floor; l = l->next) {
(gdb) next
49	    total_sum += l->sum;
(gdb) print *(Node *)l
$2 = {data = 0x555555556016 "b", len = 0, sum = 13, next = 0x5555555592e0}
(gdb) watch l
Hardware watchpoint 2: l
(gdb) continue
Continuing.

Hardware watchpoint 2: l

Old value = (Node *) 0x555555559320
New value = (Node *) 0x5555555592e0
0x000055555555537d in calc_avg (S=0x5555555592a0) at phase2.c:48
48	  for (Node* l = S->top; l != S->floor; l = l->next) {
(gdb) print *(Node *)l
$3 = {data = 0x555555556014 "a", len = 0, sum = 15, next = 0x5555555592c0}
(gdb) continue
Continuing.

Hardware watchpoint 2: l

Old value = (Node *) 0x5555555592e0
New value = (Node *) 0x5555555592c0
0x000055555555537d in calc_avg (S=0x5555555592a0) at phase2.c:48
48	  for (Node* l = S->top; l != S->floor; l = l->next) {
(gdb) print *(Node *)l
$5 = {data = 0x0, len = 0, sum = 0, next = 0x0}
(gdb) next
51	  return total_sum / S->top->len;
(gdb) print *(Node *)S->top
$9 = {data = 0x555555556016 "b", len = 0, sum = 13, next = 0x5555555592e0}
```

The `len` member of each `Node` is not set when pushed onto the stack in `void push(stack* S, char *x, int sum) `,
resulting in a division by 0 error when `calc_avg` returns.
