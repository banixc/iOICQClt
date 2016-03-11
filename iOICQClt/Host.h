#pragma once
#include "stdafx.h"



class Host
{
public:
	Host();
	Host(SOCKET&);
	Host(std::string, std::string);
	Host(std::string, std::string, SOCKET&);
	std::string toString();
	~Host();
	//protect:
	std::string name;
	std::string ip;
	SOCKET socket;
	//SOCKET socket;
};
