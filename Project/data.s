	.file	"data.cpp"
	.text
	.section	.rodata
	.type	_ZStL19piecewise_construct, @object
	.size	_ZStL19piecewise_construct, 1
_ZStL19piecewise_construct:
	.zero	1
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.text
	.globl	_ZppR5Month
	.type	_ZppR5Month, @function
_ZppR5Month:
.LFB1493:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	cmpl	$12, %eax
	je	.L2
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	jmp	.L3
.L2:
	movl	$1, %edx
.L3:
	movq	-8(%rbp), %rax
	movl	%edx, (%rax)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1493:
	.size	_ZppR5Month, .-_ZppR5Month
	.section	.text._ZN4Date5monthEv,"axG",@progbits,_ZN4Date5monthEv,comdat
	.align 2
	.weak	_ZN4Date5monthEv
	.type	_ZN4Date5monthEv, @function
_ZN4Date5monthEv:
.LFB1494:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1494:
	.size	_ZN4Date5monthEv, .-_ZN4Date5monthEv
	.text
	.align 2
	.globl	_ZN4DateC2Eiii
	.type	_ZN4DateC2Eiii, @function
_ZN4DateC2Eiii:
.LFB1498:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movl	%ecx, -20(%rbp)
	movq	-8(%rbp), %rax
	movl	-12(%rbp), %edx
	movl	%edx, (%rax)
	movq	-8(%rbp), %rax
	movl	-20(%rbp), %edx
	movl	%edx, 4(%rax)
	movq	-8(%rbp), %rax
	movl	-16(%rbp), %edx
	movl	%edx, 8(%rax)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN4Date5validEv
	xorl	$1, %eax
	testb	%al, %al
	je	.L9
	movl	$1, %edi
	call	__cxa_allocate_exception@PLT
	movl	$0, %edx
	leaq	_ZTIN4Date7invalidE(%rip), %rsi
	movq	%rax, %rdi
	call	__cxa_throw@PLT
.L9:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1498:
	.size	_ZN4DateC2Eiii, .-_ZN4DateC2Eiii
	.globl	_ZN4DateC1Eiii
	.set	_ZN4DateC1Eiii,_ZN4DateC2Eiii
	.align 2
	.globl	_ZN4Date5validEv
	.type	_ZN4Date5validEv, @function
_ZN4Date5validEv:
.LFB1500:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	testl	%eax, %eax
	jle	.L11
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	cmpl	$12, %eax
	jle	.L12
.L11:
	movl	$0, %eax
	jmp	.L10
.L12:
.L10:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1500:
	.size	_ZN4Date5validEv, .-_ZN4Date5validEv
	.globl	main
	.type	main, @function
main:
.LFB1501:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA1501
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	leaq	-36(%rbp), %rax
	movl	$1991, %ecx
	movl	$5, %edx
	movl	$23, %esi
	movq	%rax, %rdi
.LEHB0:
	call	_ZN4DateC1Eiii
.LEHE0:
	movl	$1, -40(%rbp)
	leaq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	_ZppR5Month
	leaq	-36(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN4Date5monthEv
	movl	%eax, %esi
	leaq	_ZSt4cout(%rip), %rdi
.LEHB1:
	call	_ZNSolsEi@PLT
	movq	%rax, %rdx
	movl	-40(%rbp), %eax
	movl	%eax, %esi
	movq	%rdx, %rdi
	call	_ZNSolsEi@PLT
.LEHE1:
	leaq	-36(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN4DateD1Ev@PLT
	movl	$0, %eax
	movq	-24(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L17
	jmp	.L19
.L18:
	movq	%rax, %rbx
	leaq	-36(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN4DateD1Ev@PLT
	movq	%rbx, %rax
	movq	%rax, %rdi
.LEHB2:
	call	_Unwind_Resume@PLT
.LEHE2:
.L19:
	call	__stack_chk_fail@PLT
.L17:
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1501:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA1501:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE1501-.LLSDACSB1501
.LLSDACSB1501:
	.uleb128 .LEHB0-.LFB1501
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB1501
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L18-.LFB1501
	.uleb128 0
	.uleb128 .LEHB2-.LFB1501
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSE1501:
	.text
	.size	main, .-main
	.weak	_ZTIN4Date7invalidE
	.section	.data.rel.ro._ZTIN4Date7invalidE,"awG",@progbits,_ZTIN4Date7invalidE,comdat
	.align 8
	.type	_ZTIN4Date7invalidE, @object
	.size	_ZTIN4Date7invalidE, 16
_ZTIN4Date7invalidE:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSN4Date7invalidE
	.weak	_ZTSN4Date7invalidE
	.section	.rodata._ZTSN4Date7invalidE,"aG",@progbits,_ZTSN4Date7invalidE,comdat
	.align 16
	.type	_ZTSN4Date7invalidE, @object
	.size	_ZTSN4Date7invalidE, 16
_ZTSN4Date7invalidE:
	.string	"N4Date7invalidE"
	.text
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB1982:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L22
	cmpl	$65535, -8(%rbp)
	jne	.L22
	leaq	_ZStL8__ioinit(%rip), %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	leaq	__dso_handle(%rip), %rdx
	leaq	_ZStL8__ioinit(%rip), %rsi
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rax
	movq	%rax, %rdi
	call	__cxa_atexit@PLT
.L22:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1982:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I__ZppR5Month, @function
_GLOBAL__sub_I__ZppR5Month:
.LFB1983:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1983:
	.size	_GLOBAL__sub_I__ZppR5Month, .-_GLOBAL__sub_I__ZppR5Month
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__ZppR5Month
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
