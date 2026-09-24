	.file	"ssa_example.c"
	.text
	.globl	testExpr                        # -- Begin function testExpr
	.p2align	4
	.type	testExpr,@function
testExpr:                               # @testExpr
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$11, %eax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	testExpr, .Lfunc_end0-testExpr
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	callq	testExpr
	cmpl	$11, %eax
	jne	.LBB1_2
# %bb.1:
	movabsq	$.L.str, %rdi
	movb	$0, %al
	callq	printf
.LBB1_2:
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"Hello World"
	.size	.L.str, 12

	.ident	"clang version 21.1.8 (Fedora 21.1.8-6.fc43)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym testExpr
	.addrsig_sym printf
