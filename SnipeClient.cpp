#include <iostream>
#include <array>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main(int argc, char* argv[])
{
	WSADATA data;

	WORD version = MAKEWORD(2, 2);

	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		// Not ok! Get out quickly
		cout << "Can't start Winsock! " << wsOk;
		return;
	}

	//Server Structure:
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(54000); //54000 
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); 

	//Create Socket:
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	string s(argv[1]);
	int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

	if (sendOk == SOCKET_ERROR)
	{
		cout << "That didn't work! " << WSAGetLastError() << endl;
	}


	closesocket(out);
	WSACleanup();
}