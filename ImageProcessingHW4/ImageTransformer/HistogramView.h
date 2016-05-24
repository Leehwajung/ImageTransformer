// �� MFC ���� �ҽ� �ڵ�� MFC Microsoft Office Fluent ����� �������̽�("Fluent UI")�� 
// ����ϴ� ����� ���� �ָ�, MFC C++ ���̺귯�� ����Ʈ��� ���Ե� 
// Microsoft Foundation Classes Reference �� ���� ���� ������ ���� 
// �߰������� �����Ǵ� �����Դϴ�.  
// Fluent UI�� ����, ��� �Ǵ� �����ϴ� �� ���� ��� ����� ������ �����˴ϴ�.  
// Fluent UI ���̼��� ���α׷��� ���� �ڼ��� ������ 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// HistogramView.h : CHistogramView Ŭ������ �������̽�
//

#pragma once

#include "HistogramDoc.h"

class CHistogramView : public CView
{
protected:	// serialization������ ��������ϴ�.
	CHistogramView();
	DECLARE_DYNCREATE(CHistogramView)

// Ư���Դϴ�.
public:
	CHistogramDoc* GetDocument() const;
private:
	BYTE* m_Image;		// histogram ��� �迭
	UINT m_Width;		// histogram ��� �迭�� width
	UINT m_Height;		// histogram ��� �迭�� height
	UINT m_Stride;
	FLOAT m_HeightRate;	// histogram ��� �迭���� ���� ����� height

// �۾��Դϴ�.
public:
	// histogram�� 2���� �̹��� ����
	void plotHistogramImage();
	void resetHeightRate();

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);	// �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CHistogramView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewHtgSize();
	afx_msg void OnUpdateViewHtgSize(CCmdUI *pCmdUI);
};

#ifndef _DEBUG	// HistogramView.cpp�� ����� ����
inline CHistogramDoc* CHistogramView::GetDocument() const
   { return reinterpret_cast<CHistogramDoc*>(m_pDocument); }
#endif

