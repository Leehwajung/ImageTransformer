// RAWOpenDlg.h : CRAWOpenDlg Ŭ������ �������̽��Դϴ�.
//

#pragma once
#include "afxwin.h"


// CRAWOpenDlg ��ȭ �����Դϴ�.

class CRAWOpenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRAWOpenDlg)

public:
	CRAWOpenDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRAWOpenDlg();

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
