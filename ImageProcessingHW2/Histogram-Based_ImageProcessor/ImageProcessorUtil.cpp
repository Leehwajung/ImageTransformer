// ImageProcessorUtil.cpp : CImageProcessorUtil 클래스의 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageProcessorUtil.h"


// CImageProcessorUtil

// CImageProcessorUtil 멤버 함수

// 픽셀 데이터에 대한 histogram 생성
void CImageProcessorUtil::generateHistogram(IN const BYTE pixelData[], IN const UINT pixelDataSize, OUT UINT histogramData[HTGSIZE])
{
	for (int i = 0; i < HTGSIZE; i++) {
		histogramData[i] = 0;					// histogram 배열 초기화
	}

	for (int i = 0; i < pixelDataSize; i++) {
		if (pixelData[i] >= 0 && pixelData[i] < HTGSIZE) {
			histogramData[pixelData[i]]++;	// 밝기값 빈도 수집
		}
		else {	// 잘못된 픽셀 데이터이면 종료
			return;
		}
	}
}