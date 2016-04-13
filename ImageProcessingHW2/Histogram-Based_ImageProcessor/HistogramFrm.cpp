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

// HistogramFrm.cpp : CHistogramFrame 클래스의 구현
//

#include "stdafx.h"
#include "ImageProcessor.h"

#include "HistogramFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MainFrm.h"

#include "HistogramView.h"
#include "HistogramDoc.h"


// CHistogramFrame

IMPLEMENT_DYNCREATE(CHistogramFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CHistogramFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CHistogramFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CHistogramFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHistogramFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CHistogramFrame::OnUpdateFilePrintPreview)
	ON_COMMAND(ID_VIEW_ORIGIN_SIZE, &CHistogramFrame::OnViewOriginSize)
END_MESSAGE_MAP()


// CHistogramFrame 생성/소멸

CHistogramFrame::CHistogramFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CHistogramFrame::~CHistogramFrame()
{
}


BOOL CHistogramFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.cx = 276;
	cs.cy = 299;

	return TRUE;
}


CHistogramView* CHistogramFrame::GetActiveView()
{
	return (CHistogramView*)CFrameWnd::GetActiveView();
}

CHistogramDoc* CHistogramFrame::GetActiveDocument()
{
	return (CHistogramDoc*)CFrameWnd::GetActiveDocument();
}


// CHistogramFrame 진단

#ifdef _DEBUG
void CHistogramFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CHistogramFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CHistogramFrame 메시지 처리기

void CHistogramFrame::OnFilePrint()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CHistogramFrame::OnFilePrintPreview()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 인쇄 미리 보기 모드를 닫습니다.
	}
}

void CHistogramFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}

void CHistogramFrame::ActivateFrame(int nCmdShow)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CRect winRect, cliRect;
	GetWindowRect(&winRect);
	GetClientRect(&cliRect);

	int cx = 256 + winRect.Width() - cliRect.Width() + 4;
	int cy = 256 + winRect.Height() - cliRect.Height() + 4;
	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_SHOWWINDOW);

	CMDIChildWndEx::ActivateFrame(nCmdShow);
}


void CHistogramFrame::OnViewOriginSize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ActivateFrame();
}
