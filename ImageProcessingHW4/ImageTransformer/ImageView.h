// ImageView.h : CImageView 클래스의 인터페이스입니다.
//

#pragma once

#include "ImageFrm.h"
#include "ImageDoc.h"


// CImageView 뷰입니다.

class CImageView : public CView
{
protected:	// serialization에서만 만들어집니다.
	CImageView();		// 동적 만들기에 사용되는 protected 생성자입니다.
	DECLARE_DYNCREATE(CImageView)

// 특성입니다.
public:
//	CImageFrame* GetFrame() const;
	CImageDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);	// 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();
	virtual void OnFinalRelease();

// 구현입니다.
public:
	virtual ~CImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnUpdateViewHtgSize(CCmdUI *pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

#ifndef _DEBUG	// ImageView.cpp의 디버그 버전
inline CImageFrame* CImageView::GetFrame() const
   { return reinterpret_cast<CImageFrame*>(CView::GetParentFrame()); }

inline CImageDoc* CImageView::GetDocument() const
   { return reinterpret_cast<CImageDoc*>(m_pDocument); }
#endif