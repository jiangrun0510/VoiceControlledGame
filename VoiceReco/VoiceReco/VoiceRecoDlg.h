
// VoiceRecoDlg.h : 头文件
//

#pragma once
#include "SREngine.h"


// CVoiceRecoDlg 对话框
class CVoiceRecoDlg : public CDialogEx
{
// 构造
public:
	CVoiceRecoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VOICERECO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CSREngine mSREngine;
protected:
	afx_msg LRESULT OnRecoevent(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
