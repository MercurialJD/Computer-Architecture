#include "utils.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

/* The field is the num that haven't been signed, the size is the  */
int bitSigner(unsigned int origin_num, unsigned int shift_size) {
    int num = 0;
    num |= origin_num;
    num = num << shift_size;
    return num >> shift_size;
}

/* You may find implementing this function handy */
int get_imm_operand(Instruction inst) {
    int imm = 0;
    switch(inst.opcode.opcode) {
        /* I-type */
        /* load, all signed offset */
        case 0x03:
            if(inst.itype.funct3 == 0x4 || inst.itype.funct3 == 0x5) {
                imm |= inst.itype.imm;
                return bitSigner(imm, 20);
            } else if(inst.itype.funct3 == 0x0 || inst.itype.funct3 == 0x1 || inst.itype.funct3 == 0x2) {
                imm |= inst.itype.imm;
                return bitSigner(imm, 20);
            } else {
                handle_invalid_instruction(inst);
            }
            break;
        /* addi, shift, logic; all signed offset */
        case 0x13:
            if(inst.itype.funct3 == 0x0 || inst.itype.funct3 == 0x2 || inst.itype.funct3 == 0x3 || inst.itype.funct3 == 0x4 || inst.itype.funct3 == 0x6 || inst.itype.funct3 == 0x7) {
                imm |= inst.itype.imm;
                return bitSigner(imm, 20);
            } else if(((inst.itype.funct3 == 0x1) && ((inst.itype.imm >> 5) == 0x00)) || (inst.itype.funct3 == 0x5 && (((inst.itype.imm >> 5) == 0x00) || ((inst.itype.imm >> 5) == 0x20)))) {
                int mask = 63;  /* 0b11111 */
                imm |= inst.itype.imm & mask;
                return bitSigner(imm, 26);
            } else {
                handle_invalid_instruction(inst);
            }
            break;
        /* jalr and for offset*/
        case 0x67:
            if(inst.itype.funct3 == 0x0) {
                imm |= inst.itype.imm;
                return bitSigner(imm, 20);
            } else {
                handle_invalid_instruction(inst);
            }
            break;
        /* U-type, unsigned */
        case 0x17:
            imm |= inst.utype.imm;
            return imm;
        /* break; */
        /* U-type, unsigned */
        case 0x37:
            imm |= inst.utype.imm;
            return imm;
        /* break; */
        default:
            printf("invalid input!");
    }
    return 0;
}


/* All signed offset */
int get_branch_offset(Instruction inst) {
    unsigned int offest = 0;
    unsigned int mask = 15; /* 0b1111 */
    offest |= mask & (inst.sbtype.imm5 >> 1);
    mask = 63;    /* 0b111111 */
    offest |= ((mask & (inst.sbtype.imm7)) << 4);
    mask = 1;   /* 0b1 */
    offest |= ((mask & (inst.sbtype.imm5)) << 10);
    offest |= ((mask & (inst.sbtype.imm7 >> 6)) << 11);
    offest *= 2;
    return bitSigner(offest, 19);   /* one less because of *2 */
}

int get_jump_offset(Instruction inst) {
    /* jal only*/
    unsigned offset=0;
    unsigned mask=1023; /* 0b1111111111 */
    offset |= mask & (inst.ujtype.imm >> 9);
    mask = 1;   /* 0b1 */
    offset |= (mask & (inst.ujtype.imm >> 8)) << 10;
    mask = 255; /* 0b11111111 */
    offset |= (mask & (inst.ujtype.imm)) << 11;
    mask = 1;   /* 0b1 */
    offset |= (mask & (inst.ujtype.imm >> 19)) << 19;
    offset *= 2;
    return bitSigner(offset, 11);   /* one less because of *2 */
}

int get_store_offset(Instruction inst) {
    unsigned int offset = 0;
    offset |= inst.stype.imm5;
    offset |= (inst.stype.imm7 << 5);
    return bitSigner(offset, 20);
}

void handle_invalid_instruction(Instruction inst) {
    printf("Invalid Instruction: 0x%08x\n", inst.bits);
}

void handle_invalid_read(Address address) {
    printf("Bad Read. Address: 0x%08x\n", address);
    exit(-1);
}

void handle_invalid_write(Address address) {
    printf("Bad Write. Address: 0x%08x\n", address);
    exit(-1);
}

