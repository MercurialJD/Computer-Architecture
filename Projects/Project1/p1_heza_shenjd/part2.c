#include <stdio.h> /* for stderr */
#include <stdlib.h> /* for exit() */
#include "types.h"
#include "utils.h"
#include "riscv.h"

void execute_rtype(Instruction, Processor *);
void execute_itype_except_load(Instruction, Processor *);
void execute_branch(Instruction, Processor *);
void execute_jal(Instruction, Processor *);
void execute_jalr(Instruction, Processor *);
void execute_load(Instruction, Processor *, Byte *);
void execute_store(Instruction, Processor *, Byte *);
void execute_ecall(Processor *, Byte *);
void execute_utype(Instruction, Processor *);

void execute_instruction(Instruction inst, Processor *p, Byte *memory) {
    p->R[0] = 0;
    /* Route for specific instructions */
    switch(inst.opcode.opcode) {
        /* R-type */
        case 0x33:
            execute_rtype(inst, p);
            break;
        /* load */
        case 0x03:
            execute_load(inst, p, memory);
            break;
        /* addi, shifti, logici */
        case 0x13:
            execute_itype_except_load(inst, p);
            break;
        /* jalr */
        case 0x67:
            execute_jalr(inst, p);
            break;
        /* ecall */
        case 0x73:
            execute_ecall(p, memory);
            break;
        /* S-type */
        case 0x23:
            execute_store(inst, p, memory);
            break;
        /* SB-type */
        case 0x63:
            execute_branch(inst, p);
            break;
        /* U-type-auipc */
        case 0x17:
            execute_utype(inst, p);
            break;
        /* U-type-lui */
        case 0x37:
            execute_utype(inst, p);
            break;
        /* jal */
        case 0x6f:
            execute_jal(inst, p);
            break;
        /* undefined opcode */
        default: 
            handle_invalid_instruction(inst);
            exit(-1);
            break;
    }
}

void execute_rtype(Instruction inst, Processor *p) {
    /* Watch out for signed/unsigned register ops */
    switch(inst.rtype.funct3) {
        case 0x0:
            switch(inst.rtype.funct7) {
                case 0x00:/* add */
                    p->R[inst.rtype.rd] = (Word)((sWord)(p->R[inst.rtype.rs1]) + (sWord)(p->R[inst.rtype.rs2]));
                    break;
                case 0x01:/* mul */
                    p->R[inst.rtype.rd] = (Word)((sWord)(p->R[inst.rtype.rs1]) * (sWord)(p->R[inst.rtype.rs2]));
                    break;
                case 0x20:/* sub */
                    p->R[inst.rtype.rd] = (Word)((sWord)(p->R[inst.rtype.rs1]) - (sWord)(p->R[inst.rtype.rs2]));
                    break;
                default:/* default */
                    handle_invalid_instruction(inst);
                    exit(-1);
                    break;
            }
            break;
        case 0x1:
            switch(inst.rtype.funct7) {
                case 0x00:/* sll */
                    p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] << p->R[inst.rtype.rs2];
                    break;
                case 0x01:/* mulh */
                    p->R[inst.rtype.rd] = (Word)((sDouble)bitSigner(p->R[inst.rtype.rs1], 32) * bitSigner(p->R[inst.rtype.rs2], 32) >> 32);
                    break;
                default:/* default */
                    handle_invalid_instruction(inst);
                    exit(-1);
                    break;
            }
            break;
        case 0x2:/* slt */
            if(inst.rtype.funct7 == 0x00) {
                p->R[inst.rtype.rd] = ((sWord)(p->R[inst.rtype.rs1]) < (sWord)(p->R[inst.rtype.rs2])) ? 1 : 0;
            } else {
                handle_invalid_instruction(inst);
                exit(-1);
            }
            break;
        case 0x3:/* sltu */
            if(inst.rtype.funct7 == 0x00) {
                p->R[inst.rtype.rd] = (p->R[inst.rtype.rs1] < p->R[inst.rtype.rs2]) ? 1 : 0;
            } else {
                handle_invalid_instruction(inst);
                exit(-1);
            }
            break;
        case 0x4:
            switch(inst.rtype.funct7) {
                case 0x00:/* xor */
                    p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] ^ p->R[inst.rtype.rs2];
                    break;
                case 0x01:/* div */
                    p->R[inst.rtype.rd] = (Word)((sWord)(p->R[inst.rtype.rs1]) / (sWord)(p->R[inst.rtype.rs2]));
                    break;
                default:
                    handle_invalid_instruction(inst);
                    exit(-1);
                    break;
            }
            break;
        case 0x5:
            switch(inst.rtype.funct7) {
                case 0x00:/* srl */
                    p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] >> p->R[inst.rtype.rs2];
                    break;
                case 0x20:/* sra */
                    p->R[inst.rtype.rd] = (Word)((sWord)(p->R[inst.rtype.rs1]) >> (sWord)(p->R[inst.rtype.rs2]));
                    break;
                default:
                    handle_invalid_instruction(inst);
                    exit(-1);
                    break;
            }
            break;
        case 0x6:
            switch(inst.rtype.funct7) {
                case 0x00:/* or */
                    p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] | p->R[inst.rtype.rs2];
                    break;
                case 0x01:/* rem */
                    p->R[inst.rtype.rd] = (Word)((sWord)(p->R[inst.rtype.rs1]) % (sWord)(p->R[inst.rtype.rs2]));
                    break;
                default:
                    handle_invalid_instruction(inst);
                    exit(-1);
                    break;
            }
            break;
        case 0x7:/* and */
            if(inst.rtype.funct7 == 0x00) {
                p->R[inst.rtype.rd] = p->R[inst.rtype.rs1] & p->R[inst.rtype.rs2];
                break;
            } else {
                handle_invalid_instruction(inst);
                exit(-1);
            }
            break;
        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
    }
    p->PC += 4;
}

