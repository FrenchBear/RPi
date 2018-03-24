	.arch armv6
	.eabi_attribute 27, 3	@ Tag_ABI_HardFP_use
	.eabi_attribute 28, 1	@ Tag_ABI_VFP_args
	.fpu vfp
	.eabi_attribute 20, 1	@ Tag_ABI_FP_denormal
	.eabi_attribute 21, 1	@ Tag_ABI_FP_exceptions
	.eabi_attribute 23, 3	@ Tag_ABI_FP_number_model
	.eabi_attribute 24, 1	@ Tag_ABI_align8_needed
	.eabi_attribute 25, 1	@ Tag_ABI_align8_preserved
	.eabi_attribute 26, 2	@ Tag_ABI_enum_size
	.eabi_attribute 30, 6	@ Tag_ABI_optimization_goals
	.eabi_attribute 34, 1	@ Tag_CPU_unaligned_access
	.eabi_attribute 18, 4	@ Tag_ABI_PCS_wchar_t
	.file	"f2.c"
@ GNU C (Raspbian 4.9.2-10) version 4.9.2 (arm-linux-gnueabihf)
@	compiled by GNU C version 4.9.2, GMP version 6.0.0, MPFR version 3.1.2-p3, MPC version 1.0.2
@ GGC heuristics: --param ggc-min-expand=93 --param ggc-min-heapsize=118470
@ options passed:  -imultilib . -imultiarch arm-linux-gnueabihf f2.c
@ -march=armv6 -mfloat-abi=hard -mfpu=vfp -mtls-dialect=gnu -g
@ -fverbose-asm
@ options enabled:  -faggressive-loop-optimizations -fauto-inc-dec -fcommon
@ -fdelete-null-pointer-checks -fdwarf2-cfi-asm -fearly-inlining
@ -feliminate-unused-debug-types -ffunction-cse -fgcse-lm -fgnu-runtime
@ -fgnu-unique -fident -finline-atomics -fira-hoist-pressure
@ -fira-share-save-slots -fira-share-spill-slots -fivopts
@ -fkeep-static-consts -fleading-underscore -fmath-errno
@ -fmerge-debug-strings -fpeephole -fprefetch-loop-arrays
@ -freg-struct-return -fsched-critical-path-heuristic
@ -fsched-dep-count-heuristic -fsched-group-heuristic -fsched-interblock
@ -fsched-last-insn-heuristic -fsched-rank-heuristic -fsched-spec
@ -fsched-spec-insn-heuristic -fsched-stalled-insns-dep -fshow-column
@ -fsigned-zeros -fsplit-ivs-in-unroller -fstrict-volatile-bitfields
@ -fsync-libcalls -ftrapping-math -ftree-coalesce-vars -ftree-cselim
@ -ftree-forwprop -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
@ -ftree-loop-optimize -ftree-parallelize-loops= -ftree-phiprop
@ -ftree-reassoc -ftree-scev-cprop -funit-at-a-time -fverbose-asm
@ -fzero-initialized-in-bss -marm -mglibc -mlittle-endian -mlra
@ -mpic-data-is-text-relative -msched-prolog -munaligned-access
@ -mvectorize-with-neon-quad

	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.section	.rodata
	.align	2
.LC0:
	.ascii	"%4d %4d %4d %4d\012\000"
	.align	2
