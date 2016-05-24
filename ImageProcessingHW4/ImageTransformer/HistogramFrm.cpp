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

// HistogramFrm.cpp : CHistogramFrame Ŭ������ ����
//

#include "stdafx.h"
#include "ImageTransformer.h"

#include "HistogramFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MainFrm.h"

#include "HistogramView.h"
#include "HistogramDoc.h"


// CHistogramFrame

IMPLEMENT_DYNCREATE(CHistogramFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CHistogramFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CHistogramFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CHistogramFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHistogramFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CHistogramFrame::OnUpdateFilePrintPreview)
	ON_COMMAND(ID_VIEW_ORIGIN_SIZE, &CHistogramFrame::OnViewOriginSize)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CHistogramFrame ����/�Ҹ�

CHistogramFrame::CHistogramFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CHistogramFrame::~CHistogramFrame()
{
}


BOOL CHistogramFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.cx = m_InitW;
	cs.cy = m_InitH;

	return TRUE;
}

void CHistogramFrame::ActivateFrame(int nCmdShow)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CRect winRect, cliRect;
	GetWindowRect(&winRect);
	GetClientRect(&cliRect);

	m_InitW = HTGSIZE + winRect.Width() - cliRect.Width() + 4;
	m_InitH = HTGSIZE + winRect.Height() - cliRect.Height() + 4;
	SetWindowPos(NULL, 0, 0, m_InitW, m_InitH, SWP_NOMOVE | SWP_SHOWWINDOW);

	CMDIChildWndEx::ActivateFrame(nCmdShow);
}


CHistogramView* CHistogramFrame::GetActiveView()
{
	return (CHistogramView*)CFrameWnd::GetActiveView();
}

CHistogramDoc* CHistogramFrame::GetActiveDocument()
{
	return (CHistogramDoc*)CFrameWnd::GetActiveDocument();
}


// CHistogramFrame ����

#ifdef _DEBUG
void CHistogramFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CHistogramFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG


// CHistogramFrame �޽��� ó����

void CHistogramFrame::OnFilePrint()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CHistogramFrame::OnFilePrintPreview()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // �μ� �̸� ���� ��带 �ݽ��ϴ�.
	}
}

void CHistogramFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}

void CHistogramFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	lpMMI->ptMinTrackSize.x = m_InitW;
	lpMMI->ptMinTrackSize.y = m_InitH;

	CMDIChildWndEx::OnGetMinMaxInfo(lpMMI);
}


// CHistogramFrame ���

void CHistogramFrame::OnViewOriginSize()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CHistogramView* pView = (CHistogramView*)GetActiveView();
	pView->resetHeightRate();
	ActivateFrame();
}