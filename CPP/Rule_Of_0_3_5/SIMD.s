	.arch armv8-a+flagm2+lse+dotprod+rdma+crc+aes+sha3+fp16fml+jscvt+fcma+rcpc2+frintts+bf16+sb+pauth
	.file	"SIMD.cpp"
	.text
#APP
	.globl _ZSt21ios_base_library_initv
#NO_APP
	.section	.text._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
	.align	2
	.p2align 5,,15
	.weak	_ZNKSt5ctypeIcE8do_widenEc
	.type	_ZNKSt5ctypeIcE8do_widenEc, %function
_ZNKSt5ctypeIcE8do_widenEc:
.LFB1795:
	.cfi_startproc
	mov	w0, w1
	ret
	.cfi_endproc
.LFE1795:
	.size	_ZNKSt5ctypeIcE8do_widenEc, .-_ZNKSt5ctypeIcE8do_widenEc
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC0:
	.string	" "
	.section	.text.startup,"ax",@progbits
	.align	2
	.p2align 5,,15
	.global	main
	.type	main, %function
main:
.LFB2419:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA2419
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x0, 16
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	.cfi_offset 19, -32
	.cfi_offset 20, -24
	.cfi_offset 21, -16
	.cfi_offset 22, -8
.LEHB0:
	bl	_Znwm
.LEHE0:
	adrp	x21, .LC0
	movi	v31.4s, 0x3
	mov	x20, x0
	adrp	x22, :got:_ZSt4cout
	ldr	x22, [x22, :got_lo12:_ZSt4cout]
	add	x21, x21, :lo12:.LC0
	mov	x19, 0
	str	q31, [x0]
	.p2align 5,,15
.L4:
	ldr	w1, [x20, x19]
	mov	x0, x22
.LEHB1:
	bl	_ZNSolsEi
	mov	x1, x21
	mov	x2, 1
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	add	x19, x19, 4
	cmp	x19, 16
	bne	.L4
	adrp	x1, :got:_ZSt4cout
	ldr	x1, [x1, :got_lo12:_ZSt4cout]
	ldr	x0, [x1]
	ldr	x0, [x0, -24]
	add	x0, x0, x1
	ldr	x19, [x0, 240]
	cbz	x19, .L17
	ldrb	w0, [x19, 56]
	cbz	w0, .L6
	ldrb	w1, [x19, 67]
.L7:
	adrp	x0, :got:_ZSt4cout
	ldr	x0, [x0, :got_lo12:_ZSt4cout]
	bl	_ZNSo3putEc
	bl	_ZNSo5flushEv
	mov	x0, x20
	mov	x1, 16
	bl	_ZdlPvm
	mov	w0, 0
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x29, x30, [sp], 48
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
.L6:
	.cfi_restore_state
	mov	x0, x19
	bl	_ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x1, [x19]
	adrp	x0, _ZNKSt5ctypeIcE8do_widenEc
	add	x0, x0, :lo12:_ZNKSt5ctypeIcE8do_widenEc
	ldr	x2, [x1, 48]
	cmp	x2, x0
	bne	.L18
	mov	w1, 10
	b	.L7
.L18:
	mov	x0, x19
	mov	w1, 10
	blr	x2
.LEHE1:
	mov	w1, w0
	b	.L7
.L11:
.LEHB2:
	bl	_Unwind_Resume
.LEHE2:
.L17:
.LEHB3:
	bl	_ZSt16__throw_bad_castv
.LEHE3:
.L12:
	mov	x1, 16
	mov	x19, x0
	mov	x0, x20
	bl	_ZdlPvm
	mov	x0, x19
.LEHB4:
	bl	_Unwind_Resume
.LEHE4:
	.cfi_endproc
.LFE2419:
	.section	.gcc_except_table,"a",@progbits
.LLSDA2419:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2419-.LLSDACSB2419
.LLSDACSB2419:
	.uleb128 .LEHB0-.LFB2419
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L11-.LFB2419
	.uleb128 0
	.uleb128 .LEHB1-.LFB2419
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L12-.LFB2419
	.uleb128 0
	.uleb128 .LEHB2-.LFB2419
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB3-.LFB2419
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L12-.LFB2419
	.uleb128 0
	.uleb128 .LEHB4-.LFB2419
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
.LLSDACSE2419:
	.section	.text.startup
	.size	main, .-main
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align	3
	.type	DW.ref.__gxx_personality_v0, %object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.xword	__gxx_personality_v0
	.global	__gxx_personality_v0
	.ident	"GCC: (Ubuntu 15.2.0-16ubuntu1) 15.2.0"
	.section	.note.GNU-stack,"",@progbits
