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

#ifndef REGFILE_H
#define REGFILE_H
#include "memory.h"
#include "rv32i_decode.h"
#include "hex.h"
#include <iostream> 
#include <vector>


class registerfile {
  public:
  void reset();
  void set(uint32_t r, int32_t val);
  int32_t get(uint32_t r) const;
  void dump(const std::string &hdr) const;

  private:
  std::vector<int32_t> element;
};

#endif
