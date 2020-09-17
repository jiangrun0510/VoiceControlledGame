
// VoiceRecoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VoiceReco.h"
#include "VoiceRecoDlg.h"
#include "afxdialogex.h"

#include "SREngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CVoiceRecoDlg 对话框



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


// CVoiceRecoDlg 消息处理程序

BOOL CVoiceRecoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	mSREngine.Initialize(GetSafeHwnd(), WM_RECOEVENT);
	mSREngine.LoadCmdFromXml(TEXT("CmdCtrl.xml"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVoiceRecoDlg::OnPaint()
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
		mSREngine.mSpVoice->Speak(L"语音失败失败!", SPF_ASYNC, NULL);
		//AfxMessageBox(TEXT(""));
	}
	else if (event.eEventId == SPEI_RECOGNITION)
	{
		CComPtr<ISpRecoResult> lSpRecoResult = event.RecoResult();

		CSpDynamicString resultStr;
		lSpRecoResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &resultStr, NULL);
		mSREngine.mSpVoice->Speak(resultStr, SPF_ASYNC, NULL);

		if (lstrcmpW(resultStr, L"前进") == 0)
		{
			SetTimer(1, 200, NULL);
		}
		else if (lstrcmpW(resultStr, L"后退") == 0)
		{
			SetTimer(2, 200, NULL);
		}
		else if (lstrcmpW(resultStr, L"停止") == 0)
		{
			KillTimer(1);
			KillTimer(2);

			keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
			keybd_event('S', 0, KEYEVENTF_KEYUP, 0);
		}
		else if (lstrcmpW(resultStr, L"冲") == 0)
		{
			keybd_event('W', 0, 0, 0);
			keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
			keybd_event('W', 0, 0, 0);
			keybd_event('W', 0, KEYEVENTF_KEYUP, 0);
		}
		else if (lstrcmpW(resultStr, L"跳跃") == 0)
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
