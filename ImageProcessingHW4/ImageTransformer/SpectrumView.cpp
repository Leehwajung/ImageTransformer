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

// SpectrumView.cpp : CSpectrumView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTransformer.h"
#endif

#include "SpectrumDoc.h"
#include "SpectrumView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ImageProcessorUtil.h"
#include "ImageProcessorUtilGeneric.cpp"


// CSpectrumView

IMPLEMENT_DYNCREATE(CSpectrumView, CView)

BEGIN_MESSAGE_MAP(CSpectrumView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSpectrumView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CSpectrumView 생성/소멸

CSpectrumView::CSpectrumView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

	m_ScaledData = NULL;
	m_bCreatedDataFirsthand = FALSE;
}

CSpectrumView::~CSpectrumView()
{
	deleteScaledData();
}

BOOL CSpectrumView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CSpectrumView 그리기

void CSpectrumView::OnDraw(CDC* pDC)
{
	Graphics graphicsDC(*pDC);					// gdi+ 그리기를 위한 객체
	CRect clientRect;
	GetClientRect(clientRect);
	Bitmap bmpCanvas(clientRect.right, clientRect.bottom);	// 캔버스 비트맵 생성
	Graphics graphicsCanvas(&bmpCanvas);		// 캔버스 그래픽스 생성
	graphicsCanvas.Clear(Color::Azure);			// 캔버스 배경색 지정

	CSpectrumDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 그리기 코드를 추가합니다.
	scaleData();
	if (m_bitmap) {
		graphicsCanvas.DrawImage(m_bitmap, 0, 0, m_bitmap->GetWidth(), m_bitmap->GetHeight());
	}
	graphicsDC.DrawImage(&bmpCanvas, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);	// 캔버스 그리기
}


// CSpectrumView 인쇄

void CSpectrumView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSpectrumView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CSpectrumView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CSpectrumView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CSpectrumView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSpectrumView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSpectrumView 진단

#ifdef _DEBUG
void CSpectrumView::AssertValid() const
{
	CView::AssertValid();
}

void CSpectrumView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSpectrumDoc* CSpectrumView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpectrumDoc)));
	return (CSpectrumDoc*)m_pDocument;
}
#endif //_DEBUG


// CSpectrumView 특성

void CSpectrumView::allocScaledData(UINT length)
{
	if (m_bCreatedDataFirsthand && m_ScaledData) {
		delete[] m_ScaledData;
	}
	m_ScaledData = new BYTE[length];
	m_bCreatedDataFirsthand = TRUE;
}

void CSpectrumView::deleteScaledData()
{
	if (m_bCreatedDataFirsthand && m_ScaledData) {
		delete[] m_ScaledData;
	}
	m_ScaledData = NULL;
	m_bCreatedDataFirsthand = FALSE;
}


// CSpectrumView 작업

void CSpectrumView::scaleData()
{
	CSpectrumDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	const UINT width = pDoc->m_Width;
	const UINT height = pDoc->m_Height;
	const UINT pixelDataSize = width * height;

	// BMP 데이터 생성
	BITMAPINFO* info = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
	
	// Info Header (BITMAPINFOHEADER)
	int rwsize = (((width)+31) / 32 * 4);	// 4바이트의 배수여야 함
	info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info->bmiHeader.biWidth = width;
	info->bmiHeader.biHeight = height;
	info->bmiHeader.biPlanes = 1;
	info->bmiHeader.biBitCount = 8;
	info->bmiHeader.biCompression = BI_RGB;
	info->bmiHeader.biSizeImage = (DWORD)rwsize * (DWORD)height * sizeof(BYTE);
	info->bmiHeader.biXPelsPerMeter = 0;
	info->bmiHeader.biYPelsPerMeter = 0;
	info->bmiHeader.biClrUsed = 256;
	info->bmiHeader.biClrImportant = 256;

	// Palette (RGBQUAD[256])
	for (int i = 0; i < 256; i++) {			// Palette number is 256
		info->bmiColors[i].rgbRed = info->bmiColors[i].rgbGreen = info->bmiColors[i].rgbBlue = i;
		info->bmiColors[i].rgbReserved = 0;
	}

	// Pixel Data + Scaling
	allocScaledData(pixelDataSize);
	CImageProcessorUtil::stretchContrast(pDoc->m_DctData, pixelDataSize, m_ScaledData);

	// 비트맵 생성
	m_bitmap = Bitmap::FromBITMAPINFO(info, m_ScaledData);
	m_bitmap->RotateFlip(RotateFlipType::RotateNoneFlipY);

	delete[] (BYTE*)info;
}


// CSpectrumView 메시지 처리기


BOOL CSpectrumView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//return CView::OnEraseBkgnd(pDC);
	return FALSE;
}
