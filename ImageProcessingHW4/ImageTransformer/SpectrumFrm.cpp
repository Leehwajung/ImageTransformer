// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// SpectrumFrm.cpp : CSpectrumFrame 클래스의 구현
//

#include "stdafx.h"
#include "ImageTransformer.h"

#include "SpectrumFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpectrumFrame

IMPLEMENT_DYNCREATE(CSpectrumFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CSpectrumFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CSpectrumFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CSpectrumFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSpectrumFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CSpectrumFrame::OnUpdateFilePrintPreview)
END_MESSAGE_MAP()


// CSpectrumFrame 생성/소멸

CSpectrumFrame::CSpectrumFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CSpectrumFrame::~CSpectrumFrame()
{
}


BOOL CSpectrumFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CSpectrumFrame 진단

#ifdef _DEBUG
void CSpectrumFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CSpectrumFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG


// CSpectrumFrame 메시지 처리기

void CSpectrumFrame::OnFilePrint()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CSpectrumFrame::OnFilePrintPreview()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 인쇄 미리 보기 모드를 닫습니다.
	}
}

void CSpectrumFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}
