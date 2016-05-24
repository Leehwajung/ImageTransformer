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

// RAWFrm.cpp : CRAWFrame Ŭ������ ����
//

#include "stdafx.h"
#include "ImageTransformer.h"

#include "RAWFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRAWFrame

IMPLEMENT_DYNCREATE(CRAWFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CRAWFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CRAWFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CRAWFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRAWFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CRAWFrame::OnUpdateFilePrintPreview)
END_MESSAGE_MAP()

// CRAWFrame ����/�Ҹ�

CRAWFrame::CRAWFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CRAWFrame::~CRAWFrame()
{
}


BOOL CRAWFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CRAWFrame ����

#ifdef _DEBUG
void CRAWFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CRAWFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CRAWFrame �޽��� ó����

void CRAWFrame::OnFilePrint()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CRAWFrame::OnFilePrintPreview()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // �μ� �̸� ���� ��带 �ݽ��ϴ�.
	}
}

void CRAWFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}
