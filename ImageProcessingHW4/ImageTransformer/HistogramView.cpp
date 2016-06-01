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

// HistogramView.cpp : CHistogramView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ImageTransformer.h"
#endif

#include "MainFrm.h"

#include "HistogramDoc.h"
#include "HistogramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHistogramView

IMPLEMENT_DYNCREATE(CHistogramView, CView)

BEGIN_MESSAGE_MAP(CHistogramView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHistogramView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIEW_HTG_SIZE, &CHistogramView::OnViewHtgSize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HTG_SIZE, &CHistogramView::OnUpdateViewHtgSize)
END_MESSAGE_MAP()


// CHistogramView ����/�Ҹ�

CHistogramView::CHistogramView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_Image = NULL;
	m_Width = HTGSIZE;
	m_Height = 0;
	m_Stride = ((UINT)(m_Width / 4 + 0.5)) * 4;
	resetHeightRate();
}

CHistogramView::~CHistogramView()
{
	if (m_Image) {
		delete[] m_Image;
	}
}

BOOL CHistogramView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}


// CHistogramView �׸���

void CHistogramView::OnDraw(CDC* pDC)
{
	Graphics graphicsDC(*pDC);					// gdi+ �׸��⸦ ���� ��ü
	CRect clientRect;
	GetClientRect(clientRect);
	Bitmap bmpCanvas(clientRect.right, clientRect.bottom);	// ĵ���� ��Ʈ�� ����
	Graphics graphicsCanvas(&bmpCanvas);		// ĵ���� �׷��Ƚ� ����
	graphicsCanvas.Clear(Color::Azure);			// ĵ���� ���� ����

	CHistogramDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	//AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_VIEW_HTG_SIZE);
	UINT newHeight = (UINT)(m_Height * m_HeightRate);
	if (!newHeight) {
		newHeight = (UINT)(m_Height * 0.001);
	}
	Bitmap image(m_Width, newHeight, m_Stride, PixelFormat8bppIndexed, &m_Image[(m_Height - newHeight) * m_Width]);
	graphicsCanvas.DrawImage(&image, 0, 0, clientRect.Width(), clientRect.Height());
	
	graphicsDC.DrawImage(&bmpCanvas, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);	// ĵ���� �׸���
}


// CHistogramView �μ�

void CHistogramView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHistogramView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CHistogramView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CHistogramView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CHistogramView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CHistogramView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CHistogramView ����

#ifdef _DEBUG
void CHistogramView::AssertValid() const
{
	CView::AssertValid();
}

void CHistogramView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHistogramDoc* CHistogramView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHistogramDoc)));
	return (CHistogramDoc*)m_pDocument;
}
#endif //_DEBUG


// CHistogramView �۾�

// histogram�� 2���� �̹��� ����
void CHistogramView::plotHistogramImage()
{
	CHistogramDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (m_Image) {
		delete[] m_Image;
	}

	m_Height = 0;
	for (UINT i = 0; i < m_Width; i++) {
		if (pDoc->m_HistogramData[i] > m_Height) {
			m_Height = pDoc->m_HistogramData[i];
		}
	}

	m_Image = new BYTE[m_Height * m_Width];	// hight: m_Height, width: HTGSIZE

	// histogram ȭ�� ��� �迭 ����
	for (UINT i = 0; i < m_Height * m_Width; i++) {
		m_Image[i] = HTG_DATA_COLOR;	// histogram �̹��� �ʱ�ȭ
	}

	for (UINT j = 0; j < m_Width; j++) {
		for (UINT i = 0; i < m_Height - pDoc->m_HistogramData[j]; i++) {
			m_Image[i * m_Width + j] = HTG_BKGR_COLOR;
		}
	}
}

void CHistogramView::resetHeightRate()
{
	m_HeightRate = 1.0f;
}


// CHistogramView �޽��� ó����

BOOL CHistogramView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	//return CView::OnEraseBkgnd(pDC);
	return FALSE;
}


// CHistogramView ���

void CHistogramView::OnViewHtgSize()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMFCRibbonSlider *pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, 
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_VIEW_HTG_SIZE));

	// ���� ��ġ ��������
	m_HeightRate = (FLOAT)(1.0f - (DOUBLE)pSlider->GetPos() / (DOUBLE)pSlider->GetRangeMax());

	Invalidate();
}

void CHistogramView::OnUpdateViewHtgSize(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(TRUE);
}

