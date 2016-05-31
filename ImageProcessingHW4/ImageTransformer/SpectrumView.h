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

// SpectrumView.h : CSpectrumView Ŭ������ �������̽�
//

#pragma once

class CSpectrumDoc;

class CSpectrumView : public CView
{
protected:	// serialization������ ��������ϴ�.
	CSpectrumView();
	DECLARE_DYNCREATE(CSpectrumView)

// Ư���Դϴ�.
public:
	CSpectrumDoc* GetDocument() const;
	void allocPixelData(UINT length);
	void deletePixelData();

	Bitmap* m_bitmap;
	BYTE* m_ScaledData;
	BOOL m_bCreatedDataFirsthand;	// m_RawPixelData�� ���� �Ҵ��� ��� TRUE

// �۾��Դϴ�.
public:
	void scaleData();

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
	virtual ~CSpectrumView();
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG	// SpectrumView.cpp�� ����� ����
inline CSpectrumDoc* CSpectrumView::GetDocument() const
   { return reinterpret_cast<CSpectrumDoc*>(m_pDocument); }
#endif
