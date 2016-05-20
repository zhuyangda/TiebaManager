﻿// DefriendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "DefriendDlg.h"
#include "Defriend.h"

#include <StringHelper.h>
#include <NetworkHelper.h>
#include <MiscHelper.h>

#include <TBMAPI.h>
#include <TBMCoreConfig.h>
#include <TiebaOperate.h>


// CDefriendDlg 对话框

CString CDefriendDlg::s_startPage;
CString CDefriendDlg::s_endPage;
BOOL CDefriendDlg::s_defriendNewUsers = FALSE;


IMPLEMENT_DYNAMIC(CDefriendDlg, CNormalDlg)

CDefriendDlg::CDefriendDlg(CDefriendDlg*& pThis, CWnd* pParent /*=NULL*/) : CNormalDlg(CDefriendDlg::IDD, pParent),
	m_pThis(pThis)
{

}

#pragma region MFC
CDefriendDlg::~CDefriendDlg()
{
}

void CDefriendDlg::DoDataExchange(CDataExchange* pDX)
{
	CNormalDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_startPageEdit);
	DDX_Control(pDX, IDC_EDIT2, m_endPageEdit);
	DDX_Control(pDX, IDC_BUTTON1, m_startButton);
	DDX_Control(pDX, IDC_BUTTON2, m_stopButton);
	DDX_Control(pDX, IDC_STATIC2, m_stateStatic);
	DDX_Control(pDX, IDC_CHECK1, m_defriendNewUsersCheck);
}


BEGIN_MESSAGE_MAP(CDefriendDlg, CNormalDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CDefriendDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDefriendDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &CDefriendDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()
#pragma endregion

// CDefriendDlg 消息处理程序

// 初始化
BOOL CDefriendDlg::OnInitDialog()
{
	CNormalDlg::OnInitDialog();

	if (!g_defriend.m_stopFlag)
	{
		m_startPageEdit.EnableWindow(FALSE);
		m_endPageEdit.EnableWindow(FALSE);
		m_defriendNewUsersCheck.EnableWindow(FALSE);
		m_startButton.EnableWindow(FALSE);
		m_stopButton.EnableWindow(TRUE);
		m_stateStatic.SetWindowText(_T("拉黑中"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

#pragma region UI
// 取消
void CDefriendDlg::OnCancel()
{
	DestroyWindow();
}

// 关闭窗口
void CDefriendDlg::OnClose()
{
	DestroyWindow();
}

// 释放this
void CDefriendDlg::PostNcDestroy()
{
	CNormalDlg::PostNcDestroy();

	m_pThis = NULL;
	delete this;
}
#pragma endregion

// 拉黑新关注的
void CDefriendDlg::OnBnClickedCheck1()
{
	if (m_defriendNewUsersCheck.GetCheck())
	{
		m_startPageEdit.EnableWindow(FALSE);
		m_startPageEdit.SetWindowText(_T("1"));
	}
	else
		m_startPageEdit.EnableWindow(TRUE);
}

// 开始
void CDefriendDlg::OnBnClickedButton1()
{
	m_startPageEdit.GetWindowText(s_startPage);
	m_endPageEdit.GetWindowText(s_endPage);
	int iStartPage = _ttoi(s_startPage), iEndPage = _ttoi(s_endPage);
	if (iStartPage > iEndPage)
	{
		m_startPageEdit.SetWindowText(s_endPage);
		m_endPageEdit.SetWindowText(s_startPage);
		CString st = s_startPage;
		s_startPage = s_endPage;
		s_endPage = st;
		int t = iStartPage;
		iStartPage = iEndPage;
		iEndPage = t;
	}
	if (iStartPage < 1)
	{
		AfxMessageBox(_T("起始页数应大于0！"), MB_ICONERROR);
		return;
	}
	s_defriendNewUsers = m_defriendNewUsersCheck.GetCheck();

	m_startPageEdit.EnableWindow(FALSE);
	m_endPageEdit.EnableWindow(FALSE);
	m_defriendNewUsersCheck.EnableWindow(FALSE);
	m_startButton.EnableWindow(FALSE);
	m_stopButton.EnableWindow(TRUE);
	
	g_defriend.StartDefriend(s_startPage, s_endPage, s_defriendNewUsers);
}

// 停止
void CDefriendDlg::OnBnClickedButton2()
{
	g_defriend.StopDefriend();
}
