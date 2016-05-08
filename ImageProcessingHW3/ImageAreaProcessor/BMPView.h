// BMPView.h : CBMPView 클래스의 인터페이스입니다.
//

#pragma once

#include "BMPFrm.h"
#include "BMPDoc.h"

// CBMPView 뷰입니다.

class CBMPView : public CView
{
protected:	// serialization에서만 만들어집니다.
	CBMPView();		// 동적 만들기에 사용되는 protected 생성자입니다.
	DECLARE_DYNCREATE(CBMPView)

// 특성입니다.
public:
//	CBMPFrame* GetFrame() const;
	CBMPDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);	// 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();
	virtual void OnFinalRelease();

// 구현입니다.
public:
	virtual ~CBMPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnUpdateViewHtgSize(CCmdUI *pCmdUI);
};

#ifndef _DEBUG	// BMPView.cpp의 디버그 버전
inline CBMPFrame* CBMPView::GetFrame() const
   { return reinterpret_cast<CBMPFrame*>(CView::GetParentFrame()); }

inline CBMPDoc* CBMPView::GetDocument() const
   { return reinterpret_cast<CBMPDoc*>(m_pDocument); }
#endif