void execute_itype_except_load(Instruction inst, Processor *p) {
    switch(inst.itype.funct3) {
        case 0x0:/* addi */
            p->R[inst.itype.rd] = (Word)((sWord)(p->R[inst.itype.rs1]) + get_imm_operand(inst));
            break;
        case 0x1:/* slli */
            if((inst.itype.imm >> 5) == 0) {
                p->R[inst.itype.rd] = p->R[inst.itype.rs1] << get_imm_operand(inst);
            } else {
                handle_invalid_instruction(inst);
                exit(-1);
            }
            break;
        case 0x2:/* slti */
            p->R[inst.itype.rd] = ((sWord)(p->R[inst.itype.rs1]) < get_imm_operand(inst)) ? 1 : 0;
            break;
        case 0x3:/* sltiu */
            p->R[inst.itype.rd] = (p->R[inst.itype.rs1] < (unsigned)get_imm_operand(inst)) ? 1 : 0;
            break;
        case 0x4:/* xori */
            p->R[inst.itype.rd] = p->R[inst.itype.rs1] ^ get_imm_operand(inst);
            break;
        case 0x5:
            if((inst.itype.imm >> 5) == 0x00) {/* srli */
                p->R[inst.itype.rd] = p->R[inst.itype.rs1] >> get_imm_operand(inst);
            } else if((inst.itype.imm >> 5) == 0x20) {/* srai */
                p->R[inst.itype.rd] = (Word)((sWord)(p->R[inst.itype.rs1]) >> get_imm_operand(inst));
            } else {
                handle_invalid_instruction(inst);
                exit(-1);
            }
            break;
        case 0x6:/* ori */
            p->R[inst.itype.rd] = p->R[inst.itype.rs1] | get_imm_operand(inst);
            break;
        case 0x7:/* andi */
            p->R[inst.itype.rd] = p->R[inst.itype.rs1] & get_imm_operand(inst);
            break;
        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
    }
    p->PC += 4;
}

