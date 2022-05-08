//***************************************************************************
//
//  Justin McLain
//  Z1910087
//  CSCI 463 - PE1
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************/

using namespace std;

#include "hex.h"
#include "rv32i_decode.h"
#include "memory.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <bitset>
#include <sstream>
#include <assert.h>


std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
    std::cout << hex::to_hex32(insn) <<"  ";
    switch (get_opcode(insn))
    {
    default:
        return render_illegal_insn(insn);
    case opcode_lui:
        return render_lui(insn);
    case opcode_auipc:
        return render_auipc(insn);
    case opcode_rtype:
        switch (get_funct3(insn))
        {
        default:
            return render_illegal_insn(insn);
        case funct3_add:
            switch (get_funct7(insn))
            {
            default:
                return render_illegal_insn(insn);
            case funct7_add:
                return render_rtype(insn, "add");
            case funct7_sub:
                return render_rtype(insn, "sub");
            }
        case funct3_sll:
            return render_rtype(insn, "sll");
        case funct3_slt:
            return render_rtype(insn, "slt");
        case funct3_sltu:
            return render_rtype(insn, "sltu");
        case funct3_xor:
            return render_rtype(insn, "xor");
        case funct3_srx:
            switch (get_funct7(insn))
            {
            default:
                return render_illegal_insn(insn);
            case funct7_srl:
                return render_rtype(insn, "srl");
            case funct7_sra:
                return render_rtype(insn, "sra");
            }
            assert(0 && "unrecognized funct7"); // impossible
        case funct3_or:
            return render_rtype(insn, "or");
        case funct3_and:
            return render_rtype(insn, "and");
        }
    case opcode_stype:
        switch (get_funct3(insn))
        {
        default:
            return render_illegal_insn(insn);
        case funct3_sb:
            return render_stype(insn, "sb");
        case funct3_sh:
            return render_stype(insn, "sh");
        case funct3_sw:
            return render_stype(insn, "sw");
        }
        assert(0 && "unrecognized funct3"); // impossible
    case opcode_alu_imm:
        switch (get_funct3(insn))
        {
        default:
            return render_illegal_insn(insn);
        case funct3_add:
            return render_itype_alu(insn, "addi", get_imm_i(insn));
        case funct3_slt:
            return render_itype_alu(insn, "slti", get_imm_i(insn));
        case funct3_sltu:
            return render_itype_alu(insn, "sltiu", get_imm_i(insn));
        case funct3_xor:
            return render_itype_alu(insn, "xori", get_imm_i(insn));
        case funct3_or:   
            return render_itype_alu(insn, "ori", get_imm_i(insn));
        case funct3_and:  
            return render_itype_alu(insn, "andi", get_imm_i(insn));
        case funct3_sll:
            return render_itype_alu(insn, "slli", get_imm_i(insn) % XLEN);
        case funct3_srx:
            switch (get_funct7(insn))
            {
            default:
                return render_illegal_insn(insn);
            case funct7_sra:
                return render_itype_alu(insn, "srai", get_imm_i(insn) % XLEN);
            case funct7_srl:
                return render_itype_alu(insn, "srli", get_imm_i(insn));
            }
            assert(0 && "unrecognized funct7"); // impossible
        }
    case opcode_load_imm:
        switch (get_funct3(insn))
        {
        default:
            return render_illegal_insn(insn);
        case funct3_lb:
            return render_itype_load(insn, "lb");
        case funct3_lh:
            return render_itype_load(insn, "lh");
        case funct3_lw:
            return render_itype_load(insn, "lw");
        case funct3_lbu:
            return render_itype_load(insn, "lbu");
        case funct3_lhu:
            return render_itype_load(insn, "lhu");
        }
    case opcode_btype:
        switch (get_funct3(insn))
        {
        default:
            return render_illegal_insn(insn);
        case funct3_bne:
            return render_btype(addr, insn, "bne");
        case funct3_beq:
            return render_btype(addr, insn, "beq");
        case funct3_blt:
            return render_btype(addr, insn, "blt");
        case funct3_bge:
            return render_btype(addr, insn, "bge");
        case funct3_bltu:
            return render_btype(addr, insn, "bltu");
        case funct3_bgeu:
            return render_btype(addr, insn, "bgeu");
        }
    case opcode_jal:
        return render_jal(addr, insn);
    case opcode_jalr:
        return render_jalr(insn);
        assert(0 && "unrecognized funct3"); // impossible
    case opcode_system:
        switch (insn)
        {
        default:
            switch (get_funct3(insn))
            {
            default:
                return render_illegal_insn(insn);
            case funct3_csrrw:
                return render_csrrx(insn, "csrrw");
            case funct3_csrrs:
                return render_csrrx(insn, "csrrs");
            case funct3_csrrc:
                return render_csrrx(insn, "csrrc");
            case funct3_csrrwi:
                return render_csrrxi(insn , "csrrwi");
            case funct3_csrrsi:
                return render_csrrxi(insn, "csrrsi");
            case funct3_csrrci:
                return render_csrrxi(insn, "csrrci");
            }
        case insn_ecall:
            return render_ecall(insn);
        case insn_ebreak:
            return render_ebreak(insn);
        }
    }
    assert(0 && "unrecognized opcode"); // It should be impossible to ever get here!
}


