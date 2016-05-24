// BMPView.cpp : CBMPView Ŭ������ ���� �����Դϴ�.
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ImageTransformer.h"
#endif

#include "BMPDoc.h"
#include "BMPView.h"

#include "MainFrm.h"
#include "BMPFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBMPView

IMPLEMENT_DYNCREATE(CBMPView, CView)

BEGIN_MESSAGE_MAP(CBMPView, CView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HTG_SIZE, &CBMPView::OnUpdateViewHtgSize)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CBMPView, CView)
END_DISPATCH_MAP()

// ����: IID_IBMPView�� ���� ������ �߰��Ͽ�
//  VBA���� ���� ���� ���ε��� �����մϴ�. 
//  �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {615CB34A-1700-4769-91F1-130F47067028}
static const IID IID_IBMPView =
{ 0x615CB34A, 0x1700, 0x4769,{ 0x91, 0xF1, 0x13, 0xF, 0x47, 0x6, 0x70, 0x28 } };

BEGIN_INTERFACE_MAP(CBMPView, CView)
	INTERFACE_PART(CBMPView, IID_IBMPView, Dispatch)
END_INTERFACE_MAP()


// CBMPView ����/�Ҹ��Դϴ�.

CBMPView::CBMPView()
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	EnableAutomation();
}

CBMPView::~CBMPView()
{
}

void CBMPView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	//CBMPDoc *pDoc = GetDocument();
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

void CBMPView::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ�
	// OnFinalRelease�� ȣ��˴ϴ�.  �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CView::OnFinalRelease();
}


// CBMPView �׸����Դϴ�.

void CBMPView::OnDraw(CDC* pDC)
{
	Graphics graphicsDC(*pDC);					// gdi+ �׸��⸦ ���� ��ü
	CRect clientRect;
	GetClientRect(clientRect);
	Bitmap bmpCanvas(clientRect.right, clientRect.bottom);	// ĵ���� ��Ʈ�� ����
	Graphics graphicsCanvas(&bmpCanvas);		// ĵ���� �׷��Ƚ� ����
	graphicsCanvas.Clear(Color::Azure);			// ĵ���� ���� ����

	CBMPDoc *pDoc = GetDocument();
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


// CBMPView �����Դϴ�.

#ifdef _DEBUG
void CBMPView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBMPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif

//CBMPFrame* CBMPView::GetFrame() const	// ����׵��� ���� ������ �ζ������� �����˴ϴ�.
//{
//	ASSERT(CView::GetParentFrame()->IsKindOf(RUNTIME_CLASS(CBMPFrame)));
//	return (CBMPFrame*)CView::GetParentFrame();
//}

CBMPDoc* CBMPView::GetDocument() const	// ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBMPDoc)));
	return (CBMPDoc*)m_pDocument;
}
#endif //_DEBUG


// CBMPView �޽��� ó�����Դϴ�.



// CBMPView ����Դϴ�.

void CBMPView::OnUpdateViewHtgSize(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(FALSE);
}
