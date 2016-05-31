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

// SpectrumDoc.h : CSpectrumDoc Ŭ������ �������̽�
//

#pragma once

class CSpectrumDoc : public CDocument
{
protected:	// serialization������ ��������ϴ�.
	CSpectrumDoc();
	DECLARE_DYNCREATE(CSpectrumDoc)

// Ư���Դϴ�.
public:
	double *m_DctData;
	UINT m_Height;
	UINT m_Width;

// �۾��Դϴ�.
public:
	// FDCT (Forward Discrete Cosine Transform)
	void forwardDiscreteCosineTransform(BYTE inputPixelData[], UINT height, UINT width, UINT maskWidth = 8);
	// IDCT (Inverse Discrete Cosine Transform)
	void inverseDiscreteCosineTransform(BYTE outputPixelData[], UINT pixelDataSize, UINT maskWidth = 8);

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif	// SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CSpectrumDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif	// SHARED_HANDLERS
};