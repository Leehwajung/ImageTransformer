// ImageFrm.cpp : CImageFrame 클래스의 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTransformer.h"

#include "ImageFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ImageView.h"
#include "ImageDoc.h"
#include "HistogramFrm.h"
#include "HistogramView.h"
#include "HistogramDoc.h"
#include "SpectrumFrm.h"
#include "SpectrumView.h"
#include "SpectrumDoc.h"

#include "MainFrm.h"

#define PFX_HISTOGRAM		L"histogram_of_"
#define PFX_EQUALIZATION	L"equalized_"
#define PFX_STRETCHING		L"stretched_"
#define PFX_NOISE			L"noisy_"
#define PFX_EDGE			L"edged_"
#define PFX_FILTER			L"filtered_"
#define PFX_TRANSFORM		L"transformed_"

#define FILTER_WIDTH		3


// CImageFrame

IMPLEMENT_DYNCREATE(CImageFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CImageFrame, CMDIChildWndEx)
	ON_WM_NCACTIVATE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_HTG_PLOT, &CImageFrame::OnHtgPlot)
	ON_COMMAND(ID_PP_HE, &CImageFrame::OnPpHistogramEqualization)
	ON_COMMAND(ID_PP_BCS, &CImageFrame::OnPpBasicContrastStretching)
	ON_COMMAND(ID_PP_ECS, &CImageFrame::OnPpEndsinContrastStretching)
	ON_COMMAND(ID_PP_ECSHIGH, &CImageFrame::OnPpEcsHighEnd)
	ON_COMMAND(ID_PP_ECSLOW, &CImageFrame::OnPpEcsLowEnd)
	ON_COMMAND(ID_VIEW_ORIGIN_SIZE, &CImageFrame::OnViewOriginSize)
	ON_COMMAND(ID_NS_GSN, &CImageFrame::OnNoiseGaussian)
	ON_COMMAND(ID_NS_SNR, &CImageFrame::OnNoiseSNR)
	ON_COMMAND(ID_AP_RBT, &CImageFrame::OnApRoberts)
	ON_COMMAND(ID_AP_SB, &CImageFrame::OnApSobel)
	ON_COMMAND(ID_AP_PWT, &CImageFrame::OnApPrewitt)
	ON_COMMAND(ID_AP_SG, &CImageFrame::OnApStochasticGradient)
	ON_COMMAND(ID_AP_LP, &CImageFrame::OnApLowPass)
	ON_COMMAND(ID_AP_MD, &CImageFrame::OnApMedian)
	ON_COMMAND(ID_ER_RBT, &CImageFrame::OnErRoberts)
	ON_COMMAND(ID_ER_SB, &CImageFrame::OnErSobel)
	ON_COMMAND(ID_ER_PWT, &CImageFrame::OnErPrewitt)
	ON_COMMAND(ID_ER_SG, &CImageFrame::OnErStochasticGradient)
	ON_COMMAND(ID_ER_LP, &CImageFrame::OnErLowPass)
	ON_COMMAND(ID_ER_MD, &CImageFrame::OnErMedian)
	ON_COMMAND(ID_IT_FDCT, &CImageFrame::OnItForwardDCT)
	ON_COMMAND(ID_IT_MASK, &CImageFrame::OnItMaskWidth)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CImageFrame, CMDIChildWndEx)
END_DISPATCH_MAP()

// 참고: IID_IImageFrame에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {49124138-EE54-4C37-9CB5-71965CF97CC5}
static const IID IID_IImageFrame =
{ 0x49124138, 0xEE54, 0x4C37,{ 0x9C, 0xB5, 0x71, 0x96, 0x5C, 0xF9, 0x7C, 0xC5 } };

BEGIN_INTERFACE_MAP(CImageFrame, CMDIChildWndEx)
	INTERFACE_PART(CImageFrame, IID_IImageFrame, Dispatch)
END_INTERFACE_MAP()


// CImageFrame 생성/소멸입니다.

CImageFrame::CImageFrame()
{
	EnableAutomation();
}

CImageFrame::~CImageFrame()
{
}


BOOL CImageFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.cx = 512 + 20;
	cs.cy = 512 + 44;

	return TRUE;
}

void CImageFrame::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다.  기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CMDIChildWndEx::OnFinalRelease();
}


//CImageView* CImageFrame::GetActiveView()
//{
//	return (CImageView*)CFrameWnd::GetActiveView();
//}

CImageDoc* CImageFrame::GetActiveDocument()
{
	return (CImageDoc*)CFrameWnd::GetActiveDocument();
}


// CImageFrame 진단입니다.

#ifdef _DEBUG
void CImageFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CImageFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG


// CImageFrame 작업입니다.

