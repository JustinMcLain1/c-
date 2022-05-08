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



#ifndef CPU_SINGLE_HART_H
#define CPU_SINGLE_HART_H

#include "rv32i_hart.h"

/**
* 
* @brief this file contains a class with some utility functions for formatting 
*  numbers as hex stringsfor printing
*  
********************************************************************************/ 




class cpu_single_hart : public rv32i_hart
{ public:
   cpu_single_hart(memory &mem);
   void run(uint64_t exec_limit);
};




#endif