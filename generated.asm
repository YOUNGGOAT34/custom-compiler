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
	mov rax,2
	push rax
	pop rbx
	mov QWORD[rbp-0],rbx
	pop rdi
	pop rax
	push rax
do_while0:
do_while1:
	mov rax,[rbp-0]
	push rax
	mov rax,[rbp-0]
	push rax
	mov rax,1
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
	mov rax,[rbp-0]
	mov rbx,10
	cmp rax,rbx
	jge end_do_while1
	jmp do_while1
end_do_while1:
	mov rax,[rbp-0]
	mov rbx,20
	cmp rax,rbx
	jge end_do_while0
	jmp do_while0
end_do_while0:
	mov rax,[rbp-0]
	push rax
	pop rdi
	mov rax, 60
	mov rsp, rbp
	pop rbp
	syscall
