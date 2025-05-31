section .data
section .text
	global _start
_start:
	jmp main
main:
	push rbp
	mov rbp, rsp
	sub rsp, 40
	mov rax,[rbp-0]
	push rax
	mov rax,10
	push rax
	pop rbx
	mov QWORD[rbp-0],rbx
	pop rdi
	pop rax
	push rax
	mov rax,[rbp-8]
	push rax
	mov rax,20
	push rax
	pop rbx
	mov QWORD[rbp-8],rbx
	pop rdi
	pop rax
	push rax
while0:
	mov rax, [rbp-0]
	mov rbx, 10
	cmp rax,rbx
	je end_while0
	mov rax,[rbp-16]
	push rax
	mov rax,1
	push rax
	pop rbx
	mov QWORD[rbp-16],rbx
	pop rdi
	pop rax
	push rax
	mov rax,[rbp-24]
	push rax
	mov rax,1
	push rax
	pop rbx
	mov QWORD[rbp-24],rbx
	pop rdi
	pop rax
	push rax
	mov rax,[rbp-0]
	push rax
	mov rax,[rbp-0]
	push rax
	mov rax,[rbp-24]
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rbx
	mov QWORD[rbp-0],rbx
	pop rdi
	pop rax
	push rax
	jmp while0
end_while0:
	mov rax,[rbp-32]
	push rax
	mov rax,5
	push rax
	pop rbx
	mov QWORD[rbp-32],rbx
	pop rdi
	pop rax
	push rax
	mov rax,[rbp-0]
	push rax
	mov rax,[rbp-8]
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	mov rax,[rbp-32]
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rdi
	mov rax, 60
	mov rsp, rbp
	pop rbp
	syscall
