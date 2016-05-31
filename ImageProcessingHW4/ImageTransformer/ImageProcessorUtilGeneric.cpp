// ImageProcessorUtilGeneric.cpp : CImageProcessorUtil 클래스의 구현 파일입니다. (Template)
//

#include "stdafx.h"
#include "ImageProcessorUtil.h"


// CImageProcessorUtil


// CImageProcessorUtil 멤버 함수 (Template)

// Basic Contrast Stretching
template<class NUM1, class NUM2 = NUM1>
void CImageProcessorUtil::stretchContrast(OUT NUM1 data[], IN const UINT dataSize, OUT NUM2* outputPixelData /*= NULL*/)
{
	// 가장 작거나 큰 밝기값을 구함
	NUM1 low = data[0];
	NUM1 high = data[0];
	for (UINT i = 1; i < dataSize; i++) {
		if (data[i] < low) {
			low = data[i];
		}
		else if (data[i] > high) {
			high = data[i];
		}
	}

	// Contrast Stretching
	stretchContrast(data, dataSize, low, high, outputPixelData);
}

// Ends-in Contrast Stretching
template<class NUM1, class NUM2 = NUM1>
void CImageProcessorUtil::stretchContrast(OUT NUM1 data[], IN const UINT dataSize, IN const NUM1 low, IN const NUM1 high, OUT NUM2* outputPixelData /*= NULL*/)
{
	if (outputPixelData == NULL) {
		outputPixelData = (NUM2*)data;
	}
	double scaleFactor = (double)INTENSITYMAX / ((double)high - (double)low);
	for (UINT i = 0; i < dataSize; i++) {
		if (data[i] <= low) {
			outputPixelData[i] = (NUM2)INTENSITYMIN;
		}
		else if (data[i] > high) {
			outputPixelData[i] = (NUM2)INTENSITYMAX;
		}
		else {
			outputPixelData[i] = (NUM2)(((double)data[i] - (double)low) * scaleFactor);
		}
	}
}

template<class NUM1>
inline void CImageProcessorUtil::quickSort(OUT NUM1 array[], IN const UINT arraySize)
{
	//quickSort<Num>(array, 0, arraySize);
	qsort(array, arraySize, sizeof(NUM1), CImageProcessorUtil::compare<NUM1>);
}

template<class NUM1>
inline int CImageProcessorUtil::compare(IN const void* src, IN const void* dst)
{
	if (*(NUM1*)src < *(NUM1*)dst) {
		return -1;
	}
	if (*(NUM1*)src == *(NUM1*)dst) {
		return 0;
	}
	if (*(NUM1*)src > *(NUM1*)dst) {
		return 1;
	}

	return 0;
}

