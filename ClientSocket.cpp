// ClientSocket.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
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


void receiveMessages(SOCKET clientSocket) {
	while (true) {
		char buffer[DEFAULT_BUFLEN];
		int result = recv(clientSocket, buffer, DEFAULT_BUFLEN - 1, 0);

		if (result > 0) {
			buffer[result] = '\0';
			cout << "Server: " << buffer << endl;
			if (strcmp(buffer, "exit") == 0)
				break;
		}
		else if (result == 0) {
			cout << "Server: Connection closed" << endl;
			break;
		}
		else {
			cout << "Server: Error while receiving message: " << WSAGetLastError() << endl;
			break;
		}
	}
}

void sendMessages(SOCKET clientSocket) {
	string message;
	while (true) {
		getline(cin, message);
		int totalSent = 0;
		int messageLength = message.length();
		while (totalSent < messageLength) {
			int sent = send(clientSocket, message.c_str() + totalSent, messageLength - totalSent, 0);
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
	cout << "This is a client program made in order to connect to the server in this local machine.\n";
	cout << "1. To connect to the server << \n2. To quit the program\n >";
	int choice;
	cin >> choice;
	if (choice == 1) {
		std::cout << "Connecting to the server...\n";
	}
	else if (choice == 2) {
		return 0;
	}


	//cout << "======================= \n";
	//cout << "Client code !\n";
	//cout << "======================= \n";

	SOCKET clientSocket;
	int port = 55555;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0) {
		cout << "Client: The Winsock dll not found!\n";
		return 0;
	}
	else {
		/*cout << "Client: The Winsock2 dll found\n";
		cout << "Client: The status: " << wsaData.szSystemStatus << endl;*/
		cout << " [##";

	}


	//cout << "======================= \n";
	//cout << "Create a socket !\n";
	//cout << "======================= \n";

	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		cout << "Client: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else {
		//cout << "Client: socket() is OK!\n";
		cout << "##";
	}

	//cout << "======================= \n";
	//cout << "Bind the socket !\n";
	//cout << "======================= \n";

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		cout << "Client: Failed to connect\n";
		WSACleanup();
		return 0;
	}
	else {
/*		cout << "Client: connect() is OK!\n";
		cout << "Client: Can start sending and receiving data...\n"*/;
		cout << "##] Connected to the server" << endl;
	}


	std::thread receiveThread(receiveMessages, clientSocket);
	std::thread sendThread(sendMessages, clientSocket);
	receiveThread.join();
	sendThread.join();




	// generate my Prv and Pb key
	// Get server's Pb key
	// Encrypt my Pb key with server's Pb key and send it to the server
	// Receive the encrypted message from the server
	// Decrypt the message with my Prv key
	// Get the 



	system("pause");
	WSACleanup();

	return 0;
}