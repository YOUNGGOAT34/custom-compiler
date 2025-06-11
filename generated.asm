section .data
section .text
	global _start
_start:
	jmp main
func:
	push rbp
	mov rbp, rsp
	sub rsp, 24
	push QWORD [rbp-8]
	push QWORD 3
	pop QWORD[rbp-8]
	pop rdi
	pop rax
	push rax
	push QWORD [rbp-16]
	push QWORD 4
	pop QWORD[rbp-16]
	pop rdi
	pop rax
	push rax
	push QWORD [rbp-8]
	push QWORD [rbp-16]
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rax
	mov rsp, rbp
	pop rbp
	ret
main:
	push rbp
	mov rbp, rsp
	sub rsp, 16
	call func
	mov QWORD[rbp-8],rax
	push QWORD [rbp-8]
	pop rdi
	mov rax, 60
	mov rsp, rbp
	pop rbp
	syscall
