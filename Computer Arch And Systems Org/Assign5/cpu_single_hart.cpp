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


#ifndef SINGLE_HART_CPP
#define SINGLE_HART_CPP


#include "cpu_single_hart.h"
#include "rv32i_hart.h"

/**
 * @brief Construct a new cpu single hart::cpu single hart object
 * to pass memory class instance to the constructor 
 * class in the base class
 * @param mem returns memory to the consutrctor 
 *******************************************************************/
cpu_single_hart::cpu_single_hart(memory &mem) : rv32i_hart(mem) 
{
}

/**
 * @brief way to determine how much memory the machine has 
 * @note the number of bytes in memory is also the address 
 * of first byte past the end of the simulated memory
 * @param exec_limit used as a comparison of whether zero is entering 
 * the loop if it is then enter a loop that calls tick until 
 * it is halted
 *******************************************************************/
void cpu_single_hart::run(uint64_t exec_limit)
{
    std::string hdr = "";
    regs.set(2, mem.get_size());

    while ((exec_limit == 0 || (exec_limit !=0 && get_insn_counter () < exec_limit)) && !is_halted())
    {
      tick(hdr);
    }

    if(is_halted())
    {
      std::cout <<get_halt_reason() <<std::endl;
    }

    std::cout << get_insn_counter() << " instructions executed\n";
}


#endif