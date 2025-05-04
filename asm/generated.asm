section .text
	global _start
_start:
	mov rax,60
	add rdi,3
	sub rdi,10
	add rdi,5
	add rdi,6
	add rdi,1
	add rdi,3
	syscall