/**
  @brief extract the opcode from given intrustruction
  @param insn - for opcode input
  @return value of opcode
*******************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
    uint32_t opcode = (insn & 0x0000007f);
    return opcode;
}

/**
 * @brief retrieve rd as an integer value between 0 and 31
 * 
 * @param insn to get the value that is put in
 * @return rd value
 *******************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
    uint32_t retRD = ((insn & 0x00000f80) >> (7-0));
    return retRD;
}

/**
 * @brief gives interger value between 0 and 7
 * 
 * @param insn to get the value that is put in
 * @return functThree
 *******************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
    uint32_t functThree = ((insn & 0x00007000) >> (13 - 1));
    return functThree;
}

/**
 * @brief gives interger value between 0 and 31 in rsOne field
 * 
 * @param insn to get the value that is put in
 * @return rsOne
 *******************************************************************/
uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
    uint32_t rsOne = ((insn & 0x000f8000) >> 15);
    return rsOne;
}

/**
 * @brief gives interger value between 0 and 31 in rsTwo Field
 * 
 * @param insn to get the value that is put in
 * @return rsTwo
 *******************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
    uint32_t rsTwo = ((insn & 0x01f00000) >> (25 - 5));
    return rsTwo;
}

/**
 * @brief gives interger value from 0x00 to 0x7f in the functSeven 
 * field
 * @param insn to get the value that is put in
 * @return functSeven
 *******************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
    uint32_t functSeven = ((insn & 0xfe000000) >> (30-5));
    return functSeven;
}

/**
 * @brief gives 32- bit signed integer from imm_i instruction
 * 
 * @param insn to get the value that is put in
 * @return imm_i which is a 32-bit signed integer
 *******************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{ //shifting only uses 5 bits
    int32_t imm_i = (insn & 0xfff00000) >> (31-11);
    //if the 31st bit is 1 its an arithemtic shift
    //if the 31st bit is a 0 then its a logical shift right
    if (insn & 0x80000000)
        imm_i |= 0xfffff000;
    return imm_i;
    
}

/**
 * @brief gives 32-bit signed integer from imm_u instruction
 * 
 * @param insn to get the value that is put in 
 * @return immuget which is a 32-bit signed integer
 *******************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
    int32_t immuget = (insn & 0xfffff000);
    return immuget;
}

/**
 * @brief gives 32-bit integer from imm_b instruction and shifts values
 * 
 * @param insn to get the value that is put in
 * @return imm_b as a 32-bit signed integer 
  *******************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
    int32_t imm_b = (insn & 0x00000080) << (6-2);
    imm_b |= (insn & 0x00000f00) >> (9-2);
    imm_b |= (insn & 0x7e000000) >> (30 - 10);
    imm_b |= (insn & 0x80000000) >> (26 - 7);
    //for sign extenstion
        if(insn & 0x80000000) {
            imm_b  = imm_b  | 0xffffe000;
        }

  return imm_b;
}

/**
 * @brief gives 32-bit sighned integer and shifts values
 * 
 * @param insn to get the value that is put in and shift
 * @return imms as a 32-bit signed integer shifted right
  *******************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
    int32_t imms = (insn & 0xfe000000) >> (27 - 7);
    imms |= (insn & 0x00000f80) >> (9 - 2);
    if (insn & 0x80000000)
        imms |= 0xfffff000; // sign-extend the left

    return imms;
}

/**
 * @brief 32-bit signed integer value that is shifted
 * 
 * @param insn to get the value that is put in and shift
 * @return immjSpace as a 32-bit signed integer shifted right
  *******************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
    int32_t immjSpace = (insn & 0x000ff000);           
    immjSpace |= immjSpace | (insn & 0x80000000) >> (12-1); 
    immjSpace |= immjSpace | (insn & 0x00100000) >> (10-1);  
    immjSpace |= immjSpace | (insn & 0x7fe00000) >> (25-5); 

    uint32_t fieldStatus = insn & 0x80000000;
    if (fieldStatus)
    {
        immjSpace |= 0xffe00000;
    }

    return immjSpace;
}

/**
 * @brief checks to see if the input from insn is illegal value
 * 
 * @param insn to get the value that is checked
 * @return and error if the value for insn is invalid 
  *******************************************************************/
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
    (void)insn;
    return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
 * @brief format the lui part of the code 
 * and output organization
 * @param insn hex value to be put in 
 * @return string stream value containing lui elements with formatting
  *******************************************************************/
