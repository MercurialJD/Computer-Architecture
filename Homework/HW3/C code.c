#include <stdio.h>

int input_size = 4;
int input_inorder[] = {1, 2, 3, 4};
int input_postorder[] = {1, 2, 4, 3};

/* int input_size = 10;
int input_inorder[] = {4, 3, 2, 5, 1, 6, 7, 9, 10, 8};
int input_postorder[] = {4, 3, 5, 2, 10, 9, 8, 7, 6, 1}; */

int findInInorder(int in_begin, int in_end, int num) {
    int i;
    /* i = a0 */
    /* bge a0 and a1, j ret */
    /* load address of input_inorder to t0 */
    for(i = in_begin; i < in_end; i++) {
        /* t1 = a0 << 2 */
        /* t1 += t0, &input_inorder[i] */
        /* load input_inorder[i] to t1 */
        /* beq t1 and a2, j ret */
        if(input_inorder[i] == num) {
            return i;
        }
        /* a0 += 1 */
        /* j loop */
    }
    return 0;
}

/* [begin, end) */
void transform(int in_begin, int in_end, int post_begin, int post_end) {
    int index;
    /* bge a0 and a1, ret */
    if(in_begin >= in_end) {
        return;
    }
    /* save ra, a0, a1, a2, a3, s0, s1, s2 on stack */
    /* t0 = a3 - 1, post_end - 1 */
    /* t1 = t0 << 2, int size adjust */
    /* load address of input_postorder to s0 */
    /* s0 += t1, &input_postorder[post_end - 1] */
    /* load input_postorder[post_end - 1] to s0 */
    /* ecall to print value in s0 */
    /* ecall to print ' ' */
    printf("%d ", input_postorder[post_end - 1]);
    /* load in_begin from stack to t0, load in_end from stack to t1 */
    /* t1 -= 1 */
    /* beq t0 and t1, ret */
    if(in_begin == in_end - 1) {
        return;
    }
    /* a0 = t0, a1 = in_end, a2 = s0 */
    /* jal findInInorder */
    index = findInInorder(in_begin, in_end, input_postorder[post_end - 1]);
    /* save a0 to s1, index */
    /* load in_begin from stack to t0 */
    /* t0 = s1 - t0, index - in_begin */
    /* load post_begin from stack to s2 */
    /* s2 += t0, post_begin + (index - in_begin) */
    /* load in_begin form stack to a0, a1 = s1, load post_begin from stack to a2, a3 = s2 */
    transform(in_begin, index, post_begin, post_begin + (index - in_begin));
    /* load post_end from stack to t1 */
    /* a0 = s1 + 1, load in_end from stack to a1, a2 = s2, a3 = t1 - 1 */
    transform(index + 1, in_end, post_begin + (index - in_begin), post_end - 1);
    /* recover stack */
    /* ret */
}

int main() {
    /* save ra on stack */
    /* load input_size from memory to register t0  */
    /* a0 = 0, a1 = t0, a2 = 0, a3 = t0 */
    /* jal transform */
    transform(0, input_size, 0, input_size);
    /* ecall */
    printf("\n");
    /* recover ra */
    /* ecall */
    return 0;
}