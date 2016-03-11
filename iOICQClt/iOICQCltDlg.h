
// iOICQCltDlg.h : 头文件
//

#pragma once
#include "stdafx.h"

// CiOICQCltDlg 对话框
class CiOICQCltDlg : public CDialogEx
{
// 构造
public:
	CiOICQCltDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_IOICQCLT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 两个子窗口控件，一个是状态栏，一个是列表框
	CStatusBarCtrl m_bar;
	CListBox m_listInfo;
	std::vector<std::string> localIPList;
	Host me;
	// 监听套节字句柄
	SOCKET m_socket;

	std::string server;
	int port;

	sockaddr_in sin;


	// 客户连接列表
	//SOCKET m_arClient[MAX_SOCKET];	// 套节字数组
	//std::vector<SOCKET> socketList;
	//int m_nClient;			// 上述数组的大小
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedStatus();
	afx_msg long OnSocket(WPARAM wParam, LPARAM lParam);

	afx_msg void OnEnChangeEdit();
	afx_msg void OnBnClickedTcp();
	int tcp_udp;
	afx_msg void OnBnClickedUdp();
	afx_msg void OnBnClickedClear();
};
