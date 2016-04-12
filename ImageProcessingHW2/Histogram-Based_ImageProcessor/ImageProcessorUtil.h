// ImageProcessorUtil.h : CImageProcessorUtil 클래스의 인터페이스입니다.
//

#pragma once

#define HTGSIZE	256		// histogram 데이터 너비
#define HTGMAX	255.0f	// histogram 도메인 최댓값
#define HTGMIN	0.0f	// histogram 도메인 최솟값

// CImageProcessorUtil 명령 대상입니다.

static class CImageProcessorUtil : public CObject
{
public:
	static void generateHistogram(IN const BYTE pixelData[], IN const UINT pixelDataSize, OUT UINT histogramData[HTGSIZE]);
};


