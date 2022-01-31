#include <iostream>
#include <array>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

void main()
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	//Start WindowSocket:
	int64_t wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		std::cout << "Can't start WindowSocket! " << wsOk;
		return;
	}

	//Create Socket:
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; 
	serverHint.sin_family = AF_INET; 
	serverHint.sin_port = htons(54000); 

	if (bind(in, (sockaddr*) &serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		std::cout << "Can't bind the Socket! " << WSAGetLastError() << '\n';
		return;
	}

	sockaddr_in client;
	int64_t clientLength = sizeof(client); //The size of the client information

	char Buffer[1024];

	while (true)
	{
		ZeroMemory(&client, clientLength); //Clear the client structure
		ZeroMemory(Buffer, 1024); //Clear the receive buffer

		//Convert message to bytes: Decoding
		int64_t bytesIn = recvfrom(in, Buffer, 1024, 0, (sockaddr*) &client, &clientLength);
		if (bytesIn == SOCKET_ERROR)
		{
			std::cout << "Error receiving from client " << WSAGetLastError() << '\n';
			continue;
		}

		char clientIp[256]; 
		ZeroMemory(clientIp, 256);

		//Convert 4 bytes to a string: Encoding
		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		std::cout << "Server is connected: " << clientIp << " : " << Buffer << '\n';
	}

	//Close Socket
	closesocket(in);

	//Buffer WindowSocket
	WSACleanup();
}
