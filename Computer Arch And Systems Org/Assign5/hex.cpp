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
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <sstream>

#ifndef hex_cpp
#define hex_cpp


#include "hex.h"

std::string hex::to_hex8(uint8_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
	return os.str();
}

std::string hex::to_hex0x12(uint32_t i) {
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(3) << i;
	return os.str();
}

std::string hex::to_hex0x20(uint32_t i) {
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(5) <<  i;
    return std::string("0x") + os.str();
}

std::string hex::to_hex32(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << i;
	return os.str();
}

std::string hex::to_hex0x32(uint32_t i)
{
	return std::string("0x") + to_hex32(i);
}

#endif