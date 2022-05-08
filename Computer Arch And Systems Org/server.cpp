#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdint>
#include <string>
#include <sstream>
#include <signal.h>

using namespace std;

static void usage()
{
	cerr << "usage [-l] [listener-port]" << endl;
	cerr << "-l port number for server to listen default is 0" << endl;

	exit(1);
}


void getCheckSum(uint8_t buf[], int bytes, uint16_t &currentCheckSum)
{
	for (int i = 0; i < bytes; i++)
	{
		currentCheckSum += buf[i];
	}
	
}

static ssize_t safe_write(int fd, const char* buf, size_t len)
{
	while (len > 0)
	{
		ssize_t wlen = write(fd, buf, len);
		if (wlen == -1)
		{
			cerr << "Unable to write to socket" << endl;
			return -1;
		}

		len -= wlen;
		buf += wlen;
	}

	return len;
}

int main(int argc, char **argv)
{

	signal(SIGPIPE, SIG_IGN);
	int opt;
	string portNumber = "0";

	while ((opt = getopt(argc, argv, "l:")) != -1)
	{
		switch (opt)
		{
			case 'l':
			{
				portNumber = optarg; 
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
	server.sin_port = htons(stoi(portNumber));
	if (bind(sock, (sockaddr*) &server, sizeof(server)) <  0) 
	{
		perror("binding stream socket");
		exit(1);
	}

	cout << "Socket has port #" << portNumber << endl;


	if (listen(sock, 3) < 0)
	{
		perror("listen");
		exit(1);
	}

	do
	{
	struct sockaddr_in address; // client address
	socklen_t addressLength = sizeof(address);
	int acceptSock = accept(sock, (struct sockaddr*) &address, &addressLength);
	if (acceptSock < 0)
	{
		perror("accept");
		exit(1);
	}

	char addressString[16];
	cout << "Accepted connection from '" << inet_ntop(AF_INET, (void *) &(address.sin_addr), addressString, 16) << "', port " << ntohs(address.sin_port) << endl;
	uint8_t buf[32768];
	int numOfBytesRead = 0;
	uint32_t runningTotalOfBytes = 0;
	uint16_t checkSum = 0;

	while ((numOfBytesRead = read(acceptSock, buf , 32768)) > 0)
	{
		if (numOfBytesRead < 0)
		{
			perror("reading stream message");
			exit(1);
		}

		runningTotalOfBytes += numOfBytesRead;
		getCheckSum(buf, numOfBytesRead, checkSum);
	}

	stringstream iss;
	iss << "Sum: " 
	    << checkSum
	    << " Len: "
	    << runningTotalOfBytes
	    << "\n";

	string response = iss.str();
	safe_write(acceptSock, response.c_str(), response.length()); //write back to client

	close (acceptSock);
	cout << "Ending connection" << endl;
	} while (true);

	return 0;
}
