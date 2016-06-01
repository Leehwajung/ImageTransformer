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
	m_MaskWidth = 8;
}

CSpectrumDoc::~CSpectrumDoc()
{
	//if (m_DctData) {
	//	delete[] m_DctData;
	//}
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

	if (m_DctData) {
		delete[] m_DctData;
	}

	CDocument::OnCloseDocument();
}


// CSpectrumDoc serialization

void CSpectrumDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
		ar << m_Height << m_Width << m_MaskWidth;
		ar.Write(m_DctData, m_Width * m_Height * sizeof(double));	// ó���� ����迭�� ���Ϸ� ����
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
		CFile *infile = ar.GetFile();

		UINT length = (UINT)infile->GetLength();
		if (length == 0) {					// ���� ����� �˻���.
			AfxMessageBox(_T("���� ũ�Ⱑ 0 �Դϴ�."));
			return;
		}

		ar >> m_Height >> m_Width >> m_MaskWidth;
		if (m_DctData) {
			delete[] m_DctData;
		}

		UINT pixelDataSize = m_Height * m_Width;
		m_DctData = new double[pixelDataSize];
		ar.Read(m_DctData, pixelDataSize * sizeof(double));	// ���������� �о� m_DctData �迭�� ���� 
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

// Forward Discrete Cosine Transform
void CSpectrumDoc::forwardDiscreteCosineTransform(BYTE inputPixelData[], 
	UINT height, UINT width, UINT maskWidth /*= B_size*/)
{
	// ��� �迭 �Ҵ�
	UINT pixelDataSize = height * width;
	if (!m_DctData) {
		m_DctData = new double[pixelDataSize];
	}
	else if (pixelDataSize != m_Height * m_Width) {
		delete[] m_DctData;
		m_DctData = new double[pixelDataSize];
	}

	// ���� �Ҵ�
	m_Height = height;
	m_Width = width;
	m_MaskWidth = maskWidth;

	// ����ŷ�� ����
	//int *subPixelArr = new int[maskWidth * maskWidth];
	int subPixelArr[B_size][B_size];

	// ����ũ ���� ���� �̵�
	for (UINT n = 0; n < height; n += m_MaskWidth) {		// ���� ���� ���� ���� (Image Abscissa)
		for (UINT m = 0; m < width; m += m_MaskWidth) {		// ���� ���� ���� ���� (Image Ordinate)

			// ����ũ�� ��ġ�� ���� ����
			for (UINT mr = 0; mr < m_MaskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < m_MaskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					subPixelArr[mr][mc] = inputPixelData[(n + mr) * width + (m + mc)];
				}
			}

			// FDCT
			CImageProcessorUtil::dct8x8(subPixelArr);

			// ��� �迭�� FDCT ��� ����
			for (UINT mr = 0; mr < m_MaskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < m_MaskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					m_DctData[(n + mr) * width + (m + mc)] = subPixelArr[mr][mc];
				}
			}
		}
	}
}

// Inverse Discrete Cosine Transform
void CSpectrumDoc::inverseDiscreteCosineTransform(BYTE outputPixelData[], UINT pixelDataSize)
{
	// ��� �迭�� �Ҵ�Ǿ����� �ʰų� ũ�Ⱑ ��ġ���� ������ ����
	if (!outputPixelData || pixelDataSize != m_Height * m_Width) {
		return;
	}

	// ����ŷ�� ����
	//int *subPixelArr = new int[maskWidth * maskWidth];
	int subPixelArr[B_size][B_size];

	// ����ũ ���� ���� �̵�
	for (UINT n = 0; n < m_Height; n += m_MaskWidth) {		// ���� ���� ���� ���� (Image Abscissa)
		for (UINT m = 0; m < m_Width; m += m_MaskWidth) {	// ���� ���� ���� ���� (Image Ordinate)

			// ����ũ�� ��ġ�� ���� ����
			for (UINT mr = 0; mr < m_MaskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < m_MaskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					subPixelArr[mr][mc] = (int)m_DctData[(n + mr) * m_Width + (m + mc)];
				}
			}

			// IDCT
			CImageProcessorUtil::idct8x8(subPixelArr);

			// ��� �迭�� IDCT ��� ����
			for (UINT mr = 0; mr < m_MaskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < m_MaskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					outputPixelData[(n + mr) * m_Width + (m + mc)] = subPixelArr[mr][mc];
				}
			}
		}
	}
}

