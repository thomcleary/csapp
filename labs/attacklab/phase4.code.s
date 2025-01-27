popq    %rax        # move cookie into rax, decrement rsp
ret                 # pop address of next gadget into rip
movq    %rax, %rdi  # move cookie into rdi
ret                 # pop address of touch2 into rip
