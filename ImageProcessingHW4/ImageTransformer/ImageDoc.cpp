// ImageDoc.cpp : CImageDoc Ŭ������ ���� �����Դϴ�.
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ImageTransformer.h"
#endif

#include "ImageDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "ImageProcessorUtilGeneric.cpp"

#include <gdiplus.h>
using namespace Gdiplus;

// CImageDoc

IMPLEMENT_DYNCREATE(CImageDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CImageDoc, CDocument)
END_DISPATCH_MAP()

// ����: IID_IImageDoc�� ���� ������ �߰��Ͽ�
//  VBA���� ���� ���� ���ε��� �����մϴ�. 
//  �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {56F902D5-BA11-4E73-9A1D-C873816130E7}
static const IID IID_IImageDoc =
{ 0x56F902D5, 0xBA11, 0x4E73,{ 0x9A, 0x1D, 0xC8, 0x73, 0x81, 0x61, 0x30, 0xE7 } };

BEGIN_INTERFACE_MAP(CImageDoc, CDocument)
	INTERFACE_PART(CImageDoc, IID_IImageDoc, Dispatch)
END_INTERFACE_MAP()


// CImageDoc ����/�Ҹ��Դϴ�.

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

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	return TRUE;
}

BOOL CImageDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDocument::OnSaveDocument(lpszPathName);
}

void CImageDoc::OnCloseDocument()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (m_bitmap) {
		delete m_bitmap;
	}

	CDocument::OnCloseDocument();
}

#ifndef _WIN32_WCE
// CImageDoc serialization�Դϴ�.

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
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
		//CLSID bmpClsid;
		//GetEncoderClsid(L"image/bmp", &bmpClsid);
		//m_bitmap->Save(ar.GetFile()->GetFilePath(), &bmpClsid, NULL);
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
		m_bitmap = Bitmap::FromFile(ar.GetFile()->GetFilePath());
	}
}
#endif

void CImageDoc::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ�
	// OnFinalRelease�� ȣ��˴ϴ�.  �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDocument::OnFinalRelease();
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CImageDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CImageDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CImageDoc �����Դϴ�.

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


// CImageDoc ����Դϴ�.

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
	// ������ histogram�� ���
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//������ �ȼ� �����͸� ������
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;
	UINT histogramData[HTGSIZE];
	CImageProcessorUtil::generateHistogram(
		pixelData,
		pixelDataSize,
		histogramData);

	// Histogram�� ������ �� ����ȭ�� �� ���
	UINT accruedSum = 0;
	DOUBLE normalizedSum[HTGSIZE];
	FLOAT scaleFactor = (FLOAT)INTENSITYMAX / (FLOAT)pixelDataSize;
	for (UINT i = 0; i < HTGSIZE; i++) {
		accruedSum += histogramData[i];
		normalizedSum[i] = accruedSum * scaleFactor;
	}

	// Histogram Equalization (��Ȱȭ): normalizedSum�� LUT�� ���
	for (UINT i = 0; i < pixelDataSize; i++) {
		pixelData[i] = (BYTE)(normalizedSum[pixelData[i]] + 0.5);
	}

	clearData(&bitmapData);
}

// Basic Contrast Stretching
void CImageDoc::BasicContrastStretching()
{
	// ������ pixel data�� ������
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//������ �ȼ� �����͸� ������
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// Basic Contrast Stretching
	CImageProcessorUtil::stretchContrast(pixelData, pixelDataSize);

	clearData(&bitmapData);
}

// Ends-in Contrast Stretching
void CImageDoc::EndsinContrastStretching(const BYTE low, const BYTE high)
{
	// ������ pixel data�� ������
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//������ �ȼ� �����͸� ������
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// Ends-in Contrast Stretching
	CImageProcessorUtil::stretchContrast(pixelData, pixelDataSize, low, high);

	clearData(&bitmapData);
}

// Gaussian Noise
void CImageDoc::GaussianNoise(const DOUBLE snr)
{
	// ������ pixel data�� ������
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//������ �ȼ� �����͸� ������
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// Add Gaussian Noise
	double stddevNoise = CImageProcessorUtil::getStandardDeviationOfNoise(pixelData, pixelDataSize, snr);
	CImageProcessorUtil::addGaussianNoise(pixelData, pixelDataSize, stddevNoise);

	clearData(&bitmapData);
}

// Edge Detection
void CImageDoc::detectEdge(Mask::Type maskType)
{
	// ������ pixel data�� ������
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//������ �ȼ� �����͸� ������
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	// Masking and Edge Detection
	Mask mask(maskType);
	CImageProcessorUtil::mask(pixelData, bitmapData.Width, bitmapData.Height, mask);

	clearData(&bitmapData);
}

// Low-pass Filtering
void CImageDoc::LowPassFiltering(UINT filterWidth)
{
	// ������ pixel data�� ������
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//������ �ȼ� �����͸� ������
	
	// Low-pass Filtering
	CImageProcessorUtil::filterLowPass(pixelData, bitmapData.Width, bitmapData.Height, filterWidth);

	clearData(&bitmapData);
}

