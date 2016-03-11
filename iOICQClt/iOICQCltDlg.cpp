
// iOICQCltDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "iOICQClt.h"
#include "iOICQCltDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define MAX_MSG 1024
//#define SERVER "127.0.0.1"
#define WM_SOCKET WM_USER + 1	
#define CON_SUCCESS L"���óɹ�!�ȴ��������ظ���..."
// CiOICQCltDlg �Ի���


LPSTR WideChar2MBCS(const CString& strCS)
{
	const UINT wLen = strCS.GetLength() + 1;
	UINT aLen = WideCharToMultiByte(CP_ACP, 0, strCS, wLen, NULL, 0, NULL, NULL);
	LPSTR lpa = new char[aLen];
	WideCharToMultiByte(CP_ACP, 0, strCS, wLen, lpa, aLen, NULL, NULL);
	return lpa;
}

void getTime(char * pc)
{
	time_t t;
	//struct tm *pt;
	//char pc[26];
	time(&t);
	ctime_s(pc, 26, &t);
	//return 
}


CiOICQCltDlg::CiOICQCltDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CiOICQCltDlg::IDD, pParent)
	, tcp_udp(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CiOICQCltDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_TCP, tcp_udp);
}

BEGIN_MESSAGE_MAP(CiOICQCltDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, &CiOICQCltDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_STATUS, &CiOICQCltDlg::OnBnClickedStatus)
	ON_MESSAGE(WM_SOCKET, &CiOICQCltDlg::OnSocket)
	ON_EN_CHANGE(IDC_EDIT, &CiOICQCltDlg::OnEnChangeEdit)
	ON_BN_CLICKED(IDC_TCP, &CiOICQCltDlg::OnBnClickedTcp)
	ON_BN_CLICKED(IDC_UDP, &CiOICQCltDlg::OnBnClickedUdp)
	ON_BN_CLICKED(IDC_CLEAR, &CiOICQCltDlg::OnBnClickedClear)
END_MESSAGE_MAP()


// CiOICQCltDlg ��Ϣ�������

