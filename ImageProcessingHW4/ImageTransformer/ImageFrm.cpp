// ImageFrm.cpp : CImageFrame Ŭ������ ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTransformer.h"

#include "ImageFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ImageView.h"
#include "ImageDoc.h"
#include "SpectrumFrm.h"
#include "SpectrumView.h"
#include "SpectrumDoc.h"

#include "MainFrm.h"

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
	ON_COMMAND(ID_IT_IDCT, &CImageFrame::OnItInverseDCT)
	ON_COMMAND(ID_IT_MASK, &CImageFrame::OnItMaskWidth)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CImageFrame, CMDIChildWndEx)
END_DISPATCH_MAP()

// ����: IID_IImageFrame�� ���� ������ �߰��Ͽ�
//  VBA���� ���� ���� ���ε��� �����մϴ�. 
//  �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {49124138-EE54-4C37-9CB5-71965CF97CC5}
static const IID IID_IImageFrame =
{ 0x49124138, 0xEE54, 0x4C37,{ 0x9C, 0xB5, 0x71, 0x96, 0x5C, 0xF9, 0x7C, 0xC5 } };

BEGIN_INTERFACE_MAP(CImageFrame, CMDIChildWndEx)
	INTERFACE_PART(CImageFrame, IID_IImageFrame, Dispatch)
END_INTERFACE_MAP()


// CImageFrame ����/�Ҹ��Դϴ�.

CImageFrame::CImageFrame()
{
	EnableAutomation();
}

CImageFrame::~CImageFrame()
{
}


BOOL CImageFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.cx = 20;
	cs.cy = 44;

	return TRUE;
}

void CImageFrame::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ�
	// OnFinalRelease�� ȣ��˴ϴ�.  �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

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


// CImageFrame �����Դϴ�.

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


// CImageFrame �۾��Դϴ�.

void CImageFrame::Duplicate(OUT CImageFrame** frame, OUT CImageView** view, OUT CImageDoc** document)
{
	// ���� CImageDoc�� ������
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;
	
	// �ű� Image ���� (CImageDoc) ����
	CDocTemplate *pTml = pSrcDoc->GetDocTemplate();
	pTml->OpenDocumentFile(NULL);

	// ���� CImageDoc���κ��� ����
	*frame = (CImageFrame*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame();
	*view = (CImageView*)(*frame)->GetActiveView();
	*document = (*view)->GetDocument();
	(*document)->copyFrom(pSrcDoc);
}

// Masking
void CImageFrame::runEdgeDetection(Mask::Type maskType)
{
	// ���� CImageDoc�� ������
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� Image ���� (CImageDoc) ���� �� ����
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Masking and Edge Detection
	pDstDoc->detectEdge(maskType);

	// ���� ����
	CString newTitle(PFX_EDGE);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// ���� �°� �ٽ� �׸���
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}


// CImageFrame �޽��� ó�����Դϴ�.

void CImageFrame::ActivateFrame(int nCmdShow)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// Activate/Inactivate ribbon context category: ����ó��
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	lpMMI->ptMinTrackSize.x = m_InitW;
	lpMMI->ptMinTrackSize.y = m_InitH;
	lpMMI->ptMaxTrackSize.x = m_InitW;
	lpMMI->ptMaxTrackSize.y = m_InitH;

	CMDIChildWndEx::OnGetMinMaxInfo(lpMMI);
}


// CImageFrame ����Դϴ�.

// Histogram Equalization
void CImageFrame::OnPpHistogramEqualization()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CImageDoc�� ������
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� Image ���� (CImageDoc) ���� �� ����
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);
	
	// Histogram Equalization
	pDstDoc->HistogramEqualization();

	// ���� ����
	CString newTitle(PFX_EQUALIZATION);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// ���� �°� �ٽ� �׸���
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}

// Basic Contrast Stretching
void CImageFrame::OnPpBasicContrastStretching()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CImageDoc�� ������
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� Image ���� (CImageDoc) ���� �� ����
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Basic Contrast Stretching
	pDstDoc->BasicContrastStretching();

	// ���� ����
	CString newTitle(PFX_STRETCHING);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// ���� �°� �ٽ� �׸���
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}

