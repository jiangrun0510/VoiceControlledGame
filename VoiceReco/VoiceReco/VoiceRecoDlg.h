
// VoiceRecoDlg.h : ͷ�ļ�
//

#pragma once
#include "SREngine.h"


// CVoiceRecoDlg �Ի���
class CVoiceRecoDlg : public CDialogEx
{
// ����
public:
	CVoiceRecoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VOICERECO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
