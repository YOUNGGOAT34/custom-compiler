section .data
buffer: TIMES 21 dB 0
section .text
	global _start
_start:
	jmp main
main:
	push rbp
	mov rbp, rsp
	sub rsp, 12
	mov DWORD[rbp-4],0
	mov DWORD[rbp-8],0
while0:
	mov rax, [rbp-4]
	mov rbx, 2
	cmp rax,rbx
	jge end_while0
	mov DWORD[rbp-8],0
while1:
	mov rax, [rbp-8]
	mov rbx, 3
	cmp rax,rbx
	jge end_while1
	mov eax,DWORD[rbp-8]
	mov ebx,DWORD 1
	add eax,ebx
	sub rsp,4
	mov [rsp],eax
	mov eax,DWORD[rsp]
	add rsp,4
	mov [rbp-8],eax
	jmp while1
end_while1:
	mov eax,DWORD[rbp-4]
	mov ebx,DWORD 1
	add eax,ebx
	sub rsp,4
	mov [rsp],eax
	mov eax,DWORD[rsp]
	add rsp,4
	mov [rbp-4],eax
	jmp while0
end_while0:
	mov ebx,DWORD[rbp-8]
	mov eax,DWORD[rbp-4]
	add eax,ebx
	sub rsp,4
	mov [rsp],eax
	mov edi,[rsp]
	add rsp,4
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
