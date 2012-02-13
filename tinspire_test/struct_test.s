	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"struct_test.c"
	.bss
	.align	2
_syscallvar_savedlr:
	.space	4
	.text
	.align	2
	.type	_show_msgbox_2b_swi, %function
_show_msgbox_2b_swi:
	@ Function supports interworking.
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 207 "c:/Users/Hoffa/Tools/ndless-v3.1-beta-r526/sdk/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 93
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	_show_msgbox_2b_swi, .-_show_msgbox_2b_swi
	.align	2
	.type	_show_msgbox_3b_swi, %function
_show_msgbox_3b_swi:
	@ Function supports interworking.
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 208 "c:/Users/Hoffa/Tools/ndless-v3.1-beta-r526/sdk/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 94
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	_show_msgbox_3b_swi, .-_show_msgbox_3b_swi
	.align	2
	.type	printf, %function
printf:
	@ Function supports interworking.
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 16, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
@ 257 "c:/Users/Hoffa/Tools/ndless-v3.1-beta-r526/sdk/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 10
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	printf, .-printf
	.align	2
	.type	sprintf, %function
sprintf:
	@ Function supports interworking.
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 12, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
@ 258 "c:/Users/Hoffa/Tools/ndless-v3.1-beta-r526/sdk/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 11
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	sprintf, .-sprintf
	.align	2
	.type	fprintf, %function
fprintf:
	@ Function supports interworking.
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 4, pretend = 12, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 1
@ 259 "c:/Users/Hoffa/Tools/ndless-v3.1-beta-r526/sdk/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 12
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	fprintf, .-fprintf
	.align	2
	.type	deflateInit2__swi, %function
deflateInit2__swi:
	@ Function supports interworking.
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
@ 374 "c:/Users/Hoffa/Tools/ndless-v3.1-beta-r526/sdk/bin/../include/os.h" 1
	 push {r4, r5} 
 ldr r4, 1f 
 ldr r5, 1f+4 
0:	
 add r4, pc, r4 
 ldr r4, [r4, r5] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 str lr, [r4] 
 pop {r4, r5} 
 swi 82
 ldr r1, 1f 
 ldr r2, 1f+4 
0:	
 add r1, pc, r1 
 ldr r1, [r1, r2] 
 b 2f 
 .align 2 
1: 
 .long _GLOBAL_OFFSET_TABLE_ - (0b+8) 
 .long _syscallvar_savedlr(GOT) 
2: 
 ldr pc, [r1] 

@ 0 "" 2
	mov	r3, r0
	mov	r0, r3
	.size	deflateInit2__swi, .-deflateInit2__swi
	.align	2
	.type	exit, %function
exit:
	@ Function supports interworking.
	@ Naked Function: prologue and epilogue provided by programmer.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	ldr	r3, .L8
.LPIC0:
	add	r3, pc, r3
	ldr	r2, .L8+4
	ldr	r2, [r3, r2]
	ldr	r2, [r2, #0]
	ldr	r1, .L8+8
	ldr	r3, [r3, r1]
@ 399 "c:/Users/Hoffa/Tools/ndless-v3.1-beta-r526/sdk/bin/../include/os.h" 1
	 mov sp, r2 
 mov pc, r3
@ 0 "" 2
.L9:
	.align	2
.L8:
	.word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+8)
	.word	__crt0_savedsp(GOT)
	.word	__crt0exit(GOT)
	.size	exit, .-exit
	.align	2
	.type	abort, %function
abort:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	mvn	r0, #0
	bl	exit(PLT)
	.size	abort, .-abort
	.section	.rodata
	.align	2
.LC3:
	.ascii	"foobar\000"
	.align	2
.LC4:
	.ascii	"test1.s: %s\012\000"
	.align	2
.LC5:
	.ascii	"test2.s1: %s\012\000"
	.align	2
.LC6:
	.ascii	"test2.s2: %s\012\000"
	.align	2
.LC0:
	.ascii	"foo\000"
	.align	2
.LC1:
	.ascii	"bar\000"
	.data
	.align	2
.LC2:
	.word	.LC0
	.word	.LC1
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	ldr	r3, .L12
.LPIC1:
	add	r3, pc, r3
	str	r3, [fp, #-8]
	ldr	r3, .L12+4
.LPIC2:
	add	r3, pc, r3
	sub	r2, fp, #16
	ldmia	r3, {r0, r1}
	stmia	r2, {r0, r1}
	ldr	r2, [fp, #-8]
	ldr	r3, .L12+8
.LPIC3:
	add	r3, pc, r3
	mov	r0, r3
	mov	r1, r2
	bl	printf(PLT)
	ldr	r2, [fp, #-16]
	ldr	r3, .L12+12
.LPIC4:
	add	r3, pc, r3
	mov	r0, r3
	mov	r1, r2
	bl	printf(PLT)
	ldr	r2, [fp, #-12]
	ldr	r3, .L12+16
.LPIC5:
	add	r3, pc, r3
	mov	r0, r3
	mov	r1, r2
	bl	printf(PLT)
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, lr}
	bx	lr
.L13:
	.align	2
.L12:
	.word	.LC3-(.LPIC1+8)
	.word	.LC2-(.LPIC2+8)
	.word	.LC4-(.LPIC3+8)
	.word	.LC5-(.LPIC4+8)
	.word	.LC6-(.LPIC5+8)
	.size	main, .-main
	.ident	"GCC: (GNU) 4.6.2"
