# sample input below, remeber to remove input_size, input_inorder, input_postorder before submit
	.data
input_size:
	.word	7
input_inorder:
	.word	3
	.word	2
	.word	4
	.word	1
	.word	5
	.word	7
	.word	6
input_postorder:
	.word	3
	.word	4
	.word	2
	.word	7
	.word	6
	.word	5
	.word	1


# `void exit(int);` system call
	.text
exit:
	mv	a1, a0  # set return error code 0
	li	a0, 17
	ecall   # system call
# entry point
	.globl  main
main:
    addi sp, sp, -4 # store ra on stack
    sw ra, 0(sp)

    la t0, input_size   # load input_size from memory to t0
    lw t0, 0(t0)
    mv a0, zero # set parameters to 0
    mv a1, t0   # set parameters to input_size
    mv a0, zero    # set parameters to 0
    mv a3, t0   # set parameters to input_size

    jal ra, transform   # transform(0, input_size, 0, input_size)

    addi a1, zero, '\n'   # set '\n'
	addi a0, zero, 11
	ecall   # print '\n'

    lw ra, 0(sp)    # recover ra
    addi sp, sp, 4

	li	a0, 0   # main function return
	call	exit

# transform(int in_begin, int in_end, int post_begin, int post_end)
# range for indices: [begin, end)
transform:
    bge a0, a1, transformRetrun # if in_begin >= in_end, return

    addi sp, sp, -32 # new stack
    sw ra, 28(sp)   # store ra
    sw a0, 24(sp)   # store a0, in_begin
    sw a1, 20(sp)   # store a1, in_end
    sw a2, 16(sp)   # store a2, post_begin
    sw a3, 12(sp)   # store a3, post_end
    sw s0, 8(sp)    # store s0
    sw s1, 4(sp)    # store s1
    sw s2, 0(sp)    # store s2

    la s0, input_postorder    # address of array input_postorder
    addi t0, a3, -1 # post_end - 1
    slli t1, t0, 2  # (post_end - 1) * 4
    add s0, s0, t1  # pointer: &input_postorder[post_end - 1]
    lw s0, 0(s0)    # value: input_postorder[post_end - 1]

    addi a1, s0, 0  # set input_postorder[post_end - 1] for print
	addi a0, zero, 1
	ecall   # print value
    addi a1, zero, ' '   # set ' '
	addi a0, zero, 11
	ecall   # print ' '

    lw t0, 24(sp)   # load in_begin from stack to t0
    lw t1, 20(sp)   # load in_end from stack to t1
    addi t1, t1, -1 # in_end -= 1
    beq t0, t1, transformRetrun # if in_begin == in_end - 1, transformRetrun
    
    mv a0, t0   # set parameters to in_begin
    lw t1, 20(sp)   # load in_end from stack to t1
    mv a1, t1   # set parameters to in_end
    mv a2, s0   # set parameters
    jal ra, findInInorder   # findInInorder(in_begin, in_end, input_postorder[post_end - 1])

    mv s1, a0   # save the founded index
    lw t0, 24(sp)   # load in_begin from stack to t0
    sub t0, s1, t0  # index - in_begin
    lw s2, 16(sp)   # load post_begin form stack to s2
    add s2, s2, t0  # post_begin + (index - in_begin)

    lw a0, 24(sp)   # set parameter to in_begin
    mv a1, s1   # set parameter to index
    lw a2, 16(sp)   # set parameter to post_begin
    mv a3, s2   # set parameter to (post_begin + (index - in_begin))
    jal ra, transform   # transform(in_begin, index, post_begin, post_begin + (index - in_begin))

    lw t1, 12(sp)   # load post_end from stack to t1
    addi a0, s1, 1  # set parameter to index + 1
    lw a1, 20(sp)   # set parameter to in_end
    mv a2, s2   # set parameter to (post_begin + (index - in_begin))
    addi a3, t1, -1 # set parameter to post_end - 1
    jal ra, transform   # transform(index + 1, in_end, post_begin + (index - in_begin), post_end - 1)

    lw ra, 28(sp)   # restore ra
    lw a0, 24(sp)   # restore a0, in_begin
    lw a1, 20(sp)   # restore a1, in_end
    lw a2, 16(sp)   # restore a2, post_begin
    lw a3, 12(sp)   # restore a3, post_end
    lw s0, 8(sp)    # restore s0
    lw s1, 4(sp)    # restore s1
    lw s2, 0(sp)    # restore s2
    addi sp, sp, 32 # "free" stack

transformRetrun:
    ret # return

# findInInorder(int in_begin, int in_end, int num)
# range for indices: [begin, end)
findInInorder:
    bge a0, a1, findInInorderReturn #if in_begin >= in_end, return; otherwise, loop
    la t0, input_inorder    # address of array input_inorder
    slli t1, a0, 2  # t1 = a0 * 4, a0 is the cuurent index i
    add t1, t1, t0  # pointer: &input_inorder[i]
    lw t1, 0(t1)    # value: input_inorder[i]
    beq t1, a2, findInInorderReturn # if input_inorder[i] == num, return (a0 has been set to i)
    addi a0, a0, 1   # i += 1
    j findInInorder # loop

findInInorderReturn:
    ret # return