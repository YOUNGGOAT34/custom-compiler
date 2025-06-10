section .data
section .text
	global _start
_start:
	jmp main
main:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	push QWORD [rbp-0]
	push QWORD [rbp-0]
	push QWORD 20
	pop QWORD[rbp-0]
	pop rdi
	pop rax
	push rax
while0:
	mov rax, [rbp-0]
	mov rbx, 10
	cmp rax,rbx
	jle end_while0
	DEC QWORD[rbp-0]
	jmp while0
end_while0:
	push QWORD [rbp-0]
	pop rdi
	mov rax, 60
	mov rsp, rbp
	pop rbp
	syscall
