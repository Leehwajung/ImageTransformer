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

// SpectrumDoc.cpp : CSpectrumDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ImageTransformer.h"
#endif

#include "SpectrumDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "ImageProcessorUtil.h"
#include "ImageProcessorUtilGeneric.cpp"


// CSpectrumDoc

IMPLEMENT_DYNCREATE(CSpectrumDoc, CDocument)

BEGIN_MESSAGE_MAP(CSpectrumDoc, CDocument)
END_MESSAGE_MAP()


// CSpectrumDoc ����/�Ҹ�

CSpectrumDoc::CSpectrumDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

	m_DctData = NULL;
	m_Height = 0;
	m_Width = 0;
}

CSpectrumDoc::~CSpectrumDoc()
{
	if (m_DctData) {
		delete[] m_DctData;
	}
}

BOOL CSpectrumDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}

BOOL CSpectrumDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return TRUE;
}

BOOL CSpectrumDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDocument::OnSaveDocument(lpszPathName);
}

void CSpectrumDoc::OnCloseDocument()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CDocument::OnCloseDocument();
}


// CSpectrumDoc serialization

void CSpectrumDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CSpectrumDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CSpectrumDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSpectrumDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSpectrumDoc ����

#ifdef _DEBUG
void CSpectrumDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSpectrumDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSpectrumDoc ���

void CSpectrumDoc::forwardDiscreteCosineTransform(BYTE inputPixelData[], UINT height, UINT width, UINT maskWidth /*= 8*/)
{
	UINT pixelDataSize = height * width;

	if (!m_DctData) {
		m_DctData = new double[pixelDataSize];
	}
	else if (pixelDataSize != m_Height * m_Width) {
		delete[] m_DctData;
		m_DctData = new double[pixelDataSize];
	}

	m_Height = height;
	m_Width = width;

	//int *subPixelArr = new int[maskWidth * maskWidth];
	int subPixelArr[B_size][B_size];

	for (UINT n = 0; n < height; n += maskWidth) {		// ���� ���� ���� ���� (Image Abscissa)
		for (UINT m = 0; m < width; m += maskWidth) {	// ���� ���� ���� ���� (Image Ordinate)
			for (UINT mr = 0; mr < maskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					subPixelArr[mr][mc] = inputPixelData[(n + mr) * width + (m + mc)];
				}
			}

			CImageProcessorUtil::dct8x8(subPixelArr);

			for (UINT mr = 0; mr < maskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					m_DctData[(n + mr) * width + (m + mc)] = subPixelArr[mr][mc];
				}
			}
		}
	}

	//// [min, max] ������ [0, 255] �������� scaling (����ȭ)
	//CImageProcessorUtil::stretchContrast(m_DctData, pixelDataSize);

	//for (UINT i = 0; i < pixelDataSize; i++) {
	//	inputPixelData[i] = (BYTE)m_DctData[i];
	//}
}

void CSpectrumDoc::inverseDiscreteCosineTransform(BYTE outputPixelData[], UINT pixelDataSize, UINT maskWidth /*= 8*/)
{
	if (!outputPixelData || pixelDataSize != m_Height * m_Width) {
		return;
	}

	//int *subPixelArr = new int[maskWidth * maskWidth];
	int subPixelArr[B_size][B_size];

	for (UINT n = 0; n < m_Height; n += maskWidth) {	// ���� ���� ���� ���� (Image Abscissa)
		for (UINT m = 0; m < m_Width; m += maskWidth) {	// ���� ���� ���� ���� (Image Ordinate)
			for (UINT mr = 0; mr < maskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					subPixelArr[mr][mc] = (int)m_DctData[(n + mr) * m_Width + (m + mc)];
				}
			}

			CImageProcessorUtil::idct8x8(subPixelArr);

			for (UINT mr = 0; mr < maskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					outputPixelData[(n + mr) * m_Width + (m + mc)] = subPixelArr[mr][mc];
				}
			}
		}
	}

	//// [min, max] ������ [0, 255] �������� scaling (����ȭ)
	//CImageProcessorUtil::stretchContrast(m_DctData, pixelDataSize);

	//for (UINT i = 0; i < pixelDataSize; i++) {
	//	inputPixelData[i] = (BYTE)m_DctData[i];
	//}
}

