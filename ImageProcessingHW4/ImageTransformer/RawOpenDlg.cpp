// RawOpenDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTransformer.h"
#include "RawOpenDlg.h"
#include "afxdialogex.h"


// CRawOpenDlg ��ȭ �����Դϴ�.

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


// CRawOpenDlg �޽��� ó�����Դϴ�.


BOOL CRawOpenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_RawWidth = 512;
	m_RawHeight = 512;
	m_RawDepth8.SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CRawOpenDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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
