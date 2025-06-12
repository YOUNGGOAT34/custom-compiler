section .data
	d dq 0
	e dq 0
section .text
	global _start
_start:
	mov QWORD[d],2
	mov rax,QWORD[d]
	mov QWORD[e],rax
	jmp main
func:
	push rbp
	mov rbp, rsp
	sub rsp, 32
	mov QWORD[rbp-8],3
	mov QWORD[rbp-16],4
	mov rax,QWORD[rbp-8]
	mov QWORD[rbp-24],rax
	mov rax,QWORD[rbp-24]
	mov rsp, rbp
	pop rbp
	ret
main:
	push rbp
	mov rbp, rsp
	sub rsp, 16
	mov QWORD[rbp-8],4
	mov rbx,[d]
	mov rax,2
	add rax,rbx
	push rax
	pop rax
	imul rax,QWORD[rbp-8]
	mov QWORD[rbp-8],rax
	mov rdi,QWORD[rbp-8]
	mov rax, 60
	mov rsp, rbp
	pop rbp
	syscall
