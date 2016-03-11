
// iOICQCltDlg.h : ͷ�ļ�
//

#pragma once
#include "stdafx.h"

// CiOICQCltDlg �Ի���
class CiOICQCltDlg : public CDialogEx
{
// ����
public:
	CiOICQCltDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IOICQCLT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// �����Ӵ��ڿؼ���һ����״̬����һ�����б��
	CStatusBarCtrl m_bar;
	CListBox m_listInfo;
	std::vector<std::string> localIPList;
	Host me;
	// �����׽��־��
	SOCKET m_socket;

	std::string server;
	int port;

	sockaddr_in sin;


	// �ͻ������б�
	//SOCKET m_arClient[MAX_SOCKET];	// �׽�������
	//std::vector<SOCKET> socketList;
	//int m_nClient;			// ��������Ĵ�С
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// ���ɵ���Ϣӳ�亯��
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
