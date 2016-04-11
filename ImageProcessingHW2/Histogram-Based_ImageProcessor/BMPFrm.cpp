// BMPFrm.cpp : CBMPFrame 클래스의 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageProcessor.h"

#include "BMPFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "BMPDoc.h"

#include "MainFrm.h"


// CBMPFrame

IMPLEMENT_DYNCREATE(CBMPFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CBMPFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CBMPFrame, CMDIChildWndEx)
END_DISPATCH_MAP()

// 참고: IID_IBMPFrame에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {49124138-EE54-4C37-9CB5-71965CF97CC5}
static const IID IID_IBMPFrame =
{ 0x49124138, 0xEE54, 0x4C37,{ 0x9C, 0xB5, 0x71, 0x96, 0x5C, 0xF9, 0x7C, 0xC5 } };

BEGIN_INTERFACE_MAP(CBMPFrame, CMDIChildWndEx)
	INTERFACE_PART(CBMPFrame, IID_IBMPFrame, Dispatch)
END_INTERFACE_MAP()


// CBMPFrame 생성/소멸입니다.

CBMPFrame::CBMPFrame()
{

	EnableAutomation();
}

CBMPFrame::~CBMPFrame()
{
}


BOOL CBMPFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

void CBMPFrame::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다.  기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CMDIChildWndEx::OnFinalRelease();
}


// CBMPFrame 진단입니다.

#ifdef _DEBUG
void CBMPFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CBMPFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG


// CBMPFrame 메시지 처리기입니다.

void CBMPFrame::ActivateFrame(int nCmdShow)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDocument* pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	CRect winRect, cliRect;
	GetWindowRect(&winRect);
	GetClientRect(&cliRect);

	CSize sizeImg;


	if (pDoc->IsKindOf(RUNTIME_CLASS(CBMPDoc))) {
		sizeImg.cx = ((CBMPDoc*)pDoc)->m_bitmap->GetWidth();
		sizeImg.cy = ((CBMPDoc*)pDoc)->m_bitmap->GetHeight();
		int cx = sizeImg.cx + winRect.Width() - cliRect.Width() + 4;
		int cy = sizeImg.cy + winRect.Height() - cliRect.Height() + 4;


		SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_SHOWWINDOW);
	}

	CMFCRibbonBar* rBar = ((CMainFrame*)GetTopLevelFrame())->GetRibbonBar();
	rBar->ShowContextCategories(ID_IMAGEPROCESSING, TRUE);
	rBar->ActivateContextCategory(ID_IMAGEPROCESSING);

	// 이후 반드시 호출
	rBar->RecalcLayout();
	rBar->RedrawWindow();

	SendMessage(WM_NCPAINT, 0, 0);

	CMDIChildWndEx::ActivateFrame(nCmdShow);
}