void execute_ecall(Processor *p, Byte *memory) {
    switch(p->R[10]) { /* a0 == x10 */
        case 1:/* print value of a1 as an integer */
            printf("%d", (sWord)p->R[11]);
            p->PC += 4;
            break;
        case 4:/* print the string at address a1 */
            printf("%s", &memory[p->R[11]]);
            p->PC += 4;
            break;
        case 10:/* exit or end of code indicator */
            printf("exiting the simulator\n");
            exit(0);
            break;
        case 11:/* print value of a1 as a character */
            printf("%c", p->R[11]);
            p->PC += 4;
            break;
        default: /* undefined ecall */
            printf("Illegal ecall number %d\n", p->R[10]); 
            exit(-1);
            break;
    }
}

void execute_branch(Instruction inst, Processor *p) {
    p->PC += 4;
    /* Remember that the immediate portion of branches
       is counting half-words, so make sure to account for that. */
    switch(inst.sbtype.funct3) { 
        case 0x0:/*beq*/
            if((sWord)(p->R[inst.sbtype.rs1]) == (sWord)(p->R[inst.sbtype.rs2])){
                p->PC=(Word)((sWord)(p->PC)+(sWord)(get_branch_offset(inst))-4);
            }
            break;
        case 0x1:/*bne*/
            if((sWord)(p->R[inst.sbtype.rs1]) != (sWord)(p->R[inst.sbtype.rs2])){
                p->PC=(Word)((sWord)(p->PC)+(sWord)(get_branch_offset(inst))-4);
            }
            break;
        case 0x4:/*blt*/
            if((sWord)(p->R[inst.sbtype.rs1]) < (sWord)(p->R[inst.sbtype.rs2])){
                p->PC=(Word)((sWord)(p->PC)+(sWord)(get_branch_offset(inst))-4);
            }
            break;
        case 0x5:/*bge*/
            if((sWord)(p->R[inst.sbtype.rs1]) >= (sWord)(p->R[inst.sbtype.rs2])){
                p->PC=(Word)((sWord)(p->PC)+(sWord)(get_branch_offset(inst))-4);
            }
            break;
        case 0x6:/*bltu*/
            if(p->R[inst.sbtype.rs1] < p->R[inst.sbtype.rs2]){
                p->PC=(Word)((sWord)(p->PC)+(sWord)(get_branch_offset(inst))-4);
            }
            break;
        case 0x7:/*bgeu*/
            if(p->R[inst.sbtype.rs1] >= p->R[inst.sbtype.rs2]){
                p->PC=(Word)((sWord)(p->PC)+(sWord)(get_branch_offset(inst))-4);
            }
            break;
        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
    }
}

void execute_load(Instruction inst, Processor *p, Byte *memory) {
    Address address = (Word)((sWord)p->R[inst.itype.rs1] + get_imm_operand(inst));
    switch(inst.itype.funct3) {
        case 0x0:/* lb */
            p->R[inst.itype.rd] = bitSigner(load(memory, address, LENGTH_BYTE, 0), 24);
            break;
        case 0x1:/* lh */
            p->R[inst.itype.rd] = bitSigner(load(memory, address, LENGTH_HALF_WORD, 0), 16);
            break;
        case 0x2:/* lw */
            p->R[inst.itype.rd] = load(memory, address, LENGTH_WORD, 0);
            break;
        case 0x4:/* lbu */
            p->R[inst.itype.rd] = load(memory, address, LENGTH_BYTE, 0) & (Word)255; /* 255 == 0b11111111 */
            break;
        case 0x5:/* lhu */
            p->R[inst.itype.rd] = load(memory, address, LENGTH_HALF_WORD, 0) & (Word)65535; /* 65535 == 0b11111111 11111111 */
            break;
        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
    }
    p->PC += 4;
}

