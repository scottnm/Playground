	.arch armv8-a
	.file	"upper.c"
// GNU C17 (Debian 9.2.1-25) version 9.2.1 20200123 (aarch64-linux-gnu)
//	compiled by GNU C version 9.2.1 20200123, GMP version 6.1.2, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22-GMP

// GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
// options passed:  -imultiarch aarch64-linux-gnu upper.c -mlittle-endian
// -mabi=lp64 -auxbase-strip upper.s -g -O3 -fverbose-asm
// -fasynchronous-unwind-tables
// options enabled:  -fPIC -fPIE -faggressive-loop-optimizations
// -falign-functions -falign-jumps -falign-labels -falign-loops
// -fassume-phsa -fasynchronous-unwind-tables -fauto-inc-dec
// -fbranch-count-reg -fcaller-saves -fcode-hoisting
// -fcombine-stack-adjustments -fcommon -fcompare-elim -fcprop-registers
// -fcrossjumping -fcse-follow-jumps -fdefer-pop
// -fdelete-null-pointer-checks -fdevirtualize -fdevirtualize-speculatively
// -fdwarf2-cfi-asm -fearly-inlining -feliminate-unused-debug-types
// -fexpensive-optimizations -fforward-propagate -ffp-int-builtin-inexact
// -ffunction-cse -fgcse -fgcse-after-reload -fgcse-lm -fgnu-runtime
// -fgnu-unique -fguess-branch-probability -fhoist-adjacent-loads -fident
// -fif-conversion -fif-conversion2 -findirect-inlining -finline
// -finline-atomics -finline-functions -finline-functions-called-once
// -finline-small-functions -fipa-bit-cp -fipa-cp -fipa-cp-clone -fipa-icf
// -fipa-icf-functions -fipa-icf-variables -fipa-profile -fipa-pure-const
// -fipa-ra -fipa-reference -fipa-reference-addressable -fipa-sra
// -fipa-stack-alignment -fipa-vrp -fira-hoist-pressure
// -fira-share-save-slots -fira-share-spill-slots
// -fisolate-erroneous-paths-dereference -fivopts -fkeep-static-consts
// -fleading-underscore -flifetime-dse -floop-interchange
// -floop-unroll-and-jam -flra-remat -flto-odr-type-merging -fmath-errno
// -fmerge-constants -fmerge-debug-strings -fmove-loop-invariants
// -fomit-frame-pointer -foptimize-sibling-calls -foptimize-strlen
// -fpartial-inlining -fpeel-loops -fpeephole -fpeephole2 -fplt
// -fpredictive-commoning -fprefetch-loop-arrays -free -freg-struct-return
// -freorder-blocks -freorder-functions -frerun-cse-after-loop
// -fsched-critical-path-heuristic -fsched-dep-count-heuristic
// -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
// -fsched-pressure -fsched-rank-heuristic -fsched-spec
// -fsched-spec-insn-heuristic -fsched-stalled-insns-dep -fschedule-fusion
// -fschedule-insns -fschedule-insns2 -fsection-anchors
// -fsemantic-interposition -fshow-column -fshrink-wrap
// -fshrink-wrap-separate -fsigned-zeros -fsplit-ivs-in-unroller
// -fsplit-loops -fsplit-paths -fsplit-wide-types -fssa-backprop
// -fssa-phiopt -fstdarg-opt -fstore-merging -fstrict-aliasing
// -fstrict-volatile-bitfields -fsync-libcalls -fthread-jumps
// -ftoplevel-reorder -ftrapping-math -ftree-bit-ccp
// -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars
// -ftree-copy-prop -ftree-cselim -ftree-dce -ftree-dominator-opts
// -ftree-dse -ftree-forwprop -ftree-fre -ftree-loop-distribute-patterns
// -ftree-loop-distribution -ftree-loop-if-convert -ftree-loop-im
// -ftree-loop-ivcanon -ftree-loop-optimize -ftree-loop-vectorize
// -ftree-parallelize-loops= -ftree-partial-pre -ftree-phiprop -ftree-pre
// -ftree-pta -ftree-reassoc -ftree-scev-cprop -ftree-sink
// -ftree-slp-vectorize -ftree-slsr -ftree-sra -ftree-switch-conversion
// -ftree-tail-merge -ftree-ter -ftree-vrp -funit-at-a-time
// -funswitch-loops -funwind-tables -fvar-tracking
// -fvar-tracking-assignments -fverbose-asm -fversion-loops-for-strides
// -fzero-initialized-in-bss -mfix-cortex-a53-835769
// -mfix-cortex-a53-843419 -mglibc -mlittle-endian
// -momit-leaf-frame-pointer -mpc-relative-literal-loads

	.text
.Ltext0:
	.align	2
	.p2align 3,,7
	.global	mytoupper
	.type	mytoupper, %function
mytoupper:
.LVL0:
.LFB11:
	.file 1 "upper.c"
	.loc 1 4 1 view -0
	.cfi_startproc
	.loc 1 5 2 view .LVU1
	.loc 1 6 2 view .LVU2
