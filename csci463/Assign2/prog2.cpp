//******************************************************************
//
// CSCI 463 Assignment 2
//
// Author: Justin McLain
// ZID: z1910087
// Course: CSCI463
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//******************************************************************

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <ostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
*  
*                                    
* @param x is an unsigned int that is read in as a 32-bit hexadecimal value 
* @param sig is a unsigned int that represents the significand.
* @param exp is a signed int that represents the exponent.
*                                   
* @return x which contains the input from user         
*
* @note   Function printBinFloat uses bitwise operators to extract and shift  
*         the fields of the IEEE numbers as needed to render the output.         
****************************************************************************/
uint32_t printBinFloat(uint32_t x)
{
   uint32_t sig;
   int32_t exp;
   exp = ((x & 0x7f800000) >> 23) - 0x7F; 
   sig = (x & 0x007fffff);               
   std::cout << std::hex << "0x" << std::setw(8) << std::setfill('0') << x; 
   std::cout << " = ";
   for (int i = 31; i > -1; i--) 
   {
      std::cout << (i < 31 && (i % 4) == 3 ? " " : "") << (x & 1 << i ? '1' : '0'); 
   }
   std::cout << std::endl;
   std::cout << "sign: " << (x & 0x80000000 ? '1' : '0') << std::endl;
   std::cout << " exp: 0x" << std::setw(8) << exp; 
   std::cout << " (" << std::dec << exp << ")" << std::hex << std::endl; 
   std::cout << " sig: 0x" << std::setw(8) << std::setfill('0') << sig << std::endl; 
   std::cout << (x & 0x80000000 ? '-' : '+');
   if (exp == 128 && sig == 00000000) 
      std::cout << "inf" << std::endl;
   }
   else if (exp == -127 && sig == 00000000) 
   {
      std::cout << "0" << std::endl;
   }
   else if ((int)exp >= 0) 
   {
      int digits = (int)exp + 23;                 
      std::cout << '1'; 
      sig = sig << 9;
      for (int i = 0; i < digits; i++) 
      {
         if (i == (int)exp) 
         {
            std::cout << '.'; 
         }
         std::cout << ((sig & 0x80000000) ? '1' : '0'); 
         sig = sig << 1;                               

         if (i >= (int)exp && i > 23) 
         {
            break;
         }
      }
      std::cout << std::endl;
   }

   else if (exp < 0)
   {
      std::cout << "0."; 
      sig = sig << 9; 
      int leadingZeros;           
      leadingZeros = abs(exp) - 1;

      for (int i = 0; i < leadingZeros; i++) 
      {
         std::cout << '0'; 
      }
      std::cout << '1';           
      for (int i = 0; i < 23; i++) 
      {
         if (sig & 0x80000000) 
            std::cout << '1';
         else
            std::cout << '0';

         sig = sig << 1; 
      }
      std::cout << std::endl;
   }
   return x;
}

int main()
{
   uint32_t x;
   while (std::cin >> std::hex >> x) 
   {
      printBinFloat(x);
   }
}