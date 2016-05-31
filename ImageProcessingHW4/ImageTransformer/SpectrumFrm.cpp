// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// SpectrumFrm.cpp : CSpectrumFrame Ŭ������ ����
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


// CSpectrumFrame ����/�Ҹ�

CSpectrumFrame::CSpectrumFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.

	m_TransformMaskWidth = 8;
}

CSpectrumFrame::~CSpectrumFrame()
{
}


BOOL CSpectrumFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.cx = 20;
	cs.cy = 44;

	return TRUE;
}

// CSpectrumFrame ����

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


// CSpectrumFrame �޽��� ó����

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
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // �μ� �̸� ���� ��带 �ݽ��ϴ�.
	}
}

void CSpectrumFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}

void CSpectrumFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMDIChildWndEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// Activate/Inactivate ribbon context category: ����ó��
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	lpMMI->ptMinTrackSize.x = m_InitW;
	lpMMI->ptMinTrackSize.y = m_InitH;
	lpMMI->ptMaxTrackSize.x = m_InitW;
	lpMMI->ptMaxTrackSize.y = m_InitH;

	CMDIChildWndEx::OnGetMinMaxInfo(lpMMI);
}

void CSpectrumFrame::OnItInverseDCT()
{
	// TODO: ���⿡ ���� ó���� �ڵ带 �߰��մϴ�.

	//// ���� CImageDoc�� ������
	//CImageDoc *pSrcDoc = GetActiveDocument();
	//ASSERT_VALID(pSrcDoc);
	//if (!pSrcDoc)
	//	return;

	//// �ű� Image ���� (CImageDoc) ���� �� ����
	//CImageFrame* pDstFrm;
	//CImageView* pDstView;
	//CImageDoc* pDstDoc;
	//Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	//// Masking and Edge Detection
	//OnItMaskWidth();
	//pDstDoc->inverseDiscreteCosineTransform(m_TransformMaskWidth);

	//// ���� ����
	//CString newTitle(PFX_TRANSFORM);
	//newTitle.Append(pSrcDoc->GetTitle());
	//pDstDoc->SetTitle(newTitle);

	//// ���� �°� �ٽ� �׸���
	//pDstFrm->ActivateFrame();
	//pDstView->Invalidate();
}

void CSpectrumFrame::OnItMaskWidth()
{
	// TODO: ���⿡ ���� ó���� �ڵ带 �߰��մϴ�.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_IT_MASK));
	if (pSpin != NULL) {
		m_TransformMaskWidth = (UINT)_wtof(pSpin->GetEditText());
	}
}