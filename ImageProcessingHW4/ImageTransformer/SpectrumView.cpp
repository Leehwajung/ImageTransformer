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

// SpectrumView.cpp : CSpectrumView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSpectrumView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CSpectrumView ����/�Ҹ�

CSpectrumView::CSpectrumView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

	m_ScaledData = NULL;
	m_bCreatedDataFirsthand = FALSE;
}

CSpectrumView::~CSpectrumView()
{
	deleteScaledData();
}

BOOL CSpectrumView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CSpectrumView �׸���

void CSpectrumView::OnDraw(CDC* pDC)
{
	Graphics graphicsDC(*pDC);					// gdi+ �׸��⸦ ���� ��ü
	CRect clientRect;
	GetClientRect(clientRect);
	Bitmap bmpCanvas(clientRect.right, clientRect.bottom);	// ĵ���� ��Ʈ�� ����
	Graphics graphicsCanvas(&bmpCanvas);		// ĵ���� �׷��Ƚ� ����
	graphicsCanvas.Clear(Color::Azure);			// ĵ���� ���� ����

	CSpectrumDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	scaleData();
	if (m_bitmap) {
		graphicsCanvas.DrawImage(m_bitmap, 0, 0, m_bitmap->GetWidth(), m_bitmap->GetHeight());
	}
	graphicsDC.DrawImage(&bmpCanvas, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);	// ĵ���� �׸���
}


// CSpectrumView �μ�

void CSpectrumView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSpectrumView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CSpectrumView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CSpectrumView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CSpectrumView ����

#ifdef _DEBUG
void CSpectrumView::AssertValid() const
{
	CView::AssertValid();
}

void CSpectrumView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSpectrumDoc* CSpectrumView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpectrumDoc)));
	return (CSpectrumDoc*)m_pDocument;
}
#endif //_DEBUG


// CSpectrumView Ư��

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


// CSpectrumView �۾�

void CSpectrumView::scaleData()
{
	CSpectrumDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	const UINT width = pDoc->m_Width;
	const UINT height = pDoc->m_Height;
	const UINT pixelDataSize = width * height;

	// BMP ������ ����
	BITMAPINFO* info = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
	
	// Info Header (BITMAPINFOHEADER)
	int rwsize = (((width)+31) / 32 * 4);	// 4����Ʈ�� ������� ��
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

	// ��Ʈ�� ����
	m_bitmap = Bitmap::FromBITMAPINFO(info, m_ScaledData);
	m_bitmap->RotateFlip(RotateFlipType::RotateNoneFlipY);

	delete[] (BYTE*)info;
}


// CSpectrumView �޽��� ó����


BOOL CSpectrumView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	//return CView::OnEraseBkgnd(pDC);
	return FALSE;
}
