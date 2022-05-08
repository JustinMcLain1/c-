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

#ifndef reg_cpp
#define reg_cpp

#include "registerfile.h"

/**
 * @brief implements a dump of registers.
 * 
 * @param hdr this is a string that must be printed at the beginning 
 * of the output lines.
 *******************************************************************/
void registerfile::dump(const std::string &hdr) const{

  uint32_t size = 32;
  uint32_t compare = 0;

  for (uint32_t i = 0; i < size; ++i ){
    if (compare == 8){
      std::cout<< std::endl;
      compare = 0;
    }
    if ( ((i % 8) == 0) && i < 9 ) {
      std::cout << hdr << " x" << i << " ";
    }
    else if ((i % 8) == 0){
      std::cout << hdr << "x" << i << " ";
    }
    compare++;

    if (compare % 8 == 0){
        std::cout<< hex::to_hex32(element[i]);
    }
    else{
      std::cout<< hex::to_hex32(element[i])<<" ";
    }
  }
  std::cout << std::endl;
}

/**
 * @brief reset used to initalize register x0 to zero and all other registers
 * to 0xf0f0f0f0.
 *******************************************************************/
void registerfile::reset(){

  uint32_t hart =  32;      
  uint32_t zero_reg = 0x0;            
  uint32_t all_reg = 0xf0f0f0f0;                
  uint32_t i;

  for (i = 0; i < hart; i++){
      element.push_back(i) ;
      element[i] = all_reg;
  }
    element.push_back(0);
    element[0] = zero_reg;
}

void registerfile::set(uint32_t r, int32_t val){
  if (r != 0)
   element[r]= val;

}


/**
 * @brief return the value of register r if r is zero then return zero
 * 
 * @param r the register 
 * @note if r is zero then return zero
 * @return the value of register r
 *******************************************************************/
int32_t registerfile::get(uint32_t r)const {
   uint32_t return_zero = 0;

   if (r == 0){
     return_zero = 0;
   }
   else{
     return_zero = element[r];
   }
  return return_zero;
}



#endif