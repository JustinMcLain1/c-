//***************************************************************************
//
//  Justin McLain
//  Z1910087
//  CSCI 463 - PE1
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************
#ifndef HART_CPP
#define HART_CPP

#include "rv32i_hart.h"
#include "registerfile.h"
#include <cassert>
#include <iomanip>

/**
 * @brief reset the rv32i object and the registerfile
 * 
 *******************************************************************/
void rv32i_hart::reset()
{
  pc = 0;
  regs.reset();
  insn_counter = 0;
  halt = false;
  halt_reason = "none";
}

/**
 * @brief dump the entire state of the hart
 * @note the prefix of each line printed is given by the hdr string 
 * (with the default being to not print any prefix)
 * @param hdr prefix line with this variable
 *******************************************************************/
void rv32i_hart::dump(const std::string &hdr) const
{
  regs.dump(hdr);
  std::cout << " pc " << hex::to_hex32(pc) << std::endl;
}

/**
 * @brief method function to tell the simulator to execute and 
 * the instructions
 * @param hdr required to be printed on the left and all output 
 * that is displayed as a result of calling hdr
 *******************************************************************/
void rv32i_hart::tick(const std::string &hdr)
{
  if (is_halted())
  {
    return;
  }
  else
  {
    ++insn_counter;
    if (show_registers == true)
    {
      dump(hdr);
    }
    uint32_t insn = mem.get32(pc);
    if (show_instructions == true)
    {
      std::cout << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << " ";
      exec(insn, &std::cout);
      std::cout << "\n";
    }
    else
    {
      exec(insn, nullptr);
    }
  }
}


/**
 * @brief This function will execute the given RV32I instruction
 * and extract the needed instruction fields to decode the instruction
 * @param insn returns the value of the particular opcode
 * @param pos  returns a point to pos used for organization of output
 *******************************************************************/
void rv32i_hart::exec(uint32_t insn, std::ostream *pos)
{

  switch (get_opcode(insn))
  {
  default:
    exec_illegal_insn(insn, pos);
    return;
  case opcode_lui:
    exec_lui(insn, pos);
    return;
  case opcode_auipc:
    exec_auipc(insn, pos);
    return;
  case opcode_jal:
    exec_jal(insn, pos);
    return;
  case opcode_jalr:
    exec_jalr(insn, pos);
    return;
  case opcode_btype:
    switch (get_funct3(insn))
    {
    default:
      exec_illegal_insn(insn, pos);
      return;
    case funct3_bne:
      exec_bne(insn, pos);
      return;
    case funct3_blt:
      exec_blt(insn, pos);
      return;
    case funct3_bge:
      exec_bge(insn, pos);
      return;
    case funct3_bltu:
      exec_bltu(insn, pos);
      return;
    case funct3_bgeu:
      exec_bgeu(insn, pos);
      return;
    case funct3_beq:
      exec_beq(insn, pos);
      return;
    }
    assert(0 && "unable to handle funct3"); //never reach here
  case opcode_rtype:
    switch (get_funct3(insn))
    {
    default:
      exec_illegal_insn(insn, pos);
      return;
    case funct3_add:
      switch (get_funct7(insn))
      {
      default:
        exec_illegal_insn(insn, pos);
        return;
      case funct7_add:
        exec_add(insn, pos);
        return;
      case funct7_sub:
        exec_sub(insn, pos);
        return;
      }
      assert(0 && "unable to handle funct7"); //never reach here
    case funct3_sll:
      exec_sll(insn, pos);
      return;
    case funct3_slt:
      exec_slt(insn, pos);
      return;
    case funct3_xor:
      exec_xor(insn, pos);
      return;
    case funct3_sltu:
      exec_sltu(insn, pos);
      return;
    case funct3_srx:
      switch (get_funct7(insn))
      {
      default:
        exec_illegal_insn(insn, pos);
        return;
      case funct7_sra:
        exec_sra(insn, pos);
        return;
      case funct7_srl:
        exec_srl(insn, pos);
        return;
      }
      assert(0 && "unable to handle funct7");
    case funct3_or:
      exec_or(insn, pos);
      return;
    case funct3_and:
      exec_and(insn, pos);
      return;
    }
    assert(0 && "unrecognized funct3");
  case opcode_stype:
    switch (get_funct3(insn))
    {
    default:
      exec_illegal_insn(insn, pos);
      return;
    case funct3_sb:
      exec_sb(insn, pos);
      return;
    case funct3_sh:
      exec_sh(insn, pos);
      return;
    case funct3_sw:
      exec_sw(insn, pos);
      return;
    }
    assert(0 && "unrecognized funct3"); //never reach here

  case opcode_alu_imm:
    switch (get_funct3(insn))
    {
    default:
      exec_illegal_insn(insn, pos);
      return;
    case funct3_add:
      exec_addi(insn, pos);
      return;
    case funct3_slt:
      exec_slti(insn, pos);
      return;
    case funct3_sltu:
      exec_sltiu(insn, pos);
      return;
    case funct3_xor:
      exec_xori(insn, pos);
      return;
    case funct3_or:
      exec_ori(insn, pos);
      return;
    case funct3_and:
      exec_andi(insn, pos);
      return;
    case funct3_sll:
      exec_slli(insn, pos);
      return;
    case funct3_srx:
      switch (get_funct7(insn))
      {
      default:
        exec_illegal_insn(insn, pos);
        return;
      case funct7_sra:
        exec_srai(insn, pos);
        return;
      case funct7_srl:
        exec_srli(insn, pos);
        return;
      }
      assert(0 && "unrecognized funct7"); //never reach here
    }
    assert(0 && "unrecognized funct3"); //never reach here
  case opcode_load_imm:
    switch (get_funct3(insn))
    {
    default:
      exec_illegal_insn(insn, pos);
      return;
    case funct3_lb:
      exec_lb(insn, pos);
      return;
    case funct3_lh:
      exec_lh(insn, pos);
      return;
    case funct3_lw:
      exec_lw(insn, pos);
      return;
    case funct3_lbu:
      exec_lbu(insn, pos);
      return;
    case funct3_lhu:
      exec_lhu(insn, pos);
      return;
    }
    assert(0 && "unrecognized funct3"); //never reach here
  case opcode_system:
    switch (insn)
    {
    default:
      switch (get_funct3(insn))
      {
      default:
        exec_illegal_insn(insn, pos);
        return;
      case funct3_csrrs:
        exec_csrrs(insn, pos);
        return;
      }
      assert(0 && "unrecognized funct3"); //never reach here
    case insn_ebreak:
      exec_ebreak(insn, pos);
      return;
    }
    assert(0 && "unrecognized instruction"); //never reach here
  }
  assert(0 && "unrecognized opcode");
}

