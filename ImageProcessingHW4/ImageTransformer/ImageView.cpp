// ImageView.cpp : CImageView Ŭ������ ���� �����Դϴ�.
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ImageTransformer.h"
#endif

#include "ImageDoc.h"
#include "ImageView.h"

#include "MainFrm.h"
#include "ImageFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageView

IMPLEMENT_DYNCREATE(CImageView, CView)

BEGIN_MESSAGE_MAP(CImageView, CView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HTG_SIZE, &CImageView::OnUpdateViewHtgSize)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CImageView, CView)
END_DISPATCH_MAP()

// ����: IID_IImageView�� ���� ������ �߰��Ͽ�
//  VBA���� ���� ���� ���ε��� �����մϴ�. 
//  �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {615CB34A-1700-4769-91F1-130F47067028}
static const IID IID_IImageView =
{ 0x615CB34A, 0x1700, 0x4769,{ 0x91, 0xF1, 0x13, 0xF, 0x47, 0x6, 0x70, 0x28 } };

BEGIN_INTERFACE_MAP(CImageView, CView)
	INTERFACE_PART(CImageView, IID_IImageView, Dispatch)
END_INTERFACE_MAP()


// CImageView ����/�Ҹ��Դϴ�.

CImageView::CImageView()
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	EnableAutomation();
}

CImageView::~CImageView()
{
}

void CImageView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	//CImageDoc *pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	//CRect frameRect;
	//CRect viewRect;

	//GetFrame()->GetWindowRect(&frameRect);

	//GetFrame()->GetClientRect(&viewRect);

	//GetFrame()->MoveWindow(GetFrame()->posX, GetFrame()->posY, pDoc->m_bitmap->GetWidth() + frameRect.Width() - viewRect.Width(), pDoc->m_bitmap->GetHeight() + frameRect.Height() - viewRect.Height());
	//GetFrame()->posX += 26;
	//GetFrame()->posY += 26;
}

void CImageView::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ�
	// OnFinalRelease�� ȣ��˴ϴ�.  �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CView::OnFinalRelease();
}


// CImageView �׸����Դϴ�.

void CImageView::OnDraw(CDC* pDC)
{
	Graphics graphicsDC(*pDC);					// gdi+ �׸��⸦ ���� ��ü
	CRect clientRect;
	GetClientRect(clientRect);
	Bitmap bmpCanvas(clientRect.right, clientRect.bottom);	// ĵ���� ��Ʈ�� ����
	Graphics graphicsCanvas(&bmpCanvas);		// ĵ���� �׷��Ƚ� ����
	graphicsCanvas.Clear(Color::Azure);			// ĵ���� ���� ����

	CImageDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	Bitmap* pBitmap = pDoc->m_bitmap;
	UINT gapW = abs(clientRect.Width() - (int)pBitmap->GetWidth());
	UINT gapH = abs(clientRect.Height() - (int)pBitmap->GetHeight());
	UINT gap = gapW > gapH ? gapW : gapH;

	if (pBitmap) {
		graphicsCanvas.DrawImage(pBitmap, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
	}

	graphicsDC.DrawImage(&bmpCanvas, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);	// ĵ���� �׸���
}


// CImageView �����Դϴ�.

#ifdef _DEBUG
void CImageView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif

//CImageFrame* CImageView::GetFrame() const	// ����׵��� ���� ������ �ζ������� �����˴ϴ�.
//{
//	ASSERT(CView::GetParentFrame()->IsKindOf(RUNTIME_CLASS(CImageFrame)));
//	return (CImageFrame*)CView::GetParentFrame();
//}

CImageDoc* CImageView::GetDocument() const	// ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageDoc)));
	return (CImageDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageView �޽��� ó�����Դϴ�.



// CImageView ����Դϴ�.

void CImageView::OnUpdateViewHtgSize(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(FALSE);
}
