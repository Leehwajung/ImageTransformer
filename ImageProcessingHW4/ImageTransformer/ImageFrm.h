// ImageFrm.h : CImageFrame Ŭ������ �������̽��Դϴ�.
//

#pragma once

#include "ImageView.h"
#include "ImageDoc.h"

#include "ImageProcessorUtil.h"


// CImageFrame �������Դϴ�.

class CImageView;

class CImageFrame : public CMDIChildWndEx
{
public:
	CImageFrame();
	DECLARE_DYNCREATE(CImageFrame)

// Ư���Դϴ�.
public:
//	virtual CImageView* GetActiveView();
	virtual CImageDoc* GetActiveDocument();
	BYTE m_bEcsHighEnd;
	BYTE m_bEcsLowEnd;
	DOUBLE m_snr;
	UINT m_InitW = 552;
	UINT m_InitH = 555;

// �۾��Դϴ�.
public:
	void Duplicate(OUT CImageFrame** frame, OUT CImageView** view, OUT CImageDoc** document);
	void runEdgeDetection(Mask::Type maskType);

// �������Դϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnFinalRelease();
	virtual void ActivateFrame(int nCmdShow = -1);

// �����Դϴ�.
public:
	virtual ~CImageFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnPpHistogramEqualization();
	afx_msg void OnPpBasicContrastStretching();
	afx_msg void OnPpEndsinContrastStretching();
	afx_msg void OnPpEcsHighEnd();
	afx_msg void OnPpEcsLowEnd();
	afx_msg void OnViewOriginSize();
	afx_msg void OnNoiseGaussian();
	afx_msg void OnNoiseSNR();
	afx_msg void OnApRoberts();
	afx_msg void OnApSobel();
	afx_msg void OnApPrewitt();
	afx_msg void OnApStochasticGradient();
	afx_msg void OnApLowPass();
	afx_msg void OnApMedian();
	afx_msg void OnErRoberts();
	afx_msg void OnErSobel();
	afx_msg void OnErPrewitt();
	afx_msg void OnErStochasticGradient();
	afx_msg void OnErLowPass();
	afx_msg void OnErMedian();
};