.LC1:
	.ascii	"sum digits = %d\012\000"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.file 1 "f2.c"
	.loc 1 11 0
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, fp, lr}	@,
	.cfi_def_cfa_offset 16
	.cfi_offset 4, -16
	.cfi_offset 5, -12
	.cfi_offset 11, -8
	.cfi_offset 14, -4
	add	fp, sp, #12	@,,
	.cfi_def_cfa 11, 4
	sub	sp, sp, #24	@,,
	.loc 1 12 0
	mov	r3, #1024	@ tmp119,
	str	r3, [fp, #-16]	@ tmp119, i
	.loc 1 13 0
	ldr	r3, .L4	@ tmp120,
	str	r3, [fp, #-24]	@ tmp120, base
	.loc 1 14 0
	mov	r3, #0	@ tmp121,
	str	r3, [fp, #-20]	@ tmp121, total
	.loc 1 15 0
	b	.L2	@
.L3:
.LBB2:
	.loc 1 16 0
	ldr	r3, [fp, #-16]	@ tmp122, i
	mov	r0, r3	@ D.4606, tmp122
	mov	r1, r0, asr #31	@ D.4606, D.4606,
	ldr	r3, [fp, #-24]	@ tmp123, base
	mov	r2, r3	@ D.4606, tmp123
	mov	r3, r2, asr #31	@ D.4606, D.4606,
	mul	lr, r2, r1	@ tmp124, D.4606, D.4606
	mul	ip, r0, r3	@ tmp125, D.4606, D.4606
	add	ip, lr, ip	@ tmp126, tmp124, tmp125
	umull	r2, r3, r0, r2	@ D.4606, D.4606, D.4606
	add	r1, ip, r3	@ tmp127, tmp126, D.4606
	mov	r3, r1	@ D.4606, tmp127
	mov	r4, r3	@ D.4606, D.4606,
	mov	r5, r3, asr #31	@ D.4606, D.4606,
	mov	r3, r4	@ tmp130, D.4606
	str	r3, [fp, #-28]	@ tmp130, iDiv10
	.loc 1 17 0
	ldr	r2, [fp, #-28]	@ tmp131, iDiv10
	mov	r3, r2	@ tmp132, tmp131
	mov	r3, r3, asl #2	@ tmp132, tmp132,
	add	r3, r3, r2	@ tmp132, tmp132, tmp131
	mov	r3, r3, asl #1	@ tmp133, tmp132,
	mov	ip, r3	@ D.4607, tmp132
	ldr	r3, [fp, #-28]	@ tmp134, iDiv10
	mvn	r2, #9	@ tmp135,
	mul	r2, r2, r3	@ D.4607, tmp135, tmp134
	ldr	r3, [fp, #-16]	@ tmp136, i
	add	r3, r2, r3	@ D.4607, D.4607, tmp136
	str	r3, [sp]	@ D.4607,
	ldr	r0, .L4+4	@,
	ldr	r1, [fp, #-16]	@, i
	ldr	r2, [fp, #-28]	@, iDiv10
	mov	r3, ip	@, D.4607
	bl	printf	@
	.loc 1 18 0
	ldr	r3, [fp, #-28]	@ tmp137, iDiv10
	mvn	r2, #9	@ tmp138,
	mul	r2, r2, r3	@ D.4607, tmp138, tmp137
	ldr	r3, [fp, #-16]	@ tmp139, i
	add	r3, r2, r3	@ D.4607, D.4607, tmp139
	ldr	r2, [fp, #-20]	@ tmp141, total
	add	r3, r2, r3	@ tmp140, tmp141, D.4607
	str	r3, [fp, #-20]	@ tmp140, total
	.loc 1 19 0
	ldr	r3, [fp, #-28]	@ tmp142, iDiv10
	str	r3, [fp, #-16]	@ tmp142, i
.L2:
.LBE2:
	.loc 1 15 0
	ldr	r3, [fp, #-16]	@ tmp143, i
	cmp	r3, #0	@ tmp143,
	bgt	.L3	@,
	.loc 1 21 0
	ldr	r0, .L4+8	@,
	ldr	r1, [fp, #-20]	@, total
	bl	printf	@
	.loc 1 22 0
	sub	sp, fp, #12	@,,
	.cfi_def_cfa 13, 16
	@ sp needed	@
	ldmfd	sp!, {r4, r5, fp, pc}	@
.L5:
	.align	2
.L4:
	.word	429496730
	.word	.LC0
	.word	.LC1
	.cfi_endproc
.LFE0:
	.size	main, .-main
.Letext0:
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0xc8
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF14
	.byte	0x1
	.4byte	.LASF15
	.4byte	.LASF16
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF2
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF6
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF7
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF8
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF9
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF10
	.uleb128 0x4
	.4byte	.LASF17
	.byte	0x1
	.byte	0xb
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x5
	.ascii	"i\000"
	.byte	0x1
	.byte	0xc
	.4byte	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x6
	.4byte	.LASF11
	.byte	0x1
	.byte	0xd
	.4byte	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x6
	.4byte	.LASF12
	.byte	0x1
	.byte	0xe
	.4byte	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x7
	.4byte	.LBB2
	.4byte	.LBE2-.LBB2
	.uleb128 0x6
	.4byte	.LASF13
	.byte	0x1
	.byte	0x10
	.4byte	0x4f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF6:
	.ascii	"long long int\000"
.LASF0:
	.ascii	"unsigned int\000"
.LASF13:
	.ascii	"iDiv10\000"
.LASF3:
	.ascii	"long unsigned int\000"
.LASF7:
	.ascii	"long long unsigned int\000"
.LASF16:
	.ascii	"/home/pi/asm\000"
.LASF11:
	.ascii	"base\000"
.LASF10:
	.ascii	"char\000"
.LASF1:
	.ascii	"unsigned char\000"
.LASF17:
	.ascii	"main\000"
.LASF8:
	.ascii	"long int\000"
.LASF14:
	.ascii	"GNU C 4.9.2 -march=armv6 -mfloat-abi=hard -mfpu=vfp"
	.ascii	" -mtls-dialect=gnu -g\000"
.LASF2:
	.ascii	"short unsigned int\000"
.LASF4:
	.ascii	"signed char\000"
.LASF12:
	.ascii	"total\000"
.LASF5:
	.ascii	"short int\000"
.LASF9:
	.ascii	"sizetype\000"
.LASF15:
	.ascii	"f2.c\000"
	.ident	"GCC: (Raspbian 4.9.2-10) 4.9.2"
	.section	.note.GNU-stack,"",%progbits
