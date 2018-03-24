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
	.file	"pgm1.c"
	.comm	x,4,4
	.comm	y,4,4
	.text
	.align	2
	.global	fdouble
	.type	fdouble, %function
fdouble:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	ldr	r2, [fp, #-8]
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	mov	r0, r3
	sub	sp, fp, #0
	@ sp needed
	ldr	fp, [sp], #4
	bx	lr
	.size	fdouble, .-fdouble
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	mov	r3, #3
	str	r3, [fp, #-8]
	mov	r3, #12
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-8]
	add	r2, r3, #6
	ldr	r3, [fp, #-12]
	add	r1, r3, #1
	str	r1, [fp, #-12]
	rsb	r3, r3, r2
	str	r3, [fp, #-16]
	ldr	r2, .L4
	ldr	r3, [fp, #-16]
	str	r3, [r2]
	ldr	r0, [fp, #-8]
	bl	fdouble
	mov	r2, r0
	ldr	r3, .L4+4
	str	r2, [r3]
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
.L5:
	.align	2
.L4:
	.word	y
	.word	x
	.size	main, .-main
	.ident	"GCC: (Raspbian 4.9.2-10) 4.9.2"
	.section	.note.GNU-stack,"",%progbits
