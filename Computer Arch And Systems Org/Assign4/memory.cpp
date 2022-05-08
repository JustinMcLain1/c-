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

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <ostream>
#include <vector>

#ifndef memory_cpp
#define memory_cpp

#include "hex.h"
#include "memory.h"

/**
* @brief Construct a new memory object
* @param siz is the number of bytes to create in simulated memory
* as an unsigned integer
***************************************************************************/
memory::memory(uint32_t siz)
{
    siz = (siz + 15) & 0xfffffff0; //round the length up, mod-16
    mem.resize(siz, 0xa5);
}

/** 
 * its a destructor that deallocates the simulated memory 
*****************************************************************************/
memory::~memory()
{
}

/**
* @brief checks if the address is legal
* @param i unsigned integer containing the address to be checked
* @return true if address is a valid in the simulated memory
* @return false if address is not valid in the simulated memory
***************************************************************************/
bool memory::check_illegal(uint32_t i) const
{
    if (i >= mem.size())
    {
        std::cout << "WARNING: Address out of range: " << hex::to_hex0x32(i) << std::endl;
        return true;
    }
    else
    {
        return false;
    }
}

/**
* @brief Get the size of mem
* 
* @return the containing memory size
***************************************************************************/
uint32_t memory::get_size() const
{
    return mem.size();
}

/**
* @brief checks if given addr is in simulated memory by calling 
* check_illegal()
* @note if addr is in valid range then return value of byte from sim memory
* @param addr Address of bytes being retreived
* @return the byte at the given address 
***************************************************************************/
uint8_t memory::get8(uint32_t addr) const
{
    if (!memory::check_illegal(addr))
    {
        return mem.at(addr);
    }
    else
    {
        return 0;
    }
}

/**
* @brief calls get8 function twice to get two bytes then combine them
* @note the combination will be in little endian order to create 16-bit 
* return value
* @note because we are using get8() function, job of validating address
* of the two bytes will be taken care of there
* @param addr Address of bytes being retreived
* @return the two bytes at the given address
***************************************************************************/
uint16_t memory::get16(uint32_t addr) const
{
    uint8_t callTwo = get8(addr);
    uint8_t callOne = get8(addr + 1);
    return ((uint16_t)callOne << 8) | callTwo;
}

/**
* @brief calls get16 function twice to get 4 bytes then combine them
* @note the combination will be in little endian order to create 32-bit 
* return value
* @note because we are using get16() function, job of validating address
* of the two bytes will be taken care of there
* @param addr Address of bytes being retreived
* @return the four bytes at the address
***************************************************************************/
uint32_t memory::get32(uint32_t addr) const
{
    uint16_t callTwo = get16(addr);
    uint16_t callOne = get16(addr + 2);
    return ((uint32_t)callOne << 16) | callTwo;
}

/**
* @brief Calls get8() and returns the sign extended value as a 32-bit integer
* @param addr Address of bytes being retreived
* @return The byte at the given address as sign-ext 32 bit
***************************************************************************/
int32_t memory::get8_sx(uint32_t addr) const
{
    return (int32_t)get8(addr);
}

/**
* @brief Calls get16() and returns the sign extended value as a 32-bit integer
* @param addr Address of bytes to be retrieved
* @return The byte at the given address as sign-ext 32 bit
***************************************************************************/
int32_t memory::get16_sx(uint32_t addr) const
{
    return (int32_t)get16(addr);
}

/**
* @brief Calls get32() and returns the sign extended value as a 32-bit integer
*
* @param addr Address of bytes to be retrieved
*
* @return The byte at the given address converted to a signed integer
***************************************************************************/
int32_t memory::get32_sx(uint32_t addr) const
{
    return get32(addr);
}

/**
* Sets the byte at addr to val
*
* @brief this function calls check_illegal() to verify addr
* @note if addr is valid it sets the byte in the simulated memory 
* to the given val
* @note if addr is not valid then discard the data and return to caller
* @note this and the constructor are the only code
* that will ever write values into mem vector
* @param addr Address of byte to be changed
* @param val the newly assigned value of the given byte
***************************************************************************/
void memory::set8(uint32_t addr, uint8_t val)
{
    if (!memory::check_illegal(addr))
    {
       mem.at(addr) = val;
    }
}

/**
* @brief Calls set8() twice to store the two bytes in little-endian order
*
* @note is injected into simulated memory starting at the address given in 
* addr argument
* @param addr Address of bytes to be changed
* @param val the newly assigned value of the given byte
***************************************************************************/
void memory::set16(uint32_t addr, uint16_t val)
{
    uint8_t callOne = (uint8_t)((val & 0xFF00) >> 8);
    uint8_t callTwo = (uint8_t)(val & 0x00FF);
    set8(addr + 1, callOne);
    set8(addr, callTwo);
}

/**
* @brief calls set16() twice to store the gfiven val in little-endian order
* @note is injectd into simulated memory starting at the address given in 
* the addr argument
* @param addr Address of bytes to be changed
* @param val the newly assigned value of the given byte
***************************************************************************/
void memory::set32(uint32_t addr, uint32_t val)
{
    uint16_t callOne = (uint16_t)((val & 0xFFFF0000) >> 16);
    uint16_t callTwo = (uint16_t)(val & 0x0000FFFF);
    set16(addr + 2, callOne);
    set16(addr, callTwo);
}

/**
* @brief dump the contents of the simulated memory in hex with corresponding 
* ASCII
* @note the characters on the right exactly, space-for-space in the format 
* shown within the handout
***************************************************************************/
void memory::dump() const
{

    std::string endOfLine = "";
    for (uint32_t addr = 0; addr < get_size(); ++addr)
    {
        if (addr % 16 == 0)
            std::cout << to_hex32(addr) << ": ";

        std::cout << hex::to_hex8(get8(addr)) << " ";

        uint8_t ch = get8(addr); 
        ch = isprint(ch) ? ch : '.';
        endOfLine += ch;

        if (addr % 16 == 15) // modulus input counter
        {
            std::cout << "*" << endOfLine << "*" << std::endl;
            endOfLine.clear();
        }

        if (addr % 16 == 7) // modulus space counter
            std::cout << " ";

    }
}

/**
* Opens file named fname in binary mode and reads contents into memory
***************************************************************************/
bool memory::load_file(const std::string &fname)
{
    std::ifstream infile(fname, std::ios::in | std::ios::binary);
    if (!infile.is_open())
    {
        std::cerr << "Can't open file '" << fname << "' for reading." << std::endl;
        return false;
    
    }
    else
    {
        uint8_t i;
        infile >> std::noskipws; // read file contents with the extract opperator
        for (uint32_t addr = 0; infile >> i; ++addr)
        {
            if (memory::check_illegal(addr))
            {
                std::cerr << "Program too big." << std::endl;
                return false;
            }
            set8(addr, i);
        }
        return true;
    }
}

#endif