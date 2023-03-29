# Walkthrough

Upon connecting to level0, I noticed a binary file in the home directory.

```
level0@RainFall:~$ ls -l
total 732
-rwsr-x---+ 1 level1 users 747441 Mar  6  2016 level0
```

I attempted to reverse engineer it and discovered that the program extracts the second argument of the argv array and passes it to the atoi function.

```
0x08048ec9 <+9>:  mov    eax,DWORD PTR [ebp+0xc]
0x08048ecc <+12>: add    eax,0x4
0x08048ecf <+15>: mov    eax,DWORD PTR [eax]
0x08048ed1 <+17>: mov    DWORD PTR [esp],eax
0x08048ed4 <+20>: call   0x8049710 <atoi>
```

 If the result of the atoi function is not equal to 0x1a7, the program prints "NO !\n" to the standard error output.

```
0x08048ed9 <+25>: cmp    eax,0x1a7
0x08048ede <+30>: jne    0x8048f58 <main+152>

0x08048f58 <+152>: mov    eax,ds:0x80ee170
0x08048f5d <+157>: mov    edx,eax
0x08048f5f <+159>: mov    eax,0x80c5350
0x08048f64 <+164>: mov    DWORD PTR [esp+0xc],edx
0x08048f68 <+168>: mov    DWORD PTR [esp+0x8],0x5
0x08048f70 <+176>: mov    DWORD PTR [esp+0x4],0x1
0x08048f78 <+184>: mov    DWORD PTR [esp],eax       // "NO !\n"
0x08048f7b <+187>: call   0x804a230 <fwrite>
```

However, if the result of the atoi function is equal to 0x1a7, the program duplicates the string "/bin/sh" and stores it using strdup. It then uses the result of the getegid() function as the parameter for setresgid() and the result of the geteuid() function as the parameter for setresuid(). Finally, it calls the execv function with the duplicated string and an array of strings ["/bin/sh", NULL].

```
0x08048ee0 <+32>: mov    DWORD PTR [esp],0x80c5348
0x08048ee7 <+39>: call   0x8050bf0 <strdup>
0x08048eec <+44>: mov    DWORD PTR [esp+0x10],eax
0x08048ef0 <+48>: mov    DWORD PTR [esp+0x14],0x0
0x08048ef8 <+56>: call   0x8054680 <getegid>
0x08048efd <+61>: mov    DWORD PTR [esp+0x1c],eax
0x08048f01 <+65>: call   0x8054670 <geteuid>
0x08048f06 <+70>: mov    DWORD PTR [esp+0x18],eax
0x08048f0a <+74>: mov    eax,DWORD PTR [esp+0x1c]
0x08048f0e <+78>: mov    DWORD PTR [esp+0x8],eax
0x08048f12 <+82>: mov    eax,DWORD PTR [esp+0x1c]
0x08048f16 <+86>: mov    DWORD PTR [esp+0x4],eax
0x08048f1a <+90>: mov    eax,DWORD PTR [esp+0x1c]
0x08048f1e <+94>: mov    DWORD PTR [esp],eax
0x08048f21 <+97>: call   0x8054700 <setresgid>
0x08048f26 <+102>: mov    eax,DWORD PTR [esp+0x18]
0x08048f2a <+106>: mov    DWORD PTR [esp+0x8],eax
0x08048f2e <+110>: mov    eax,DWORD PTR [esp+0x18]
0x08048f32 <+114>: mov    DWORD PTR [esp+0x4],eax
0x08048f36 <+118>: mov    eax,DWORD PTR [esp+0x18]
0x08048f3a <+122>: mov    DWORD PTR [esp],eax
0x08048f3d <+125>: call   0x8054690 <setresuid>
0x08048f42 <+130>: lea    eax,[esp+0x10]
0x08048f46 <+134>: mov    DWORD PTR [esp+0x4],eax
0x08048f4a <+138>: mov    DWORD PTR [esp],0x80c5348
0x08048f51 <+145>: call   0x8054640 <execv>
0x08048f56 <+150>: jmp    0x8048f80 <main+192>
```

Upon running the program with the argument of 423, I noticed that I gained access to the level01 shell. With this access, I was able to use the "cat" command to read the contents of the ".pass" file.

```
level0@RainFall:~$ ./level0 423
$ pwd
/home/user/level0
$ cd ../level1
$ ls -la
total 17
dr-xr-x---+ 1 level1 level1   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level1 level1  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level1 level1 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 level1 level1   65 Sep 23  2015 .pass
-rw-r--r--  1 level1 level1  675 Apr  3  2012 .profile
-rwsr-s---+ 1 level2 users  5138 Mar  6  2016 level1
$ cat .pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```
