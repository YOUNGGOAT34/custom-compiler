section .data
	x dq 0
	y dq 0
section .text
	global _start
_start:
	mov rax,[x]
	push rax
	mov rax,5
	push rax
	pop rbx
	mov [x],rbx
	pop rdi
	pop rax
	push rax
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
while1:
	mov rax, [x]
	mov rbx, 20
	cmp rax,rbx
	je end_while1
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
	jmp while1
end_while1:
while2:
	mov rax, [x]
	mov rbx, 30
	cmp rax,rbx
	je end_while2
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
	jmp while2
end_while2:
	mov rax,[y]
	push rax
	mov rax,5
	push rax
	pop rbx
	mov [y],rbx
	pop rdi
	pop rax
	push rax
	mov rax, [y]
	mov rbx, 5
	cmp rax,rbx
	je ._else0
	mov rax,[y]
	push rax
	mov rax,[y]
	push rax
	mov rax,5
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rbx
	mov [y],rbx
	pop rdi
	pop rax
	push rax
	jmp .end_if0
._else0:
	mov rax,[y]
	push rax
	mov rax,[y]
	push rax
	mov rax,15
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rbx
	mov [y],rbx
	pop rdi
	pop rax
	push rax
	mov rax, [y]
	mov rbx, 20
	cmp rax,rbx
	je ._else1
	mov rax,[y]
	push rax
	mov rax,[y]
	push rax
	mov rax,1
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rbx
	mov [y],rbx
	pop rdi
	pop rax
	push rax
	jmp .end_if1
._else1:
	mov rax,[y]
	push rax
	mov rax,[y]
	push rax
	mov rax,20
	push rax
	pop rdi
	pop rax
	add rax,rdi
	push rax
	pop rbx
	mov [y],rbx
	pop rdi
	pop rax
	push rax
.end_if1:
.end_if0:
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
