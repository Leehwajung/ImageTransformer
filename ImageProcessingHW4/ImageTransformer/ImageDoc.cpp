// ImageDoc.cpp : CImageDoc 클래스의 구현 파일입니다.
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageTransformer.h"
#endif

#include "ImageDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include <gdiplus.h>
using namespace Gdiplus;

// CImageDoc

IMPLEMENT_DYNCREATE(CImageDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CImageDoc, CDocument)
END_DISPATCH_MAP()

// 참고: IID_IImageDoc에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {56F902D5-BA11-4E73-9A1D-C873816130E7}
static const IID IID_IImageDoc =
{ 0x56F902D5, 0xBA11, 0x4E73,{ 0x9A, 0x1D, 0xC8, 0x73, 0x81, 0x61, 0x30, 0xE7 } };

BEGIN_INTERFACE_MAP(CImageDoc, CDocument)
	INTERFACE_PART(CImageDoc, IID_IImageDoc, Dispatch)
END_INTERFACE_MAP()


// CImageDoc 생성/소멸입니다.

CImageDoc::CImageDoc()
{
	EnableAutomation();
	m_bitmap = NULL;
}

CImageDoc::~CImageDoc()
{
	//if (m_bitmap) {
	//	delete m_bitmap;
	//}
}

BOOL CImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

BOOL CImageDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return TRUE;
}

BOOL CImageDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDocument::OnSaveDocument(lpszPathName);
}

void CImageDoc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_bitmap) {
		delete m_bitmap;
	}

	CDocument::OnCloseDocument();
}

#ifndef _WIN32_WCE
// CImageDoc serialization입니다.

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

void CImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
		//CLSID bmpClsid;
		//GetEncoderClsid(L"image/bmp", &bmpClsid);
		//m_bitmap->Save(ar.GetFile()->GetFilePath(), &bmpClsid, NULL);
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
		m_bitmap = Bitmap::FromFile(ar.GetFile()->GetFilePath());
	}
}
#endif

void CImageDoc::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다.  기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDocument::OnFinalRelease();
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CImageDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageDoc::SetSearchContent(const CString& value)
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

// CImageDoc 진단입니다.

#ifdef _DEBUG
void CImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG


// CImageDoc 명령입니다.

void CImageDoc::copyFrom(const CImageDoc* bmpDoc)
{
	Bitmap *src = bmpDoc->m_bitmap;
	this->m_bitmap = src->Clone(0, 0, src->GetWidth(), src->GetHeight(), PixelFormat8bppIndexed);

	CString newTitle("copied_");
	newTitle.Append(bmpDoc->GetTitle());
	this->SetTitle(newTitle);
}

BYTE* CImageDoc::getData(BitmapData* bitmapData, const PixelFormat pixelFormat)
{
	Rect imageArea(0, 0, m_bitmap->GetWidth(), m_bitmap->GetHeight());
	m_bitmap->LockBits(
		&imageArea,
		pixelFormat,
		PixelFormat8bppIndexed,
		bitmapData);

	return (BYTE*)bitmapData->Scan0;
}

void CImageDoc::clearData(BitmapData* bitmapData)
{
	m_bitmap->UnlockBits(bitmapData);
}

// Histogram Equalization
void CImageDoc::HistogramEqualization()
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
	FLOAT scaleFactor = (FLOAT)INTENSITYMAX / (FLOAT)pixelDataSize;
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
void CImageDoc::BasicContrastStretching()
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// 가장 작거나 큰 밝기값을 구함
	UINT low = (UINT)INTENSITYMAX;
	UINT high = (UINT)INTENSITYMIN;
	for (UINT i = 0; i < pixelDataSize; i++) {
		if (pixelData[i] < low) {
			low = pixelData[i];
		}
		else if (pixelData[i] > high) {
			high = pixelData[i];
		}
	}

	// Basic Contrast Stretching
	CImageProcessorUtil::stretchContrast(pixelData, pixelDataSize, low, high);

	clearData(&bitmapData);
}

// Ends-in Contrast Stretching
void CImageDoc::EndsinContrastStretching(const BYTE low, const BYTE high)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// Ends-in Contrast Stretching
	CImageProcessorUtil::stretchContrast(pixelData, pixelDataSize, low, high);

	clearData(&bitmapData);
}

// Gaussian Noise
void CImageDoc::GaussianNoise(const DOUBLE snr)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// Add Gaussian Noise
	double stddevNoise = CImageProcessorUtil::getStandardDeviationOfNoise(pixelData, pixelDataSize, snr);
	CImageProcessorUtil::addGaussianNoise(pixelData, pixelDataSize, stddevNoise);

	clearData(&bitmapData);
}

