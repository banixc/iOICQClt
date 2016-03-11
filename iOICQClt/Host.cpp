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
	// ȡ�öԷ���IP��ַ�Ͷ˿ںţ�ʹ��getpeername������
	// Peer�Է��ĵ�ַ��Ϣ
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	getpeername(socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	// ת��Ϊ�����ֽ�˳��
	int nPeerPort = ntohs(sockAddr.sin_port);
	// ת��Ϊ�ַ���IP
	//CString sPeerIP;
	char sPeerIP[20];
	sprintf_s(sPeerIP, "%s", inet_ntoa(sockAddr.sin_addr));
	//sPeerIP.Format(L"%s", inet_ntoa(sockAddr.sin_addr));

	this->ip = sPeerIP;


	// ȡ�öԷ�����������
	// ȡ�������ֽ�˳���IPֵ
	DWORD dwIP = inet_addr(sPeerIP);
	// ��ȡ�������ƣ�ע�����е�һ��������ת��
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

