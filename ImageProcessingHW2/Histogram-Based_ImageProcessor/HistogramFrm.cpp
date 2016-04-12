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

#include "HistogramDoc.h"
#include "BMPDoc.h"

#include "MainFrm.h"


// CHistogramFrame

IMPLEMENT_DYNCREATE(CHistogramFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CHistogramFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CHistogramFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CHistogramFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHistogramFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CHistogramFrame::OnUpdateFilePrintPreview)
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

	cs.style &= ~WS_MAXIMIZEBOX;
	cs.cx = 256;
	cs.cy = 512;

	//CDocument* pDoc = GetActiveDocument();
	//if (pDoc->IsKindOf(RUNTIME_CLASS(CHistogramDoc))) {
	//	CHistogramDoc* pBMPDoc = (CHistogramDoc*)pDoc;
	//	//cs.cx = ;
	//	//cs.cy = ;
	//}
	//else if (pDoc->IsKindOf(RUNTIME_CLASS(CBMPDoc))) {
	//	CBMPDoc* pBMPDoc = (CBMPDoc*)pDoc;
	//	cs.cx = pBMPDoc->m_bitmap->GetWidth();
	//	cs.cy = pBMPDoc->m_bitmap->GetHeight();
	//}


		//CBMPDoc* pBMPDoc = (CBMPDoc*)GetActiveDocument();
		//cs.cx = pBMPDoc->m_bitmap->GetWidth();
		//cs.cy = pBMPDoc->m_bitmap->GetHeight();

	//void CHistogramFrame::OnSize(UINT nType, int cx, int cy)
	//{
	//	CBMPDoc* pBMPDoc = (CBMPDoc*)GetActiveDocument();
	//	if (pBMPDoc) {
	//		cx = pBMPDoc->m_bitmap->GetWidth();
	//		cy = pBMPDoc->m_bitmap->GetHeight();
	//	}

	//	CMDIChildWndEx::OnSize(nType, cx, cy);

	//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//}


	//void CHistogramFrame::OnSizing(UINT fwSide, LPRECT pRect)
	//{
	//	CBMPDoc* pBMPDoc = (CBMPDoc*)GetActiveDocument();
	//	if (pBMPDoc) {
	//		pRect->left = 0;
	//		pRect->top = 0;
	//		pRect->right = pBMPDoc->m_bitmap->GetWidth();
	//		pRect->bottom = pBMPDoc->m_bitmap->GetHeight();
	//	}

	//	CMDIChildWndEx::OnSizing(fwSide, pRect);

	//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//}


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

	CDocument* pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	

	CMFCRibbonBar* rBar = ((CMainFrame*)GetTopLevelFrame())->GetRibbonBar();
	rBar->ShowContextCategories(ID_IMAGEPROCESSING, FALSE);

	// 이후 반드시 호출
	rBar->RecalcLayout();
	rBar->RedrawWindow();

	SendMessage(WM_NCPAINT, 0, 0);

	CMDIChildWndEx::ActivateFrame(nCmdShow);
}
