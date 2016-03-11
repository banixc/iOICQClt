
// iOICQCltDlg.cpp : 实现文件
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
#define CON_SUCCESS L"配置成功!等待服务器回复中..."
// CiOICQCltDlg 对话框


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


// CiOICQCltDlg 消息处理程序

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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)//返回0表示成功
	{
		MessageBox(L"初始化失败！");
		return FALSE;
	}



	m_bar.Create(WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	//m_bar.SetBkColor(RGB(0xa6, 0xca, 0xf0));		// 背景色
	int arWidth[] = {300, -1 };
	m_bar.SetParts(2, arWidth);				// 分栏


	// 设置列表框控件到m_listInfo对象的关联
	m_listInfo.SubclassDlgItem(IDC_LIST, this);
	//m_listInfo.InsertString(0, L"TEST");

	// 初始化监听套节字和连接列表
	m_socket = INVALID_SOCKET;
	//m_nClient = 0;

	// 获取本机IP
	char szHost[256];
	gethostname(szHost, 256);
	// 通过本机名称取得地址信息
	HOSTENT *pHost = gethostbyname(szHost);
	for (int i = 0;; i++)
	{
		if (!pHost->h_addr_list[i])//rendb add
			break;
		std::string str = inet_ntoa(*(IN_ADDR*)(pHost->h_addr_list[i]));
		localIPList.push_back(str);
	}

	// 显示本机IP
	std::string localhostIP = "";
	for (unsigned int i = 0; i < localIPList.size(); i++)
	{
		localhostIP += localIPList[i];
		localhostIP += "|";
	}
	localhostIP[localhostIP.size() - 1] = '\0';
	m_bar.SetText(CString(L"本机IP: ") + CString(localhostIP.c_str()), 0, 0);

	me = Host(szHost, localIPList[0]);

	//OnBnClickedStatus();
	//GetDlgItem(IDC_LIST)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_SERVER)->SetWindowText(L"127.0.0.1");
	GetDlgItem(IDC_PORT)->SetWindowText(L"9999");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CiOICQCltDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	if (m_socket == INVALID_SOCKET)  // 开启服务
	{
		m_bar.SetText(L"TCP连接中...", 1, 0);

		// 取得服务器地址
		CString sAddr;
		GetDlgItem(IDC_SERVER)->GetWindowText(sAddr);
		if (sAddr.IsEmpty())
		{
			MessageBox(L"请输入服务器地址！");
			return;
		}
		char * lp = WideChar2MBCS(sAddr);
		server = std::string(lp);
		delete lp;

		// 取得端口号
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);

		lp = WideChar2MBCS(sPort);
		port = atoi(lp);
		delete lp;
		if (port < 1 || port > 65535)
		{
			MessageBox(L"端口号错误！");
			return;
		}

		GetDlgItem(IDC_STATUS)->SetWindowText(L"取消");
		GetDlgItem(IDC_TCP)->EnableWindow(FALSE);
		GetDlgItem(IDC_UDP)->EnableWindow(FALSE);
		GetDlgItem(IDC_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORT)->EnableWindow(FALSE);

		if (tcp_udp == 0)	//TCP
		{
			// 创建套节字
			m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (m_socket == INVALID_SOCKET)
			{
				return;
			}

			// 设置socket为窗口通知消息类型
			WSAAsyncSelect(m_socket, m_hWnd,
				WM_SOCKET, FD_CONNECT | FD_CLOSE | FD_WRITE | FD_READ);

			// 假定szAddr是IP地址
			ULONG uAddr = inet_addr(server.c_str());
			if (uAddr == INADDR_NONE)
			{
				// 不是IP地址，就认为这是主机名称
				// 从主机名取得IP地址
				hostent* pHost = gethostbyname(server.c_str());
				if (pHost == NULL)
				{
					::closesocket(m_socket);
					m_socket = INVALID_SOCKET;
					return;
				}
				// 得到以网络字节顺序排列的IP地址
				uAddr = ((struct in_addr*)*(pHost->h_addr_list))->s_addr;
			}

			// 填写服务器地址信息
			sockaddr_in remote;
			remote.sin_addr.S_un.S_addr = uAddr;
			remote.sin_family = AF_INET;
			remote.sin_port = htons(port);

			// 连接到远程机
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

			sendto(m_socket, "正在尝试连接到服务器", strlen("正在尝试连接到服务器"), 0, (sockaddr *) &sin, (int) sizeof(sin));

			GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_SEND)->EnableWindow(FALSE);

			m_bar.SetText(CON_SUCCESS, 1, 0);


		}
	

	}
	else				// 停止服务
	{
		// 关闭所有连接
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;

		// 设置相关子窗口控件状态
		GetDlgItem(IDC_STATUS)->SetWindowText(L"连接");
		GetDlgItem(IDC_TCP)->EnableWindow(TRUE);
		GetDlgItem(IDC_UDP)->EnableWindow(TRUE);
		GetDlgItem(IDC_SERVER)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
		m_bar.SetText(L"未连接", 1, 0);
		GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);

	}



}



afx_msg long CiOICQCltDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	// 取得有事件发生的套节字句柄
	SOCKET s = wParam;
	char szText[1024];
	std::stringstream ss;
	std::string sPort;

	// 查看是否出错
	if (WSAGETSELECTERROR(lParam))
	{
		if (m_socket != SOCKET_ERROR)
		{
			OnBnClickedStatus();
			m_bar.SetText(L"连接出错！", 1, 0);
		}
		
		return 0;
	}
	// 处理发生的事件
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:	// 套节字正确的连接到服务器
	
		// 设置用户界面
		GetDlgItem(IDC_STATUS)->SetWindowText(L"断开连接");

		GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SEND)->EnableWindow(FALSE);


		ss << port;
		sPort = ss.str();

		m_bar.SetText(CString((std::string("[TCP]已经连接到服务器[") + server + "]:" + sPort).c_str()), 1, 0);
	
		break;

	//case FD_OOB:
	case FD_READ:		// 套接字接受到对方发送过来的数据包
		// 从服务器接受数据
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
				m_bar.SetText(CString((std::string("[UDP]已经连接到服务器[") + server + "]:" + sPort).c_str()), 1, 0);

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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

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
	// TODO:  在此添加控件通知处理程序代码
	tcp_udp = 0;

	GetDlgItem(IDC_STATUS)->SetWindowText(L"连接");


}





void CiOICQCltDlg::OnBnClickedUdp()
{
	tcp_udp = 1;

	GetDlgItem(IDC_STATUS)->SetWindowText(L"配置");


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