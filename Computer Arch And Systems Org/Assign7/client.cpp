
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

/*
 Copyright (c) 1986, 1993
 The Regents of the University of California.  All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. All advertising materials mentioning features or use of this software
    must display the following acknowledgement:
 This product includes software developed by the University of
 California, Berkeley and its contributors.
 4. Neither the name of the University nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

	Modifications to make this build & run on Linux by John Winans, 2021
*/

/*
 * This program creates a socket and initiates a connection with the socket
 * given in the command line.  One message is sent over the connection and
 * then the socket is closed, ending the connection. The form of the command
 * line is streamwrite hostname portnumber
 */

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <cstdint>
#include <string>
#include <sstream>
#include <signal.h>

/**
 * @brief error checking output for client 
 * 
 ********************************************************************/
static void usage()
{
	std::cerr << "Usage: [-s] [port number]" << std::endl;
	std::cerr << "-s specify servers IPv4 address in dotted quad format. default is 127.0.0.1" << std::endl;
	std::cerr << "must include server port number" << std::endl;

	exit(1);
}

/**
 * @brief completes any partially written data
 * 
 * @param fd the file descriptor
 * @param wlen returns -1 if it wasnt printed properly
 * @param buf the buffer 
 * @param len the length of the buffer
 * @return the full request if we get to "return len" or 
 * return -1 if there is an error
 ********************************************************************/
static ssize_t safe_write(int fd, const char *buf, size_t len)
{
	while (len > 0)
	{
		ssize_t wlen = write(fd, buf, len);
		if (wlen == -1)
		{
			return -1;
		}

		len -= wlen; // reduce the remaining number of bytes
		buf += wlen; //advance the buffer past the written data
	}

	return len;
}

/**
 * @brief read data from given socket prints until EOF
 * 
 * @param fd the given socket (file descriptor)
 * @return the given socket until eof
 ********************************************************************/
static int print_response(int fd)
{
	char buf[1024];

	int rval; //prime the loop
	if ((rval = read(fd, buf, sizeof(buf) - 1)) < 0)
	{
		perror("reading stream message");
		return -1; //lets the caller know the bad things happening
	}

	else
	{
		buf[rval] = '\0'; //zero terminate buffer
		std::cout << buf;
	}

	return 0;
}


/**
 * @brief sets the default address ip checks for s
 * if no s return usage if s print the response
 * @param argc arugment counter checks for s
 * @param argv arugment value check for the s value
 * @return the output of the file given to the client 
 * and the socket value
 ********************************************************************/
int main(int argc, char **argv)
{
	int opt;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	std::string def_address = "127.0.0.1";

	struct sockaddr_in server;
	server.sin_family = AF_INET;

	while ((opt = getopt(argc, argv, "s:")) != -1)
	{
		switch (opt)
		{
		case 's':
		{
			def_address = optarg;
			break;
		}
		default:
		{
			usage();
			break;
		}
		}
	}

	if ((argc - optind) != 1)
	{
		usage();
	}

	try
	{
		server.sin_port = htons(std::stoi(argv[optind])); //get port number
	}

	catch (std::exception const &e)
	{
		std::cerr << "unable to convert [" << argv[optind] << "[ to port:" << std::endl;
	}

	if (inet_pton(AF_INET, def_address.c_str(), (void *)&server.sin_addr) <= 0)
	{
		std::cerr << "invalid address format " << def_address.c_str() << std::endl;
		exit(2);
	}

	if (connect(sock, (sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connecting stream socket");
		exit(1);
	}

	char data[2048];

	ssize_t len = read(fileno(stdin), data, sizeof(data));

	while (len > 0)
	{
		if (safe_write(sock, data, len) < 0)
		{
			perror("writing on stream socket");
		}

		len = read(fileno(stdin), data, sizeof(data));
	}

	//subsequent read calls to return EOF on server side
	shutdown(sock, SHUT_WR);

	print_response(sock);

	return 0;
}