std::string rv32i_decode::render_lui(uint32_t insn)
{
    uint32_t rd_field = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    std::ostringstream os;
    os << render_mnemonic("lui") << render_reg(rd_field) << ","
       << hex::to_hex0x20((imm_u >> 12) & 0x0fffff);
    return os.str();
}

/**
 * @brief format for auipc value 
 * 
 * @param insn hex value going in to be retrieved
 * @return string stream containing auipc elmenents with formatting
  *******************************************************************/
std::string rv32i_decode::render_auipc(uint32_t insn)
{   
    uint32_t rd_field = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    std::ostringstream os;
    os << render_mnemonic("auipc") << render_reg(rd_field) << ","
       << hex::to_hex0x20((imm_u >> 12) & 0x0fffff);
    return os.str();
}

/**
 * @brief jal organization for output and formatting 
 * @note ses 20-bit encoding for destination address 
 * @note saves the actual address +4 in rd
 * @param addr address register for computer and acts as iterator
 * @param insn for hex value
 * @return string stream containing jal elements with 
 * formatting of jal
 *******************************************************************/
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
    uint32_t rd_field = get_rd(insn);
    uint32_t computer = addr;
    int32_t imm_Jal = get_imm_j(insn) + computer;

    std::ostringstream os;
    os << render_mnemonic("jal") << render_reg(rd_field) << "," << hex::to_hex0x32(imm_Jal);
    return os.str();
}

/**
 * @brief jalr organization for output and formatting
 * @note uses indirect address plus a constant 12 bits 
 * @note like jal it also saves the actual address +4 in rd
 * @param addr address register for computer and acts as iterator
 * @param insn for hex value
 * @return string stream containing jal elements with 
 * formatting of jal
 *******************************************************************/
std::string rv32i_decode::render_jalr(uint32_t insn)
{
    uint32_t rd_field = get_rd(insn);
    int32_t rsOne = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);

    std::ostringstream os;
    os << render_mnemonic("jalr") << render_reg(rd_field) << "," << render_base_disp(rsOne, imm_i);
    return os.str();
}

/**
 * @brief used for branch instructions that require immediate value
 * @note is used to determine the branch target address as an offset 
 * from current instructions
 * @param addr address where value is stored and acts as iterator
 * @param insn for hex value
 * @param mnemonic indicates the operation the cpu performs
 * @return std::string
 *******************************************************************/
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
  int32_t rsOne = get_rs1(insn);
  int32_t rsTwo = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn) + addr;

  std::ostringstream os;
  os << render_mnemonic(mnemonic) << render_reg(rsOne) << "," << render_reg(rsTwo) << "," << hex::to_hex0x32(imm_b);
  return os.str();
}

/**
 * @brief used to encode with 12-bit signed immediate operand
 * @note has a range of [−2048..2047]
 * @param insn for hex values
 * @param mnemonic indicates the operation the cpu performs
 * @param imm_i the number of bit positions to shift show in rvalp
 * @return the value for itype alu with formatting including imm i 
 * formmatting
 *******************************************************************/
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) {
    uint32_t rd_field = get_rd(insn);
    int32_t r1 = get_rs1(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd_field) << "," << render_reg(r1) << "," << imm_i;
    return os.str();

}

/**
 * @brief format for itype load 
 * 
 * @param insn the value of the hex's ment to be input
 * @param mnemonic indicates the operation the cpu performs
 * @return format for itype_load using string stream 
 */
std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
    uint32_t rd_field = get_rd(insn);
    int32_t rsOne = get_rs1(insn);
    int32_t immgeti = get_imm_i(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd_field) << "," <<render_base_disp(rsOne, immgeti);
    return os.str();
}

