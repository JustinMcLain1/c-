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

using namespace std;

static void usage()
{
	cerr << "Usage: [-s] [port number]" << endl;
	cerr << "-s specify servers IPv4 address in dotted quad format. default is 127.0.0.1" << endl;
	cerr << "must include server port number" << endl;

	exit (1);
}

static int print_response(int fd)
{
	char buf[1024];

	int rval;	
	if ((rval = read(fd, buf, sizeof(buf)-1)) < 0 )
	{
		perror("reading stream message");
		return -1;
	}

	else
	{
		buf[rval] = '\0';
		cout << buf;
	}

	return 0;
}

static ssize_t safe_write(int fd, const char* buf, size_t len)
{
	while (len > 0)
	{
		ssize_t wlen = write(fd, buf, len);
		if (wlen == -1)
		{
			return -1;
		}

		len -= wlen;
		buf += wlen;
	}

	return len;
}


int main(int argc, char **argv)
{
	int opt;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	string address = "127.0.0.1";

	struct sockaddr_in server;
	server.sin_family = AF_INET;

	while ((opt = getopt(argc, argv, "s:")) != -1)
	{
		switch (opt)
		{
			case 's':
			{
				address = optarg;
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
		server.sin_port = htons(stoi(argv[optind])); //get port number
	}

	catch (std::exception const &e)
	{
		cerr << "unable to convert [" << argv[optind] << "[ to port:" << endl;
	}
				
	if (inet_pton(AF_INET, address.c_str(), (void *)& server.sin_addr) <= 0)
	{
		cerr << "invalid address format " << address.c_str() << endl;
		exit(2);
	}


	if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0)
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

	shutdown(sock, SHUT_WR);

	print_response(sock);



	return 0;
}
