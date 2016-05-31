// RAWOpenDlg.h : CRAWOpenDlg 클래스의 인터페이스입니다.
//

#pragma once
#include "afxwin.h"


// CRAWOpenDlg 대화 상자입니다.

class CRAWOpenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRAWOpenDlg)

public:
	CRAWOpenDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRAWOpenDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RAWOPEN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