// Median Filtering
void CImageDoc::MedianFiltering(UINT windowWidth)
{
	// ������ pixel data�� ������
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead | ImageLockModeWrite);	//������ �ȼ� �����͸� ������

	// Median Filtering
	CImageProcessorUtil::filterMedian(pixelData, bitmapData.Width, bitmapData.Height, windowWidth);

	clearData(&bitmapData);
}

// ������ ���
DOUBLE CImageDoc::getErrorRate(Mask::Type maskType, const DOUBLE snr)
{
	// ������ pixel data�� ������
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead);	//������ �ȼ� �����͸� ������
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	BYTE *origDetectedData = new BYTE[pixelDataSize];
	BYTE *noisyDetectedData = new BYTE[pixelDataSize];

	// ����
	for (UINT i = 0; i < pixelDataSize; i++) {
		origDetectedData[i] = pixelData[i];
		noisyDetectedData[i] = pixelData[i];
	}

	// ������ ��輱 ����
	Mask mask(maskType);
	CImageProcessorUtil::mask(origDetectedData, bitmapData.Width, bitmapData.Height, mask);

	// ����� ÷���� �̹����� ��輱 ����
	double stddevNoise = CImageProcessorUtil::getStandardDeviationOfNoise(noisyDetectedData, pixelDataSize, snr);
	CImageProcessorUtil::addGaussianNoise(noisyDetectedData, pixelDataSize, stddevNoise);
	CImageProcessorUtil::mask(noisyDetectedData, bitmapData.Width, bitmapData.Height, mask);

	// �ȼ� ���� Ȯ��
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
	// ������ pixel data�� ������
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead);	//������ �ȼ� �����͸� ������
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	BYTE *origDetectedData = new BYTE[pixelDataSize];
	BYTE *noisyDetectedData = new BYTE[pixelDataSize];

	// ����
	for (UINT i = 0; i < pixelDataSize; i++) {
		origDetectedData[i] = pixelData[i];
		noisyDetectedData[i] = pixelData[i];
	}

	// ����þ� ������ ÷��
	double stddevNoise = CImageProcessorUtil::getStandardDeviationOfNoise(noisyDetectedData, pixelDataSize, snr);
	CImageProcessorUtil::addGaussianNoise(noisyDetectedData, pixelDataSize, stddevNoise);

	// ���͸�
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
	// ������ pixel data�� ������
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead);	//������ �ȼ� �����͸� ������
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	double *g = new double[pixelDataSize];

	int length = maskWidth;	// ����ũ�� ���ο� ���� ����

	//int *subPixelArr = new int[maskWidth * maskWidth];
	int subPixelArr[B_size][B_size];

	for (UINT n = 0; n < bitmapData.Height; n += maskWidth) {		// ���� ���� ���� ���� (Image Abscissa)
		for (UINT m = 0; m < bitmapData.Width; m += maskWidth) {	// ���� ���� ���� ���� (Image Ordinate)
			for (UINT mr = 0; mr < maskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					subPixelArr[mr][mc] = pixelData[(n + mr) * bitmapData.Width + (m + mc)];
				}
			}
			
			CImageProcessorUtil::dct8x8(subPixelArr);

			for (UINT mr = 0; mr < maskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					g[(n + mr) * bitmapData.Width + (m + mc)] = subPixelArr[mr][mc];
				}
			}
		}
	}

	// [min, max] ������ [0, 255] �������� scaling (����ȭ)
	CImageProcessorUtil::stretchContrast(g, pixelDataSize);

	for (UINT i = 0; i < pixelDataSize; i++) {
		pixelData[i] = (BYTE)g[i];
	}

	// ���� �Ҵ� �޸� ����
	delete[] g;

	clearData(&bitmapData);
}

void CImageDoc::inverseDiscreteCosineTransform(UINT maskWidth /*= 8*/)
{
	// ������ pixel data�� ������
	BitmapData bitmapData;
	BYTE *pixelData = getData(&bitmapData, ImageLockModeRead);	//������ �ȼ� �����͸� ������
	UINT pixelDataSize = bitmapData.Width * bitmapData.Height;

	double *g = new double[pixelDataSize];

	int length = maskWidth;	// ����ũ�� ���ο� ���� ����

	//int *subPixelArr = new int[maskWidth * maskWidth];
	int subPixelArr[B_size][B_size];

	for (UINT n = 0; n < bitmapData.Height; n += maskWidth) {		// ���� ���� ���� ���� (Image Abscissa)
		for (UINT m = 0; m < bitmapData.Width; m += maskWidth) {		// ���� ���� ���� ���� (Image Ordinate)
			for (UINT mr = 0; mr < maskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					subPixelArr[mr][mc] = pixelData[(n + mr) * maskWidth + (m + mc)];
				}
			}

			CImageProcessorUtil::idct8x8(subPixelArr);

			for (UINT mr = 0; mr < maskWidth; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (UINT mc = 0; mc < maskWidth; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					g[(n + mr) * bitmapData.Width + (m + mc)] = subPixelArr[mr][mc];
				}
			}
		}
	}

	// [min, max] ������ [0, 255] �������� scaling (����ȭ)
	CImageProcessorUtil::stretchContrast(g, pixelDataSize);

	for (UINT i = 0; i < pixelDataSize; i++) {
		pixelData[i] = (BYTE)g[i];
	}

	// ���� �Ҵ� �޸� ����
	delete[] g;

	clearData(&bitmapData);
}
