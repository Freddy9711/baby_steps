#include <stdio.h>
#include <WinSock2.h>
#include <iostream>

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

int main() {

	/*初始化套接字 WSAStartup*/

	WSADATA			wsaData;
	WORD			w = MAKEWORD(2, 0);

	::WSAStartup(w, &wsaData);

	/*创建套接字*/

	SOCKET			s, s1;
	s = ::socket(AF_INET, SOCK_STREAM, 0);

	/*创建套接字地址结构*/

	sockaddr_in		addr, addr2;

	addr.sin_family				= AF_INET;
	addr.sin_port				= htons(80);
	addr.sin_addr.S_un.S_addr	= INADDR_ANY;

	int adsize2					= sizeof(addr2);

	/*绑定连接套接字与地址*/

	::bind(s, (sockaddr*)&addr, sizeof(addr));

	/*侦听*/

	::listen(s, 2);				// int backlog 指明了等待队列的最大长度

	printf("The server has been initiated...\r\n");

	/*收发字段缓存区，若预留长度不够，会导致recv出错，影响send的执行*/

	char recvbuf[1024] = { 0 };
	char sendbuf[1024] = HTTP_RESP;

	printf("The prepared Message is: \r\n\r\n%s\r\n\r\n", HTTP_RESP);

	/*为收到的请求分配新的套接字*/

	while (true) {
		s1 = ::accept(s, (sockaddr*)&addr2, &adsize2);

		if (s1 != NULL) {

			printf("%s has connected to the server...\r\n", inet_ntoa(addr2.sin_addr));

			/*接受客户端传送的报文*/

			::recv(s1, recvbuf, sizeof(recvbuf), 0);

			printf("The received Message from the client is: \r\n\r\n%s\r\n\r\n", recvbuf);

			/*传送准备好的报文*/

			::send(s1, sendbuf, sizeof(sendbuf), 0);

			printf("The Message has been send to the client. ");

		}

		/*关闭套接字*/

		::closesocket(s);
		::closesocket(s1);
		::WSACleanup();

		return 0;

	}

}
