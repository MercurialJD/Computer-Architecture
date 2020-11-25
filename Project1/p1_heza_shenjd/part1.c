#include <stdio.h> /* for stderr */
#include <stdlib.h> /* for exit() */
#include "types.h"
#include "utils.h"

void print_rtype(char *, Instruction);
void print_itype_except_load(char *, Instruction);
void print_load(char *, Instruction);
void print_store(char *, Instruction);
void print_branch(char *, Instruction);
void print_utype(char *, Instruction);
void write_rtype(Instruction);
void write_itype_except(Instruction); 
void write_load(Instruction);
void write_store(Instruction);
void write_branch(Instruction);
void write_utype(Instruction);
void write_jal(Instruction);
void write_jalr(Instruction);
void write_ecall(Instruction);


void decode_instruction(Instruction inst) {
    switch(inst.opcode.opcode) {
        /* R-type */
        case 0x33:
            write_rtype(inst);
            break;
        /* load */
        case 0x03:
            write_load(inst);
            break;
        /* addi, shifti, logici */
        case 0x13:
            write_itype_except(inst);
            break;
        /* jalr */
        case 0x67:
            write_jalr(inst);
            break;
        /* ecall */
        case 0x73:
            write_ecall(inst);
            break;
        /* S-type */
        case 0x23:
            write_store(inst);
            break;
        /* SB-type */
        case 0x63:
            write_branch(inst);
            break;
        /* U-type-auipc */
        case 0x17:
            write_utype(inst);
            break;
        /* U-type-lui */
        case 0x37:
            write_utype(inst);
            break;
        /* jal */
        case 0x6f:
            write_jal(inst);
            break;
        /* undefined opcode */
        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_rtype(Instruction inst) {
    switch(inst.rtype.funct3) { /* What do we switch on? */
        case 0x0:
            switch(inst.rtype.funct7) {
                case 0x00:
                    print_rtype("add", inst);
                    break;
                case 0x01:
                    print_rtype("mul", inst);
                    break;
                case 0x20:
                    print_rtype("sub", inst);
                    break;
                default:
                    handle_invalid_instruction(inst);
                    break;
            }
            break;
        case 0x1:
            switch(inst.rtype.funct7) {
                case 0x00:
                    print_rtype("sll", inst);
                    break;
                case 0x01:
                    print_rtype("mulh", inst);
                    break;
                default:
                    handle_invalid_instruction(inst);
                    break;
            }
            break;
        case 0x2:
            if(inst.rtype.funct7 == 0x00) {
                print_rtype("slt", inst);
            } else {
                handle_invalid_instruction(inst);
            }
            break;
        case 0x3:
            if(inst.rtype.funct7 == 0x00) {
                print_rtype("sltu", inst);
            } else {
                handle_invalid_instruction(inst);
            }
            break;
        case 0x4:
            switch(inst.rtype.funct7) {
                case 0x00:
                    print_rtype("xor", inst);
                    break;
                case 0x01:
                    print_rtype("div", inst);
                    break;
                default:
                    handle_invalid_instruction(inst);
                    break;
            }
            break;
        case 0x5:
            switch(inst.rtype.funct7) {
                case 0x00:
                    print_rtype("srl", inst);
                    break;
                case 0x20:
                    print_rtype("sra", inst);
                    break;
                default:
                    handle_invalid_instruction(inst);
                    break;
            }
            break;
        case 0x6:
            switch(inst.rtype.funct7) {
                case 0x00:
                    print_rtype("or", inst);
                    break;
                case 0x01:
                    print_rtype("rem", inst);
                    break;
                default:
                    handle_invalid_instruction(inst);
                    break;
            }
            break;
        case 0x7:
            if(inst.rtype.funct7 == 0x00) {
                print_rtype("and", inst);
            } else {
                handle_invalid_instruction(inst);
            }
            break;
        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_itype_except(Instruction inst) {
    switch(inst.itype.funct3) { /* What do we switch on? */
        case 0x0:
            print_itype_except_load("addi", inst);
            break;
        case 0x1:
            if((inst.itype.imm >> 5) == 0) {
                print_itype_except_load("slli", inst);
            } else {
                handle_invalid_instruction(inst);
            }
            break;
        case 0x2:
            print_itype_except_load("slti", inst);
            break;
        case 0x3:
            print_itype_except_load("sltiu", inst);
            break;
        case 0x4:
            print_itype_except_load("xori", inst);
            break;
        case 0x5:
            if((inst.itype.imm >> 5) == 0x00) {
                print_itype_except_load("srli", inst);
            } else if((inst.itype.imm >> 5) == 0x20) {
                print_itype_except_load("srai", inst);
            } else {
                handle_invalid_instruction(inst);
            }
            break;
        case 0x6:
            print_itype_except_load("ori", inst);
            break;
        case 0x7:
            print_itype_except_load("andi", inst);
            break;
        default:
            handle_invalid_instruction(inst);
            break;  
    }
}

void write_load(Instruction inst) {
    switch(inst.itype.funct3) { /* What do we switch on? */
        case 0x0:
            print_load("lb", inst);
            break;
        case 0x1:
            print_load("lh", inst);
            break;
        case 0x2:
            print_load("lw", inst);
            break;
        case 0x4:
            print_load("lbu", inst);
            break;
        case 0x5:
            print_load("lhu", inst);
            break;
        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_store(Instruction inst) {
    switch(inst.stype.funct3) { /* What do we switch on? */
        case 0x0:
            print_store("sb", inst);
            break;
        case 0x1:
            print_store("sh", inst);
            break;
        case 0x2:
            print_store("sw", inst);
            break;
        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_branch(Instruction inst) {
    switch(inst.sbtype.funct3) { /* What do we switch on? */
        case 0x0:
            print_branch("beq", inst);
            break;
        case 0x1:
            print_branch("bne", inst);
            break;
        case 0x4:
            print_branch("blt", inst);
            break;
        case 0x5:
            print_branch("bge", inst);
            break;
        case 0x6:
            print_branch("bltu", inst);
            break;
        case 0x7:
            print_branch("bgeu", inst);
            break;
        default:
            handle_invalid_instruction(inst);
            break;
    }
}

/* For the writes, probably a good idea to take a look at utils.h */

void write_utype(Instruction inst) {
    switch(inst.utype.opcode) { /* What do we switch on? */
        case 0x17:
            print_utype("auipc", inst);
            break;
        case 0x37:
            print_utype("lui", inst);
            break;
        default:
            handle_invalid_instruction(inst);
            break;
    }
}

void write_jal(Instruction inst) {
    printf(JAL_FORMAT, inst.ujtype.rd, get_jump_offset(inst));
}

void write_jalr(Instruction inst) {
    if(inst.itype.funct3 == 0x0) {
        printf(BRANCH_FORMAT, "jalr", inst.itype.rd, inst.itype.rs1, get_imm_operand(inst));
    } else {
        handle_invalid_instruction(inst);
    }
}

void write_ecall(Instruction inst) {
    if(inst.itype.funct3 == 0x0 && inst.itype.imm == 0x000) {
        printf(ECALL_FORMAT);
    } else {
        handle_invalid_instruction(inst);
    }
}

void print_rtype(char *name, Instruction inst) {
    printf(RTYPE_FORMAT, name, inst.rtype.rd, inst.rtype.rs1, inst.rtype.rs2);
}

void print_itype_except_load(char *name, Instruction inst) {
    printf(ITYPE_FORMAT, name, inst.itype.rd, inst.itype.rs1, get_imm_operand(inst));
}

void print_load(char *name, Instruction inst) {
    printf(MEM_FORMAT, name, inst.itype.rd, get_imm_operand(inst), inst.itype.rs1);
}

void print_store(char *name, Instruction inst) {
    printf(MEM_FORMAT, name, inst.stype.rs2, get_store_offset(inst), inst.stype.rs1);
}

void print_branch(char *name, Instruction inst) {
    printf(BRANCH_FORMAT, name, inst.sbtype.rs1, inst.stype.rs2, get_branch_offset(inst));
}

void print_utype(char *name, Instruction inst) {
    printf(UTYPE_FORMAT, name, inst.utype.rd, get_imm_operand(inst));
}