BOOL CiOICQCltDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	//CString params = AfxGetApp()->m_lpCmdLine;
	//CString params = AfxGetApp()->m_lpCmdLine;

	//std::string strStl = std::string(params.GetBuffer());

	//TCHAR *psz = (TCHAR*)(LPCTSTR)params;
	//std::wstring w = std::wstring(params);

	//if (params != "")
	//{
		//server = std::string(w.begin(), w.end());

	//}
	//else
	//{
		//server = SERVER;
	//}
	//std::wstring = params

		//port = PORT;
	//ModifyStyle(m_hWnd, WS_THICKFRAME, 0, 0);

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)//����0��ʾ�ɹ�
	{
		MessageBox(L"��ʼ��ʧ�ܣ�");
		return FALSE;
	}



	m_bar.Create(WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	//m_bar.SetBkColor(RGB(0xa6, 0xca, 0xf0));		// ����ɫ
	int arWidth[] = {300, -1 };
	m_bar.SetParts(2, arWidth);				// ����


	// �����б��ؼ���m_listInfo����Ĺ���
	m_listInfo.SubclassDlgItem(IDC_LIST, this);
	//m_listInfo.InsertString(0, L"TEST");

	// ��ʼ�������׽��ֺ������б�
	m_socket = INVALID_SOCKET;
	//m_nClient = 0;

	// ��ȡ����IP
	char szHost[256];
	gethostname(szHost, 256);
	// ͨ����������ȡ�õ�ַ��Ϣ
	HOSTENT *pHost = gethostbyname(szHost);
	for (int i = 0;; i++)
	{
		if (!pHost->h_addr_list[i])//rendb add
			break;
		std::string str = inet_ntoa(*(IN_ADDR*)(pHost->h_addr_list[i]));
		localIPList.push_back(str);
	}

	// ��ʾ����IP
	std::string localhostIP = "";
	for (unsigned int i = 0; i < localIPList.size(); i++)
	{
		localhostIP += localIPList[i];
		localhostIP += "|";
	}
	localhostIP[localhostIP.size() - 1] = '\0';
	m_bar.SetText(CString(L"����IP: ") + CString(localhostIP.c_str()), 0, 0);

	me = Host(szHost, localIPList[0]);

	//OnBnClickedStatus();
	//GetDlgItem(IDC_LIST)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_SERVER)->SetWindowText(L"127.0.0.1");
	GetDlgItem(IDC_PORT)->SetWindowText(L"9999");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CiOICQCltDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CiOICQCltDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CiOICQCltDlg::OnBnClickedSend()
{
	if (!GetDlgItem(IDC_SEND)->IsWindowEnabled())return;

	CString sText;
	char time[26];
	getTime(time);


	GetDlgItem(IDC_EDIT)->GetWindowText(sText);

	LPSTR lp = WideChar2MBCS(sText);

	//if (sText.GetLength() == 0) return;

	if (tcp_udp == 0)
	{
		if (send(m_socket, lp, strlen(lp) + 1, 0) != -1)
		{
			//AddStringToList(sText, FALSE);
			GetDlgItem(IDC_EDIT)->SetWindowText(L"");


			CString str = L"[LOCALHOST][" + CString((localIPList[0].c_str())) + "][" + time + "]: " + lp;
			m_listInfo.InsertString(-1, str);


		}
	}
	else
	{
		int len = sizeof(sin);

		if (sendto(m_socket, lp, strlen(lp), 0, (sockaddr *)&sin, len) != -1)
		{


			GetDlgItem(IDC_EDIT)->SetWindowText(L"");

			CString str = L"[LOCALHOST][" + CString((localIPList[0].c_str())) + "][" + time + "]: " + lp;
			m_listInfo.InsertString(-1, str);

		}
	}

	delete lp;



}

void CiOICQCltDlg::OnCancel()
{
	closesocket(m_socket);
	CDialog::OnCancel();
}


void CiOICQCltDlg::OnBnClickedStatus()
{
	if (m_socket == INVALID_SOCKET)  // ��������
	{
		m_bar.SetText(L"TCP������...", 1, 0);

		// ȡ�÷�������ַ
		CString sAddr;
		GetDlgItem(IDC_SERVER)->GetWindowText(sAddr);
		if (sAddr.IsEmpty())
		{
			MessageBox(L"�������������ַ��");
			return;
		}
		char * lp = WideChar2MBCS(sAddr);
		server = std::string(lp);
		delete lp;

		// ȡ�ö˿ں�
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);

		lp = WideChar2MBCS(sPort);
		port = atoi(lp);
		delete lp;
		if (port < 1 || port > 65535)
		{
			MessageBox(L"�˿ںŴ���");
			return;
		}

		GetDlgItem(IDC_STATUS)->SetWindowText(L"ȡ��");
		GetDlgItem(IDC_TCP)->EnableWindow(FALSE);
		GetDlgItem(IDC_UDP)->EnableWindow(FALSE);
		GetDlgItem(IDC_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORT)->EnableWindow(FALSE);

		if (tcp_udp == 0)	//TCP
		{
			// �����׽���
			m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (m_socket == INVALID_SOCKET)
			{
				return;
			}

			// ����socketΪ����֪ͨ��Ϣ����
			WSAAsyncSelect(m_socket, m_hWnd,
				WM_SOCKET, FD_CONNECT | FD_CLOSE | FD_WRITE | FD_READ);

			// �ٶ�szAddr��IP��ַ
			ULONG uAddr = inet_addr(server.c_str());
			if (uAddr == INADDR_NONE)
			{
				// ����IP��ַ������Ϊ������������
				// ��������ȡ��IP��ַ
				hostent* pHost = gethostbyname(server.c_str());
				if (pHost == NULL)
				{
					::closesocket(m_socket);
					m_socket = INVALID_SOCKET;
					return;
				}
				// �õ��������ֽ�˳�����е�IP��ַ
				uAddr = ((struct in_addr*)*(pHost->h_addr_list))->s_addr;
			}

			// ��д��������ַ��Ϣ
			sockaddr_in remote;
			remote.sin_addr.S_un.S_addr = uAddr;
			remote.sin_family = AF_INET;
			remote.sin_port = htons(port);

			// ���ӵ�Զ�̻�
			connect(m_socket, (sockaddr*)&remote, sizeof(sockaddr));
		}
		else		//UDP
		{

			m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

			//sockaddr_in sin;
			sin.sin_family = AF_INET;
			sin.sin_port = htons(port);
			sin.sin_addr.S_un.S_addr = inet_addr(server.c_str());

			WSAAsyncSelect(m_socket, m_hWnd,
				WM_SOCKET, FD_WRITE | FD_READ);

			sendto(m_socket, "���ڳ������ӵ�������", strlen("���ڳ������ӵ�������"), 0, (sockaddr *) &sin, (int) sizeof(sin));

			GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_SEND)->EnableWindow(FALSE);

			m_bar.SetText(CON_SUCCESS, 1, 0);


		}
	

	}
	else				// ֹͣ����
	{
		// �ر���������
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;

		// ��������Ӵ��ڿؼ�״̬
		GetDlgItem(IDC_STATUS)->SetWindowText(L"����");
		GetDlgItem(IDC_TCP)->EnableWindow(TRUE);
		GetDlgItem(IDC_UDP)->EnableWindow(TRUE);
		GetDlgItem(IDC_SERVER)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
		m_bar.SetText(L"δ����", 1, 0);
		GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);

	}



}



