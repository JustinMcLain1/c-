
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

using namespace std;

#include "memory.h"
#include "hex.h"
#include "rv32i_decode.h"
#include "cpu_single_hart.h"
#include "registerfile.h"
#include <getopt.h>
#include <string>

/**
 * @brief this function prints an appropriate “Usage” error message and “Pattern” to
 * stderr and terminates the program in the traditional manner
 *
 ********************************************************************************/
static void usage()
{
	cerr << "Usage: rv32i [-d] [-i] [-l execution-limit] [-m hex-mem-size] [-r] [-z] infile" << endl;
	cerr << "-d show a disassembly before simulation begins " << endl;
	cerr << "-i Show instruction printing during execution " << endl;
	cerr << "-l Specifies the maximum limit of instructions to execute. By default there is no limit (run forever.)" << endl;
	cerr << "-m Specifies the size of the simulated memory. By default the size must be 0x10000." << endl;
	cerr << "-r Show a dump of the hart (GP-registers and pc) status before each instruction in simulated." << endl;
	cerr << "-z Show a dump of the hart status and memory after the simulation has halted." << endl;
	exit(1);
}

/**
 * @brief decodes and prints each instruction in a loop
 * @return memory address, instruction hex value, instruction mnemonic
 ********************************************************************************/
static void disassemble(const memory &mem)
{
	uint32_t computer = 0;
	while (computer < mem.get_size())
	{
		std::cout << hex::to_hex32(computer) << ": ";
		std::cout << rv32i_decode::decode(computer, mem.get32(computer)) << endl; // store 32bit val into mem
		computer += 4;															  //post increment
	}
}

/**
 * @brief Read a input rv32 file with the right format.
 * 
 * @param argc used for reading the correct input for rv32 file w/ format
 * @param argv used for input cases
 * @return hart cases
 */

int main(int argc, char **argv)
{
	uint32_t mem_limit = 0x100; // default memory size = 256 bytes
	uint64_t exec_limit = 0;

	bool z_stat = false; 
	bool disem = false;
	bool reg_show = false;
	bool instruction_show = false;

	int opt = (getopt(argc, argv, "dil:m:rz")); 

	while (opt != -1)
	{
		switch (opt)
		{

		case 'd':
		{
			disem = true;
			break;
		}
		case 'i':
		{
			instruction_show = true;
			break;
		}
		case 'l':
		{
			std::istringstream iss(optarg);
			iss >> std::hex >> exec_limit;
			break;
		}
		case 'm':
		{
			std::istringstream iss(optarg);
			iss >> std::hex >> mem_limit;
			break;
		}
		case 'r':
		{
			reg_show = true;
			break;
		}
		case 'z':
		{
			z_stat = true;
			break;
			default:
			usage();
		}
		}
		//update
		opt = (getopt(argc, argv, "dil:m:rz")); 
	}
	if (optind >= argc)
		usage(); 

	memory mem(mem_limit);

	if (!mem.load_file(argv[optind]))
		usage();

	if (disem)
	{
		disassemble(mem);
	}

	cpu_single_hart process(mem);

	process.reset();
	process.set_show_instructions(instruction_show);
	process.set_show_registers(reg_show);
    process.run(exec_limit);

	if (z_stat == true)
	{
		process.dump();
		mem.dump();
	}
	return 0;
}