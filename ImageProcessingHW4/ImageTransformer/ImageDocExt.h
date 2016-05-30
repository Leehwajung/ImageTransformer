// ImageDocExt.h : ImageDoc Ȯ�� Ŭ������ �������̽��Դϴ�.
//

#pragma once

#include "ImageDoc.h"


class CBMPDoc : public CImageDoc
{

};


class CRAWDoc : public CImageDoc
{
protected:	// serialization������ ��������ϴ�.
	CRAWDoc();
	DECLARE_DYNCREATE(CRAWDoc)

// Ư���Դϴ�.
private:
	void allocRawPixelData(UINT length);
	void deleteRawPixelData();

	BYTE* m_RawPixelData;

// �۾��Դϴ�.
private:

// �������Դϴ�.
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);	// ���� ��/����� ���� �����ǵǾ����ϴ�.
#endif
};