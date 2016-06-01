// ImageDocExt.cpp : ImageDoc 확장 클래스의 구현 파일입니다.
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTransformer.h"
#endif

#include "ImageDocExt.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "RAWOpenDlg.h"


IMPLEMENT_DYNCREATE(CRAWDoc, CImageDoc)

CRAWDoc::CRAWDoc()
	:CImageDoc()
{
	m_RawPixelData = NULL;
	m_bCreatedDataFirsthand = FALSE;
}

BOOL CRAWDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	if (!m_RawPixelData) {
		AfxMessageBox(_T("이미지를 불러오지 못하였습니다."));
		return FALSE;
	}

	// Raw Image 열기 설정 다이얼로그
	CRAWOpenDlg dlg;

	int result = dlg.DoModal();
	if (result != IDOK) {
		return FALSE;
	}

	const UINT width = dlg.m_RawWidth;
	const UINT height = dlg.m_RawHeight;
	const UINT depth = dlg.m_RawDepth;

	// BMP 데이터 생성 및 Palette
	BITMAPINFO* info;

	if (depth == 8) {
		info = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];

		info->bmiHeader.biClrUsed = 256;
		info->bmiHeader.biClrImportant = 256;

		// Palette (RGBQUAD[256])
		for (int i = 0; i < 256; i++) {		// Palette number is 256
			info->bmiColors[i].rgbRed = info->bmiColors[i].rgbGreen = info->bmiColors[i].rgbBlue = i;
			info->bmiColors[i].rgbReserved = 0;
		}
	}
	else {
		info = new BITMAPINFO();

		info->bmiHeader.biClrUsed = 0;
		info->bmiHeader.biClrImportant = 0;
		info->bmiColors->rgbRed = info->bmiColors->rgbGreen
			= info->bmiColors->rgbBlue = info->bmiColors->rgbReserved = 0;
	}

	// Info Header (BITMAPINFOHEADER)
	int rwsize = (((width) + 31) / 32 * 4);	// 4바이트의 배수여야 함
	info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info->bmiHeader.biWidth = width;
	info->bmiHeader.biHeight = height;
	info->bmiHeader.biPlanes = 1;
	info->bmiHeader.biBitCount = depth;
	info->bmiHeader.biCompression = BI_RGB;
	info->bmiHeader.biSizeImage = (DWORD)rwsize * (DWORD)height * sizeof(BYTE);
	info->bmiHeader.biXPelsPerMeter = 0;
	info->bmiHeader.biYPelsPerMeter = 0;

	// 비트맵 생성
	m_bitmap = Bitmap::FromBITMAPINFO(info, m_RawPixelData);
	m_bitmap->RotateFlip(RotateFlipType::RotateNoneFlipY);

	// 메모리 해제
	if (depth == 8) {
		delete[] (BYTE*)info;
	}
	else {
		delete info;
	}

	return TRUE;
}

BOOL CRAWDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	if (!m_bCreatedDataFirsthand && !m_RawPixelData) {
		BitmapData bitmapData;
		m_RawPixelData = getData(&bitmapData, ImageLockModeRead);	//영상의 픽셀 데이터를 가져옴

		clearData(&bitmapData);
		m_bCreatedDataFirsthand = FALSE;
	}

	return CDocument::OnSaveDocument(lpszPathName);
}

void CRAWDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	deleteRawPixelData();
	
	CImageDoc::OnCloseDocument();
}

#ifndef _WIN32_WCE
// CRAWDoc serialization입니다.

void CRAWDoc::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		ar.Write(m_RawPixelData, m_bitmap->GetWidth() * m_bitmap->GetHeight());	// 처리된 영상배열을 파일로 저장
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

		allocRawPixelData(length);
		ar.Read(m_RawPixelData, length);	// 영상파일을 읽어 m_RawPixelData 배열에 저장 
	}
}
#endif

void CRAWDoc::allocRawPixelData(UINT length)
{
	if (m_bCreatedDataFirsthand && m_RawPixelData) {
		delete[] m_RawPixelData;
	}
	m_RawPixelData = new BYTE[length];
	m_bCreatedDataFirsthand = TRUE;
}

void CRAWDoc::deleteRawPixelData()
{
	if (m_bCreatedDataFirsthand && m_RawPixelData) {
		delete[] m_RawPixelData;
	}
	m_RawPixelData = NULL;
	m_bCreatedDataFirsthand = FALSE;
}

