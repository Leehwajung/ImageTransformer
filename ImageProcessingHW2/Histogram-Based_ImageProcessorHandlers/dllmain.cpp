// dllmain.cpp : DllMain의 구현입니다.

#include "stdafx.h"
#include "resource.h"
#include "HistogramBased_ImageProcessorHandlers_i.h"
#include "dllmain.h"
#include "xdlldata.h"

CHistogramBased_ImageProcessorHandlersModule _AtlModule;

class CHistogramBased_ImageProcessorHandlersApp : public CWinApp
{
public:

// 재정의
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CHistogramBased_ImageProcessorHandlersApp, CWinApp)
END_MESSAGE_MAP()

CHistogramBased_ImageProcessorHandlersApp theApp;

BOOL CHistogramBased_ImageProcessorHandlersApp::InitInstance()
{
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
	return CWinApp::InitInstance();
}

int CHistogramBased_ImageProcessorHandlersApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
