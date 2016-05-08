// BMPDoc.cpp : CBMPDoc 클래스의 구현 파일입니다.
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageAreaProcessor.h"
#endif

#include "ImageProcessorUtil.h"

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

// 참고: IID_IBMPDoc에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {56F902D5-BA11-4E73-9A1D-C873816130E7}
static const IID IID_IBMPDoc =
{ 0x56F902D5, 0xBA11, 0x4E73,{ 0x9A, 0x1D, 0xC8, 0x73, 0x81, 0x61, 0x30, 0xE7 } };

BEGIN_INTERFACE_MAP(CBMPDoc, CDocument)
	INTERFACE_PART(CBMPDoc, IID_IBMPDoc, Dispatch)
END_INTERFACE_MAP()


// CBMPDoc 생성/소멸입니다.

CBMPDoc::CBMPDoc()
{
	EnableAutomation();
	m_bitmap = NULL;
}

CBMPDoc::~CBMPDoc()
{
	//if (m_bitmap) {
	//	delete m_bitmap;
	//}
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

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//CLSID bmpClsid;
	//GetEncoderClsid(L"image/png", &bmpClsid);
	//m_bitmap->Save(lpszPathName, &bmpClsid, NULL);

	return CDocument::OnSaveDocument(lpszPathName);
}

void CBMPDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_bitmap) delete m_bitmap;

	CDocument::OnCloseDocument();
}

#ifndef _WIN32_WCE
// CBMPDoc serialization입니다.

void CBMPDoc::Serialize(CArchive& ar)
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
#endif

void CBMPDoc::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다.  기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDocument::OnFinalRelease();
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CBMPDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void CBMPDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CBMPDoc 진단입니다.

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


// CBMPDoc 명령입니다.

void CBMPDoc::copyFrom(const CBMPDoc* bmpDoc)
{
	Bitmap *src = bmpDoc->m_bitmap;
	this->m_bitmap = src->Clone(0, 0, src->GetWidth(), src->GetHeight(), PixelFormat8bppIndexed);

	CString newTitle("copied_");
	newTitle.Append(bmpDoc->GetTitle());
	this->SetTitle(newTitle);
}

BYTE* CBMPDoc::getData(BitmapData* bitmapData, const PixelFormat pixelFormat)
{
	Rect imageArea(0, 0, m_bitmap->GetWidth(), m_bitmap->GetHeight());
	m_bitmap->LockBits(
		&imageArea,
		pixelFormat,
		PixelFormat8bppIndexed,
		bitmapData);

	return (BYTE*)bitmapData->Scan0;
}

void CBMPDoc::clearData(BitmapData* bitmapData)
{
	m_bitmap->UnlockBits(bitmapData);
}

// Histogram Equalization
void CBMPDoc::HistogramEqualization()
{
	// 영상의 histogram을 계산
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;
	UINT histogramData[HTGSIZE];
	CImageProcessorUtil::generateHistogram(
		pixelData,
		pixelDataSize,
		histogramData);

	// Histogram의 누적합 및 정규화된 합 계산
	UINT accruedSum = 0;
	DOUBLE normalizedSum[HTGSIZE];
	FLOAT scaleFactor = HTGMAX / (float)pixelDataSize;
	for (UINT i = 0; i < HTGSIZE; i++) {
		accruedSum += histogramData[i];
		normalizedSum[i] = accruedSum * scaleFactor;
	}

	// Histogram Equalization (평활화): normalizedSum을 LUT로 사용
	for (UINT i = 0; i < pixelDataSize; i++) {
		pixelData[i] = (BYTE)(normalizedSum[pixelData[i]] + 0.5);
	}

	clearData(&bitmapData);
}

// Basic Contrast Stretching
void CBMPDoc::BasicContrastStretching()
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// 가장 작거나 큰 밝기값을 구함
	UINT low = (UINT)HTGMAX;
	UINT high = (UINT)HTGMIN;
	for (UINT i = 0; i < pixelDataSize; i++) {
		if (pixelData[i] < low) {
			low = pixelData[i];
		}
		else if (pixelData[i] > high) {
			high = pixelData[i];
		}
	}

	// Basic Contrast Stretching
	FLOAT scaleFactor = HTGMAX / (FLOAT)(high - low);
	for (UINT i = 0; i < pixelDataSize; i++) {
		pixelData[i] = (BYTE)((pixelData[i] - low) * scaleFactor);
	}

	clearData(&bitmapData);
}

// Ends-in Contrast Stretching
void CBMPDoc::EndsinContrastStretching(const BYTE low, const BYTE high)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// Ends-in Contrast Stretching
	float scaleFactor = HTGMAX / (FLOAT)(high - low);
	for (UINT i = 0; i < pixelDataSize; i++) {
		if (pixelData[i] <= low) {
			pixelData[i] = (BYTE)HTGMIN;
		}
		else if (pixelData[i] > high) {
			pixelData[i] = (BYTE)HTGMAX;
		}
		else {
			pixelData[i] = (BYTE)((pixelData[i] - low) * scaleFactor);
		}
	}

	clearData(&bitmapData);
}