// upper.c:4: {
	.loc 1 4 1 is_stmt 0 view .LVU3
	mov	x4, x1	// outstr, outstr
.LVL1:
.L2:
	.loc 1 8 2 is_stmt 1 view .LVU4
	.loc 1 10 3 view .LVU5
	.loc 1 13 4 view .LVU6
// upper.c:10: 		cur = *instr;
	.loc 1 10 7 is_stmt 0 view .LVU7
	ldrb	w2, [x0], 1	// cur, MEM[base: instr_4, offset: 0B]
.LVL2:
	.loc 1 11 3 is_stmt 1 view .LVU8
	.loc 1 15 3 view .LVU9
	.loc 1 15 3 view .LVU10
	add	x4, x4, 1	// outstr, outstr,
.LVL3:
// upper.c:11: 		if ((cur >= 'a') && (cur <='z')) 
	.loc 1 11 20 is_stmt 0 view .LVU11
	sub	w3, w2, #97	// tmp98, cur,
// upper.c:13: 			cur = cur - ('a'-'A');
	.loc 1 13 8 view .LVU12
	sub	w5, w2, #32	// tmp100, cur,
// upper.c:11: 		if ((cur >= 'a') && (cur <='z')) 
	.loc 1 11 6 view .LVU13
	and	w3, w3, 255	// tmp99, tmp98
	cmp	w3, 25	// tmp99,
	bhi	.L3		//,
.LVL4:
.L6:
// upper.c:15: 		*outstr++ = cur;
	.loc 1 15 13 view .LVU14
	strb	w5, [x4, -1]	// tmp100, MEM[base: _28, offset: -1B]
	.loc 1 16 3 is_stmt 1 view .LVU15
.LVL5:
	.loc 1 17 10 view .LVU16
	.loc 1 8 2 view .LVU17
	.loc 1 10 3 view .LVU18
	.loc 1 13 4 view .LVU19
	add	x4, x4, 1	// outstr, outstr,
.LVL6:
// upper.c:10: 		cur = *instr;
	.loc 1 10 7 is_stmt 0 view .LVU20
	ldrb	w2, [x0], 1	// cur, MEM[base: instr_4, offset: 0B]
.LVL7:
	.loc 1 11 3 is_stmt 1 view .LVU21
	.loc 1 15 3 view .LVU22
	.loc 1 15 3 view .LVU23
// upper.c:11: 		if ((cur >= 'a') && (cur <='z')) 
	.loc 1 11 20 is_stmt 0 view .LVU24
	sub	w3, w2, #97	// tmp98, cur,
// upper.c:13: 			cur = cur - ('a'-'A');
	.loc 1 13 8 view .LVU25
	sub	w5, w2, #32	// tmp100, cur,
// upper.c:11: 		if ((cur >= 'a') && (cur <='z')) 
	.loc 1 11 6 view .LVU26
	and	w3, w3, 255	// tmp99, tmp98
	cmp	w3, 25	// tmp99,
	bls	.L6		//,
.LVL8:
.L3:
// upper.c:15: 		*outstr++ = cur;
	.loc 1 15 13 view .LVU27
	strb	w2, [x4, -1]	// cur, MEM[base: _28, offset: -1B]
	.loc 1 16 3 is_stmt 1 view .LVU28
.LVL9:
	.loc 1 17 10 view .LVU29
// upper.c:17: 	} while (cur != '\0');
	.loc 1 17 2 is_stmt 0 view .LVU30
	cbnz	w2, .L2	// cur,
	.loc 1 18 2 is_stmt 1 view .LVU31
// upper.c:19: }
	.loc 1 19 1 is_stmt 0 view .LVU32
	sub	w0, w4, w1	//, outstr, outstr
.LVL10:
	.loc 1 19 1 view .LVU33
	ret	
	.cfi_endproc
.LFE11:
	.size	mytoupper, .-mytoupper
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC0:
	.string	"Input: %s\nOutput: %s\n"
	.section	.text.startup,"ax",@progbits
	.align	2
	.p2align 3,,7
	.global	main
	.type	main, %function
main:
.LFB12:
	.loc 1 27 1 is_stmt 1 view -0
	.cfi_startproc
	.loc 1 28 2 view .LVU35
// upper.c:27: {
	.loc 1 27 1 is_stmt 0 view .LVU36
	stp	x29, x30, [sp, -16]!	//,,,
	.cfi_def_cfa_offset 16
	.cfi_offset 29, -16
	.cfi_offset 30, -8
// upper.c:28: 	mytoupper(tstStr, outStr);
	.loc 1 28 2 view .LVU37
	adrp	x0, .LANCHOR0	// tmp99,
	adrp	x2, :got:outStr	// tmp110,
// upper.c:27: {
	.loc 1 27 1 view .LVU38
	mov	x29, sp	//,
