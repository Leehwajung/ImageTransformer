// BMPDoc.cpp : CBMPDoc 클래스의 구현 파일입니다.
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageAreaProcessor.h"
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
void CBMPDoc::BasicContrastStretching()
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
void CBMPDoc::EndsinContrastStretching(const BYTE low, const BYTE high)
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
void CBMPDoc::GaussianNoise(const DOUBLE snr)
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
void CBMPDoc::detectEdge(Mask::Type maskType)
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
void CBMPDoc::LowPassFiltering(UINT filterWidth)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// Low-pass Filtering
	double *result = new double[pixelDataSize];						// 필터링 결과 임시 저장 배열
	const double denominator = (double)(filterWidth * filterWidth);	// 저주파 통과 필터의 분모
	const int indicator = filterWidth / 2;							// 필터 중앙을 찾기 위한 보정

	for (UINT n = 0; n < bitmapData.Height; n++) {					// 영상 세로 방향 루프 (Image Abscissa)
		for (UINT m = 0; m < bitmapData.Width; m++) {				// 영상 가로 방향 루프 (Image Ordinate)
			double *newPixel = &result[n * bitmapData.Width + m];	// 필터링 후 픽셀의 신규 값
			*newPixel = 0;											// 신규 값 초기화
			for (UINT mr = 0; mr < filterWidth; mr++) {				// 필터 세로 방향 루프 (Filter Row)
				for (UINT mc = 0; mc < filterWidth; mc++) {			// 필터 가로 방향 루프 (Filter Column)
					int ir = n + mr - indicator;						// 계산할 영상 픽셀 세로 위치 (Image Row)
					int ic = m + mc - indicator;						// 계산할 영상 픽셀 가로 위치 (Image Column)

					// 경계부분 처리
					//	경계를 넘어가는 영상 픽셀 위치는 가장 가까운 픽셀로 변경하여,
					//	영상의 경계 부분을 복사해 Resolution을 증가시키는 것과 같은 효과를 얻음
					ir = ir >= (int)bitmapData.Height ? (int)bitmapData.Height - 1 : ir < 0 ? 0 : ir;
					ic = ic >= (int)bitmapData.Width ? (int)bitmapData.Width - 1 : ic < 0 ? 0 : ic;

					// 신규 값은 영상 픽셀과 필터 셀의 곱
					UINT ip = ir * bitmapData.Width + ic;
					*newPixel += (1.0 / denominator) * (double)pixelData[ip];
				}
			}
		}
	}

	// 결과 배열에서 원래 배열로 복사
	for (UINT i = 0; i < pixelDataSize; i++) {
		pixelData[i] = (BYTE)(result[i] + 0.5);
	}

	// 동적 할당 메모리 해제
	delete[] result;

	clearData(&bitmapData);
}

// Median Filtering
void CBMPDoc::MedianFiltering(UINT windowWidth)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// windowWidth가 짝수인 경우 홀수로 바꿔줌
	if (windowWidth % 2 == 0) {
		windowWidth++;
	}

	// Median Filtering
	BYTE *result = new BYTE[pixelDataSize];				// 필터링 결과 임시 저장 배열
	BYTE *sample = new BYTE[windowWidth * windowWidth];	// 윈도우에 포함된 픽셀 값 수집 배열
	const int indicator = windowWidth / 2;				// 윈도우 중앙을 찾기 위한 보정

	for (UINT n = 0; n < bitmapData.Height; n++) {		// 영상 세로 방향 루프 (Image Abscissa)
		for (UINT m = 0; m < bitmapData.Width; m++) {	// 영상 가로 방향 루프 (Image Ordinate)

			// 경계부분 처리
			//	이미지 픽셀 범위 내의 픽셀 값만을 수집하고 정렬한 다음 그 중간값을 얻어,
			//	첫 번째 샘플과 마지막 샘플을 복사하여 빈 셀에 채우는 것과 같은 효과를 얻음
			UINT pixelNum = 0;											// 윈도우에 포함된 픽셀 수 (샘플 수)
			for (UINT mr = 0; mr < windowWidth; mr++) {					// 윈도우 세로 방향 루프 (Window Row)
				int ir = n + mr - indicator;								// 계산할 영상 픽셀 세로 위치 (Image Row)
				if (ir < (int)bitmapData.Height && ir >= 0) {			// 세로 방향 이미지 픽셀 범위 내
					for (UINT mc = 0; mc < windowWidth; mc++) {			// 윈도우 가로 방향 루프 (Window Column)
						int ic = m + mc - indicator;						// 계산할 영상 픽셀 가로 위치 (Image Column)
						if (ic < (int)bitmapData.Width && ic >= 0) {	// 가로 방향 이미지 픽셀 범위 내
							// 샘플 수집
							UINT ip = ir * bitmapData.Width + ic;
							sample[pixelNum++] = pixelData[ip];
						}
					}
				}
			}

			// 샘플 정렬
			CImageProcessorUtil::quickSort<BYTE>(sample, pixelNum);

			// 중간값 가져오기
			result[n * bitmapData.Width + m] = sample[pixelNum / 2];
		}
	}

	// 결과 배열에서 원래 배열로 복사
	for (UINT i = 0; i < pixelDataSize; i++) {
		pixelData[i] = result[i];
	}

	// 동적 할당 메모리 해제
	delete[] result;
	delete[] sample;

	clearData(&bitmapData);
}

// 에러율 계산
DOUBLE CBMPDoc::getErrorRate(Mask::Type maskType, const DOUBLE snr)
{
	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead);	//영상의 픽셀 데이터를 가져옴
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	BYTE *origDetectedData = new BYTE[pixelDataSize];
	BYTE *noisyDetectedData = new BYTE[pixelDataSize];

	// 복사
	for (int i = 0; i < pixelDataSize; i++) {
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
	for (int i = 0; i < pixelDataSize; i++) {
		if (origDetectedData[i] == INTENSITYMAX) {
			n0++;
		}
		if (noisyDetectedData[i] == INTENSITYMAX) {
			n1++;
		}
	}

	clearData(&bitmapData);

	return n1 / n0;
}
