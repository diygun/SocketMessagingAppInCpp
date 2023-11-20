// ServerSocket.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <string>
#include <thread>


#pragma comment(lib,"WS2_32")

#define DEFAULT_BUFLEN 512

using namespace std;

void receiveMessages(SOCKET acceptSocket) {
	while (true) {
		char buffer[DEFAULT_BUFLEN];
		int result = recv(acceptSocket, buffer, DEFAULT_BUFLEN - 1, 0);

		if (result > 0) {
			buffer[result] = '\0';
			cout << "Client: " << buffer << endl;
			if (strcmp(buffer, "exit") == 0)
				break;
		}
		else if (result == 0) {
			cout << "Client: Connection closed" << endl;
			break;
		}
		else {
			cout << "Client: Error while receiving message: " << WSAGetLastError() << endl;
			break;
		}
	}
}
void sendMessages(SOCKET acceptSocket) {
	string message;
	while (true) {
		getline(cin, message);
		int totalSent = 0;
		int messageLength = message.length();
		while (totalSent < messageLength) {
			int sent = send(acceptSocket, message.c_str() + totalSent, messageLength - totalSent, 0);
			if (sent == SOCKET_ERROR) {
				cerr << "Error in sending message: " << WSAGetLastError() << endl;
				return;
			}
			totalSent += sent;
		}

		if (message == "exit") break;
	}
}

int main()
{
	cout << "======================= \n";
	cout << "Server\n";
	cout << "======================= \n";

	SOCKET serverSocket, acceptSocket;
	int port = 55555;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0) {
		cout << "Server: The Winsock dll not found!\n";
		return 0;
	}
	else {
		//cout << "Server: The Winsock2 dll found\n";
		//cout << "Server: The status: " << wsaData.szSystemStatus << endl;
		cout << " [##";
	}


	/*cout << "======================= \n";
	cout << "Setup a socket !\n";
	cout << "======================= \n";*/
	cout << "##";


	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		cout << "Server: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else {
		//cout << "Server: socket() is OK!\n";
		cout << "##";
	}

	/*cout << "======================= \n";
	cout << "Bind the socket !\n";
	cout << "======================= \n";*/



	sockaddr_in serverService;
	serverService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &serverService.sin_addr.s_addr);
	serverService.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&serverService, sizeof(serverService)) == SOCKET_ERROR) {
		cout << "Server: bind() failed: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {
		//cout << "Server: bind() is OK!\n";
		cout << "##";
	}



	/*cout << "======================= \n";
	cout << "Listen the socket !\n";
	cout << "======================= \n";*/

	if (listen(serverSocket, 1) == SOCKET_ERROR) {
		cout << "Server: listen(): Error listening on socket " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {
		//cout << "Server: listen() is OK, I'm waiting for connections...\n";
		cout << "##";
	}

	/*cout << "======================= \n";
	cout << "Accept the socket !\n";
	cout << "======================= \n";*/

	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET) {
		cout << "accept failed: " << WSAGetLastError() << endl;
		WSACleanup();
		closesocket(serverSocket);
		return -1;
	}
	else {
		//cout << "Accepted connection from client" << endl;
		cout << "##] Client connected successfully";
		
	}

	std::thread receiveThread(receiveMessages, acceptSocket);
	std::thread sendThread(sendMessages, acceptSocket);
	receiveThread.join();
	sendThread.join();


	// Generate my Prv and Pb key
	// Get Client Pb key
	// Encrypt my Pb key with Client Pb key
	// Send my encrypted Pb key to Client

	// Client is ready to send me encrypted message - first message is my Pb key encrypted with his Pb key
	// I decrypt the message with my Prv key
	// It is the handshake





	system("pause");
	WSACleanup();





	return 0;
}