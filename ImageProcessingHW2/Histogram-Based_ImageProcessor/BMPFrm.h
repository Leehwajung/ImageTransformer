// BMPFrm.h : CBMPFrame 클래스의 인터페이스입니다.
//

#pragma once


// CBMPFrame

class CBMPFrame : public CMDIChildWndEx
{
	DECLARE_DYNAMIC(CBMPFrame)
public:
	CBMPFrame();

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
	virtual void OnFinalRelease();

// 구현입니다.
public:
	virtual ~CBMPFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
