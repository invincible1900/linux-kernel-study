#!/bin/bash
make && make clean
scp tc.ko root@192.168.53.76:~
scp a.out root@192.168.53.76:~
ssh root@192.168.53.76 "insmod tc.ko && ./a.out && rmmod tc.ko && dmesg|tail"


#[ 1817.570676] a.out, 1028
#[ 1817.572311] sp: 00007fffffffea48, 0000555555555236
#[ 1817.573442] ip: 00007ffff7ee2fb2, 5677fffff0003d48

# 用 GDB 调试，断点下在read 的返回地址处，可以看到 sp 和ip 对应的内容
#(gdb) disassemble spin
# ...
#   0x0000555555555231 <+104>:	callq  0x5555555550c0 <read@plt>
#=> 0x0000555555555236 <+109>:	mov    -0x18(%rbp),%eax


# (gdb) x/1xg 0x0007ffff7ee2fb2
# 0x7ffff7ee2fb2 <__GI___libc_read+18>:	0x5677fffff0003d48

#(gdb) disassemble /r  0x0007ffff7ee2fb2, 0x0007ffff7ee2fb2+9
#Dump of assembler code from 0x7ffff7ee2fb2 to 0x7ffff7ee2fbb:
#   0x00007ffff7ee2fb2 <__GI___libc_read+18>:	48 3d 00 f0 ff ff	cmp    $0xffffx
#   0x00007ffff7ee2fb8 <__GI___libc_read+24>:	77 56	ja     0x7ffff7ee3010 <__GI___>
#   0x00007ffff7ee2fba <__GI___libc_read+26>:	c3	retq
#End of assembler dump.


