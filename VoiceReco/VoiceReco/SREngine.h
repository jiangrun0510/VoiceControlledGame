#pragma once

#include <atlbase.h>
#include <sapi.h>
#include <sphelper.h>
#include "atlcomcli.h"

#define WM_RECOEVENT WM_USER+100

class CSREngine
{
public:
	CSREngine();
	~CSREngine();

private:
	CComPtr<ISpRecognizer> mSpReco;
	CComPtr<ISpAudio> mSpInput;
	CComPtr<ISpRecoGrammar> mSpRecoGrammar;

public:
	HRESULT LoadCmdFromXml(CString strXmlPath);
	HRESULT Initialize(HWND hWnd, UINT uMsg);
	CComPtr<ISpRecoContext> mSpRecoContext;
	CComPtr<ISpVoice> mSpVoice;

};

