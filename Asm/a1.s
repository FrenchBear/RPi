@ Hailstone sequence
@ 2016-05-19	PV

	.arch armv6
	.eabi_attribute 27, 3
	.eabi_attribute 28, 1
	.fpu vfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"f3.c"
	.section	.rodata
	.align	2

.LC0:
	.ascii	"%d\012\000"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	stmfd	sp!, {r3, r4, fp, lr}
	add	fp, sp, #12

	@ Hailstone seq begin
	mov r0, #3
	mov r1, #0
again: add r1, r1, #1
	ands r7, r0, #1
	beq even
	add r0, r0, r0, lsl #1
	add r0, r0, #1
	b again
even: mov r0, r0, asr #1
	subs r7, r0, #1
	bne again

	mov	r4, #2
	ldr	r0, .L2
	@mov	r1, #3
	bl	printf
	ldmfd	sp!, {r3, r4, fp, pc}

.L2:
	.align	2
	.word	.LC0
	.size	main, .-main
	.ident	"GCC: (Raspbian 4.9.2-10) 4.9.2"
	.section	.note.GNU-stack,"",%progbits
