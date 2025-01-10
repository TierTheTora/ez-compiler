section .text
global _start
_start:
section .data
str1: db "Hello, World!", 0
section .text
mov rax, 1
mov rdi, 1
mov rsi, str1
mov rdx, 16
syscall
section .bss
my_inp: resb 100
section .text
mov rax, 0
mov rdi, 0
mov rsi, my_inp
mov rdx, 255
syscall
section .text
mov rax, 1
mov rdi, 1
mov rsi, my_inp
mov rdx, 100
syscall
global _exit
_exit:
mov rax, 60
mov rdi, 66
syscall
