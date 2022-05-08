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
 * This program creates a socket and then begins an infinite loop. Each time
 * through the loop it accepts a connection and prints out messages from it.
 * When the connection breaks, or a termination message comes through, the
 * program accepts a new connection.
 */

/**
 * @brief error checks for server client
 * 
 ********************************************************************/
static void usage()
{
	std::cerr << "usage [-l] [listener-port]" << std::endl;
	std::cerr << "-l port number for server to listen default is 0" << std::endl;

	exit(1);
}

/**
 * @brief Get the Check Sum object
 * 
 * @param buf  buffer value
 * @param bytes value to hold bytes
 * @param checkCurrentSum reads all the bytes from client while
 * adding them to uint16_t
 ********************************************************************/
void checkSumGet(uint8_t buf[], int bytes, uint16_t &checkCurrentSum)
{
	for (int i = 0; i < bytes; i++)
	{
		checkCurrentSum += buf[i];
	}
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
			std::cerr << "Unable to write to socket" << std::endl;
			return -1;
		}
		len -= wlen;
		buf += wlen;
	}

	return len;
}

/**
 * @brief takes in information from client
 * error checks and outputs request if correct then tells user server
 * connect is ended
 * @note SIGPIPE is a process if tried to write to a socket
 * that has been shutdown for writing or isnt connected anymore
 * @param argc argument counter
 * @param argv argument value for input comparison
 * @return print statement from client 
 ********************************************************************/
int main(int argc, char **argv)
{

	signal(SIGPIPE, SIG_IGN);
	std::string portVal = "0";
	int opt;

	while ((opt = getopt(argc, argv, "l:")) != -1)
	{
		switch (opt)
		{
		case 'l':
		{
			portVal = optarg;
			break;
		}
		default:
		{
			usage();
			break;
		}
		}
	}

	if ((argc - optind) > 0)
	{
		usage();
	}

	struct sockaddr_in server;
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0)
	{
		perror("opening stream socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(std::stoi(portVal));
	if (bind(sock, (sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("binding stream socket");
		exit(1);
	}

	std::cout << "Socket has port #" << portVal << std::endl;

	if (listen(sock, 3) < 0)
	{
		perror("listen");
		exit(1);
	}

	do
	{
		struct sockaddr_in address; // client address
		socklen_t addLen = sizeof(address);
		int acceptSock = accept(sock, (struct sockaddr *)&address, &addLen);
		if (acceptSock < 0)
		{
			perror("accept");
			exit(1);
		}

		char strAddress[16];
		std::cout << "Accepted connection from '" << inet_ntop(AF_INET, (void *)&(address.sin_addr), strAddress, 16) << "', port " << ntohs(address.sin_port) << std::endl;
		uint8_t buf[32136];
		uint32_t runTotBytes = 0;
		uint16_t checkSum = 0;
		int readBytesNum = 0;

		while ((readBytesNum = read(acceptSock, buf, 32136)) > 0)
		{
			if (readBytesNum < 0)
			{
				perror("reading stream message");
				exit(1);
			}

			runTotBytes += readBytesNum;
			checkSumGet(buf, readBytesNum, checkSum);
		}

		std::stringstream iss;
		iss << "Sum: " << checkSum << " Len: " << runTotBytes << "\n";

		std::string stringResp = iss.str();
		safe_write(acceptSock, stringResp.c_str(), stringResp.length()); //write back to client

		close(acceptSock);
		std::cout << "Ending connection" << std::endl;

		return 0;

	} while (true);
}
