#include <stdio.h>
#include <WinSock2.h>
#include <iostream>
#include <process.h>

#pragma comment(lib, "WS2_32.lib")

#define HTTP_RESP	\
"HTTP/1.1 200 OK\r\n\
Content-Length: 255\r\n\
Content-Type: text/html\r\n\
\r\n\
<html>\
<body>\
<h1>Hello, World!</h1>\
</body>\
</html>"

unsigned int __stdcall sayHello(void *args);

int main() {

	/*初始化套接字 WSAStartup*/

	WSADATA			wsaData;
	WORD			w = MAKEWORD(2, 0);

	::WSAStartup(w, &wsaData);

	/*创建套接字*/

	SOCKET			sockServer;
	sockServer = ::socket(AF_INET, SOCK_STREAM, 0);

	/*创建套接字地址结构*/

	sockaddr_in		addrServer, addrClient;

	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(80);
	addrServer.sin_addr.S_un.S_addr = INADDR_ANY;

	int addrSize = sizeof(addrClient);

	/*绑定连接套接字与地址*/

	::bind(sockServer, (sockaddr*)&addrServer, sizeof(addrServer));

	/*侦听*/

	::listen(sockServer, 10);				// int backlog 指明了等待队列的最大长度

	printf("The server has been initiated...\r\n");

	/*创建多线程事件*/

	HANDLE h_thread;

	printf("The prepared Message is: \r\n\r\n%s\r\n\r\n", HTTP_RESP);

	while (true) {

		/*为收到的请求分配新的套接字*/

		SOCKET sockClient = accept(sockServer, (sockaddr*)&addrClient, &addrSize);
		printf("Client connected. From: %s: %d\r\n", inet_ntoa(addrClient.sin_addr), addrClient.sin_port);

		/*创建新线程*/

		_beginthreadex(nullptr, 0, sayHello, (void*)sockClient, 0, nullptr);

	}

	/*关闭套接字*/

	::closesocket(sockServer);
	::WSACleanup();

	return 0;

}

unsigned int __stdcall sayHello(void* args) {

	/*收发报文存放区*/

	char sendBuf[1024] = HTTP_RESP;
	char recvBuf[1024] = { 0 };

	/*通话套接字*/
	SOCKET Client = (SOCKET)args;

	int sendSize = -1;
	int recvSize = -1;

	do {

		/*收取客户端报文*/

		recvSize = ::recv(Client, recvBuf, sizeof(recvBuf), 0);

		if (recvSize > 0) {

			printf("The Client Message is : \r\n\r\n%s\r\n\r\n", recvBuf);

			/*向用户发送报文*/

			::send(Client, sendBuf, sizeof(sendBuf), 0);

		}
		else if (recvSize == 0) {

			printf("Client lost...\r\n\r\n");

		}
		else {

			printf("%d socket closed. Received size: %d\r\nr\\n", Client, recvSize);
			closesocket(Client);
			break;

		}

	} while (recvSize > 0);

	closesocket(Client);
	return 0;

}
