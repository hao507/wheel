#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include "socket.h"

#if 0
#include <winsock2.h>
//#include <WinSock2.h>
//#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")          //add ws2_32.lib  

int server()
{
#include <winsock2.h>  
#include <iostream>  

#include <string.h>  
	using namespace std;

#pragma comment(lib, "ws2_32.lib")          //add ws2_32.lib  


	const int DEFAULT_PORT = 8000;
	WORD    wVersionRequested;
	WSADATA wsaData;
	int     err, iLen;
	wVersionRequested = MAKEWORD(2, 2);//create 16bit data  
									   //(1)Load WinSock  
	err = WSAStartup(wVersionRequested, &wsaData); //load win socket  
	if (err != 0)
	{
		cout << "Load WinSock Failed!";
		return -1;
	}
	//(2)create socket  
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	if (sockSrv == INVALID_SOCKET) {
		cout << "socket() fail:" << WSAGetLastError() << endl;
		return -2;
	}
	//(3)server IP  
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.s_addr = htonl(INADDR_ANY);//Auto IP, byte sequence change  
	addrSrv.sin_port = htons(DEFAULT_PORT);
	//(4)bind  
	err = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (err != 0)
	{

		cout << "bind()fail" << WSAGetLastError() << endl;
		return -3;
	}
	//  
	//(5)listen  
	err = listen(sockSrv, 5);
	if (err != 0)
	{

		cout << "listen()fail" << WSAGetLastError() << endl;
		return -4;
	}
	cout << "Server waitting...:" << endl;
	//(6)client ip  
	SOCKADDR_IN addrClt;
	int len = sizeof(SOCKADDR);

	SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClt, &len);
	char buf[1024];
	while (1)
	{
		iLen = recv(sockConn, buf, 1024, 0);
		if (iLen == 0)
			break;
		buf[iLen] = '\0';
		cout << buf;
	}
	return 0;

	while (1)
	{
		//(7)accept  
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClt, &len);
		char sendBuf[1024], hostname[100];
		if (gethostname(hostname, 100) != 0)    //get host name  
			strcpy(hostname, "None");
		//sprintf(sendBuf, "Welecome %s connected to %s!", inet_ntoa(addrClt.sin_addr), hostname);
		//(8)send recv  
		err = send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);

		char recvBuf[1024] = "\0";
		iLen = recv(sockConn, recvBuf, 1024, 0);

		recvBuf[iLen] = '\0';
		cout << recvBuf << endl;
		//(9)close connected sock  
		//closesocket(sockConn);
	}
	//(10)close server sock  
	closesocket(sockSrv);
	//(11)clean up winsock  
	WSACleanup();
	return 0;
}

int client()
{
	WORD    wVersionRequested;
	WSADATA wsaData;
	int     err, iLen;
	wVersionRequested = MAKEWORD(2, 2);//create 16bit data  
									   //(1)Load WinSock  
	err = WSAStartup(wVersionRequested, &wsaData); //load win socket  
	if (err != 0)
	{
		cout << "Load WinSock Failed!";
		return -1;
	}

	//(2)create socket  
	SOCKET sockClt = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClt == INVALID_SOCKET) {
		cout << "socket() fail:" << WSAGetLastError() << endl;
		return -2;
	}

	//(3)IP  
	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	//inet_pton(AF_INET, "127.0.0.1", &addrSrv);
	//addrSrv.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrSrv.sin_port = htons(9999);

	//(5)connect  
	err = connect(sockClt, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	if (err == INVALID_SOCKET)
	{
		cout << "connect() fail" << WSAGetLastError() << endl;
		return -1;
	}

	char sendBuf[1024], hostname[100];
	if (gethostname(hostname, 100) != 0)    //get host name  
		strcpy(hostname, "None");
	strcpy(sendBuf, hostname);
	strcat(sendBuf, "have connected to you!");
	err = send(sockClt, sendBuf, strlen(sendBuf) + 1, 0);

	char recvBuf[1024] = "\0";
	iLen = recv(sockClt, recvBuf, 1024, 0);

	if (iLen == 0)
		return -3;
	else if (iLen == SOCKET_ERROR) {
		cout << "recv() fail:" << WSAGetLastError() << endl;
		return -4;
	}
	else
	{
		recvBuf[iLen] = '\0';
		cout << recvBuf << endl;
	}
	closesocket(sockClt);

	WSACleanup();
	system("PAUSE");
	return 0;


	system("pause");
	return EXIT_SUCCESS;


	system("pause");
	return EXIT_SUCCESS;
}

#endif

void pexit(const char* err)
{
	printf("error: %s\n", err);
	exit(1);
}

void server()
{

	WORD    wVersionRequested;
	WSADATA wsaData;
	int     err, iLen;
	wVersionRequested = MAKEWORD(2, 2);//create 16bit data  
									   //(1)Load WinSock  
	err = WSAStartup(wVersionRequested, &wsaData); //load win socket  

	socket_t sock = Socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		pexit("Socket");

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = -1;

	ret = Bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	if (ret != 0)
		pexit("Bind");

	ret = Listen(sock, 128);
	if (ret != 0)
		pexit("Listen");

	socket_t csock = Accept(sock, NULL, NULL);
	if (csock < 0)
		pexit("Socket");

	char buf[BUFSIZ] = { 0 };

	while (1)
	{
		memset(buf, 0, BUFSIZ);
		int len = recv(csock, buf, BUFSIZ, 0);
		if (len == 0 || len < 0)
		{
			printf("len = %d\n", len);
			break;
		}
		for (int i = 0; i < len; i++)
		{
			if (buf[i] >= 'a' && buf[i] <= 'z')
			{
				buf[i] -= 'a' - 'A';
			}
		}
		printf("%s", buf);
		send(csock, buf, len, 0);
	}
}

int main2()
{
	server();

}