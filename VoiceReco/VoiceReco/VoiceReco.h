
// VoiceReco.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVoiceRecoApp: 
// �йش����ʵ�֣������ VoiceReco.cpp
//

class CVoiceRecoApp : public CWinApp
{
public:
	CVoiceRecoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVoiceRecoApp theApp;