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

/**
 * This is to define the objects within the class for hex.h
 * 
 * @param i This acts as an argument for every hex input where it is meant to act
 * for the string input which is turned into a hex value
 ********************************************************************************/

#include <iostream>
#include <string>
#include <sstream>


/*! A hex class */
/*! these form the defintions for the hex classes*/

class hex
{
public:
    /**
     * @brief returns string with two hex digits repreenting 8 bits 
     * of argument i 
     * @note static_cast is neccessary here to prevent inseration of << from
     * treating the 8-bit intager as a character and printing improperly
     * @return string
     ********************************************************************************/
    static std::string to_hex8(uint8_t i);
    
    /**
     * @brief 3 hex digit represents 12 bits of the i asrgument
     * 
     * @param i input from hex string
     * @return std::string 
     ********************************************************************************/
    static std::string to_hex0x12(uint32_t i);

    /**
     * @brief 5 hex digit representing 20 bits of the argument i
     * 
     * @param i input from hex string
     * @return std::string 
     ********************************************************************************/
    static std::string to_hex0x20(uint32_t i);

    /**
     * @brief returns 8 hex digits representing 32 bits of the i argument 
     * 
     * @param i input from hex string
     * @return string
     ********************************************************************************/
    static std::string to_hex32(uint32_t i);

    /**
     * @brief returns string with 0x followed by 8 hex digits representing 32 bits of 
     * i argument
     * @note it is implemeneted by creating a string by concatenating a 0x to the 
     * output of to_hex32()
     * @return string with the value "0x" in front
      ********************************************************************************/
    static std::string to_hex0x32(uint32_t i);
};
