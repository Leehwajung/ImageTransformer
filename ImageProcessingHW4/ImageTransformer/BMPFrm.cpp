// BMPFrm.cpp : CBMPFrame Ŭ������ ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTransformer.h"

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

#define FILTER_WIDTH		3


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
	ON_COMMAND(ID_ER_LP, &CBMPFrame::OnErLowPass)
	ON_COMMAND(ID_ER_MD, &CBMPFrame::OnErMedian)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CBMPFrame, CMDIChildWndEx)
END_DISPATCH_MAP()

// ����: IID_IBMPFrame�� ���� ������ �߰��Ͽ�
//  VBA���� ���� ���� ���ε��� �����մϴ�. 
//  �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {49124138-EE54-4C37-9CB5-71965CF97CC5}
static const IID IID_IBMPFrame =
{ 0x49124138, 0xEE54, 0x4C37,{ 0x9C, 0xB5, 0x71, 0x96, 0x5C, 0xF9, 0x7C, 0xC5 } };

BEGIN_INTERFACE_MAP(CBMPFrame, CMDIChildWndEx)
	INTERFACE_PART(CBMPFrame, IID_IBMPFrame, Dispatch)
END_INTERFACE_MAP()


// CBMPFrame ����/�Ҹ��Դϴ�.

CBMPFrame::CBMPFrame()
{
	EnableAutomation();
}

CBMPFrame::~CBMPFrame()
{
}


BOOL CBMPFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.cx = 20;
	cs.cy = 44;

	return TRUE;
}

void CBMPFrame::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ�
	// OnFinalRelease�� ȣ��˴ϴ�.  �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

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


// CBMPFrame �����Դϴ�.

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


// CBMPFrame �۾��Դϴ�.

void CBMPFrame::Duplicate(OUT CBMPFrame** frame, OUT CBMPView** view, OUT CBMPDoc** document)
{
	// ���� CBMPDoc�� ������
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;
	
	// �ű� BMP ���� (CBMPDoc) ����
	CDocTemplate *pTml = pSrcDoc->GetDocTemplate();
	pTml->OpenDocumentFile(NULL);

	// ���� CBMPDoc���κ��� ����
	*frame = (CBMPFrame*)((CMainFrame*)AfxGetMainWnd())->GetActiveFrame();
	*view = (CBMPView*)(*frame)->GetActiveView();
	*document = (*view)->GetDocument();
	(*document)->copyFrom(pSrcDoc);
}

// Masking
void CBMPFrame::runEdgeDetection(Mask::Type maskType)
{
	// ���� CBMPDoc�� ������
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� BMP ���� (CBMPDoc) ���� �� ����
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
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


// CBMPFrame �޽��� ó�����Դϴ�.

void CBMPFrame::ActivateFrame(int nCmdShow)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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

void CBMPFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	lpMMI->ptMinTrackSize.x = m_InitW;
	lpMMI->ptMinTrackSize.y = m_InitH;
	lpMMI->ptMaxTrackSize.x = m_InitW;
	lpMMI->ptMaxTrackSize.y = m_InitH;

	CMDIChildWndEx::OnGetMinMaxInfo(lpMMI);
}


// CBMPFrame ����Դϴ�.

// Histogram Equalization
void CBMPFrame::OnPpHistogramEqualization()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CBMPDoc�� ������
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� BMP ���� (CBMPDoc) ���� �� ����
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
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
void CBMPFrame::OnPpBasicContrastStretching()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CBMPDoc�� ������
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� BMP ���� (CBMPDoc) ���� �� ����
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
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
void CBMPFrame::OnPpEndsinContrastStretching()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CBMPDoc�� ������
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� BMP ���� (CBMPDoc) ���� �� ����
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
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
void CBMPFrame::OnPpEcsHighEnd()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_PP_ECSHIGH));
	if (pSpin != NULL) {
		m_bEcsHighEnd = (BYTE)_wtof(pSpin->GetEditText());
	}
}

// Ends-in Contrast Stretching�� ������ ����
void CBMPFrame::OnPpEcsLowEnd()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_PP_ECSLOW));
	if (pSpin != NULL) {
		m_bEcsLowEnd = (BYTE)_wtof(pSpin->GetEditText());
	}
}

void CBMPFrame::OnViewOriginSize()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	ActivateFrame();
}


// Gaussian Noise
void CBMPFrame::OnNoiseGaussian()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CBMPDoc�� ������
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� BMP ���� (CBMPDoc) ���� �� ����
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
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
void CBMPFrame::OnNoiseSNR()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	CMFCRibbonEdit *pSpin = DYNAMIC_DOWNCAST(CMFCRibbonEdit,
		((CMainFrame*)GetTopLevelFrame())->GetRibbonBar()->FindByID(ID_NS_SNR));
	if (pSpin != NULL) {
		m_snr = _wtof(pSpin->GetEditText());
	}
}

// Roberts Masking
void CBMPFrame::OnApRoberts()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	runEdgeDetection(Mask::Roberts);
}

// Sobel Masking
void CBMPFrame::OnApSobel()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	runEdgeDetection(Mask::Sobel);
}

// Prewitt Masking
void CBMPFrame::OnApPrewitt()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	runEdgeDetection(Mask::Prewitt);
}

// Stochastic Gradient Masking
void CBMPFrame::OnApStochasticGradient()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	runEdgeDetection(Mask::StochasticGradient);
}

// Low-pass Filtering
void CBMPFrame::OnApLowPass()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CBMPDoc�� ������
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� BMP ���� (CBMPDoc) ���� �� ����
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
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
void CBMPFrame::OnApMedian()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// ���� CBMPDoc�� ������
	CBMPDoc *pSrcDoc = GetActiveDocument();
	ASSERT_VALID(pSrcDoc);
	if (!pSrcDoc)
		return;

	// �ű� BMP ���� (CBMPDoc) ���� �� ����
	CBMPFrame* pDstFrm;
	CBMPView* pDstView;
	CBMPDoc* pDstDoc;
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


void CBMPFrame::OnErRoberts()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CBMPDoc�� ������
	CBMPDoc *pDoc = GetActiveDocument();
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

void CBMPFrame::OnErSobel()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CBMPDoc�� ������
	CBMPDoc *pDoc = GetActiveDocument();
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

void CBMPFrame::OnErPrewitt()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CBMPDoc�� ������
	CBMPDoc *pDoc = GetActiveDocument();
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

void CBMPFrame::OnErStochasticGradient()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CBMPDoc�� ������
	CBMPDoc *pDoc = GetActiveDocument();
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

void CBMPFrame::OnErLowPass()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CBMPDoc�� ������
	CBMPDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Mean Square Error
	OnNoiseSNR();	// get SNR
	double mse = pDoc->getMSE(0, m_snr, FILTER_WIDTH);

	// �޽ùڽ� ����
	CString msg;
	msg.Format(_T("MSE: %f"), mse);
	MessageBox(msg, _T("Mean Square Error"));
}

void CBMPFrame::OnErMedian()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	// CBMPDoc�� ������
	CBMPDoc *pDoc = GetActiveDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get Mean Square Error
	OnNoiseSNR();	// get SNR
	double mse = pDoc->getMSE(1, m_snr, FILTER_WIDTH);

	// �޽ùڽ� ����
	CString msg;
	msg.Format(_T("MSE: %f"), mse);
	MessageBox(msg, _T("Mean Square Error"));
}
