section .data
buffer: TIMES 21 dB 0
section .text
	global _start
_start:
	jmp main
main:
	push rbp
	mov rbp, rsp
	sub rsp, 8
	jmp .after0
.msg0: db 72,101,108,108,111,32
.len0: equ $-.msg0
.after0:
	mov rax,1
	mov rdi,1
	lea rsi,[rel .msg0]
	mov rdx,.len0
	syscall
	mov rdx, 1
	call print_int
	jmp .after1
.msg1: db 32,115,101,99,111,110,100,32,118,97,108,117,101,32
.len1: equ $-.msg1
.after1:
	mov rax,1
	mov rdi,1
	lea rsi,[rel .msg1]
	mov rdx,.len1
	syscall
	mov rdx, 2
	call print_int
	jmp .after2
.msg2: db 32
.len2: equ $-.msg2
.after2:
	mov rax,1
	mov rdi,1
	lea rsi,[rel .msg2]
	mov rdx,.len2
	syscall
	mov rdx, 4
	call print_int
	jmp .after3
.msg3: db 32
.len3: equ $-.msg3
.after3:
	mov rax,1
	mov rdi,1
	lea rsi,[rel .msg3]
	mov rdx,.len3
	syscall
	jmp .after4
.msg4: db 37,100,32,100,111,110,101,10
.len4 equ $-.msg4
.after4:
	mov rax, 1
	mov rdi, 1
	lea rsi, [rel .msg4]
	mov rdx, .len4
	syscall
	mov rdi,1
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
.loop:
	xor rdx,rdx
	idiv rbx
	add dl,'0'
	dec rsi
	mov [rsi],dl
	test rax,rax
	jnz .loop
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