/**
 * @brief format for rtype loading, used for operations that set
 * a destination register rd to the result of an arithmetic or logical 
 * shift
 * @param insn the value for the hex
 * @param mnemonic which operation the cpu will perform
 * @return format for r type instructions using string stream output
 *******************************************************************/
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
    uint32_t rd_field = get_rd(insn);
    int32_t rsOne = get_rs1(insn);
    int32_t rsTwo = get_rs2(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd_field) << "," << render_reg(rsOne) << "," << render_reg(rsTwo);
    return os.str();
}

/**
 * @brief format for s type instructions
 * @note used to encode instructions with signed 12 bit immediate 
 * operand also has a range of [-2048...2047]
 * @param insn the value for the hex input
 * @param mnemonic what operation the cpu will perform
 * @return string stream with formatting for s type insructions
 *******************************************************************/
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
    uint32_t immgets = get_imm_s(insn);
    int32_t rsOne = get_rs1 (insn);
    int32_t rsTwo = get_rs2(insn);
    
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rsTwo) << "," << render_base_disp(rsOne, immgets);
    return os.str();
}

/**
 * @brief used to transfer control back to debugging enviornment
 * 
 * @param insn hex value checker
 * @return ebreak for specific instructions of debugging
 *******************************************************************/
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
    return ("ebreak");
}

/**
 * @brief like ebreak except instead of debugging it goes into
 * an execution enviornement.
 * @param insn hex value input check
 * @return ebreak for execution enviornment of specific insn
 *******************************************************************/
std::string rv32i_decode::render_ecall(uint32_t insn)
{
    return ("ecall");
}

/**
 * @brief formatting for csrrx type instructions
 * @note csrrw reads old value of csr zero extends the value to xlen bits then 
 * writes to rd the inital value in rs1 is treated as a bitmask specificing position
 * @note csrrs reads csr value and zero extends to xlen bits writes to register rd
 * any bit high in rs1 causes corresponding bit to be set in the csr if the bit 
 * is writable
 * @note csrrc reads csr value zero extends to xlen bits and writes to register rd
 * any bit high in rs1 will cause corresponding bits to be cleared in the csr
 * @param insn input value for hex 
 * @param mnemonic what operation the cpu will perform
 * @return format for the csrrx field using string stream as an oganizer
 *******************************************************************/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
    uint32_t rd_field = get_rd(insn);
    int32_t immget = get_imm_i(insn);
    uint32_t rsOne = get_rs1(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd_field) << ",0x" << hex::to_hex0x12(immget) << ",x" << rsOne;

    return os.str();
}

/**
 * @brief formatting for csrrxi type instructions
 * @note like csrrx type values such as csrrw and csrrs and csrrc
 * act to their corresponding varients, csrrwi, csrrsi and csrrci except
 * they update the csr using xlen bit value, obtained by zero extending
 * a 5-bit unsigned immediate (uimm[4:0]) field encoded
 * in the rs1 instead of a value from an integer register
 * @param insn input value for hex 
 * @param mnemonic what operation the cpu will perform
 * @return format for the csrrxi field using string stream as an oganizer
 *******************************************************************/
std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
    uint32_t rd_field = get_rd(insn);
    int32_t rsOne = get_rs1(insn);
    
    int32_t imm_i = (insn & 0xfff00000) >> (31-11);
    if (insn & 0x80000000)
        imm_i |= 0x00000000;

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd_field) << ",0x" << hex::to_hex0x12(imm_i) << "," << rsOne;
    return os.str();
    

}

/**
 * @brief takes in the value for r and applys an x infront of that value
 * for formatting purposes
 * @param r the name of the register with it respective name
 * @return ostring stream value formating ith an x infrom of the respective 
 * r number
 *******************************************************************/
std::string rv32i_decode::render_reg(int r)
{
    std::ostringstream os;
    os << "x" << std::setfill('0') << r;
    return os.str();
}

/**
 * @brief makes all mnemonics a uniform width for printing
 * 
 * @param mnemonic address pointer to cpu for aligning 
 * @return string stream for formatting specific widths of each output
 *******************************************************************/
std::string rv32i_decode::render_mnemonic(const std::string &mnemonic)
{
    
    std::ostringstream os;
    os << std::setw(mnemonic_width) << setfill(' ') << left << mnemonic; //8 is mnemonic width
    return os.str();
}

/**
 * @brief renders the instructions of a specific addressing mode and helps to format output
 * 
 * @param base value to be put into the string for output
 * @param imm addressing for the string 
 * @note imm value is printed in decimal 
 * @return std::string 
 *******************************************************************/
std::string rv32i_decode::render_base_disp(uint32_t base, int32_t imm)
{
    std::ostringstream os;
    os << imm << "(x" << base << ")";
    return os.str();
}