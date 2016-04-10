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

// ChildFrm.cpp : CChildFrame 클래스의 구현
//

#include "stdafx.h"
#include "ImageProcessor.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "HistogramDoc.h"
#include "BMPDoc.h"


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CChildFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CChildFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CChildFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CChildFrame::OnUpdateFilePrintPreview)
END_MESSAGE_MAP()

int CChildFrame::posX = 0;
int CChildFrame::posY = 0;

// CChildFrame 생성/소멸

CChildFrame::CChildFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

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

	//void CChildFrame::OnSize(UINT nType, int cx, int cy)
	//{
	//	CBMPDoc* pBMPDoc = (CBMPDoc*)GetActiveDocument();
	//	if (pBMPDoc) {
	//		cx = pBMPDoc->m_bitmap->GetWidth();
	//		cy = pBMPDoc->m_bitmap->GetHeight();
	//	}

	//	CMDIChildWndEx::OnSize(nType, cx, cy);

	//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//}


	//void CChildFrame::OnSizing(UINT fwSide, LPRECT pRect)
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

// CChildFrame 진단

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 메시지 처리기

void CChildFrame::OnFilePrint()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CChildFrame::OnFilePrintPreview()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 인쇄 미리 보기 모드를 닫습니다.
	}
}

void CChildFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}
