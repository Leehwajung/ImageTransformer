#pragma once
#include "afxwin.h"


// CRawOpenDlg ��ȭ �����Դϴ�.

class CRawOpenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRawOpenDlg)

public:
	CRawOpenDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRawOpenDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RAWOPEN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
public:
	UINT m_RawWidth;
	UINT m_RawHeight;
	UINT m_RawDepth;
private:
	CButton m_RawDepth8;

	
};
