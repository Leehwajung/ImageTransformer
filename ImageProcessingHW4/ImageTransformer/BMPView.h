// BMPView.h : CBMPView Ŭ������ �������̽��Դϴ�.
//

#pragma once

#include "BMPFrm.h"
#include "BMPDoc.h"

// CBMPView ���Դϴ�.

class CBMPView : public CView
{
protected:	// serialization������ ��������ϴ�.
	CBMPView();		// ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	DECLARE_DYNCREATE(CBMPView)

// Ư���Դϴ�.
public:
//	CBMPFrame* GetFrame() const;
	CBMPDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);	// �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual void OnInitialUpdate();
	virtual void OnFinalRelease();

// �����Դϴ�.
public:
	virtual ~CBMPView();
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
};

#ifndef _DEBUG	// BMPView.cpp�� ����� ����
inline CBMPFrame* CBMPView::GetFrame() const
   { return reinterpret_cast<CBMPFrame*>(CView::GetParentFrame()); }

inline CBMPDoc* CBMPView::GetDocument() const
   { return reinterpret_cast<CBMPDoc*>(m_pDocument); }
#endif