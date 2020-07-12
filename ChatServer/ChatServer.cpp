
#include <iostream>
using namespace std;
#include<winsock2.h>
#include<WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


int main()
{
    //1.初始化网络库
	WORD wVersionRequested;
	WSADATA wsData;
	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsData)) {
		cout << "初始化呢WSA失败" << endl;
		return 0;
	}

	//2.创建socket套接字
	SOCKET s = socket(
		AF_INET, //INET协议族
		SOCK_STREAM, //表示使用的是TCP协议
		0);
	if (s == INVALID_SOCKET) {
		cout << "创建socket失败！" << endl;
		return 0;
	}

	//3. bind/listen 绑定/监听窗口
	sockaddr_in addr;
	addr.sin_family = AF_INET; //协议族
	inet_pton(PF_INET, "0.0.0.0", &addr.sin_addr); //ip地址，0.0.0.0表示接受所有网卡
	addr.sin_port = htons(10086);  //网络字节序（大尾方式）， 本地字节序（小尾方式），需要转换
	int nRet = bind(s, (sockaddr*)&addr, sizeof(addr)); //绑定
	if (SOCKET_ERROR == nRet) {
		cout << "绑定端口失败！" << endl;
		return 0;
	}
	nRet = listen(s, 5); //监听
	if (SOCKET_ERROR == nRet) {
		cout << "监听失败" << endl;
		return 0;
	}

	//4. accept接受请求、
	else cout << "等待客户端连接。。。。" << endl;
	sockaddr_in clientAddr = { 0 };
	clientAddr.sin_family = AF_INET;
	int addrlen = sizeof(clientAddr);
	SOCKET sClient = accept(s, (sockaddr*)&clientAddr, &addrlen);
	if (sClient == INVALID_SOCKET) {
		cout << "连接客户端失败！！" << endl;
		return 0;
	}else

		cout << "有客户端连接！" <<endl;

	//5.send/recv 发送/接受消息
	while (true)
	{
		cout << "请您输入需要发送的数据：";
		char sendMsg[256] = { 0 };
		cin.getline(sendMsg, 256);
		send(sClient, sendMsg, strlen(sendMsg) + 1, 0);
		cout << "数据发送成功，等待对面回复。。。。" << endl;

		char szBuf[256] = { 0 };
		recv(sClient, szBuf, 256, 0);
		cout << "客户端：" << szBuf << endl;
	}

	//6.关闭socket
	closesocket(s);

	//7.反初始化操作
	WSACleanup();

	return 1;
}