void CImageFrame::Duplicate(OUT CImageFrame** frame, OUT CImageView** view, OUT CImageDoc** document)
{
	// 기존 CImageDoc을 가져옴
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;
	
	// 신규 Image 문서 (CImageDoc) 생성
	CDocTemplate *pTml = pSrcDoc->GetDocTemplate();
	pTml->OpenDocumentFile(NULL);

	// 기존 CImageDoc으로부터 복제
	*frame = (CImageFrame*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame();
	*view = (CImageView*)(*frame)->GetActiveView();
	*document = (*view)->GetDocument();
	(*document)->copyFrom(pSrcDoc);
}

// Masking
void CImageFrame::runEdgeDetection(Mask::Type maskType)
{
	// 기존 CImageDoc을 가져옴
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 Image 문서 (CImageDoc) 생성 및 복제
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
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


// CImageFrame 메시지 처리기입니다.

void CImageFrame::ActivateFrame(int nCmdShow)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CImageDoc *pDoc = GetActiveDocument();
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

BOOL CImageFrame::OnNcActivate(BOOL bActive)
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

void CImageFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//lpMMI->ptMinTrackSize.x = m_InitW;
	//lpMMI->ptMinTrackSize.y = m_InitH;
	//lpMMI->ptMaxTrackSize.x = m_InitW;
	//lpMMI->ptMaxTrackSize.y = m_InitH;

	CMDIChildWndEx::OnGetMinMaxInfo(lpMMI);
}


// CImageFrame 명령입니다.

// Plot Histogram
void CImageFrame::OnHtgPlot()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CImageDoc을 가져옴
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 신규 Historam 문서 (CHistogramDoc) 생성
	CImageTransformerApp *app = (CImageTransformerApp*)AfxGetApp();
	POSITION pos = app->GetFirstDocTemplatePosition();
	CDocTemplate *pTml;
	for (int i = 0; i < 3; i++) {
		pTml = app->GetNextDocTemplate(pos);
	}
	pTml->OpenDocumentFile(NULL);

	// Destination(Historam)을 가져옴
	CMainFrame *pMainFrm = (CMainFrame*)(AfxGetMainWnd());					// Main Frame
	CHistogramFrame *pHtgFrm = (CHistogramFrame*)pMainFrm->MDIGetActive();	// Histogram Frame
	CHistogramView *pHtgView = (CHistogramView*)(pHtgFrm->GetActiveView());	// Histogram View
	CHistogramDoc *pHtgDoc = pHtgView->GetDocument();						// Histogram Document

	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = pDoc->getData(&bitmapData, ImageLockModeRead);	//영상의 픽셀 데이터를 가져옴

	// Source의 픽셀 데이터를 기반으로하여 Destination에 Histogram 생성
	pHtgDoc->plotHistogram(pixelData, bitmapData.Width * bitmapData.Height);
	pHtgView->plotHistogramImage();
	pDoc->clearData(&bitmapData);

	// 제목 변경
	CString newTitle(PFX_HISTOGRAM);
	newTitle.Append(pDoc->GetTitle());
	pHtgDoc->SetTitle(newTitle);

	// 영상에 맞게 다시 그리기
	pHtgFrm->ActivateFrame();
	pHtgView->Invalidate();
}

// Histogram Equalization
void CImageFrame::OnPpHistogramEqualization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CImageDoc을 가져옴
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 Image 문서 (CImageDoc) 생성 및 복제
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
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
void CImageFrame::OnPpBasicContrastStretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CImageDoc을 가져옴
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 Image 문서 (CImageDoc) 생성 및 복제
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
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
void CImageFrame::OnPpEndsinContrastStretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CImageDoc을 가져옴
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 Image 문서 (CImageDoc) 생성 및 복제
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
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
void CImageFrame::OnPpEcsHighEnd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_PP_ECSHIGH));
	if (pSpin != NULL) {
		m_bEcsHighEnd = (BYTE)_wtof(pSpin->GetEditText());
	}
}

// Ends-in Contrast Stretching의 최저값 설정
void CImageFrame::OnPpEcsLowEnd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_PP_ECSLOW));
	if (pSpin != NULL) {
		m_bEcsLowEnd = (BYTE)_wtof(pSpin->GetEditText());
	}
}

void CImageFrame::OnViewOriginSize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	ActivateFrame();
}


// Gaussian Noise
void CImageFrame::OnNoiseGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CImageDoc을 가져옴
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 Image 문서 (CImageDoc) 생성 및 복제
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
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
void CImageFrame::OnNoiseSNR()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_NS_SNR));
	if (pSpin != NULL) {
		m_snr = _wtof(pSpin->GetEditText());
	}
}

// Roberts Masking
void CImageFrame::OnApRoberts()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	runEdgeDetection(Mask::Roberts);
}

// Sobel Masking
void CImageFrame::OnApSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	runEdgeDetection(Mask::Sobel);
}

// Prewitt Masking
void CImageFrame::OnApPrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	runEdgeDetection(Mask::Prewitt);
}

// Stochastic Gradient Masking
void CImageFrame::OnApStochasticGradient()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	runEdgeDetection(Mask::StochasticGradient);
}

