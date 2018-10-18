/* This assembly file uses GNU syntax */
	.text
	.section	.rodata
	.align	2
	MatrizC: .space	6*4	@6loc bytes
	
.LC0:
	
	.text
	.align	2
	.global	main
	.type main function
	
main:
	push {r3, lr}
	add	r3, sp, #4
	
	ldr R8, =MatrizA	@App_MatrizA
	
	ldr R3, =MatrizC	@App_MatrizC
	
	mov R6,#RenA		@i	
cicloi:
	mov R5,#ColB		@j
	ldr R2, =MatrizB	@App_MatrizB
cicloj:
	mov R4,#ColARenB	@k
	eor	R0,R0			@R0=0

ciclok: 
	ldr	R7,[R8]			@Elemento matrizA
	ldr R1,[R2]			@Elemento matrizB
	mul R7,R1			@R7=R7*R1
	add R0,R7			@Acumulamos
	add R8,#4			@Ajusto apuntador R8
	add	R2,#4*ColB
	@subs R4,#1			@subs R4,#1 por error del compilador
						@cmp R4,#0
	sub R4,#1
	cmp R4,#0
	
	bne ciclok
	str R0,(R3,#0)			@guardar resultado
	add R3,#4			@ajust Ap MatrizC
	sub R8,#ColARenB*4	@ajuste ap MatrizA
	sub	R2,#(ColARenB*ColB-1)*4	@ajuste ap MatrizB
	@subs R5,#1
	sub R5,#1
	cmp R5,#0

	
	bne	cicloj
	sub R8,#ColARenB*4	@ajuste ap MatrizA

	@subs R6,#1
	sub R6,#1
	cmp R6,#0
	
	bne cicloi
	
		
	mov	r3, #0
	mov	r0, r3
	pop {r3, pc}
	
	.align	2
.L3:
	.word	.LC0
MatrizA: word 1,2,3,4,5,6,7,8,9
MatrizB: word 1,2,3,4,5,6
	.set RenA, 3
	.set ColB, 2
	.set ColARenB, 3
	.end
