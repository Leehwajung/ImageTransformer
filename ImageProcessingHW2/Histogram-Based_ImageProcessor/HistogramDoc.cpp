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

// HistogramDoc.cpp : CHistogramDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageProcessor.h"
#endif

#include "HistogramDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHistogramDoc

IMPLEMENT_DYNCREATE(CHistogramDoc, CDocument)

BEGIN_MESSAGE_MAP(CHistogramDoc, CDocument)
END_MESSAGE_MAP()


// CHistogramDoc 생성/소멸

CHistogramDoc::CHistogramDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CHistogramDoc::~CHistogramDoc()
{
}

BOOL CHistogramDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}


// CHistogramDoc serialization

void CHistogramDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CHistogramDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CHistogramDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CHistogramDoc::SetSearchContent(const CString& value)
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


// CHistogramDoc 진단

#ifdef _DEBUG
void CHistogramDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHistogramDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHistogramDoc 명령

// 픽셀 데이터에 대한 히스토그램 생성
void CHistogramDoc::generateHistogram(BYTE pixelData[], const UINT pixelDataSize)
{
	for (int i = 0; i < HTGSIZE; i++) {
		m_HistogramData[i] = 0;					// 히스토그램 배열 초기화
	}

	for (int i = 0; i < pixelDataSize; i++) {
		if (pixelData[i] >= 0 && pixelData[i] < HTGSIZE) {
			m_HistogramData[pixelData[i]]++;	// 밝기값 빈도 수집
		}
		else {	// 잘못된 픽셀 데이터이면 종료
			return;
		}
	}
	
	// 화면 출력을 위한 히스토그램 크기 정규화
	int vmin = 1000000;
	int vmax = 0;

	for (int i = 0; i < HTGSIZE; i++) {
		if (m_HistogramData[i] <= vmin) {
			vmin = m_HistogramData[i];
		}
		if (m_HistogramData[i] >= vmax) {
			vmax = m_HistogramData[i];
		}
	}

	if (vmax == vmin) {
		return;
	}

	// 히스토그램 화면 출력 배열 구성
	for (int i = 0; i < HTGSIZE; i++) {
		for (int j = 0; j < HTGSIZE; j++) {
			m_HistogramImage[i][j] = HTG_BKGR_COLOR;	// 히스토그램 이미지 초기화
		}
	}

	float vd = (float) (vmax - vmin);
	for (int j = 0; j < HTGSIZE; j++) {
		for (int i = 0; i < (int)((m_HistogramData[j] - vmin) * 255 / vd); i++) {
			m_HistogramImage[i][j] = HTG_DATA_COLOR;
		}
	}
}