/**
 * @brief set the halt flag and if the ostream parameter is
 * not nullptr then use render_illegal_insn to render the proper error 
 * message
 * @param insn called for the output of illegal if ostream is not nullptr
 * @param pos used to determine the ostream parameter
 *******************************************************************/
void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream *pos)
{

  (void)insn;
  if (pos)
  {
    *pos << render_illegal_insn(insn);
    halt = true;
    halt_reason = "Illegal instruction ";
  }
}

/**
 * @brief format the output of the exec_lui
 * 
 * @param insn get the insn value of imm_u
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_lui(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  if (pos)
  {
    std::string s = render_lui(insn);

    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " + " << hex::to_hex0x32(imm_u);
  }
  regs.set(rd, imm_u);
  pc += 4;
}

/**
 * @brief format the output of the exec_aupic
 * 
 * @param insn get the insn value of imm_u
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_auipc(uint32_t insn, std::ostream *pos)
{

  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  if (pos)
  {
    std::string s = render_auipc(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_u) << " = "
         << hex::to_hex0x32(pc + imm_u);
  }
  regs.set(rd, pc + imm_u);
  pc += 4;
}

/**
 * @brief format the output of the exec_jal
 * 
 * @param insn insn value of imm_j
 * @param pos ostream formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_jal(uint32_t insn, std::ostream *pos)
{
  uint32_t origPc = pc;
  std::string s = render_jal(pc, insn);
  uint32_t space = 4;

  if (pos)
  {
    s.resize(instruction_width, ' ');
    pc += get_imm_j(insn);
    *pos << s << "// " << render_reg(get_rd(insn)) << " = " << to_hex0x32(origPc + space) << ", "
         << " pc = " << to_hex0x32(origPc) << " + " << to_hex0x32(get_imm_j(insn)) << " = " << to_hex0x32(pc);
  }
  regs.set(get_rd(insn), origPc + space);
  pc = origPc + get_imm_j(insn);
}

/**
 * @brief format the output of the exec_jalr
 * 
 * @param insn used for fetching value inside render_jal 
 * and address value 
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_jalr(uint32_t insn, std::ostream *pos)
{
  uint32_t space = 4;
  std::string add = " + ";
  std::string equal = " = ";
  std::string s = render_jalr(insn);
  uint32_t ogPC = pc;

  pc = (regs.get(get_rs1(insn)) + get_imm_i(insn)) & 0xfffffffe;
  if (pos)
  {
    s.resize(instruction_width, ' ');
    *pos << s << "// " << render_reg(get_rd(insn)) << equal << to_hex0x32(ogPC + space) << ", pc = (" << to_hex0x32(get_imm_i(insn)) << add << to_hex0x32(regs.get(get_rs1(insn))) << ") & 0xfffffffe" << equal << to_hex0x32(pc);
  }
  regs.set(get_rd(insn), ogPC + space);
}

/**
 * @brief format the output of the exec_bne
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_bne(uint32_t insn, std::ostream *pos)
{
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t value = (regs.get(rsOne) != regs.get(rs2)) ? pc += imm_b : pc += 4;

  if (pos)
  {
    std::string s = render_btype(pc, insn, "bne");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "pc += (" << hex::to_hex0x32(regs.get(rsOne)) << " != " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(value);
  }
}

/**
 * @brief format the output of the exec_blt
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_blt(uint32_t insn, std::ostream *pos)
{
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t value = (regs.get(rsOne) < regs.get(rs2)) ? pc += imm_b : pc += 4;

  if (pos)
  {
    std::string s = render_btype(pc, insn, "blt");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "pc += (" << hex::to_hex0x32(regs.get(rsOne)) << " < " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(value);
  }
}

/**
 * @brief format the output of the exec_bge
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart ::exec_bge(uint32_t insn, std::ostream *pos)
{
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t value = (regs.get(rsOne) >= regs.get(rs2)) ? pc += imm_b : pc += 4;

  if (pos)
  {
    std::string s = render_btype(pc, insn, "bge");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "pc += (" << hex::to_hex0x32(regs.get(rsOne)) << " >= " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(value);
  }
}

/**
 * @brief format the output of the exec_bltu
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart ::exec_bltu(uint32_t insn, std::ostream *pos)
{
  int32_t imm_b = get_imm_b(insn);
  int32_t value = (get_rs1(insn) < get_rs2(insn)) ? pc += imm_b : pc += 4;

  if (pos)
  {
    std::string s = render_btype(pc, insn, "bltu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "pc += (" << hex::to_hex0x32(get_rs1(insn)) << " <U " << hex::to_hex0x32(get_rs2(insn)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(value);
  }
}

/**
 * @brief format the output of the exec_bgeu
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream *pos)
{
  uint32_t rsOne = regs.get(get_rs1(insn)); 
  uint32_t rs2 = regs.get(get_rs2(insn));
  int32_t immB = get_imm_b(insn);
  uint32_t space = 4;

  if (pos)
  {
    std::string s = render_btype(pc, insn, "bgeu");
    s.resize(instruction_width, ' ');
    rsOne >= rs2 ? pc += immB : pc += space;
    *pos << s << "// " << std::dec << "pc += (" << to_hex0x32(rsOne) << " >=U " << to_hex0x32(rs2) << " ? " << to_hex0x32(immB) << " : 4) = " << to_hex0x32(pc);
  }
  if (pos == nullptr)
  {
    rsOne >= rs2 ? pc += immB : pc += space;
  }
}

/**
 * @brief format the output of the exec_beq
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_beq(uint32_t insn, std::ostream *pos)
{
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);
  int32_t imm_b = get_imm_b(insn);
  int32_t value = (regs.get(rsOne) == regs.get(rs2)) ? pc += imm_b : pc += 4;

  if (pos)
  {
    std::string s = render_btype(pc, insn, "beq");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "pc += (" << hex::to_hex0x32(regs.get(rsOne)) << " == " << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(value);
  }
}

/**
 * @brief format the output of the exec_lb
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_lb(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  int32_t rsOne = get_rs1(insn);

  int32_t value = mem.get8(regs.get(rsOne) + imm_i) | 0xffffff00; 

  regs.set(rd, value);
  pc += 4;

  if (pos)
  {
    std::string s = render_itype_load(insn, "lb");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = sx(m8(" << hex::to_hex0x32(regs.get(rsOne)) << " + " << hex::to_hex0x32(imm_i) << " )) " << hex::to_hex0x32(value);
  }
}

/**
 * @brief format the output of the exec_lh
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_lh(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  int32_t rsOne = get_rs1(insn);

  int32_t value = mem.get16(regs.get(rsOne) + imm_i) | 0xffff0000; 

  regs.set(rd, value);
  pc += 4;

  if (pos)
  {
    std::string s = render_itype_load(insn, "lh");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = sx(m16(" << hex::to_hex0x32(regs.get(rsOne)) << " + " << hex::to_hex0x32(imm_i) << " )) " << hex::to_hex0x32(value);
  }
}

/**
 * @brief format the output of the exec_lw
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_lw(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  int32_t rsOne = get_rs1(insn);

  int32_t value = mem.get32(regs.get(rsOne) + imm_i); 
  regs.set(rd, value);
  pc += 4;

  if (pos)
  {
    std::string s = render_itype_load(insn, "lh");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = sx(m32(" << hex::to_hex0x32(regs.get(rsOne)) << " + " << hex::to_hex0x32(regs.get(imm_i)) << " )) " << hex::to_hex0x32(value);
  }
}

/**
 * @brief format the output of the exec_lbu
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_lbu(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  int32_t rsOne = get_rs1(insn);

  int32_t value = mem.get8(regs.get(rsOne) + imm_i);

  regs.set(rd, value);

  if (pos)
  {
    std::string s = render_itype_load(insn, "lbu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = zx(m8(" << hex::to_hex0x32(regs.get(rsOne)) << " + " << hex::to_hex0x32(regs.get(imm_i)) << " )) " << hex::to_hex0x32(value);
  }
  pc += 4;
}

/**
 * @brief format the output of the exec_lhu
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_lhu(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  int32_t rsOne = get_rs1(insn);

  int32_t value = mem.get16(regs.get(rsOne) + imm_i);

  regs.set(rd, value);

  if (pos)
  {
    std::string s = render_itype_load(insn, "lhu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = zx(m16(" << hex::to_hex0x32(regs.get(rsOne)) << " + " << hex::to_hex0x32(imm_i) << " )) " << hex::to_hex0x32(value);
  }
  pc += 4;
}

/**
 * @brief format the output of the exec_sb
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_sb(uint32_t insn, std::ostream *pos)
{
  uint32_t imm_s = get_imm_s(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t value_first = regs.get(rsOne) + imm_s;
  int32_t value_two = (regs.get(rs2) & 0x000000ff);

  if (pos)
  {
    std::string s = render_stype(insn, "sb");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "m8(" << hex::to_hex0x32(regs.get(rsOne)) << " + " << hex::to_hex0x32(imm_s) << " )) " << hex::to_hex0x32(value_two);
  }
  mem.set8(value_first, value_two);
  pc += 4;
}

/**
 * @brief format the output of the exec_sh
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_sh(uint32_t insn, std::ostream *pos)
{
  uint32_t imm_s = get_imm_s(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t value_first = regs.get(rsOne) + imm_s;
  int32_t value_two = (regs.get(rs2) & 0x0000ffff);

  if (pos)
  {
    std::string s = render_stype(insn, "sh");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "m16(" << hex::to_hex0x32(regs.get(rsOne)) << " + " << hex::to_hex0x32(imm_s) << ")= " << hex::to_hex0x32(value_two);
  }
  mem.set16(value_first, value_two);
  pc += 4;
}

/**
 * @brief format the output of the exec_sw
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_sw(uint32_t insn, std::ostream *pos)
{
  uint32_t imm_s = get_imm_s(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t value_first = regs.get(rsOne) + imm_s;
  int32_t value_two = (regs.get(rs2) & 0xffffffff);

  if (pos)
  {
    std::string s = render_stype(insn, "sw");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "m32(" << hex::to_hex0x32(regs.get(rsOne)) << " + " << hex::to_hex0x32(imm_s) << ") = " << hex::to_hex0x32(value_two);
  }
  mem.set32(value_first, value_two);
  pc += 4;
}

/**
 * @brief format the output of the exec_addi
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_addi(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t value = regs.get(rsOne) + imm_i;
  if (pos)
  {
    std::string s = render_itype_alu(insn, "addi", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " + " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_slti
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_slti(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t value = (regs.get(rsOne) < imm_i) ? 1 : 0;

  if (pos)
  {
    std::string s = render_itype_alu(insn, "slti ", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rsOne)) << " < " << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_sltiu
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t value = (regs.get(rsOne) < imm_i) ? 1 : 0;

  if (pos)
  {
    std::string s = render_itype_alu(insn, "sltiu ", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rsOne)) << " <U " << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_xori
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_xori(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t value = regs.get(rsOne) ^ imm_i;

  if (pos)
  {
    std::string s = render_itype_alu(insn, "xori", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
        << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " ^ " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_ori
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_ori(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t value = regs.get(rsOne) | imm_i;

  if (pos)
  {
    std::string s = render_itype_alu(insn, "ori", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
        << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " | " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_andi
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_andi(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t value = regs.get(rsOne) & imm_i;

  if (pos)
  {
    std::string s = render_itype_alu(insn, "andi", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " & " << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_slli
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_slli(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t value = regs.get(rsOne) << imm_i;

  if (pos)
  {
    std::string s = render_itype_alu(insn, "slli", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " << " << imm_i << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_srli
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_srli(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t value = regs.get(rsOne) >> imm_i;

  if (pos)
  {
    std::string s = render_itype_alu(insn, "srli", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " >> " << imm_i << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_srai
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_srai(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t imm_i = get_imm_i(insn);

  int32_t value = regs.get(rsOne) >> imm_i;

  if (pos)
  {
    std::string s = render_itype_alu(insn, "srai", imm_i);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " >> " << imm_i << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_add
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_add(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t value = (regs.get(rsOne) + regs.get(rs2));

  if (pos)
  {
    std::string s = render_rtype(insn, "add");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " + " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_sub
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_sub(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t value = (regs.get(rsOne) - regs.get(rs2));

  if (pos)
  {
    std::string s = render_rtype(insn, "sub");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " - " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_sll
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_sll(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t rs2_len = regs.get(rs2) % XLEN;
  int32_t value = (regs.get(rsOne) << rs2_len);

  if (pos)
  {
    std::string s = render_rtype(insn, "sll");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " << " << rs2_len << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_slt
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_slt(uint32_t insn, std ::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  uint32_t rsOne = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  int32_t value = (regs.get(rsOne) < regs.get(rs2)) ? 1 : 0;

  if (pos)
  {
    std::string s = render_rtype(insn, " slt ");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rsOne)) << " < " << hex ::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex ::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_sltu
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_sltu(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  uint32_t rsOne = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  int32_t value = (regs.get(rsOne) < regs.get(rs2)) ? 1 : 0;

  if (pos)
  {
    std::string s = render_rtype(insn, "sltu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rsOne)) << " <U " << hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_xor
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_xor(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  uint32_t rsOne = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  int32_t value = (regs.get(rsOne) ^ regs.get(rs2));

  if (pos)
  {
    std::string s = render_rtype(insn, "xor");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
        << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " ^ " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_srl
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_srl(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t rs2_len = regs.get(rs2) % XLEN;
  int32_t value = (regs.get(rsOne) >> rs2_len);

  if (pos)
  {
    std::string s = render_rtype(insn, "srl");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
        << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " >> " << rs2_len << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_sra
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_sra(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t rs2_len = regs.get(rs2) % XLEN;
  int32_t value = (regs.get(rsOne) >> rs2_len);

  if (pos)
  {
    std::string s = render_rtype(insn, "sra");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
        << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " >> " << rs2_len << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_or
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_or(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t value = (regs.get(rsOne) | regs.get(rs2));

  if (pos)
  {
    std::string s = render_rtype(insn, "or");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
        << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " | " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_and
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_and(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  int32_t rsOne = get_rs1(insn);
  int32_t rs2 = get_rs2(insn);

  int32_t value = (regs.get(rsOne) & regs.get(rs2));

  if (pos)
  {
    std::string s = render_rtype(insn, "and");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
        << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rsOne)) << " & " << hex::to_hex0x32(regs.get(rs2)) << " = " << hex::to_hex0x32(value);
  }
  regs.set(rd, value);
  pc += 4;
}

/**
 * @brief format the output of the exec_csrrs
 * 
 * @param insn assign value to get functions
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream *pos)
{
  uint32_t rd = get_rd(insn);
  uint32_t rsOne = get_rs1(insn);

  int32_t value = (regs.get(rsOne));

  if (pos)
  {
    std::string s = render_csrrx(insn, "csrrs");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
        << render_reg(rd) << " = " << regs.get(rsOne);
  }
   regs.set(rd, value);
   pc += 4;
}

/**
 * @brief format the output of the exec_ebreak
 * 
 * @param insn assign value for the render_ebreak
 * @param pos formatting for output of exec
 *******************************************************************/
void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream *pos)
{
  if (pos)
  {
    std::string s = render_ebreak(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// HALT";
  }
  halt = true;
  halt_reason = "Execution terminated. Reason: EBREAK instruction ";
}

#endif