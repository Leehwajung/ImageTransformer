// ImageDoc.h : CImageDoc Ŭ������ �������̽��Դϴ�.
//

#pragma once

#include "ImageProcessorUtil.h"

using Gdiplus::Bitmap;


// CImageDoc �����Դϴ�.

class CImageDoc : public CDocument
{
protected:	// serialization������ ��������ϴ�.
	CImageDoc();
	DECLARE_DYNCREATE(CImageDoc)

// Ư���Դϴ�.
public:
	Bitmap *m_bitmap;

// �۾��Դϴ�.
public:
	void copyFrom(const CImageDoc* bmpDoc);
	BYTE* getData(BitmapData* bitmapData, const PixelFormat pixelFormat);
	void clearData(BitmapData* bitmapData);

	// Histogram Equalization
	void HistogramEqualization();

	// Basic Contrast Stretching
	void BasicContrastStretching();

	// Ends-in Contrast Stretching
	void EndsinContrastStretching(const BYTE low, const BYTE high);

	// Gaussian Noise
	void CImageDoc::GaussianNoise(const DOUBLE snr);

	// Edge Detection
	void CImageDoc::detectEdge(const Mask::Type maskType);

	// Low-pass Filtering
	void CImageDoc::LowPassFiltering(const UINT filterWidth);

	// Median Filtering
	void CImageDoc::MedianFiltering(const UINT windowWidth);

	// ������ ���
	DOUBLE getErrorRate(const Mask::Type maskType, const DOUBLE snr);

	// MSE (filterType: 0 = Low-Pass, 1 = Median)
	DOUBLE getMeanSquareError(const INT filterType, const DOUBLE snr, const UINT filterWidth);

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);	// ���� ��/����� ���� �����ǵǾ����ϴ�.
#endif
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS
	virtual void OnFinalRelease();

// �����Դϴ�.
public:
	virtual ~CImageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	
// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void forwardDiscreteCosineTransform(UINT maskWidth = 8);
	void inverseDiscreteCosineTransform(UINT maskWidth = 8);
};