// Ends-in Contrast Stretching
void CImageFrame::OnPpEndsinContrastStretching()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CImageDoc�� ������
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� Image ���� (CImageDoc) ���� �� ����
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Basic Contrast Stretching
	pDstDoc->EndsinContrastStretching(m_bEcsLowEnd, m_bEcsHighEnd);

	// ���� ����
	CString newTitle(PFX_STRETCHING);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// ���� �°� �ٽ� �׸���
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}

// Ends-in Contrast Stretching�� �ְ� ����
void CImageFrame::OnPpEcsHighEnd()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_PP_ECSHIGH));
	if (pSpin != NULL) {
		m_bEcsHighEnd = (BYTE)_wtof(pSpin->GetEditText());
	}
}

// Ends-in Contrast Stretching�� ������ ����
void CImageFrame::OnPpEcsLowEnd()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_PP_ECSLOW));
	if (pSpin != NULL) {
		m_bEcsLowEnd = (BYTE)_wtof(pSpin->GetEditText());
	}
}

void CImageFrame::OnViewOriginSize()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	ActivateFrame();
}


// Gaussian Noise
void CImageFrame::OnNoiseGaussian()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CImageDoc�� ������
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� Image ���� (CImageDoc) ���� �� ����
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Add Gaussian Noise
	OnNoiseSNR();	// get SNR
	pDstDoc->GaussianNoise(m_snr);

	// ���� ����
	CString newTitle(PFX_NOISE);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// ���� �°� �ٽ� �׸���
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}

// SNR of Gaussian Noise
void CImageFrame::OnNoiseSNR()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_NS_SNR));
	if (pSpin != NULL) {
		m_snr = _wtof(pSpin->GetEditText());
	}
}

// Roberts Masking
void CImageFrame::OnApRoberts()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	runEdgeDetection(Mask::Roberts);
}

// Sobel Masking
void CImageFrame::OnApSobel()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	runEdgeDetection(Mask::Sobel);
}

// Prewitt Masking
void CImageFrame::OnApPrewitt()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	runEdgeDetection(Mask::Prewitt);
}

// Stochastic Gradient Masking
void CImageFrame::OnApStochasticGradient()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	runEdgeDetection(Mask::StochasticGradient);
}

// Low-pass Filtering
void CImageFrame::OnApLowPass()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CImageDoc�� ������
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� Image ���� (CImageDoc) ���� �� ����
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Low-pass Filtering
	pDstDoc->LowPassFiltering(FILTER_WIDTH);

	// ���� ����
	CString newTitle(PFX_FILTER);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// ���� �°� �ٽ� �׸���
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}

// Median Filtering
void CImageFrame::OnApMedian()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CImageDoc�� ������
	CImageDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� Image ���� (CImageDoc) ���� �� ����
	CImageFrame* pDstFrm;
	CImageView* pDstView;
	CImageDoc* pDstDoc;
	Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	// Median Filtering
	pDstDoc->MedianFiltering(FILTER_WIDTH);

	// ���� ����
	CString newTitle(PFX_FILTER);
	newTitle.Append(pSrcDoc->GetTitle());
	pDstDoc->SetTitle(newTitle);

	// ���� �°� �ٽ� �׸���
	pDstFrm->ActivateFrame();
	pDstView->Invalidate();
}


