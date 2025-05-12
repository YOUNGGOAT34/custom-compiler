section .data
	c dq 0
	a dq 0
section .text
	global _start
_start:
  mov rax,6
  mov [c],rax
  mov rax,7
  mov [a],rax
  mov rdi,[a]
  mov rbx,[c]
  cmp rbx,rdi
  JNE _here

	
_here:
  mov rax,60
  mov rdi,8
  syscall