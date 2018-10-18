/* This assembly file uses GNU syntax */
	.text
	.section	.rodata
	.align	2
	
	
.LC0:
	
	.text
	.align	2
	.global	main
	.type main function
	
main:
	push {r3, lr}
	add	r3, sp, #4
	
	ldr	r1,=lista_datos	@ldr r1,(PC)
	mov r0, #long-1
	ldr	r2,[r1]			@dato del arreglo
	cambio:
		add r1,#1
		ldr r3,[r1]
		cmp	r2,r3
		bls	no_cambio
		mov	r2,r3
	no_cambio:
		sub.s r0,#1
		bne	ciclo:	
	
	
	mov	r3, #0
	mov	r0, r3
	pop {r3, pc}
	
	.align	2
.L3:
	.word	.LC0
	lista_datos: .word 58,34,25,3,9,18	@queda en Mem código,constantes
	.set long, 6						@igual que define
	.end
