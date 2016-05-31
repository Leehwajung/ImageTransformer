// ImageView.h : CImageView Ŭ������ �������̽��Դϴ�.
//

#pragma once

#include "ImageFrm.h"
#include "ImageDoc.h"


// CImageView ���Դϴ�.

class CImageView : public CView
{
protected:	// serialization������ ��������ϴ�.
	CImageView();		// ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	DECLARE_DYNCREATE(CImageView)

// Ư���Դϴ�.
public:
//	CImageFrame* GetFrame() const;
	CImageDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);	// �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual void OnInitialUpdate();
	virtual void OnFinalRelease();

// �����Դϴ�.
public:
	virtual ~CImageView();
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
public:
	afx_msg void OnUpdateViewHtgSize(CCmdUI *pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG	// ImageView.cpp�� ����� ����
inline CImageFrame* CImageView::GetFrame() const
   { return reinterpret_cast<CImageFrame*>(CView::GetParentFrame()); }

inline CImageDoc* CImageView::GetDocument() const
   { return reinterpret_cast<CImageDoc*>(m_pDocument); }
#endif