// ImageProcessorUtilGeneric.cpp : CImageProcessorUtil 클래스의 구현 파일입니다. (Template)
//

#include "stdafx.h"
#include "ImageProcessorUtil.h"


// CImageProcessorUtil


// CImageProcessorUtil 멤버 함수 (Template)

// Basic Contrast Stretching
template<class Num>
void CImageProcessorUtil::stretchContrast(OUT Num data[], IN const UINT dataSize)
{
	// 가장 작거나 큰 밝기값을 구함
	Num low = data[0];
	Num high = data[0];
	for (UINT i = 1; i < dataSize; i++) {
		if (data[i] < low) {
			low = data[i];
		}
		else if (data[i] > high) {
			high = data[i];
		}
	}

	// Contrast Stretching
	stretchContrast(data, dataSize, low, high);
}

// Ends-in Contrast Stretching
template<class Num>
void CImageProcessorUtil::stretchContrast(OUT Num data[], IN const UINT dataSize, IN const Num low, IN const Num high)
{
	double scaleFactor = (double)INTENSITYMAX / ((double)high - (double)low);
	for (UINT i = 0; i < dataSize; i++) {
		if (data[i] <= low) {
			data[i] = (Num)INTENSITYMIN;
		}
		else if (data[i] > high) {
			data[i] = (Num)INTENSITYMAX;
		}
		else {
			data[i] = (Num)(((double)data[i] - (double)low) * scaleFactor);
		}
	}
}

template<class Num>
inline void CImageProcessorUtil::quickSort(OUT Num array[], IN const UINT arraySize)
{
	//quickSort<Num>(array, 0, arraySize);
	qsort(array, arraySize, sizeof(Num), CImageProcessorUtil::compare<Num>);
}

template<class Num>
inline int CImageProcessorUtil::compare(IN const void* src, IN const void* dst)
{
	if (*(Num*)src < *(Num*)dst) {
		return -1;
	}
	if (*(Num*)src == *(Num*)dst) {
		return 0;
	}
	if (*(Num*)src > *(Num*)dst) {
		return 1;
	}

	return 0;
}

