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

// SpectrumDoc.cpp : CSpectrumDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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


// CSpectrumDoc 생성/소멸

CSpectrumDoc::CSpectrumDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

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

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

BOOL CSpectrumDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return TRUE;
}

BOOL CSpectrumDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDocument::OnSaveDocument(lpszPathName);
}

void CSpectrumDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

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
		// TODO: 여기에 저장 코드를 추가합니다.
		ar << m_Height << m_Width << m_MaskWidth;
		ar.Write(m_DctData, m_Width * m_Height * sizeof(double));	// 처리된 영상배열을 파일로 저장
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		CFile *infile = ar.GetFile();

		UINT length = (UINT)infile->GetLength();
		if (length == 0) {					// 파일 사이즈를 검사함.
			AfxMessageBox(_T("파일 크기가 0 입니다."));
			return;
		}

		ar >> m_Height >> m_Width >> m_MaskWidth;
		if (m_DctData) {
			delete[] m_DctData;
		}

		UINT pixelDataSize = m_Height * m_Width;
		m_DctData = new double[pixelDataSize];
		ar.Read(m_DctData, pixelDataSize * sizeof(double));	// 영상파일을 읽어 m_DctData 배열에 저장 
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CSpectrumDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void CSpectrumDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CSpectrumDoc 진단

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


// CSpectrumDoc 명령

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
	m_MaskWidth = maskWidth;

	//int *subPixelArr = new int[maskWidth * maskWidth];
	int subPixelArr[B_size][B_size];

	for (UINT n = 0; n < height; n += m_MaskWidth) {		// 영상 세로 방향 루프 (Image Abscissa)
		for (UINT m = 0; m < width; m += m_MaskWidth) {	// 영상 가로 방향 루프 (Image Ordinate)
			for (UINT mr = 0; mr < m_MaskWidth; mr++) {		// 마스크 세로 방향 루프 (Mask Row)
				for (UINT mc = 0; mc < m_MaskWidth; mc++) {	// 마스크 가로 방향 루프 (Mask Column)
					subPixelArr[mr][mc] = inputPixelData[(n + mr) * width + (m + mc)];
				}
			}

			CImageProcessorUtil::dct8x8(subPixelArr);

			for (UINT mr = 0; mr < m_MaskWidth; mr++) {		// 마스크 세로 방향 루프 (Mask Row)
				for (UINT mc = 0; mc < m_MaskWidth; mc++) {	// 마스크 가로 방향 루프 (Mask Column)
					m_DctData[(n + mr) * width + (m + mc)] = subPixelArr[mr][mc];
				}
			}
		}
	}
}

void CSpectrumDoc::inverseDiscreteCosineTransform(BYTE outputPixelData[], UINT pixelDataSize)
{
	if (!outputPixelData || pixelDataSize != m_Height * m_Width) {
		return;
	}

	//int *subPixelArr = new int[maskWidth * maskWidth];
	int subPixelArr[B_size][B_size];

	for (UINT n = 0; n < m_Height; n += m_MaskWidth) {	// 영상 세로 방향 루프 (Image Abscissa)
		for (UINT m = 0; m < m_Width; m += m_MaskWidth) {	// 영상 가로 방향 루프 (Image Ordinate)
			for (UINT mr = 0; mr < m_MaskWidth; mr++) {		// 마스크 세로 방향 루프 (Mask Row)
				for (UINT mc = 0; mc < m_MaskWidth; mc++) {	// 마스크 가로 방향 루프 (Mask Column)
					subPixelArr[mr][mc] = (int)m_DctData[(n + mr) * m_Width + (m + mc)];
				}
			}

			CImageProcessorUtil::idct8x8(subPixelArr);

			for (UINT mr = 0; mr < m_MaskWidth; mr++) {		// 마스크 세로 방향 루프 (Mask Row)
				for (UINT mc = 0; mc < m_MaskWidth; mc++) {	// 마스크 가로 방향 루프 (Mask Column)
					outputPixelData[(n + mr) * m_Width + (m + mc)] = subPixelArr[mr][mc];
				}
			}
		}
	}
}

