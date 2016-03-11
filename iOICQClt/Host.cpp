#include "stdafx.h"
#include "Host.h"

Host::Host()
{
	this->name = "Localhost";
	this->ip = "127.0.0.2";
}

Host::Host(std::string name, std::string ip)
{
	this->name = name;
	this->ip = ip;
}


Host::Host(std::string name, std::string ip, SOCKET& socket)
{
	this->name = name;
	this->ip = ip;
	this->socket = socket;
}

Host::Host(SOCKET& s)
{
	this->socket = s;
	//Host host;
	// 取得对方的IP地址和端口号（使用getpeername函数）
	// Peer对方的地址信息
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	getpeername(socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	// 转化为主机字节顺序
	int nPeerPort = ntohs(sockAddr.sin_port);
	// 转化为字符串IP
	//CString sPeerIP;
	char sPeerIP[20];
	sprintf_s(sPeerIP, "%s", inet_ntoa(sockAddr.sin_addr));
	//sPeerIP.Format(L"%s", inet_ntoa(sockAddr.sin_addr));

	this->ip = sPeerIP;


	// 取得对方的主机名称
	// 取得网络字节顺序的IP值
	DWORD dwIP = inet_addr(sPeerIP);
	// 获取主机名称，注意其中第一个参数的转化
	hostent* pHost = gethostbyaddr((LPSTR)&dwIP, 4, AF_INET);

	//char szHostName[256];

	if (pHost == NULL)
	{
		//strncpy_s(szHostName, "Unknow!", 256);
		this->name = "Unknow";
	}
	else
	{
		//strncpy_s(szHostName, pHost->h_name, 256);
		this->name = pHost->h_name;
	}

}

Host::~Host()
{
}


std::string Host::toString()
{
	return name + " [" + ip + "]";
}

