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

// HistogramView.cpp : CHistogramView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageAreaProcessor.h"
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHistogramView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_VIEW_HTG_SIZE, &CHistogramView::OnViewHtgSize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HTG_SIZE, &CHistogramView::OnUpdateViewHtgSize)
END_MESSAGE_MAP()


// CHistogramView 생성/소멸

CHistogramView::CHistogramView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}


// CHistogramView 그리기

void CHistogramView::OnDraw(CDC* pDC)
{
	Graphics graphicsDC(*pDC);					// gdi+ 그리기를 위한 객체
	CRect clientRect;
	GetClientRect(clientRect);
	Bitmap bmpCanvas(clientRect.right, clientRect.bottom);	// 캔버스 비트맵 생성
	Graphics graphicsCanvas(&bmpCanvas);		// 캔버스 그래픽스 생성
	graphicsCanvas.Clear(Color::Azure);			// 캔버스 배경색 지정

	CHistogramDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	//AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_VIEW_HTG_SIZE);
	UINT newHeight = (UINT)(m_Height * m_HeightRate);
	if (!newHeight) {
		newHeight = (UINT)(m_Height * 0.001);
	}
	Bitmap image(m_Width, newHeight, m_Stride, PixelFormat8bppIndexed, &m_Image[(m_Height - newHeight) * m_Width]);
	graphicsCanvas.DrawImage(&image, 0, 0, clientRect.Width(), clientRect.Height());
	
	graphicsDC.DrawImage(&bmpCanvas, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);	// 캔버스 그리기
}


// CHistogramView 인쇄


void CHistogramView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHistogramView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CHistogramView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CHistogramView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
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


// CHistogramView 진단

#ifdef _DEBUG
void CHistogramView::AssertValid() const
{
	CView::AssertValid();
}

void CHistogramView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHistogramDoc* CHistogramView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHistogramDoc)));
	return (CHistogramDoc*)m_pDocument;
}
#endif //_DEBUG


// CHistogramView 작업

// histogram의 2차원 이미지 생성
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

	// histogram 화면 출력 배열 구성
	for (UINT i = 0; i < m_Height * m_Width; i++) {
		m_Image[i] = HTG_DATA_COLOR;	// histogram 이미지 초기화
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


// CHistogramView 메시지 처리기



// CHistogramView 명령

void CHistogramView::OnViewHtgSize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMFCRibbonSlider *pSlider = DYNAMIC_DOWNCAST(CMFCRibbonSlider, 
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_VIEW_HTG_SIZE));

	// 현재 위치 가져오기
	m_HeightRate = (FLOAT)(1.0f - (DOUBLE)pSlider->GetPos() / (DOUBLE)pSlider->GetRangeMax());

	Invalidate();
}

void CHistogramView::OnUpdateViewHtgSize(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(TRUE);
}