void CImageFrame::OnErRoberts()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CImageDoc�� ������
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Error Rate of Roberts
	OnNoiseSNR();	// get SNR
	const double errorRate = pDoc->getErrorRate(Mask::Roberts, m_snr);

	// �޽ùڽ� ����
	CString msg;
	msg.Format(_T("������: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}

void CImageFrame::OnErSobel()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CImageDoc�� ������
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Error Rate of Sobel
	OnNoiseSNR();	// get SNR
	const double errorRate = pDoc->getErrorRate(Mask::Sobel, m_snr);

	// �޽ùڽ� ����
	CString msg;
	msg.Format(_T("������: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}

void CImageFrame::OnErPrewitt()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CImageDoc�� ������
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Error Rate of Prewitt
	OnNoiseSNR();	// get SNR
	const double errorRate = pDoc->getErrorRate(Mask::Prewitt, m_snr);

	// �޽ùڽ� ����
	CString msg;
	msg.Format(_T("������: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}

void CImageFrame::OnErStochasticGradient()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CImageDoc�� ������
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Error Rate of Stochastic Gradient
	OnNoiseSNR();	// get SNR
	const double errorRate = pDoc->getErrorRate(Mask::StochasticGradient, m_snr);

	// �޽ùڽ� ����
	CString msg;
	msg.Format(_T("������: %f"), errorRate);
	MessageBox(msg, _T("Error Rate"));
}

void CImageFrame::OnErLowPass()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CImageDoc�� ������
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Mean Square Error
	OnNoiseSNR();	// get SNR
	double mse = pDoc->getMeanSquareError(0, m_snr, FILTER_WIDTH);

	// �޽ùڽ� ����
	CString msg;
	msg.Format(_T("MSE: %f"), mse);
	MessageBox(msg, _T("Mean Square Error"));
}

void CImageFrame::OnErMedian()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CImageDoc�� ������
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Mean Square Error
	OnNoiseSNR();	// get SNR
	double mse = pDoc->getMeanSquareError(1, m_snr, FILTER_WIDTH);

	// �޽ùڽ� ����
	CString msg;
	msg.Format(_T("MSE: %f"), mse);
	MessageBox(msg, _T("Mean Square Error"));
}

void CImageFrame::OnItForwardDCT()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CImageDoc�� ������
	CImageDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	OnItMaskWidth();

	// �ű� Spectrum ���� (CSpectrumDoc) ����
	CImageTransformerApp *app = (CImageTransformerApp*)AfxGetApp();
	POSITION pos = app->GetFirstDocTemplatePosition();
	CDocTemplate *pTml;
	for (int i = 0; i < 4; i++) {
		pTml = app->GetNextDocTemplate(pos);
	}
	pTml->OpenDocumentFile(NULL);

	// Destination(Spectrum)�� ������
	CMainFrame *pMainFrm = (CMainFrame*)(AfxGetMainWnd());		// Main Frame
	CSpectrumFrame *pStmFrm = (CSpectrumFrame*)pMainFrm->MDIGetActive();	// Histogram Frame
	CSpectrumView *pStmView = (CSpectrumView*)(pStmFrm->GetActiveView());	// Histogram View
	CSpectrumDoc *pStmDoc = pStmView->GetDocument();						// Histogram Document

	// ������ pixel data�� ������
	BitmapData bitmapData;
	BYTE *pixelData = pDoc->getData(&bitmapData, ImageLockModeRead);	//������ �ȼ� �����͸� ������

	// Forward Discrete Cosine Transform
	pStmDoc->forwardDiscreteCosineTransform(pixelData, bitmapData.Height, bitmapData.Width, m_TransformMaskWidth);
	pDoc->clearData(&bitmapData);

	// ���� ����
	CString newTitle(PFX_TRANSFORM);
	newTitle.Append(pDoc->GetTitle());
	pStmDoc->SetTitle(newTitle);

	// ���� �°� �ٽ� �׸���
	pStmFrm->ActivateFrame();
	pStmView->Invalidate();
	Invalidate();
}

void CImageFrame::OnItInverseDCT()
{
	//// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	//// ���� CImageDoc�� ������
	//CImageDoc *pSrcDoc = GetActiveDocument();
	//ASSERT_VALID(pSrcDoc);
	//if (!pSrcDoc)
	//	return;

	//// �ű� Image ���� (CImageDoc) ���� �� ����
	//CImageFrame* pDstFrm;
	//CImageView* pDstView;
	//CImageDoc* pDstDoc;
	//Duplicate(&pDstFrm, &pDstView, &pDstDoc);

	//// Masking and Edge Detection
	//OnItMaskWidth();
	//pDstDoc->inverseDiscreteCosineTransform(m_TransformMaskWidth);

	//// ���� ����
	//CString newTitle(PFX_TRANSFORM);
	//newTitle.Append(pSrcDoc->GetTitle());
	//pDstDoc->SetTitle(newTitle);

	//// ���� �°� �ٽ� �׸���
	//pDstFrm->ActivateFrame();
	//pDstView->Invalidate();
}

void CImageFrame::OnItMaskWidth()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_IT_MASK));
	if (pSpin != NULL) {
		m_TransformMaskWidth = (UINT)_wtof(pSpin->GetEditText());
	}
}
