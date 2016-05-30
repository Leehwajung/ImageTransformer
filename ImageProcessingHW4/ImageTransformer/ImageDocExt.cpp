// ImageDocExt.cpp : ImageDoc Ȯ�� Ŭ������ ���� �����Դϴ�.
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ImageTransformer.h"
#endif

#include "ImageDocExt.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "RawOpenDlg.h"


IMPLEMENT_DYNCREATE(CRAWDoc, CImageDoc)

CRAWDoc::CRAWDoc()
	:CImageDoc()
{
	m_RawPixelData = NULL;
}

BOOL CRAWDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	if (!m_RawPixelData) {
		AfxMessageBox(_T("�̹����� �ҷ����� ���Ͽ����ϴ�."));
		return FALSE;
	}

	CRawOpenDlg dlg;

	int result = dlg.DoModal();
	if (result != IDOK) {
		return FALSE;
	}

	const UINT width = dlg.m_RawWidth;
	const UINT height = dlg.m_RawHeight;
	const UINT depth = dlg.m_RawDepth;

	BITMAPINFO* info;
	
	if (depth == 8) {
		info = (BITMAPINFO*)new BYTE[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];

		info->bmiHeader.biClrUsed = 256;
		info->bmiHeader.biClrImportant = 256;

		for (int i = 0; i < 256; i++) {		// Palette number is 256
			info->bmiColors[i].rgbRed = info->bmiColors[i].rgbGreen = info->bmiColors[i].rgbBlue = i;
			info->bmiColors[i].rgbReserved = 0;
		}
	}
	else {
		info = new BITMAPINFO();

		info->bmiHeader.biClrUsed = 0;
		info->bmiHeader.biClrImportant = 0;
	}

	int rwsize = (((width) + 31) / 32 * 4);	// 4����Ʈ�� ������� ��
	info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info->bmiHeader.biWidth = width;
	info->bmiHeader.biHeight = height;
	info->bmiHeader.biPlanes = 1;
	info->bmiHeader.biBitCount = depth;
	info->bmiHeader.biCompression = BI_RGB;
	info->bmiHeader.biSizeImage = (DWORD)rwsize * (DWORD)height;
	info->bmiHeader.biXPelsPerMeter = 0;
	info->bmiHeader.biYPelsPerMeter = 0;

	m_bitmap = Bitmap::FromBITMAPINFO(info, m_RawPixelData);
	m_bitmap->RotateFlip(RotateFlipType::RotateNoneFlipY);

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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDocument::OnSaveDocument(lpszPathName);
}

void CRAWDoc::OnCloseDocument()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	deleteRawPixelData();
	
	CImageDoc::OnCloseDocument();
}

#ifndef _WIN32_WCE
// CRAWDoc serialization�Դϴ�.

void CRAWDoc::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
		ar.Write(m_RawPixelData, m_bitmap->GetWidth() * m_bitmap->GetHeight());	// ó���� ����迭�� ���Ϸ� ���� 
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
		CFile *infile = ar.GetFile(); // �Է��� ������ ����Ʈ�� ������.

		UINT length = (UINT)infile->GetLength();
		if (length == 0) {	// ���� ����� �˻���.
			AfxMessageBox(_T("���� ũ�Ⱑ 0 �Դϴ�."));
			return;
		}

		allocRawPixelData(length);
		ar.Read(m_RawPixelData, length); // ���������� �о� m_RawPixelData�迭�� ���� 
	}
}
#endif

void CRAWDoc::allocRawPixelData(UINT length)
{
	if (m_RawPixelData) {
		delete[] m_RawPixelData;
	}
	m_RawPixelData = new BYTE[length];
}

void CRAWDoc::deleteRawPixelData()
{
	if (m_RawPixelData) {
		delete[] m_RawPixelData;
		m_RawPixelData = NULL;
	}
}

