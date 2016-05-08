// BMPView.cpp : CBMPView 클래스의 구현 파일입니다.
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageAreaProcessor.h"
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

// 참고: IID_IBMPView에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {615CB34A-1700-4769-91F1-130F47067028}
static const IID IID_IBMPView =
{ 0x615CB34A, 0x1700, 0x4769,{ 0x91, 0xF1, 0x13, 0xF, 0x47, 0x6, 0x70, 0x28 } };

BEGIN_INTERFACE_MAP(CBMPView, CView)
	INTERFACE_PART(CBMPView, IID_IBMPView, Dispatch)
END_INTERFACE_MAP()


// CBMPView 생성/소멸입니다.

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

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
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
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다.  기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CView::OnFinalRelease();
}


// CBMPView 그리기입니다.

void CBMPView::OnDraw(CDC* pDC)
{
	Graphics graphicsDC(*pDC);					// gdi+ 그리기를 위한 객체
	CRect clientRect;
	GetClientRect(clientRect);
	Bitmap bmpCanvas(clientRect.right, clientRect.bottom);	// 캔버스 비트맵 생성
	Graphics graphicsCanvas(&bmpCanvas);		// 캔버스 그래픽스 생성
	graphicsCanvas.Clear(Color::Azure);			// 캔버스 배경색 지정

	CBMPDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 그리기 코드를 추가합니다.
	Bitmap* pBitmap = pDoc->m_bitmap;
	UINT gapW = abs(clientRect.Width() - (int)pBitmap->GetWidth());
	UINT gapH = abs(clientRect.Height() - (int)pBitmap->GetHeight());
	UINT gap = gapW > gapH ? gapW : gapH;

	if (pBitmap) {
		graphicsCanvas.DrawImage(pBitmap, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
	}

	graphicsDC.DrawImage(&bmpCanvas, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);	// 캔버스 그리기
}


// CBMPView 진단입니다.

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

//CBMPFrame* CBMPView::GetFrame() const	// 디버그되지 않은 버전은 인라인으로 지정됩니다.
//{
//	ASSERT(CView::GetParentFrame()->IsKindOf(RUNTIME_CLASS(CBMPFrame)));
//	return (CBMPFrame*)CView::GetParentFrame();
//}

CBMPDoc* CBMPView::GetDocument() const	// 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBMPDoc)));
	return (CBMPDoc*)m_pDocument;
}
#endif //_DEBUG


// CBMPView 메시지 처리기입니다.



// CBMPView 명령입니다.

void CBMPView::OnUpdateViewHtgSize(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(FALSE);
}
