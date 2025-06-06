section .data
section .text
	global _start
_start:
	jmp main
main:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	mov rax,[rbp-0]
	push rax
	mov rax,3
	push rax
	pop rbx
	mov QWORD[rbp-0],rbx
	pop rdi
	pop rax
	push rax
	DEC QWORD[rbp-0]
	mov rax,[rbp-0]
	push rax
	pop rdi
	mov rax, 60
	mov rsp, rbp
	pop rbp
	syscall
