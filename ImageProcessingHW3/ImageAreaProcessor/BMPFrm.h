// BMPFrm.h : CBMPFrame 클래스의 인터페이스입니다.
//

#pragma once

#include "BMPView.h"
#include "BMPDoc.h"

// CBMPFrame

class CBMPView;

class CBMPFrame : public CMDIChildWndEx
{
public:
	CBMPFrame();
	DECLARE_DYNCREATE(CBMPFrame)

// 특성입니다.
public:
//	virtual CBMPView* GetActiveView();
	virtual CBMPDoc* GetActiveDocument();
	BYTE m_bEcsHighEnd;
	BYTE m_bEcsLowEnd;
	DOUBLE m_snr;
	UINT m_InitW = 552;
	UINT m_InitH = 555;

// 작업입니다.
public:
	void Duplicate(OUT CBMPFrame** frame, OUT CBMPView** view, OUT CBMPDoc** document);

// 재정의입니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnFinalRelease();
	virtual void ActivateFrame(int nCmdShow = -1);

// 구현입니다.
public:
	virtual ~CBMPFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
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
};