// upper.c:28: 	mytoupper(tstStr, outStr);
	.loc 1 28 2 view .LVU39
	ldr	x1, [x0, #:lo12:.LANCHOR0]	// tstStr.1_1, tstStr
.LVL11:
.LBB4:
.LBI4:
	.loc 1 3 5 is_stmt 1 view .LVU40
.LBB5:
	.loc 1 5 2 view .LVU41
	.loc 1 6 2 view .LVU42
	.loc 1 6 2 is_stmt 0 view .LVU43
.LBE5:
.LBE4:
// upper.c:28: 	mytoupper(tstStr, outStr);
	.loc 1 28 2 view .LVU44
	ldr	x4, [x2, #:got_lo12:outStr]	// outstr, tmp110,
	mov	x5, x1	// instr, tstStr.1_1
.LVL12:
.L8:
.LBB7:
.LBB6:
	.loc 1 8 2 is_stmt 1 view .LVU45
	.loc 1 10 3 view .LVU46
	.loc 1 13 4 view .LVU47
// upper.c:10: 		cur = *instr;
	.loc 1 10 7 is_stmt 0 view .LVU48
	ldrb	w0, [x5], 1	// cur, MEM[base: instr_4, offset: 0B]
.LVL13:
	.loc 1 11 3 is_stmt 1 view .LVU49
	.loc 1 15 3 view .LVU50
	.loc 1 15 3 view .LVU51
	add	x4, x4, 1	// outstr, outstr,
.LVL14:
// upper.c:11: 		if ((cur >= 'a') && (cur <='z')) 
	.loc 1 11 20 is_stmt 0 view .LVU52
	sub	w3, w0, #97	// tmp100, cur,
// upper.c:13: 			cur = cur - ('a'-'A');
	.loc 1 13 8 view .LVU53
	sub	w6, w0, #32	// tmp102, cur,
// upper.c:11: 		if ((cur >= 'a') && (cur <='z')) 
	.loc 1 11 6 view .LVU54
	and	w3, w3, 255	// tmp101, tmp100
	cmp	w3, 25	// tmp101,
	bhi	.L9		//,
.LVL15:
.L12:
// upper.c:15: 		*outstr++ = cur;
	.loc 1 15 13 view .LVU55
	strb	w6, [x4, -1]	// tmp102, MEM[base: _14, offset: -1B]
	.loc 1 16 3 is_stmt 1 view .LVU56
.LVL16:
	.loc 1 17 10 view .LVU57
	.loc 1 8 2 view .LVU58
	.loc 1 10 3 view .LVU59
	.loc 1 13 4 view .LVU60
	add	x4, x4, 1	// outstr, outstr,
.LVL17:
// upper.c:10: 		cur = *instr;
	.loc 1 10 7 is_stmt 0 view .LVU61
	ldrb	w0, [x5], 1	// cur, MEM[base: instr_4, offset: 0B]
.LVL18:
	.loc 1 11 3 is_stmt 1 view .LVU62
	.loc 1 15 3 view .LVU63
	.loc 1 15 3 view .LVU64
// upper.c:11: 		if ((cur >= 'a') && (cur <='z')) 
	.loc 1 11 20 is_stmt 0 view .LVU65
	sub	w3, w0, #97	// tmp100, cur,
// upper.c:13: 			cur = cur - ('a'-'A');
	.loc 1 13 8 view .LVU66
	sub	w6, w0, #32	// tmp102, cur,
// upper.c:11: 		if ((cur >= 'a') && (cur <='z')) 
	.loc 1 11 6 view .LVU67
	and	w3, w3, 255	// tmp101, tmp100
	cmp	w3, 25	// tmp101,
	bls	.L12		//,
.LVL19:
.L9:
// upper.c:15: 		*outstr++ = cur;
	.loc 1 15 13 view .LVU68
	strb	w0, [x4, -1]	// cur, MEM[base: _14, offset: -1B]
	.loc 1 16 3 is_stmt 1 view .LVU69
.LVL20:
	.loc 1 17 10 view .LVU70
// upper.c:17: 	} while (cur != '\0');
	.loc 1 17 2 is_stmt 0 view .LVU71
	cbnz	w0, .L8	// cur,
	.loc 1 18 2 is_stmt 1 view .LVU72
.LVL21:
	.loc 1 18 2 is_stmt 0 view .LVU73
.LBE6:
.LBE7:
	.loc 1 29 2 is_stmt 1 view .LVU74
	ldr	x2, [x2, #:got_lo12:outStr]	//, tmp110,
	adrp	x0, .LC0	// tmp108,
	add	x0, x0, :lo12:.LC0	//, tmp108,
	bl	printf		//
.LVL22:
	.loc 1 31 2 view .LVU75
// upper.c:32: }
	.loc 1 32 1 is_stmt 0 view .LVU76
	mov	w0, 0	//,
	ldp	x29, x30, [sp], 16	//,,,
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret	
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.comm	outStr,250,8
	.global	tstStr
	.section	.rodata.str1.8
	.align	3
.LC1:
	.string	"This is a test!"
	.section	.data.rel.local,"aw"
	.align	3
	.set	.LANCHOR0,. + 0
	.type	tstStr, %object
	.size	tstStr, 8
tstStr:
	.xword	.LC1
	.text
.Letext0:
	.file 2 "/usr/lib/gcc/aarch64-linux-gnu/9/include/stddef.h"
	.file 3 "/usr/include/aarch64-linux-gnu/bits/types.h"
	.file 4 "/usr/include/aarch64-linux-gnu/bits/types/struct_FILE.h"
	.file 5 "/usr/include/aarch64-linux-gnu/bits/types/FILE.h"
	.file 6 "/usr/include/stdio.h"
	.file 7 "/usr/include/aarch64-linux-gnu/bits/sys_errlist.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0x44b
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.4byte	.LASF54
	.byte	0xc
	.4byte	.LASF55
	.4byte	.LASF56
	.4byte	.Ldebug_ranges0+0x30
	.8byte	0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.4byte	.LASF7
	.byte	0x2
	.byte	0xd1
	.byte	0x17
	.4byte	0x35
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.4byte	.LASF0
	.uleb128 0x4
	.byte	0x8
	.uleb128 0x5
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.4byte	.LASF2
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.4byte	.LASF3
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.4byte	.LASF4
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.4byte	.LASF5
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.4byte	.LASF6
	.uleb128 0x2
	.4byte	.LASF8
	.byte	0x3
	.byte	0x98
	.byte	0x19
	.4byte	0x68
	.uleb128 0x2
	.4byte	.LASF9
	.byte	0x3
	.byte	0x99
	.byte	0x1b
	.4byte	0x68
	.uleb128 0x6
	.byte	0x8
	.4byte	0x8d
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.4byte	.LASF10
	.uleb128 0x7
	.4byte	0x8d
	.uleb128 0x8
	.4byte	.LASF57
	.byte	0xd8
	.byte	0x4
	.byte	0x31
	.byte	0x8
	.4byte	0x220
	.uleb128 0x9
	.4byte	.LASF11
	.byte	0x4
	.byte	0x33
	.byte	0x7
	.4byte	0x3e
	.byte	0
	.uleb128 0x9
	.4byte	.LASF12
	.byte	0x4
	.byte	0x36
	.byte	0x9
	.4byte	0x87
	.byte	0x8
	.uleb128 0x9
	.4byte	.LASF13
	.byte	0x4
	.byte	0x37
	.byte	0x9
	.4byte	0x87
	.byte	0x10
	.uleb128 0x9
	.4byte	.LASF14
	.byte	0x4
	.byte	0x38
	.byte	0x9
	.4byte	0x87
	.byte	0x18
	.uleb128 0x9
	.4byte	.LASF15
	.byte	0x4
	.byte	0x39
	.byte	0x9
	.4byte	0x87
	.byte	0x20
	.uleb128 0x9
	.4byte	.LASF16
	.byte	0x4
	.byte	0x3a
	.byte	0x9
	.4byte	0x87
	.byte	0x28
	.uleb128 0x9
	.4byte	.LASF17
	.byte	0x4
	.byte	0x3b
	.byte	0x9
	.4byte	0x87
	.byte	0x30
	.uleb128 0x9
	.4byte	.LASF18
	.byte	0x4
	.byte	0x3c
	.byte	0x9
	.4byte	0x87
	.byte	0x38
	.uleb128 0x9
	.4byte	.LASF19
	.byte	0x4
	.byte	0x3d
	.byte	0x9
	.4byte	0x87
	.byte	0x40
	.uleb128 0x9
	.4byte	.LASF20
	.byte	0x4
	.byte	0x40
	.byte	0x9
	.4byte	0x87
	.byte	0x48
	.uleb128 0x9
	.4byte	.LASF21
	.byte	0x4
	.byte	0x41
	.byte	0x9
	.4byte	0x87
	.byte	0x50
	.uleb128 0x9
	.4byte	.LASF22
	.byte	0x4
	.byte	0x42
	.byte	0x9
	.4byte	0x87
	.byte	0x58
	.uleb128 0x9
	.4byte	.LASF23
	.byte	0x4
	.byte	0x44
	.byte	0x16
	.4byte	0x239
	.byte	0x60
	.uleb128 0x9
	.4byte	.LASF24
	.byte	0x4
	.byte	0x46
	.byte	0x14
	.4byte	0x23f
	.byte	0x68
	.uleb128 0x9
	.4byte	.LASF25
	.byte	0x4
	.byte	0x48
	.byte	0x7
	.4byte	0x3e
	.byte	0x70
	.uleb128 0x9
	.4byte	.LASF26
	.byte	0x4
	.byte	0x49
	.byte	0x7
	.4byte	0x3e
	.byte	0x74
	.uleb128 0x9
	.4byte	.LASF27
	.byte	0x4
	.byte	0x4a
	.byte	0xb
	.4byte	0x6f
	.byte	0x78
	.uleb128 0x9
	.4byte	.LASF28
	.byte	0x4
	.byte	0x4d
	.byte	0x12
	.4byte	0x4c
	.byte	0x80
	.uleb128 0x9
	.4byte	.LASF29
	.byte	0x4
	.byte	0x4e
	.byte	0xf
	.4byte	0x5a
	.byte	0x82
	.uleb128 0x9
	.4byte	.LASF30
	.byte	0x4
	.byte	0x4f
	.byte	0x8
	.4byte	0x245
	.byte	0x83
	.uleb128 0x9
	.4byte	.LASF31
	.byte	0x4
	.byte	0x51
	.byte	0xf
	.4byte	0x255
	.byte	0x88
	.uleb128 0x9
	.4byte	.LASF32
	.byte	0x4
	.byte	0x59
	.byte	0xd
	.4byte	0x7b
	.byte	0x90
	.uleb128 0x9
	.4byte	.LASF33
	.byte	0x4
	.byte	0x5b
	.byte	0x17
	.4byte	0x260
	.byte	0x98
	.uleb128 0x9
	.4byte	.LASF34
	.byte	0x4
	.byte	0x5c
	.byte	0x19
	.4byte	0x26b
	.byte	0xa0
	.uleb128 0x9
	.4byte	.LASF35
	.byte	0x4
	.byte	0x5d
	.byte	0x14
	.4byte	0x23f
	.byte	0xa8
	.uleb128 0x9
	.4byte	.LASF36
	.byte	0x4
	.byte	0x5e
	.byte	0x9
	.4byte	0x3c
	.byte	0xb0
	.uleb128 0x9
	.4byte	.LASF37
	.byte	0x4
	.byte	0x5f
	.byte	0xa
	.4byte	0x29
	.byte	0xb8
	.uleb128 0x9
	.4byte	.LASF38
	.byte	0x4
	.byte	0x60
	.byte	0x7
	.4byte	0x3e
	.byte	0xc0
	.uleb128 0x9
	.4byte	.LASF39
	.byte	0x4
	.byte	0x62
	.byte	0x8
	.4byte	0x271
	.byte	0xc4
	.byte	0
	.uleb128 0x2
	.4byte	.LASF40
	.byte	0x5
	.byte	0x7
	.byte	0x19
	.4byte	0x99
	.uleb128 0xa
	.4byte	.LASF58
	.byte	0x4
	.byte	0x2b
	.byte	0xe
	.uleb128 0xb
	.4byte	.LASF41
	.uleb128 0x6
	.byte	0x8
	.4byte	0x234
	.uleb128 0x6
	.byte	0x8
	.4byte	0x99
	.uleb128 0xc
	.4byte	0x8d
	.4byte	0x255
	.uleb128 0xd
	.4byte	0x35
	.byte	0
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.4byte	0x22c
	.uleb128 0xb
	.4byte	.LASF42
	.uleb128 0x6
	.byte	0x8
	.4byte	0x25b
	.uleb128 0xb
	.4byte	.LASF43
	.uleb128 0x6
	.byte	0x8
	.4byte	0x266
	.uleb128 0xc
	.4byte	0x8d
	.4byte	0x281
	.uleb128 0xd
	.4byte	0x35
	.byte	0x13
	.byte	0
	.uleb128 0xe
	.4byte	.LASF44
	.byte	0x6
	.byte	0x89
	.byte	0xe
	.4byte	0x28d
	.uleb128 0x6
	.byte	0x8
	.4byte	0x220
	.uleb128 0xe
	.4byte	.LASF45
	.byte	0x6
	.byte	0x8a
	.byte	0xe
	.4byte	0x28d
	.uleb128 0xe
	.4byte	.LASF46
	.byte	0x6
	.byte	0x8b
	.byte	0xe
	.4byte	0x28d
	.uleb128 0xe
	.4byte	.LASF47
	.byte	0x7
	.byte	0x1a
	.byte	0xc
	.4byte	0x3e
	.uleb128 0xc
	.4byte	0x2cd
	.4byte	0x2c2
	.uleb128 0xf
	.byte	0
	.uleb128 0x7
	.4byte	0x2b7
	.uleb128 0x6
	.byte	0x8
	.4byte	0x94
	.uleb128 0x7
	.4byte	0x2c7
	.uleb128 0xe
	.4byte	.LASF48
	.byte	0x7
	.byte	0x1b
	.byte	0x1a
	.4byte	0x2c2
	.uleb128 0x10
	.4byte	.LASF49
	.byte	0x1
	.byte	0x17
	.byte	0x7
	.4byte	0x87
	.uleb128 0x9
	.byte	0x3
	.8byte	tstStr
	.uleb128 0xc
	.4byte	0x8d
	.4byte	0x304
	.uleb128 0xd
	.4byte	0x35
	.byte	0xf9
	.byte	0
	.uleb128 0x10
	.4byte	.LASF50
	.byte	0x1
	.byte	0x18
	.byte	0x6
	.4byte	0x2f4
	.uleb128 0x9
	.byte	0x3
	.8byte	outStr
	.uleb128 0x11
	.4byte	.LASF59
	.byte	0x1
	.byte	0x1a
	.byte	0x5
	.4byte	0x3e
	.8byte	.LFB12
	.8byte	.LFE12-.LFB12
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x3af
	.uleb128 0x12
	.4byte	0x3af
	.8byte	.LBI4
	.byte	.LVU40
	.4byte	.Ldebug_ranges0+0
	.byte	0x1
	.byte	0x1c
	.byte	0x2
	.4byte	0x393
	.uleb128 0x13
	.4byte	0x3cc
	.4byte	.LLST4
	.4byte	.LVUS4
	.uleb128 0x13
	.4byte	0x3c0
	.4byte	.LLST5
	.4byte	.LVUS5
	.uleb128 0x14
	.4byte	.Ldebug_ranges0+0
	.uleb128 0x15
	.4byte	0x3d8
	.4byte	.LLST6
	.4byte	.LVUS6
	.uleb128 0x16
	.4byte	0x3e4
	.uleb128 0xa
	.byte	0x3
	.8byte	outStr
	.byte	0x9f
	.byte	0
	.byte	0
	.uleb128 0x17
	.8byte	.LVL22
	.4byte	0x441
	.uleb128 0x18
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x9
	.byte	0x3
	.8byte	.LC0
	.byte	0
	.byte	0
	.uleb128 0x19
	.4byte	.LASF60
	.byte	0x1
	.byte	0x3
	.byte	0x5
	.4byte	0x3e
	.byte	0x1
	.4byte	0x3f1
	.uleb128 0x1a
	.4byte	.LASF51
	.byte	0x1
	.byte	0x3
	.byte	0x15
	.4byte	0x87
	.uleb128 0x1a
	.4byte	.LASF52
	.byte	0x1
	.byte	0x3
	.byte	0x22
	.4byte	0x87
	.uleb128 0x1b
	.string	"cur"
	.byte	0x1
	.byte	0x5
	.byte	0x7
	.4byte	0x8d
	.uleb128 0x1c
	.4byte	.LASF53
	.byte	0x1
	.byte	0x6
	.byte	0x8
	.4byte	0x87
	.byte	0
	.uleb128 0x1d
	.4byte	0x3af
	.8byte	.LFB11
	.8byte	.LFE11-.LFB11
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x441
	.uleb128 0x13
	.4byte	0x3c0
	.4byte	.LLST0
	.4byte	.LVUS0
	.uleb128 0x13
	.4byte	0x3cc
	.4byte	.LLST1
	.4byte	.LVUS1
	.uleb128 0x15
	.4byte	0x3d8
	.4byte	.LLST2
	.4byte	.LVUS2
	.uleb128 0x15
	.4byte	0x3e4
	.4byte	.LLST3
	.4byte	.LVUS3
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF61
	.4byte	.LASF61
	.byte	0x6
	.2byte	0x14c
	.byte	0xc
	.byte	0
	.section	.debug_abbrev,"",@progbits
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
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
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
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x5
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
	.uleb128 0x6
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x21
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x11
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
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x2138
	.uleb128 0xb
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x57
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.uleb128 0x2137
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x34
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.uleb128 0x2137
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x34
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x410a
	.byte	0
	.uleb128 0x2
	.uleb128 0x18
	.uleb128 0x2111
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x19
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
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x6e
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LVUS4:
	.uleb128 .LVU40
	.uleb128 .LVU45
	.uleb128 .LVU45
	.uleb128 .LVU52
	.uleb128 .LVU52
	.uleb128 .LVU55
	.uleb128 .LVU58
	.uleb128 .LVU61
	.uleb128 .LVU61
	.uleb128 .LVU65
	.uleb128 .LVU65
	.uleb128 .LVU68
.LLST4:
	.8byte	.LVL11
	.8byte	.LVL12
	.2byte	0xa
	.byte	0x3
	.8byte	outStr
	.byte	0x9f
	.8byte	.LVL12
	.8byte	.LVL14
	.2byte	0x1
	.byte	0x54
	.8byte	.LVL14
	.8byte	.LVL15
	.2byte	0x3
	.byte	0x74
	.sleb128 1
	.byte	0x9f
	.8byte	.LVL16
	.8byte	.LVL17
	.2byte	0x1
	.byte	0x54
	.8byte	.LVL17
	.8byte	.LVL18
	.2byte	0x3
	.byte	0x74
	.sleb128 -1
	.byte	0x9f
	.8byte	.LVL18
	.8byte	.LVL19
	.2byte	0x3
	.byte	0x74
	.sleb128 1
	.byte	0x9f
	.8byte	0
	.8byte	0
.LVUS5:
	.uleb128 .LVU40
	.uleb128 .LVU45
	.uleb128 .LVU45
	.uleb128 .LVU49
	.uleb128 .LVU49
	.uleb128 .LVU55
	.uleb128 .LVU57
	.uleb128 .LVU58
	.uleb128 .LVU58
	.uleb128 .LVU62
	.uleb128 .LVU62
	.uleb128 .LVU68
	.uleb128 .LVU70
	.uleb128 .LVU73
.LLST5:
	.8byte	.LVL11
	.8byte	.LVL12
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL12
	.8byte	.LVL13
	.2byte	0x1
	.byte	0x55
	.8byte	.LVL13
	.8byte	.LVL15
	.2byte	0x3
	.byte	0x75
	.sleb128 -1
	.byte	0x9f
	.8byte	.LVL16
	.8byte	.LVL16
	.2byte	0x3
	.byte	0x75
	.sleb128 1
	.byte	0x9f
	.8byte	.LVL16
	.8byte	.LVL18
	.2byte	0x1
	.byte	0x55
	.8byte	.LVL18
	.8byte	.LVL19
	.2byte	0x3
	.byte	0x75
	.sleb128 -1
	.byte	0x9f
	.8byte	.LVL20
	.8byte	.LVL21
	.2byte	0x3
	.byte	0x75
	.sleb128 1
	.byte	0x9f
	.8byte	0
	.8byte	0
.LVUS6:
	.uleb128 .LVU49
	.uleb128 .LVU50
	.uleb128 .LVU50
	.uleb128 .LVU51
	.uleb128 .LVU51
	.uleb128 .LVU58
	.uleb128 .LVU62
	.uleb128 .LVU63
	.uleb128 .LVU63
	.uleb128 .LVU64
	.uleb128 .LVU64
	.uleb128 .LVU73
.LLST6:
	.8byte	.LVL13
	.8byte	.LVL13
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL13
	.8byte	.LVL13
	.2byte	0x3
	.byte	0x70
	.sleb128 -32
	.byte	0x9f
	.8byte	.LVL13
	.8byte	.LVL16
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL18
	.8byte	.LVL18
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL18
	.8byte	.LVL18
	.2byte	0x3
	.byte	0x70
	.sleb128 -32
	.byte	0x9f
	.8byte	.LVL18
	.8byte	.LVL21
	.2byte	0x1
	.byte	0x50
	.8byte	0
	.8byte	0
.LVUS0:
	.uleb128 0
	.uleb128 .LVU4
	.uleb128 .LVU4
	.uleb128 .LVU8
	.uleb128 .LVU8
	.uleb128 .LVU14
	.uleb128 .LVU16
	.uleb128 .LVU17
	.uleb128 .LVU17
	.uleb128 .LVU21
	.uleb128 .LVU21
	.uleb128 .LVU27
	.uleb128 .LVU29
	.uleb128 .LVU33
.LLST0:
	.8byte	.LVL0
	.8byte	.LVL1
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL1
	.8byte	.LVL2
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL2
	.8byte	.LVL4
	.2byte	0x3
	.byte	0x70
	.sleb128 -1
	.byte	0x9f
	.8byte	.LVL5
	.8byte	.LVL5
	.2byte	0x3
	.byte	0x70
	.sleb128 1
	.byte	0x9f
	.8byte	.LVL5
	.8byte	.LVL7
	.2byte	0x1
	.byte	0x50
	.8byte	.LVL7
	.8byte	.LVL8
	.2byte	0x3
	.byte	0x70
	.sleb128 -1
	.byte	0x9f
	.8byte	.LVL9
	.8byte	.LVL10
	.2byte	0x3
	.byte	0x70
	.sleb128 1
	.byte	0x9f
	.8byte	0
	.8byte	0
.LVUS1:
	.uleb128 0
	.uleb128 .LVU4
	.uleb128 .LVU4
	.uleb128 .LVU11
	.uleb128 .LVU11
	.uleb128 .LVU14
	.uleb128 .LVU17
	.uleb128 .LVU20
	.uleb128 .LVU20
	.uleb128 .LVU24
	.uleb128 .LVU24
	.uleb128 .LVU27
.LLST1:
	.8byte	.LVL0
	.8byte	.LVL1
	.2byte	0x1
	.byte	0x51
	.8byte	.LVL1
	.8byte	.LVL3
	.2byte	0x1
	.byte	0x54
	.8byte	.LVL3
	.8byte	.LVL4
	.2byte	0x3
	.byte	0x74
	.sleb128 1
	.byte	0x9f
	.8byte	.LVL5
	.8byte	.LVL6
	.2byte	0x1
	.byte	0x54
	.8byte	.LVL6
	.8byte	.LVL7
	.2byte	0x3
	.byte	0x74
	.sleb128 -1
	.byte	0x9f
	.8byte	.LVL7
	.8byte	.LVL8
	.2byte	0x3
	.byte	0x74
	.sleb128 1
	.byte	0x9f
	.8byte	0
	.8byte	0
.LVUS2:
	.uleb128 .LVU8
	.uleb128 .LVU9
	.uleb128 .LVU9
	.uleb128 .LVU10
	.uleb128 .LVU10
	.uleb128 .LVU17
	.uleb128 .LVU21
	.uleb128 .LVU22
	.uleb128 .LVU22
	.uleb128 .LVU23
	.uleb128 .LVU23
	.uleb128 0
.LLST2:
	.8byte	.LVL2
	.8byte	.LVL2
	.2byte	0x1
	.byte	0x52
	.8byte	.LVL2
	.8byte	.LVL2
	.2byte	0x3
	.byte	0x72
	.sleb128 -32
	.byte	0x9f
	.8byte	.LVL2
	.8byte	.LVL5
	.2byte	0x1
	.byte	0x52
	.8byte	.LVL7
	.8byte	.LVL7
	.2byte	0x1
	.byte	0x52
	.8byte	.LVL7
	.8byte	.LVL7
	.2byte	0x3
	.byte	0x72
	.sleb128 -32
	.byte	0x9f
	.8byte	.LVL7
	.8byte	.LFE11
	.2byte	0x1
	.byte	0x52
	.8byte	0
	.8byte	0
.LVUS3:
	.uleb128 .LVU3
	.uleb128 0
.LLST3:
	.8byte	.LVL0
	.8byte	.LFE11
	.2byte	0x1
	.byte	0x51
	.8byte	0
	.8byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x3c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x8
	.byte	0
	.2byte	0
	.2byte	0
	.8byte	.Ltext0
	.8byte	.Letext0-.Ltext0
	.8byte	.LFB12
	.8byte	.LFE12-.LFB12
	.8byte	0
	.8byte	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.8byte	.LBB4
	.8byte	.LBE4
	.8byte	.LBB7
	.8byte	.LBE7
	.8byte	0
	.8byte	0
	.8byte	.Ltext0
	.8byte	.Letext0
	.8byte	.LFB12
	.8byte	.LFE12
	.8byte	0
	.8byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF19:
	.string	"_IO_buf_end"
.LASF27:
	.string	"_old_offset"
.LASF47:
	.string	"sys_nerr"
.LASF22:
	.string	"_IO_save_end"
.LASF5:
	.string	"short int"
.LASF7:
	.string	"size_t"
.LASF32:
	.string	"_offset"
.LASF16:
	.string	"_IO_write_ptr"
.LASF11:
	.string	"_flags"
.LASF18:
	.string	"_IO_buf_base"
.LASF54:
	.string	"GNU C17 9.2.1 20200123 -mlittle-endian -mabi=lp64 -g -O3 -fasynchronous-unwind-tables"
.LASF23:
	.string	"_markers"
.LASF13:
	.string	"_IO_read_end"
.LASF36:
	.string	"_freeres_buf"
.LASF52:
	.string	"outstr"
.LASF60:
	.string	"mytoupper"
.LASF46:
	.string	"stderr"
.LASF49:
	.string	"tstStr"
.LASF31:
	.string	"_lock"
.LASF6:
	.string	"long int"
.LASF61:
	.string	"printf"
.LASF28:
	.string	"_cur_column"
.LASF57:
	.string	"_IO_FILE"
.LASF51:
	.string	"instr"
.LASF1:
	.string	"unsigned char"
.LASF4:
	.string	"signed char"
.LASF33:
	.string	"_codecvt"
.LASF3:
	.string	"unsigned int"
.LASF41:
	.string	"_IO_marker"
.LASF30:
	.string	"_shortbuf"
.LASF15:
	.string	"_IO_write_base"
.LASF39:
	.string	"_unused2"
.LASF12:
	.string	"_IO_read_ptr"
.LASF37:
	.string	"__pad5"
.LASF2:
	.string	"short unsigned int"
.LASF10:
	.string	"char"
.LASF59:
	.string	"main"
.LASF55:
	.string	"upper.c"
.LASF34:
	.string	"_wide_data"
.LASF35:
	.string	"_freeres_list"
.LASF56:
	.string	"/home/smist08/asm64/Chapter 15"
.LASF42:
	.string	"_IO_codecvt"
.LASF0:
	.string	"long unsigned int"
.LASF17:
	.string	"_IO_write_end"
.LASF9:
	.string	"__off64_t"
.LASF8:
	.string	"__off_t"
.LASF24:
	.string	"_chain"
.LASF43:
	.string	"_IO_wide_data"
.LASF21:
	.string	"_IO_backup_base"
.LASF44:
	.string	"stdin"
.LASF26:
	.string	"_flags2"
.LASF38:
	.string	"_mode"
.LASF14:
	.string	"_IO_read_base"
.LASF53:
	.string	"orig_outstr"
.LASF29:
	.string	"_vtable_offset"
.LASF20:
	.string	"_IO_save_base"
.LASF48:
	.string	"sys_errlist"
.LASF25:
	.string	"_fileno"
.LASF40:
	.string	"FILE"
.LASF50:
	.string	"outStr"
.LASF45:
	.string	"stdout"
.LASF58:
	.string	"_IO_lock_t"
	.ident	"GCC: (Debian 9.2.1-25) 9.2.1 20200123"
	.section	.note.GNU-stack,"",@progbits
