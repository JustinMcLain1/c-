
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
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
using namespace std;


#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"
#include <getopt.h>

/**
 * this function prints an appropriate “Usage” error message and “Pattern” to
 * stderr and terminates the program in the traditional manner
 *
 ********************************************************************************/
static void usage()
{
	std::cerr << "Usage: rv32i [-m hex-mem-size] infile" << std::endl;
	std::cerr << "    -m specify memory size (default = 0x100)" << std::endl;
	exit(1);
}

/**
 * @brief decodes and prints each instruction in a loop
 * @return memory address, instruction hex value, instruction mnemonic
 ********************************************************************************/
static void disassemble(const memory &mem)
{
	uint32_t computer= 0;
    while (computer < mem.get_size())
    {
        std::cout << hex::to_hex32(computer) << ": ";
        std::cout << rv32i_decode::decode(computer,mem.get32(computer)) << endl; // store 32bit val into mem
        computer += 4; //post increment
    }

}

/**
 * @brief helps to organize the output and determine the inputs
 * @param argc determines if input is valid
 * @param argv determines if files are valid when comparing indexs
 * @returns custom mem and specific strings
  ********************************************************************************/
int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x100; // default memory size = 256 bytes
	int opt;
	while ((opt = getopt(argc, argv, "m:")) != -1)
	{
		switch (opt)
		{
		case 'm':
		{
			std::istringstream iss(optarg);
			iss >> std::hex >> memory_limit;
		}
		break;
		default: /* ’?’ */
			usage();
		}
	}
	if (optind >= argc)
		usage(); // missing filename
	memory mem(memory_limit);
	if (!mem.load_file(argv[optind]))
		usage();
	disassemble(mem);
	mem.dump();
	return 0;
}