# Rainfall_42

## About

The subject of this project is the exploitation of elf-like binary in i386 systems. An elf-like binary is a type of executable file format used in Unix and Linux operating systems. It contains machine code that can be executed directly by the computer's processor.

Exploiting an elf-like binary involves identifying vulnerabilities in the code and using them to gain unauthorized access or control over the system. This can be done for testing purposes, to identify weaknesses in a system's security, or for malicious purposes, such as stealing data or taking control of a system.

The project aims to teach you how to identify these vulnerabilities and exploit them for testing purposes. By doing so, you will gain a better understanding of program security and be able to design more secure programs in the future.

The project also emphasizes the importance of avoiding common programming malpractice that can lead to security breaches. By developing logic and reflexes that help you identify potential vulnerabilities before they can be exploited, you will be better equipped to design bug-free programs that are less vulnerable to attack.

Overall, this project is an excellent way to gain valuable experience in IT security and improve your programming skills.

## Subject
+ [Rainfall Subject](/Rainfall_subject.pdf)

## Explanation of Key Terms Used in Rainfall Project.


- Elf-like binary: An ELF (Executable and Linkable Format) binary is a type of file format used for executables, object code, shared libraries, and core dumps. The term "elf-like" refers to a binary file that follows the same structure and conventions as an ELF file.

- Exploitation: In the context of software security, exploitation refers to the act of identifying and taking advantage of vulnerabilities in a system or program for malicious purposes or testing purposes.

- Binary exploitation: Binary exploitation is the process of finding and exploiting vulnerabilities in a compiled program or binary file. This can involve techniques such as buffer overflow, format string vulnerability, and return-to-libc attack.

- i386 system: The i386 architecture is a 32-bit architecture used by Intel processors. It is commonly used in desktop and laptop computers running Windows or Linux operating systems. The i386 system refers to any system that uses this architecture.

- Vulnerability: A vulnerability is a weakness or flaw in a system that can be exploited by an attacker to gain unauthorized access or perform malicious actions.

- Buffer overflow: A buffer overflow is a type of vulnerability where an attacker sends more data than a program can handle, causing it to overwrite adjacent memory locations and potentially execute arbitrary code.

- Format string vulnerability: A format string vulnerability is a type of vulnerability where an attacker can control the format string argument passed to a function, allowing them to read or write arbitrary memory locations.

- Return-to-libc attack: A return-to-libc attack is a type of exploit where an attacker overwrites the return address on the stack with the address of a libc function, allowing them to execute arbitrary code without injecting any shellcode. This technique is often used when stack protection mechanisms such as stack canaries are enabled.

- Shellcode: A shellcode  is a small piece of code that is used as the payload in an exploit. It is typically written in assembly language and designed to be injected into a vulnerable program's memory space. Once executed, the shellcode can perform various actions such as opening a shell, downloading and executing additional malware, or modifying system files.

## Steps Involved in Binary Exploitation

1. Identify the target: The first step in binary exploitation is to identify the target program or system that you want to exploit.

2. Analyze the target: Once you have identified the target, you will need to analyze it to identify potential vulnerabilities. This may involve reverse engineering the program or system, looking for buffer overflows, format string vulnerabilities, or other types of vulnerabilities.

3. Develop an exploit: Once you have identified a vulnerability, you will need to develop an exploit that takes advantage of it. This may involve writing code that overflows a buffer or injects malicious code into the program's memory.

4. Test the exploit: Once you have developed an exploit, you will need to test it to make sure it works as expected..

## Content

Each folder at every level contains the following files:

- `flag`: The password needed for the next user.
- `source`: Pseudocode illustrating the binary code.
- `walkthrough`: A document providing instructions on how to discover the password.