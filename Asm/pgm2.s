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
	.file	"pgm2.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	stmfd	sp!, {r4, fp}
	add	fp, sp, #4
	mov	r4, #1024
	b	.L2
.L3:
	ldr	r3, .L4
	smull	r2, r3, r3, r4
	mov	r2, r3, asr #2
	mov	r3, r4, asr #31
	rsb	r2, r3, r2
	mov	r3, r2
	mov	r3, r3, asl #2
	add	r3, r3, r2
	mov	r3, r3, asl #1
	rsb	r2, r3, r4
	ldr	r3, .L4
	smull	r2, r3, r3, r4
	mov	r2, r3, asr #2
	mov	r3, r4, asr #31
	rsb	r3, r3, r2
	mov	r4, r3
.L2:
	cmp	r4, #0
	bgt	.L3
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {r4, fp}
	bx	lr
.L5:
	.align	2
.L4:
	.word	1717986919
	.size	main, .-main
	.ident	"GCC: (Raspbian 4.9.2-10) 4.9.2"
	.section	.note.GNU-stack,"",%progbits
