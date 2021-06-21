#include <iostream>
#include <array>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	//Start WindowSocket:
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		cout << "Can't start WindowSocket! " << wsOk;
		return;
	}

	//Create Socket:
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; 
	serverHint.sin_family = AF_INET; 
	serverHint.sin_port = htons(54000); 

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		cout << "Can't bind the Socket! " << WSAGetLastError() << endl;
		return;
	}

	sockaddr_in client;
	int clientLength = sizeof(client); //The size of the client information

	char Buffer[1024];

	while (true)
	{
		ZeroMemory(&client, clientLength); //Clear the client structure
		ZeroMemory(Buffer, 1024); //Clear the receive buffer

		//Convert message to bytes: Decoding
		int bytesIn = recvfrom(in, Buffer, 1024, 0, (sockaddr*)&client, &clientLength);
		if (bytesIn == SOCKET_ERROR)
		{
			cout << "Error receiving from client " << WSAGetLastError() << endl;
			continue;
		}

		char clientIp[256]; 
		ZeroMemory(clientIp, 256);

		//Convert 4 bytes to a string: Encoding
		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		cout << "Server is connected: " << clientIp << " : " << Buffer << endl;
	}

	//Close Socket
	closesocket(in);

	//Buffer WindowSocket
	WSACleanup();
}