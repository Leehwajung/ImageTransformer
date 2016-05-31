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

#include "SpectrumView.h"
#include "SpectrumDoc.h"

#include "ImageFrm.h"
#include "ImageView.h"
#include "ImageDoc.h"

#include "MainFrm.h"


// CSpectrumFrame

IMPLEMENT_DYNCREATE(CSpectrumFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CSpectrumFrame, CMDIChildWndEx)
	ON_WM_ACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_FILE_PRINT, &CSpectrumFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CSpectrumFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSpectrumFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CSpectrumFrame::OnUpdateFilePrintPreview)
	ON_COMMAND(ID_IT_IDCT, &CSpectrumFrame::OnItInverseDCT)
	ON_COMMAND(ID_IT_MASK, &CSpectrumFrame::OnItMaskWidth)
END_MESSAGE_MAP()


// CSpectrumFrame 생성/소멸

CSpectrumFrame::CSpectrumFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.

	m_TransformMaskWidth = 8;
}

CSpectrumFrame::~CSpectrumFrame()
{
}


BOOL CSpectrumFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.cx = 20;
	cs.cy = 44;

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

void CSpectrumFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMDIChildWndEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CSpectrumView *pView = (CSpectrumView*)GetActiveView();
	ASSERT_VALID(pView);
	if (!pView)
		return;

	CSpectrumDoc *pDoc = (CSpectrumDoc*)GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect winRect, cliRect;
	GetWindowRect(&winRect);
	GetClientRect(&cliRect);

	Bitmap *pBitmap = pView->m_bitmap;
	if (pBitmap) {
		m_InitW = pBitmap->GetWidth() + winRect.Width() - cliRect.Width() + 4 - 20;
		m_InitH = pBitmap->GetHeight() + winRect.Height() - cliRect.Height() + 4;
		SetWindowPos(NULL, 0, 0, m_InitW, m_InitH, SWP_NOMOVE | SWP_SHOWWINDOW);
	}

	OnItMaskWidth();
}

BOOL CSpectrumFrame::OnNcActivate(BOOL bActive)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// Activate/Inactivate ribbon context category: 영상처리
	CMFCRibbonBar *pRibbon = ((CMainFrame*)GetTopLevelFrame())->GetRibbonBar();
	if (bActive) {
		pRibbon->ShowContextCategories(ID_IMAGETRANSFORMING, TRUE);
		pRibbon->ActivateContextCategory(ID_IMAGETRANSFORMING);
	}
	else {
		pRibbon->ShowContextCategories(ID_IMAGETRANSFORMING, FALSE);
	}
	pRibbon->RecalcLayout();
	pRibbon->RedrawWindow();

	return CMDIChildWndEx::OnNcActivate(bActive);
}

void CSpectrumFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	lpMMI->ptMinTrackSize.x = m_InitW;
	lpMMI->ptMinTrackSize.y = m_InitH;
	lpMMI->ptMaxTrackSize.x = m_InitW;
	lpMMI->ptMaxTrackSize.y = m_InitH;

	CMDIChildWndEx::OnGetMinMaxInfo(lpMMI);
}

void CSpectrumFrame::OnItInverseDCT()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	//// 기존 CImageDoc을 가져옴
	//CImageDoc *pSrcDoc = GetActiveDocument();
	//ASSERT_VALID(pSrcDoc);
	//if (!pSrcDoc)
	//	return;

	//// 신규 Image 문서 (CImageDoc) 생성 및 복제
	//CImageFrame* pDstFrm;
	//CImageView* pDstView;
	//CImageDoc* pDstDoc;
	//Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	//// Masking and Edge Detection
	//OnItMaskWidth();
	//pDstDoc->inverseDiscreteCosineTransform(m_TransformMaskWidth);

	//// 제목 변경
	//CString newTitle(PFX_TRANSFORM);
	//newTitle.Append(pSrcDoc->GetTitle());
	//pDstDoc->SetTitle(newTitle);

	//// 영상에 맞게 다시 그리기
	//pDstFrm->ActivateFrame();
	//pDstView->Invalidate();
}

void CSpectrumFrame::OnItMaskWidth()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_IT_MASK));
	if (pSpin != NULL) {
		m_TransformMaskWidth = (UINT)_wtof(pSpin->GetEditText());
	}
}