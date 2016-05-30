// RawOpenDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTransformer.h"
#include "RawOpenDlg.h"
#include "afxdialogex.h"


// CRawOpenDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRawOpenDlg, CDialogEx)

CRawOpenDlg::CRawOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RAWOPEN, pParent)
	, m_RawWidth(512)
	, m_RawHeight(512)
{

}

CRawOpenDlg::~CRawOpenDlg()
{
}

void CRawOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_OCInt(pDX, IDC_SIZE_WIDTH, DISPID_VALUE, m_RawWidth);
	//DDX_OCInt(pDX, IDC_SIZE_HEIGHT, DISPID_VALUE, m_RawHeight);
	//DDX_OCBool(pDX, IDC_DEPTH_8, DISPID_VALUE, m_RawDepth8);
	DDX_Control(pDX, IDC_DEPTH_8, m_RawDepth8);
}


BEGIN_MESSAGE_MAP(CRawOpenDlg, CDialogEx)
END_MESSAGE_MAP()


// CRawOpenDlg 메시지 처리기입니다.


BOOL CRawOpenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_RawWidth = 512;
	m_RawHeight = 512;
	m_RawDepth8.SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CRawOpenDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_RawWidth = GetDlgItemInt(IDC_SIZE_WIDTH);
	m_RawHeight = GetDlgItemInt(IDC_SIZE_HEIGHT);
	if ((BOOL)m_RawDepth8.GetCheck()) {
		m_RawDepth = 8;
	}
	else {
		m_RawDepth = 8;
	}

	CDialogEx::OnOK();
}