void execute_store(Instruction inst, Processor *p, Byte *memory) {
    Address address = (Word)((sWord)p->R[inst.stype.rs1] + get_store_offset(inst));
    switch(inst.stype.funct3) {
        case 0x0:/* sb */
            store(memory, address, LENGTH_BYTE, p->R[inst.stype.rs2], 0);
            break;
        case 0x1:/* sh */
            store(memory, address, LENGTH_HALF_WORD, p->R[inst.stype.rs2], 0);
            break;
        case 0x2:/* sw */
            store(memory, address, LENGTH_WORD, p->R[inst.stype.rs2], 0);
            break;
        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
    }
    p->PC += 4;
}

void execute_jal(Instruction inst , Processor *p ) {
    sWord PC_present = p->PC;
    p->R[inst.ujtype.rd] = (Word)(p->PC + 4);
    p->PC = (Word)(PC_present + get_jump_offset(inst));
}

void execute_jalr(Instruction inst, Processor *p) {
    sWord rs1_value = p->R[inst.itype.rs1];
    p->R[inst.itype.rd] = (Word)(p->PC + 4);
    p->PC = (Word)(rs1_value + get_imm_operand(inst));    
}

void execute_utype(Instruction inst, Processor *p ) {
    switch(inst.utype.opcode) { 
        case 0x17:/*auipc*/
            p->R[inst.utype.rd] = (Word)((sWord)(p->PC)+(sWord)(get_imm_operand(inst) << 12));
            break;
        case 0x37:/*lui*/
            p->R[inst.utype.rd] = (Word)(get_imm_operand(inst) << 12);
            break;
        default:
            handle_invalid_instruction(inst);
            exit(-1);
            break;
    }
    p->PC += 4;
}

/* Checks that the address is aligned correctly */
int check(Address address, Alignment alignment) {
    if(address > 0 && address < MEMORY_SPACE) {
        if(alignment == LENGTH_BYTE) {
            return 1;
        }
        else if(alignment == LENGTH_HALF_WORD) {
            return address%2 == 0;
        }
        else if(alignment == LENGTH_WORD) {
            return address%4 ==0;
        }
    }
    
    return 0;
}

void store(Byte *memory, Address address, Alignment alignment, Word value, int check_align) {
    if((check_align && !check(address, alignment)) || (address >= MEMORY_SPACE)) {
        handle_invalid_write(address);
    }

    switch(alignment) {
        case LENGTH_BYTE:
            memory[address] = (Byte)(value & (Word)255); /* 255 == 0xff */
            break;
        case LENGTH_HALF_WORD:
            memory[address] = (Byte)(value & (Word)255); /* 255 == 0xff */;
            memory[address + 1] = (Byte)((value & (Word)65280) >> 8); /* 65280 == 0xff00 */;
            break;
        case LENGTH_WORD:
            memory[address] = (Byte)(value & (Word)255); /* 255 == 0xff */;
            memory[address + 1] = (Byte)((value & (Word)65280) >> 8); /* 65280 == 0xff00 */;
            memory[address + 2] = (Byte)((value & (Word)16711680) >> 16); /* ‭16711680‬ == 0xff0000 */;
            memory[address + 3] = (Byte)((value & (Word)4278190080) >> 24); /* ‭4278190080‬‬ == 0xff000000 */;
            break;
        default:
            break;
    }

}

Word load(Byte *memory, Address address, Alignment alignment, int check_align) {
    Word data = 0; /* initialize our return value to zero */

    if((check_align && !check(address, alignment)) || (address >= MEMORY_SPACE)) {
        handle_invalid_read(address);
    }
    
    switch(alignment) {
        case LENGTH_BYTE:
            data |= memory[address];
            break;
        case LENGTH_HALF_WORD:
            data |= memory[address];
            data |= memory[address + 1] << 8;
            break;
        case LENGTH_WORD:
            data |= memory[address];
            data |= memory[address + 1] << 8;
            data |= memory[address + 2] << 16;
            data |= memory[address + 3] << 24;
            break;
        default:
            break;
    }

    return data;
}