// Edge Detection
void CImageDoc::detectEdge(Mask::Type maskType)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// Masking and Edge Detection
	Mask mask(maskType);
	CImageProcessorUtil::mask(pixelData, bitmapData.Width, bitmapData.Height, mask);

	clearData(&bitmapData);
}

// Low-pass Filtering
void CImageDoc::LowPassFiltering(UINT filterWidth)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//영상의 픽셀 데이터를 가져옴
	
	// Low-pass Filtering
	CImageProcessorUtil::filterLowPass(pixelData, bitmapData.Width, bitmapData.Height, filterWidth);

	clearData(&bitmapData);
}

// Median Filtering
void CImageDoc::MedianFiltering(UINT windowWidth)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//영상의 픽셀 데이터를 가져옴

	// Median Filtering
	CImageProcessorUtil::filterMedian(pixelData, bitmapData.Width, bitmapData.Height, windowWidth);

	clearData(&bitmapData);
}

// 에러율 계산
DOUBLE CImageDoc::getErrorRate(Mask::Type maskType, const DOUBLE snr)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	BYTE *origDetectedData = new BYTE[pixelDataSize];
	BYTE *noisyDetectedData = new BYTE[pixelDataSize];

	// 복사
	for (UINT i = 0; i < pixelDataSize; i++) {
		origDetectedData[i] = pixelData[i];
		noisyDetectedData[i] = pixelData[i];
	}

	// 원본의 경계선 검출
	Mask mask(maskType);
	CImageProcessorUtil::mask(origDetectedData, bitmapData.Width, bitmapData.Height, mask);

	// 노이즈가 첨가된 이미지의 경계선 검출
	double stddevNoise = CImageProcessorUtil::getStandardDeviationOfNoise(noisyDetectedData, pixelDataSize, snr);
	CImageProcessorUtil::addGaussianNoise(noisyDetectedData, pixelDataSize, stddevNoise);
	CImageProcessorUtil::mask(noisyDetectedData, bitmapData.Width, bitmapData.Height, mask);

	// 픽셀 개수 확인
	double n0 = 0;
	double n1 = 0;
	for (UINT i = 0; i < pixelDataSize; i++) {
		if (origDetectedData[i] == INTENSITYMAX) {
			n0++;
		}
		if (noisyDetectedData[i] == INTENSITYMAX) {
			n1++;
		}
	}

	delete[] origDetectedData;
	delete[] noisyDetectedData;

	clearData(&bitmapData);

	return n1 / n0;
}

DOUBLE CImageDoc::getMeanSquareError(const INT filterType, const DOUBLE snr, const UINT filterWidth)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	BYTE *origDetectedData = new BYTE[pixelDataSize];
	BYTE *noisyDetectedData = new BYTE[pixelDataSize];

	// 복사
	for (UINT i = 0; i < pixelDataSize; i++) {
		origDetectedData[i] = pixelData[i];
		noisyDetectedData[i] = pixelData[i];
	}

	// 가우시안 노이즈 첨가
	double stddevNoise = CImageProcessorUtil::getStandardDeviationOfNoise(noisyDetectedData, pixelDataSize, snr);
	CImageProcessorUtil::addGaussianNoise(noisyDetectedData, pixelDataSize, stddevNoise);

	// 필터링
	switch (filterType) {
	case 0:	// Low-pass Filtering
		CImageProcessorUtil::filterLowPass(origDetectedData, bitmapData.Width, bitmapData.Height, filterWidth);
		CImageProcessorUtil::filterLowPass(noisyDetectedData, bitmapData.Width, bitmapData.Height, filterWidth);
		break;
	case 1:	// Median Filtering
		CImageProcessorUtil::filterMedian(origDetectedData, bitmapData.Width, bitmapData.Height, filterWidth);
		CImageProcessorUtil::filterMedian(noisyDetectedData, bitmapData.Width, bitmapData.Height, filterWidth);
		break;
	}

	// Mean Square Error
	double mse = CImageProcessorUtil::obtainMeanSquareError(origDetectedData, noisyDetectedData, pixelDataSize);
	
	delete[] origDetectedData;
	delete[] noisyDetectedData;

	clearData(&bitmapData);

	return mse;
}

