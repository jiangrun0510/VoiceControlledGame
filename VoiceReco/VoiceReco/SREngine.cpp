#include "stdafx.h"
#include "SREngine.h"


CSREngine::CSREngine()
{
	mSpReco = NULL;
	mSpRecoContext = NULL;
	mSpInput = NULL;
	mSpVoice = NULL;
	mSpRecoGrammar = NULL;
}


CSREngine::~CSREngine()
{
}


HRESULT CSREngine::Initialize(HWND hWnd, UINT uMsg)
{
	CoInitialize(NULL);

	HRESULT hr = mSpReco.CoCreateInstance(CLSID_SpInprocRecognizer);
	if (FAILED(hr))
	{
		AfxMessageBox(TEXT("语音识别接口创建失败！"));
		return hr;
	}

	mSpReco->CreateRecoContext(&mSpRecoContext);

	mSpRecoContext->SetNotifyWindowMessage(hWnd, uMsg, 0, 0);

	ULONGLONG uInterest = SPFEI(SPEI_RECOGNITION) | SPFEI(SPEI_FALSE_RECOGNITION);
	mSpRecoContext->SetInterest(uInterest, uInterest);

	SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &mSpInput);

	mSpReco->SetInput(mSpInput, TRUE);


	//语音合成
	mSpVoice.CoCreateInstance(CLSID_SpVoice);
	mSpVoice->SetRate(0);
	mSpVoice->SetVolume(100);
	mSpVoice->Speak(L"欢迎使用语音识别系统", SPF_ASYNC, NULL);

	return hr;
}


HRESULT CSREngine::LoadCmdFromXml(CString strXmlPath)
{
	HRESULT hr = mSpRecoContext->CreateGrammar(123, &mSpRecoGrammar);
	if (FAILED(hr))
	{
		AfxMessageBox(TEXT("创建文法接口失败!"));
		return hr;
	}

	mSpRecoGrammar->LoadCmdFromFile(strXmlPath, SPLO_DYNAMIC);
	if (FAILED(hr))
	{
		AfxMessageBox(TEXT("没有找到xml文件!"));
		return hr;
	}

	mSpRecoGrammar->SetRuleState(NULL, NULL, SPRS_ACTIVE);

	return hr;
}
