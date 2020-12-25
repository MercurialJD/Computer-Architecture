.data
    hs: .string "TRASHH"

.section .text
.globl draw
.type draw, @function

draw:
    # Circle
    li a0, 25;
    li a1, 40;
    li a2, 20;
    li a3, 0b0000000000011111;
    jal LCD_DrawCircle

    # Square
    li a0, 50;
    li a1, 20;
    li a2, 90;
    li a3, 60;
    li a4, 0b1111100000000000;
    jal LCD_DrawRectangle;

    # Char
	la t0, hs;
    li a0, 95;
    li a1, 32;
    mv a2, t0;
    li a3, 0b0000011111100000;
    jal LCD_ShowString;

