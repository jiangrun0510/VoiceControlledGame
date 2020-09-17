
// VoiceRecoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VoiceReco.h"
#include "VoiceRecoDlg.h"
#include "afxdialogex.h"

#include "SREngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVoiceRecoDlg �Ի���



CVoiceRecoDlg::CVoiceRecoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVoiceRecoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVoiceRecoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVoiceRecoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_RECOEVENT, &CVoiceRecoDlg::OnRecoevent)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CVoiceRecoDlg ��Ϣ�������

BOOL CVoiceRecoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	mSREngine.Initialize(GetSafeHwnd(), WM_RECOEVENT);
	mSREngine.LoadCmdFromXml(TEXT("CmdCtrl.xml"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVoiceRecoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVoiceRecoDlg::OnPaint()
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
HCURSOR CVoiceRecoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



afx_msg LRESULT CVoiceRecoDlg::OnRecoevent(WPARAM wParam, LPARAM lParam)
{
	CSpEvent event;
	event.GetFrom(mSREngine.mSpRecoContext);

	if (event.eEventId == SPEI_FALSE_RECOGNITION)
	{
		mSREngine.mSpVoice->Speak(L"����ʧ��ʧ��!", SPF_ASYNC, NULL);
		//AfxMessageBox(TEXT(""));
	}
	else if (event.eEventId == SPEI_RECOGNITION)
	{
		CComPtr<ISpRecoResult> lSpRecoResult = event.RecoResult();

		CSpDynamicString resultStr;
		lSpRecoResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &resultStr, NULL);
		mSREngine.mSpVoice->Speak(resultStr, SPF_ASYNC, NULL);

		if (lstrcmpW(resultStr, L"ǰ��") == 0)
		{
			SetTimer(1, 200, NULL);
		}
		else if (lstrcmpW(resultStr, L"����") == 0)
		{
			SetTimer(2, 200, NULL);
		}
		else if (lstrcmpW(resultStr, L"ֹͣ") == 0)
		{
			KillTimer(1);
			KillTimer(2);

			keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
			keybd_event('S', 0, KEYEVENTF_KEYUP, 0);
		}
		else if (lstrcmpW(resultStr, L"��") == 0)
		{
			keybd_event('W', 0, 0, 0);
			keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
			keybd_event('W', 0, 0, 0);
			keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
		}
		else if (lstrcmpW(resultStr, L"��Ծ") == 0)
		{
			keybd_event(VK_SPACE, 0, 0, 0);
			keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
		}
	}

	return 0;
}


void CVoiceRecoDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		keybd_event('W', 0, 0, 0);
	}
	else if (nIDEvent == 2)
	{
		keybd_event('S', 0, 0, 0);
	}
}
