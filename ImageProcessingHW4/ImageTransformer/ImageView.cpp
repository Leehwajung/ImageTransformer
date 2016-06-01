// ImageView.cpp : CImageView 클래스의 구현 파일입니다.
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CImageView, CView)
END_DISPATCH_MAP()

// 참고: IID_IImageView에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {615CB34A-1700-4769-91F1-130F47067028}
static const IID IID_IImageView =
{ 0x615CB34A, 0x1700, 0x4769,{ 0x91, 0xF1, 0x13, 0xF, 0x47, 0x6, 0x70, 0x28 } };

BEGIN_INTERFACE_MAP(CImageView, CView)
	INTERFACE_PART(CImageView, IID_IImageView, Dispatch)
END_INTERFACE_MAP()


// CImageView 생성/소멸입니다.

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

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
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
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다.  기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CView::OnFinalRelease();
}


// CImageView 그리기입니다.

void CImageView::OnDraw(CDC* pDC)
{
	Graphics graphicsDC(*pDC);					// gdi+ 그리기를 위한 객체
	CRect clientRect;
	GetClientRect(clientRect);
	Bitmap bmpCanvas(clientRect.right, clientRect.bottom);	// 캔버스 비트맵 생성
	Graphics graphicsCanvas(&bmpCanvas);		// 캔버스 그래픽스 생성
	graphicsCanvas.Clear(Color::Azure);			// 캔버스 배경색 지정

	CImageDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 그리기 코드를 추가합니다.
	Bitmap* pBitmap = pDoc->m_bitmap;

	if (pBitmap) {
		//graphicsCanvas.DrawImage(pBitmap, 0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
		graphicsCanvas.DrawImage(pBitmap, 0, 0, clientRect.Width(), clientRect.Height());
	}

	graphicsDC.DrawImage(&bmpCanvas, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);	// 캔버스 그리기
}


// CImageView 진단입니다.

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

//CImageFrame* CImageView::GetFrame() const	// 디버그되지 않은 버전은 인라인으로 지정됩니다.
//{
//	ASSERT(CView::GetParentFrame()->IsKindOf(RUNTIME_CLASS(CImageFrame)));
//	return (CImageFrame*)CView::GetParentFrame();
//}

CImageDoc* CImageView::GetDocument() const	// 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageDoc)));
	return (CImageDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageView 메시지 처리기입니다.



// CImageView 명령입니다.

void CImageView::OnUpdateViewHtgSize(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(FALSE);
}


BOOL CImageView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//return CView::OnEraseBkgnd(pDC);
	return FALSE;
}
