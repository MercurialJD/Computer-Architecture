.section .text
.globl drawUI
.type drawUI, @function
drawUI:
	addi sp, sp, -52
    sw ra, 0(sp)    # store ra
    sw s0, 4(sp)    # store s0
    sw s1, 8(sp)    # store s1
    sw s2, 12(sp)   # store s2
    sw s3, 16(sp)   # store s3
    sw s4, 20(sp)   # store s4
    sw s5, 24(sp)   # store s5
    sw s6, 28(sp)   # store s6
    sw s7, 32(sp)   # store s7
    sw s8, 36(sp)   # store s8
    sw s9, 40(sp)   # store s9
    sw s10, 44(sp)  # store s10
    sw s11, 48(sp)  # store s11

    # Clear
    li a0, 0x0000   # clear screen to black
    jal LCD_Clear
    
    # Draw rectangle
    li a0, 0        # x_sta
    li a1, 0        # y_sta
    li a2, 159      # x_end
    li a3, 79       # y_end
    li a4, 0xFFFF   # white rectangle
    jal LCD_DrawRectangle

    # Draw center line
    li a0, 80       # x_sta
    li a1, 1        # y_sta
    li a2, 80       # x_end
    li a3, 79       # y_end
    li a4, 0xBDF7   # light-grey line
    jal LCD_DrawLine

    lw ra, 0(sp)    # load ra
    lw s0, 4(sp)    # load s0
    lw s1, 8(sp)    # load s1
    lw s2, 12(sp)   # load s2
    lw s3, 16(sp)   # load s3
    lw s4, 20(sp)   # load s4
    lw s5, 24(sp)   # load s5
    lw s6, 28(sp)   # load s6
    lw s7, 32(sp)   # load s7
    lw s8, 36(sp)   # load s8
    lw s9, 40(sp)   # load s9
    lw s10, 44(sp)  # load s10
    lw s11, 48(sp)  # load s11
    addi sp ,sp, 52

    ret