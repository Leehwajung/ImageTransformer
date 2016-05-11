// BMPFrm.cpp : CBMPFrame 클래스의 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageAreaProcessor.h"

#include "BMPFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "BMPView.h"
#include "BMPDoc.h"

#include "MainFrm.h"

#define PFX_EQUALIZATION	L"equalized_"
#define PFX_STRETCHING		L"stretched_"
#define PFX_NOISE			L"noisy_"
#define PFX_EDGE			L"edged_"
#define PFX_FILTER			L"filtered_"


// CBMPFrame

IMPLEMENT_DYNCREATE(CBMPFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CBMPFrame, CMDIChildWndEx)
	ON_WM_NCACTIVATE()
	ON_COMMAND(ID_PP_HE, &CBMPFrame::OnPpHistogramEqualization)
	ON_COMMAND(ID_PP_BCS, &CBMPFrame::OnPpBasicContrastStretching)
	ON_COMMAND(ID_PP_ECS, &CBMPFrame::OnPpEndsinContrastStretching)
	ON_COMMAND(ID_PP_ECSHIGH, &CBMPFrame::OnPpEcsHighEnd)
	ON_COMMAND(ID_PP_ECSLOW, &CBMPFrame::OnPpEcsLowEnd)
	ON_COMMAND(ID_VIEW_ORIGIN_SIZE, &CBMPFrame::OnViewOriginSize)
	ON_COMMAND(ID_NS_GSN, &CBMPFrame::OnNoiseGaussian)
	ON_COMMAND(ID_NS_SNR, &CBMPFrame::OnNoiseSNR)
	ON_COMMAND(ID_AP_RBT, &CBMPFrame::OnApRoberts)
	ON_COMMAND(ID_AP_SB, &CBMPFrame::OnApSobel)
	ON_COMMAND(ID_AP_PWT, &CBMPFrame::OnApPrewitt)
	ON_COMMAND(ID_AP_SG, &CBMPFrame::OnApStochasticGradient)
	ON_COMMAND(ID_AP_LP, &CBMPFrame::OnApLowPass)
	ON_COMMAND(ID_AP_MD, &CBMPFrame::OnApMedian)
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_ER_RBT, &CBMPFrame::OnErRoberts)
	ON_COMMAND(ID_ER_SB, &CBMPFrame::OnErSobel)
	ON_COMMAND(ID_ER_PWT, &CBMPFrame::OnErPrewitt)
	ON_COMMAND(ID_ER_SG, &CBMPFrame::OnErStochasticGradient)
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

	cs.cx = 20;
	cs.cy = 44;

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


//CBMPView* CBMPFrame::GetActiveView()
//{
//	return (CBMPView*)CFrameWnd::GetActiveView();
//}

CBMPDoc* CBMPFrame::GetActiveDocument()
{
	return (CBMPDoc*)CFrameWnd::GetActiveDocument();
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


// CBMPFrame 작업입니다.

void CBMPFrame::Duplicate(OUT CBMPFrame** frame, OUT CBMPView** view, OUT CBMPDoc** document)
{
	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;
	
	// 신규 BMP 문서 (CBMPDoc) 생성
	CDocTemplate *pTml = pSrcDoc->GetDocTemplate();
	pTml->OpenDocumentFile(NULL);

	// 기존 CBMPDoc으로부터 복제
	*frame = (CBMPFrame*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame();
	*view = (CBMPView*)(*frame)->GetActiveView();
	*document = (*view)->GetDocument();
	(*document)->copyFrom(pSrcDoc);
}

// Masking
void CBMPFrame::runEdgeDetection(Mask::Type maskType)
{
	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 BMP 문서 (CBMPDoc) 생성 및 복제
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Masking and Edge Detection
	pDstDoc->detectEdge(maskType);

	// 제목 변경
	CString newTitle(PFX_EDGE);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// 영상에 맞게 다시 그리기
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}


// CBMPFrame 메시지 처리기입니다.

void CBMPFrame::ActivateFrame(int nCmdShow)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CBMPDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect winRect, cliRect;
	GetWindowRect(&winRect);
	GetClientRect(&cliRect);

	Bitmap *pBitmap = pDoc->m_bitmap;
	if (pBitmap) {
		m_InitW = pBitmap->GetWidth() + winRect.Width() - cliRect.Width() + 4;
		m_InitH = pBitmap->GetHeight() + winRect.Height() - cliRect.Height() + 4;
		SetWindowPos(NULL, 0, 0, m_InitW, m_InitH, SWP_NOMOVE | SWP_SHOWWINDOW);
	}

	OnPpEcsHighEnd();
	OnPpEcsLowEnd();

	CMDIChildWndEx::ActivateFrame(nCmdShow);
}

BOOL CBMPFrame::OnNcActivate(BOOL bActive)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// Activate/Inactivate ribbon context category: 영상처리
	CMFCRibbonBar *pRibbon = ((CMainFrame*)GetTopLevelFrame())->GetRibbonBar();
	if (bActive) {
		pRibbon->ShowContextCategories(ID_IMAGEPROCESSING, TRUE);
		pRibbon->ActivateContextCategory(ID_IMAGEPROCESSING);
	}
	else {
		pRibbon->ShowContextCategories(ID_IMAGEPROCESSING, FALSE);
	}
	pRibbon->RecalcLayout();
	pRibbon->RedrawWindow();

	return CMDIChildWndEx::OnNcActivate(bActive);
}

void CBMPFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	lpMMI->ptMinTrackSize.x = m_InitW;
	lpMMI->ptMinTrackSize.y = m_InitH;
	lpMMI->ptMaxTrackSize.x = m_InitW;
	lpMMI->ptMaxTrackSize.y = m_InitH;

	CMDIChildWndEx::OnGetMinMaxInfo(lpMMI);
}


// CBMPFrame 명령입니다.

// Histogram Equalization
void CBMPFrame::OnPpHistogramEqualization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 BMP 문서 (CBMPDoc) 생성 및 복제
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);
	
	// Histogram Equalization
	pDstDoc->HistogramEqualization();

	// 제목 변경
	CString newTitle(PFX_EQUALIZATION);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// 영상에 맞게 다시 그리기
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}

// Basic Contrast Stretching
void CBMPFrame::OnPpBasicContrastStretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 BMP 문서 (CBMPDoc) 생성 및 복제
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Basic Contrast Stretching
	pDstDoc->BasicContrastStretching();

	// 제목 변경
	CString newTitle(PFX_STRETCHING);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// 영상에 맞게 다시 그리기
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}

// Ends-in Contrast Stretching
void CBMPFrame::OnPpEndsinContrastStretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 BMP 문서 (CBMPDoc) 생성 및 복제
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Basic Contrast Stretching
	pDstDoc->EndsinContrastStretching(m_bEcsLowEnd, m_bEcsHighEnd);

	// 제목 변경
	CString newTitle(PFX_STRETCHING);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// 영상에 맞게 다시 그리기
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}

// Ends-in Contrast Stretching의 최고값 설정
void CBMPFrame::OnPpEcsHighEnd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_PP_ECSHIGH));
	if (pSpin != NULL) {
		m_bEcsHighEnd = (BYTE)_wtof(pSpin->GetEditText());
	}
}

// Ends-in Contrast Stretching의 최저값 설정
void CBMPFrame::OnPpEcsLowEnd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_PP_ECSLOW));
	if (pSpin != NULL) {
		m_bEcsLowEnd = (BYTE)_wtof(pSpin->GetEditText());
	}
}

void CBMPFrame::OnViewOriginSize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	ActivateFrame();
}


// Gaussian Noise
void CBMPFrame::OnNoiseGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 BMP 문서 (CBMPDoc) 생성 및 복제
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Add Gaussian Noise
	OnNoiseSNR();	// get SNR
	pDstDoc->GaussianNoise(m_snr);

	// 제목 변경
	CString newTitle(PFX_NOISE);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// 영상에 맞게 다시 그리기
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}

// SNR of Gaussian Noise
void CBMPFrame::OnNoiseSNR()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_NS_SNR));
	if (pSpin != NULL) {
		m_snr = _wtof(pSpin->GetEditText());
	}
}

// Roberts Masking
void CBMPFrame::OnApRoberts()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	runEdgeDetection(Mask::Roberts);
}

// Sobel Masking
void CBMPFrame::OnApSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	runEdgeDetection(Mask::Sobel);
}

// Prewitt Masking
void CBMPFrame::OnApPrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	runEdgeDetection(Mask::Prewitt);
}

// Stochastic Gradient Masking
void CBMPFrame::OnApStochasticGradient()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	runEdgeDetection(Mask::StochasticGradient);
}

// Low-pass Filtering
void CBMPFrame::OnApLowPass()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 BMP 문서 (CBMPDoc) 생성 및 복제
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Low-pass Filtering
	pDstDoc->LowPassFiltering(3);

	// 제목 변경
	CString newTitle(PFX_FILTER);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// 영상에 맞게 다시 그리기
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}

// Median Filtering
void CBMPFrame::OnApMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 BMP 문서 (CBMPDoc) 생성 및 복제
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Median Filtering
	pDstDoc->MedianFiltering(3);

	// 제목 변경
	CString newTitle(PFX_FILTER);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// 영상에 맞게 다시 그리기
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}


void CBMPFrame::OnErRoberts()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// Get Error Rate of Roberts
	OnNoiseSNR();	// get SNR
	const double errorRate = pSrcDoc->getErrorRate(Mask::Roberts, m_snr);

	// 메시박스 띄우기
	CString msg;
	msg.Format(_T("에러율: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}

void CBMPFrame::OnErSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// Get Error Rate of Sobel
	OnNoiseSNR();	// get SNR
	const double errorRate = pSrcDoc->getErrorRate(Mask::Sobel, m_snr);

	// 메시박스 띄우기
	CString msg;
	msg.Format(_T("에러율: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}

void CBMPFrame::OnErPrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// Get Error Rate of Prewitt
	OnNoiseSNR();	// get SNR
	const double errorRate = pSrcDoc->getErrorRate(Mask::Prewitt, m_snr);

	// 메시박스 띄우기
	CString msg;
	msg.Format(_T("에러율: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}

void CBMPFrame::OnErStochasticGradient()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CBMPDoc을 가져옴
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// Get Error Rate of Stochastic Gradient
	OnNoiseSNR();	// get SNR
	const double errorRate = pSrcDoc->getErrorRate(Mask::StochasticGradient, m_snr);

	// 메시박스 띄우기
	CString msg;
	msg.Format(_T("에러율: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}
