
// iOICQClt.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CiOICQCltApp: 
// �йش����ʵ�֣������ iOICQClt.cpp
//

class CiOICQCltApp : public CWinApp
{
public:
	CiOICQCltApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CiOICQCltApp theApp;