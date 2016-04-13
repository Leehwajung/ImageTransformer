// BMPFrm.h : CBMPFrame 클래스의 인터페이스입니다.
//

#pragma once

#include "BMPView.h"
#include "BMPDoc.h"

// CBMPFrame

class CBMPFrame : public CMDIChildWndEx
{
public:
	CBMPFrame();
	DECLARE_DYNCREATE(CBMPFrame)

// 특성입니다.
public:
//	virtual CBMPView* GetActiveView();
	virtual CBMPDoc* GetActiveDocument();
	BYTE bEcsHighEnd;
	BYTE bEcsLowEnd;

// 작업입니다.
public:

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
	afx_msg void OnIpHistogramEqualization();
	afx_msg void OnIpBasicContrastStretching();
	afx_msg void OnIpEndsinContrastStretching();
	afx_msg void OnIpEcsHighEnd();
	afx_msg void OnIpEcsLowEnd();
	afx_msg void OnViewOriginSize();
};