afx_msg long CiOICQCltDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	// ȡ�����¼��������׽��־��
	SOCKET s = wParam;
	char szText[1024];
	std::stringstream ss;
	std::string sPort;

	// �鿴�Ƿ����
	if (WSAGETSELECTERROR(lParam))
	{
		if (m_socket != SOCKET_ERROR)
		{
			OnBnClickedStatus();
			m_bar.SetText(L"���ӳ���", 1, 0);
		}
		
		return 0;
	}
	// ���������¼�
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:	// �׽�����ȷ�����ӵ�������
	
		// �����û�����
		GetDlgItem(IDC_STATUS)->SetWindowText(L"�Ͽ�����");

		GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SEND)->EnableWindow(FALSE);


		ss << port;
		sPort = ss.str();

		m_bar.SetText(CString((std::string("[TCP]�Ѿ����ӵ�������[") + server + "]:" + sPort).c_str()), 1, 0);
	
		break;

	//case FD_OOB:
	case FD_READ:		// �׽��ֽ��ܵ��Է����͹��������ݰ�
		// �ӷ�������������
		//char szText[1024] = { 0 };

		if (tcp_udp == 0)
		{
			if (recv(s, szText, 1024, 0) != -1)
			{

				m_listInfo.InsertString(-1, CString(szText));
			}

		}
		else
		{
			//sockaddr_in sin;

			int len = sizeof(sin);
			if (recvfrom(s, szText, 1024, 0, (sockaddr *)&sin, &len) > 0)
			{
				ss << port;
				sPort = ss.str();
				m_bar.SetText(CString((std::string("[UDP]�Ѿ����ӵ�������[") + server + "]:" + sPort).c_str()), 1, 0);

				m_listInfo.InsertString(-1, CString(szText));
			}
		}

		break;

	case FD_CLOSE:
		OnBnClickedStatus();
		break;
	}

	return 0;
}


void CiOICQCltDlg::OnEnChangeEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	CString sText;
	GetDlgItem(IDC_EDIT)->GetWindowText(sText);
	if (sText.GetLength() < 1 || sText.GetLength() > MAX_MSG)
	{
		GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
	}
}


void CiOICQCltDlg::OnBnClickedTcp()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	tcp_udp = 0;

	GetDlgItem(IDC_STATUS)->SetWindowText(L"����");


}





void CiOICQCltDlg::OnBnClickedUdp()
{
	tcp_udp = 1;

	GetDlgItem(IDC_STATUS)->SetWindowText(L"����");


}


void CiOICQCltDlg::OnBnClickedClear()
{
	m_listInfo.ResetContent();
}


BOOL CiOICQCltDlg::PreTranslateMessage(MSG* pMsg)
{
	//if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) OnBnClickedSend();
	else
		return CDialog::PreTranslateMessage(pMsg);
}