void CImageDoc::forwardDiscreteCosineTransform(UINT maskWidth /*= 8*/)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	double *g = new double[pixelDataSize];

	int length = maskWidth;	// 마스크의 가로와 세로 길이

	//int *subPixelArr = new int[maskWidth * maskWidth];
	int subPixelArr[B_size][B_size];

	for (UINT n = 0; n < bitmapData.Height; n += maskWidth) {		// 영상 세로 방향 루프 (Image Abscissa)
		for (UINT m = 0; m < bitmapData.Width; m += maskWidth) {		// 영상 가로 방향 루프 (Image Ordinate)
			for (UINT mr = 0; mr < maskWidth; mr++) {		// 마스크 세로 방향 루프 (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// 마스크 가로 방향 루프 (Mask Column)
					subPixelArr[mr][mc] = pixelData[(n + mr) * maskWidth + (m + mc)];
				}
			}

			CImageProcessorUtil::dct8x8(subPixelArr);

			for (UINT mr = 0; mr < maskWidth; mr++) {		// 마스크 세로 방향 루프 (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// 마스크 가로 방향 루프 (Mask Column)
					g[(n + mr) * bitmapData.Width + (m + mc)] = subPixelArr[mr][mc];
				}
			}
		}
	}

	// 정규화를 위해 가장 작거나 큰 값을 구함
	double min = INT_MAX;
	double max = INT_MIN;
	for (UINT i = 1; i < pixelDataSize; i++) {
		if (g[i] < min) {
			min = g[i];
		}
		else if (g[i] > max) {
			max = g[i];
		}
	}

	// [min, max] 구간을 [0, 255]값으로 변환
	double scaleFactor = (double)INTENSITYMAX / (max - min);
	double translator = -(double)INTENSITYMAX * min / (max - min);
	for (UINT i = 0; i < pixelDataSize; i++) {
		// 정규화
		double normalized = scaleFactor * g[i] + translator + 0.5;

		// 임계값 적용
		//if (normalized > THRESH) {
		//	pixelData[i] = INTENSITYMAX;
		//}
		//else {
		//	pixelData[i] = INTENSITYMIN;
		//}

		// 임계값 비적용
		pixelData[i] = (BYTE)normalized;
	}

	// 동적 할당 메모리 해제
	delete[] g;

	clearData(&bitmapData);
}

void CImageDoc::inverseDiscreteCosineTransform(UINT maskWidth /*= 8*/)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	double *g = new double[pixelDataSize];

	int length = maskWidth;	// 마스크의 가로와 세로 길이

							//int *subPixelArr = new int[maskWidth * maskWidth];
	int subPixelArr[B_size][B_size];

	for (UINT n = 0; n < bitmapData.Height; n += maskWidth) {		// 영상 세로 방향 루프 (Image Abscissa)
		for (UINT m = 0; m < bitmapData.Width; m += maskWidth) {		// 영상 가로 방향 루프 (Image Ordinate)
			for (UINT mr = 0; mr < maskWidth; mr++) {		// 마스크 세로 방향 루프 (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// 마스크 가로 방향 루프 (Mask Column)
					subPixelArr[mr][mc] = pixelData[(n + mr) * maskWidth + (m + mc)];
				}
			}

			CImageProcessorUtil::idct8x8(subPixelArr);

			for (UINT mr = 0; mr < maskWidth; mr++) {		// 마스크 세로 방향 루프 (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// 마스크 가로 방향 루프 (Mask Column)
					g[(n + mr) * bitmapData.Width + (m + mc)] = subPixelArr[mr][mc];
				}
			}
		}
	}

	// 정규화를 위해 가장 작거나 큰 값을 구함
	double min = INT_MAX;
	double max = INT_MIN;
	for (UINT i = 1; i < pixelDataSize; i++) {
		if (g[i] < min) {
			min = g[i];
		}
		else if (g[i] > max) {
			max = g[i];
		}
	}

	// [min, max] 구간을 [0, 255]값으로 변환
	double scaleFactor = (double)INTENSITYMAX / (max - min);
	double translator = -(double)INTENSITYMAX * min / (max - min);
	for (UINT i = 0; i < pixelDataSize; i++) {
		// 정규화
		double normalized = scaleFactor * g[i] + translator + 0.5;

		// 임계값 적용
		//if (normalized > THRESH) {
		//	pixelData[i] = INTENSITYMAX;
		//}
		//else {
		//	pixelData[i] = INTENSITYMIN;
		//}

		// 임계값 비적용
		pixelData[i] = (BYTE)normalized;
	}

	// 동적 할당 메모리 해제
	delete[] g;

	clearData(&bitmapData);
}