// Low-pass Filtering
void CImageFrame::OnApLowPass()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CImageDoc을 가져옴
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 Image 문서 (CImageDoc) 생성 및 복제
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Low-pass Filtering
	pDstDoc->LowPassFiltering(FILTER_WIDTH);

	// 제목 변경
	CString newTitle(PFX_FILTER);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// 영상에 맞게 다시 그리기
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}

// Median Filtering
void CImageFrame::OnApMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CImageDoc을 가져옴
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// 신규 Image 문서 (CImageDoc) 생성 및 복제
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Median Filtering
	pDstDoc->MedianFiltering(FILTER_WIDTH);

	// 제목 변경
	CString newTitle(PFX_FILTER);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// 영상에 맞게 다시 그리기
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}


void CImageFrame::OnErRoberts()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// CImageDoc을 가져옴
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Error Rate of Roberts
	OnNoiseSNR();	// get SNR
	const double errorRate = pDoc->getErrorRate(Mask::Roberts, m_snr);

	// 메시박스 띄우기
	CString msg;
	msg.Format(_T("에러율: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}

void CImageFrame::OnErSobel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// CImageDoc을 가져옴
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Error Rate of Sobel
	OnNoiseSNR();	// get SNR
	const double errorRate = pDoc->getErrorRate(Mask::Sobel, m_snr);

	// 메시박스 띄우기
	CString msg;
	msg.Format(_T("에러율: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}

void CImageFrame::OnErPrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// CImageDoc을 가져옴
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Error Rate of Prewitt
	OnNoiseSNR();	// get SNR
	const double errorRate = pDoc->getErrorRate(Mask::Prewitt, m_snr);

	// 메시박스 띄우기
	CString msg;
	msg.Format(_T("에러율: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}

void CImageFrame::OnErStochasticGradient()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// CImageDoc을 가져옴
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Error Rate of Stochastic Gradient
	OnNoiseSNR();	// get SNR
	const double errorRate = pDoc->getErrorRate(Mask::StochasticGradient, m_snr);

	// 메시박스 띄우기
	CString msg;
	msg.Format(_T("에러율: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}

void CImageFrame::OnErLowPass()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// CImageDoc을 가져옴
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Mean Square Error
	OnNoiseSNR();	// get SNR
	double mse = pDoc->getMeanSquareError(0, m_snr, FILTER_WIDTH);

	// 메시박스 띄우기
	CString msg;
	msg.Format(_T("MSE: %f"), mse);
	MessageBox(msg, _T("Mean Square Error"));
}

void CImageFrame::OnErMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// CImageDoc을 가져옴
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Mean Square Error
	OnNoiseSNR();	// get SNR
	double mse = pDoc->getMeanSquareError(1, m_snr, FILTER_WIDTH);

	// 메시박스 띄우기
	CString msg;
	msg.Format(_T("MSE: %f"), mse);
	MessageBox(msg, _T("Mean Square Error"));
}

void CImageFrame::OnItForwardDCT()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 기존 CImageDoc을 가져옴
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	OnItMaskWidth();

	// 신규 Spectrum 문서 (CSpectrumDoc) 생성
	CImageTransformerApp *app = (CImageTransformerApp*)AfxGetApp();
	POSITION pos = app->GetFirstDocTemplatePosition();
	CDocTemplate *pTml;
	for (int i = 0; i < 4; i++) {
		pTml = app->GetNextDocTemplate(pos);
	}
	pTml->OpenDocumentFile(NULL);

	// Destination(Spectrum)을 가져옴
	CMainFrame *pMainFrm = (CMainFrame*)(AfxGetMainWnd());					// Main Frame
	CSpectrumFrame *pStmFrm = (CSpectrumFrame*)pMainFrm->MDIGetActive();	// Spectrum Frame
	CSpectrumView *pStmView = (CSpectrumView*)(pStmFrm->GetActiveView());	// Spectrum View
	CSpectrumDoc *pStmDoc = pStmView->GetDocument();						// Spectrum Document

	// 영상의 pixel data를 가져옴
	BitmapData bitmapData;
	BYTE *pixelData = pDoc->getData(&bitmapData, ImageLockModeRead);	//영상의 픽셀 데이터를 가져옴
	pStmFrm->m_PrevFrame = this;		// 이전 프레임 정보 저장

	// Forward Discrete Cosine Transform
	pStmDoc->forwardDiscreteCosineTransform(pixelData, bitmapData.Height, bitmapData.Width, m_TransformMaskWidth);
	pDoc->clearData(&bitmapData);

	// 제목 변경
	CString newTitle(PFX_TRANSFORM);
	newTitle.Append(pDoc->GetTitle());
	pStmDoc->SetTitle(newTitle);

	// 영상에 맞게 다시 그리기
	pStmFrm->ActivateFrame();
	pStmView->Invalidate();
}

void CImageFrame::OnItMaskWidth()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_IT_MASK));
	if (pSpin != NULL) {
		m_TransformMaskWidth = (UINT)_wtof(pSpin->GetEditText());
	}
}

