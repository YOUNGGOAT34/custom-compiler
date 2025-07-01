section .data
buffer: TIMES 21 dB 0
	b dq 0
section .text
	global _start
_start:
	jmp main
main:
	push rbp
	mov rbp, rsp
	sub rsp, 12
	mov QWORD[rbp-4],10
	mov rax, QWORD [rbp-4]
	mov rbx,1
	cmp rax,rbx
	je end_if0
	DEC QWORD[rbp-4]
	jmp end_if0
end_if0:
	mov rdi,QWORD[rbp-4]
	mov rax, 60
	mov rsp, rbp
	pop rbp
	syscall
print_int:
	push rbp
	mov rbp, rsp
	mov rax,rdx
	mov rsi,buffer+20
	mov rbx,10
	xor rcx,rcx
	test rax,rax
	jns .loop
	neg rax
	mov rcx,1
.loop:
	xor rdx,rdx
	idiv rbx
	add dl,'0'
	dec rsi
	mov [rsi],dl
	test rax,rax
	jnz .loop
	cmp rcx,1
	jne .print
	dec rsi
	mov byte[rsi],'-'
.print: 
	mov rax,1
	 mov rdi,1
	mov rdx,buffer+20
	sub rdx,rsi
	syscall
	mov rdi,buffer
	mov rcx,21
	mov al,0
	rep stosb
	mov rsp,rbp
	pop rbp
	ret
	print_char:
	push rbp
	mov rbp,rsp
	sub rbp,1
	mov [rbp],dl
	lea rsi,[rbp]
	mov rax,1
	mov rdx,1
	mov rdi,1
	syscall
	mov rbp,rsp
	pop rbp
	ret
