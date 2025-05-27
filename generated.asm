section .data
	x dq 0
	y dq 0
section .text
	global _start
_start:
	jmp main
main:
	mov rax,[x]
	push rax
	mov rax,4
	push rax
	pop rbx
	mov [x],rbx
	pop rdi
	pop rax
	push rax
	mov rax, [x]
	mov rbx, 4
	cmp rax,rbx
	je _else0
	jmp end_if0
_else0:
while0:
	mov rax, [x]
	mov rbx, 10
	cmp rax,rbx
	je end_while0
	mov rax,[x]
	push rax
	mov rax,[x]
	push rax
	mov rax,1
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rbx
	mov [x],rbx
	pop rdi
	pop rax
	push rax
	jmp while0
end_while0:
end_if0:
	mov rax,[y]
	push rax
	mov rax,2
	push rax
	pop rbx
	mov [y],rbx
	pop rdi
	pop rax
	push rax
	mov rax,[x]
	push rax
	mov rax,[y]
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rdi
	mov rax, 60
	syscall
