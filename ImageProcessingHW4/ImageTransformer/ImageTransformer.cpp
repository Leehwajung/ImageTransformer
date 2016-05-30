// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// ImageTransformer.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ImageTransformer.h"
#include "MainFrm.h"

#include "ImageFrm.h"
#include "ImageDoc.h"
#include "ImageView.h"
#include "RAWFrm.h"
#include "RAWDoc.h"
#include "RAWView.h"
#include "HistogramFrm.h"
#include "HistogramDoc.h"
#include "HistogramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PFX_HISTOGRAM	L"histogram_of_"


// CImageTransformerApp

BEGIN_MESSAGE_MAP(CImageTransformerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CImageTransformerApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_HTG_PLOT, &CImageTransformerApp::OnHtgPlot)
END_MESSAGE_MAP()


// CImageTransformerApp 생성

CImageTransformerApp::CImageTransformerApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ImageTransformer.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CImageTransformerApp 개체입니다.

CImageTransformerApp theApp;


// CImageTransformerApp 초기화

BOOL CImageTransformerApp::InitInstance()
{
	CWinAppEx::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(10);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_BMPFileTYPE,
		RUNTIME_CLASS(CImageDoc),
		RUNTIME_CLASS(CImageFrame), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CImageView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_RAWFileTYPE,
		RUNTIME_CLASS(CRAWDoc),
		RUNTIME_CLASS(CImageFrame), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CImageView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_HistogramFileTYPE,
		RUNTIME_CLASS(CHistogramDoc),
		RUNTIME_CLASS(CHistogramFrame), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CHistogramView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  MDI 응용 프로그램에서는 m_pMainWnd를 설정한 후 바로 이러한 호출이 발생해야 합니다.
	// 끌어서 놓기에 대한 열기를 활성화합니다.
	m_pMainWnd->DragAcceptFiles();

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// DDE Execute 열기를 활성화합니다.
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// GDI plus를 시작합니다.
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// 시간을 시드로 사용하여 랜덤값을 만듭니다.
	srand((UINT)time(NULL));

	// 파일 열기 다이얼로그를 활성화합니다.
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FILE_OPEN);

	return TRUE;
}

int CImageTransformerApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);
	GdiplusShutdown(gdiplusToken);

	return CWinAppEx::ExitInstance();
}

// CImageTransformerApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CImageTransformerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CImageTransformerApp 사용자 지정 로드/저장 메서드

void CImageTransformerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CImageTransformerApp::LoadCustomState()
{
}

void CImageTransformerApp::SaveCustomState()
{
}


// CImageTransformerApp 메시지 처리기

void CImageTransformerApp::OnHtgPlot()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// Source(image)를 가져옴
	CMainFrame *pMainFrm = (CMainFrame*)(AfxGetMainWnd());		// Main Frame
	CImageDoc *pImageDoc = ((CImageFrame*)pMainFrm->MDIGetActive())->GetActiveDocument();

	// 신규 Historam 문서 (CHistogramDoc) 생성
	POSITION pos = GetFirstDocTemplatePosition();
	CDocTemplate *pTml;
	for (int i = 0; i < 3; i++) {
		pTml = GetNextDocTemplate(pos);
	}
	pTml->OpenDocumentFile(NULL);

	// Destination(histogram)을 가져옴
	CHistogramFrame *pHtgFrm = (CHistogramFrame*)pMainFrm->MDIGetActive();	// Histogram Frame
	CHistogramView *pHtgView = (CHistogramView*)(pHtgFrm->GetActiveView());	// Histogram View
	CHistogramDoc *pHtgDoc = pHtgView->GetDocument();						// Histogram Document

	// Source의 픽셀 데이터를 기반으로하여 Destination에 histogram 생성
	BitmapData bitmapData;
	BYTE* pixelData = pImageDoc->getData(&bitmapData, ImageLockModeRead);
	pHtgDoc->plotHistogram(pixelData, bitmapData.Width * bitmapData.Height);
	pHtgView->plotHistogramImage();
	pImageDoc->clearData(&bitmapData);

	// 제목 변경
	CString newTitle(PFX_HISTOGRAM);
	newTitle.Append(pImageDoc->GetTitle());
	pHtgDoc->SetTitle(newTitle);

	// Histogram에 맞게 다시 그리기
	pHtgFrm->ActivateFrame();
	pHtgView->Invalidate();
}
