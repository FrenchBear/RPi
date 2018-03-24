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
	.file	"f1.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"%x\012\000"
	.align	2
.LC1:
	.ascii	"%llx\012\000"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	ldr	r0, .L2
	ldr	r1, .L2+4
	bl	printf
	ldr	r2, .L2+8
	mov	r3, #0
	strd	r2, [fp, #-12]
	ldrd	r0, [fp, #-12]
	mov	r2, r0
	mov	r3, r1
	adds	r2, r2, r2
	adc	r3, r3, r3
	adds	r2, r2, r0
	adc	r3, r3, r1
	ldr	r0, .L2+12
	bl	printf
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
.L3:
	.align	2
.L2:
	.word	.LC0
	.word	1717986919
	.word	429496729
	.word	.LC1
	.size	main, .-main
	.ident	"GCC: (Raspbian 4.9.2-10) 4.9.2"
	.section	.note.GNU-stack,"",%progbits
