// ImageDocExt.h : ImageDoc 확장 클래스의 인터페이스입니다.
//

#pragma once

#include "ImageDoc.h"


class CBMPDoc : public CImageDoc
{

};


class CRAWDoc : public CImageDoc
{
protected:	// serialization에서만 만들어집니다.
	CRAWDoc();
	DECLARE_DYNCREATE(CRAWDoc)

// 특성입니다.
private:
	void allocRawPixelData(UINT length);
	void deleteRawPixelData();

	BYTE* m_RawPixelData;
	BOOL m_bCreatedDataFirsthand;	// m_RawPixelData를 직접 할당한 경우 TRUE

// 작업입니다.
private:

// 재정의입니다.
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);	// 문서 입/출력을 위해 재정의되었습니다.
#endif
};