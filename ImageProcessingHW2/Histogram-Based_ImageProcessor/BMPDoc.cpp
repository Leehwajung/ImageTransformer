// BMPDoc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BMPDoc.h"


// CBMPDoc

IMPLEMENT_DYNCREATE(CBMPDoc, CDocument)

CBMPDoc::CBMPDoc()
{
	EnableAutomation();
}

BOOL CBMPDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CBMPDoc::~CBMPDoc()
{
}

void CBMPDoc::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다.  기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDocument::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CBMPDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CBMPDoc, CDocument)
END_DISPATCH_MAP()

// 참고: IID_IBMPDoc에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {56F902D5-BA11-4E73-9A1D-C873816130E7}
static const IID IID_IBMPDoc =
{ 0x56F902D5, 0xBA11, 0x4E73, { 0x9A, 0x1D, 0xC8, 0x73, 0x81, 0x61, 0x30, 0xE7 } };

BEGIN_INTERFACE_MAP(CBMPDoc, CDocument)
	INTERFACE_PART(CBMPDoc, IID_IBMPDoc, Dispatch)
END_INTERFACE_MAP()


// CBMPDoc 진단입니다.

#ifdef _DEBUG
void CBMPDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CBMPDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CBMPDoc serialization입니다.

void CBMPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}
#endif


// CBMPDoc 명령입니다.
