// BMPDoc.cpp : CBMPDoc Ŭ������ ���� �����Դϴ�.
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ImageProcessor.h"
#endif

#include "BMPDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include <gdiplus.h>
using namespace Gdiplus;

// CBMPDoc

IMPLEMENT_DYNCREATE(CBMPDoc, CDocument)

BEGIN_MESSAGE_MAP(CBMPDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CBMPDoc, CDocument)
END_DISPATCH_MAP()

// ����: IID_IBMPDoc�� ���� ������ �߰��Ͽ�
//  VBA���� ���� ���� ���ε��� �����մϴ�. 
//  �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {56F902D5-BA11-4E73-9A1D-C873816130E7}
static const IID IID_IBMPDoc =
{ 0x56F902D5, 0xBA11, 0x4E73,{ 0x9A, 0x1D, 0xC8, 0x73, 0x81, 0x61, 0x30, 0xE7 } };

BEGIN_INTERFACE_MAP(CBMPDoc, CDocument)
	INTERFACE_PART(CBMPDoc, IID_IBMPDoc, Dispatch)
END_INTERFACE_MAP()


// CBMPDoc ����/�Ҹ��Դϴ�.

CBMPDoc::CBMPDoc()
{
	EnableAutomation();
	m_bitmap = NULL;
}

CBMPDoc::~CBMPDoc()
{
}

BOOL CBMPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

BOOL CBMPDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	//m_bitmap.LoadBitmapW(lpszPathName);
	m_bitmap = Bitmap::FromFile(lpszPathName);

	return TRUE;
}

//int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
//{
//	UINT  num = 0;          // number of image encoders
//	UINT  size = 0;         // size of the image encoder array in bytes
//
//	ImageCodecInfo* pImageCodecInfo = NULL;
//
//	GetImageEncodersSize(&num, &size);
//	if (size == 0)
//		return -1;  // Failure
//
//	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
//	if (pImageCodecInfo == NULL)
//		return -1;  // Failure
//
//	GetImageEncoders(num, size, pImageCodecInfo);
//
//	for (UINT j = 0; j < num; ++j)
//	{
//		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
//		{
//			*pClsid = pImageCodecInfo[j].Clsid;
//			free(pImageCodecInfo);
//			return j;  // Success
//		}
//	}
//
//	free(pImageCodecInfo);
//	return -1;  // Failure
//}

BOOL CBMPDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	//CLSID bmpClsid;
	//GetEncoderClsid(L"image/png", &bmpClsid);
	//m_bitmap->Save(lpszPathName, &bmpClsid, NULL);

	return CDocument::OnSaveDocument(lpszPathName);
}

void CBMPDoc::OnCloseDocument()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (m_bitmap) delete m_bitmap;

	CDocument::OnCloseDocument();
}

#ifndef _WIN32_WCE
// CBMPDoc serialization�Դϴ�.

void CBMPDoc::Serialize(CArchive& ar)
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
#endif

void CBMPDoc::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ�
	// OnFinalRelease�� ȣ��˴ϴ�.  �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDocument::OnFinalRelease();
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CBMPDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CBMPDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CBMPDoc::SetSearchContent(const CString& value)
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

// CBMPDoc �����Դϴ�.

#ifdef _DEBUG
void CBMPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CBMPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG


// CBMPDoc �����Դϴ�.
