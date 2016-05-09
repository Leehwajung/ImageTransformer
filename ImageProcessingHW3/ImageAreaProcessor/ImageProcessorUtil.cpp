// ImageProcessorUtil.cpp : CImageProcessorUtil 클래스의 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageProcessorUtil.h"


// CImageProcessorUtil

// CImageProcessorUtil 멤버 함수

// 픽셀 데이터에 대한 histogram 생성
void CImageProcessorUtil::generateHistogram(IN const BYTE pixelData[], IN const UINT pixelDataSize, OUT UINT histogramData[HTGSIZE])
{
	for (UINT i = 0; i < HTGSIZE; i++) {
		histogramData[i] = 0;					// histogram 배열 초기화
	}

	for (UINT i = 0; i < pixelDataSize; i++) {
		if (pixelData[i] >= 0 && pixelData[i] < HTGSIZE) {
			histogramData[pixelData[i]]++;	// 밝기값 빈도 수집
		}
		else {	// 잘못된 픽셀 데이터이면 종료
			return;
		}
	}
}

// Ends-in Contrast Stretching
void CImageProcessorUtil::stretchContrast(OUT BYTE pixelData[], IN const UINT pixelDataSize, IN const BYTE low, IN const BYTE high)
{
	float scaleFactor = HTGMAX / (FLOAT)(high - low);
	for (UINT i = 0; i < pixelDataSize; i++) {
		if (pixelData[i] <= low) {
			pixelData[i] = (BYTE)HTGMIN;
		}
		else if (pixelData[i] > high) {
			pixelData[i] = (BYTE)HTGMAX;
		}
		else {
			pixelData[i] = (BYTE)((pixelData[i] - low) * scaleFactor);
		}
	}
}

// Get Power of Pixels
double CImageProcessorUtil::getImagePower(IN const BYTE pixelData[], IN const UINT pixelDataSize)
{
	return DOUBLE();
}

// Get Standard Deviation of Noise
double CImageProcessorUtil::getStandardDeviationOfNoise(IN const BYTE pixelData[], IN const UINT pixelDataSize, IN const double snr)
{
	double variance = getImagePower(pixelData, pixelDataSize);
	return sqrt(variance / pow(10.0, snr / 10.0));
}

// Add Gaussian Noise
void CImageProcessorUtil::addGaussianNoise(OUT BYTE pixelData[], IN const UINT pixelDataSize, IN const double sigma)
{
	for (UINT i = 0; i < pixelDataSize; i++) {
		double s = (double)pixelData[i] + gaussian(sigma);
		pixelData[i] = (BYTE)(s > INTMAX ? INTMAX : s < INTMIN ? INTMIN : s);
	}
}

// get gaussian
double CImageProcessorUtil::gaussian(IN const double sd)
{
	static int ready = 0;
	static float gstore;
	float v1, v2, r, fac, gaus;
	int r1, r2;

	if (ready == 0) {
		do {
			r1 = rand();
			r2 = rand();
			v1 = (float)(2.f * ((float)r1 / (float)RAND_MAX - 0.5));
			v2 = (float)(2.f * ((float)r2 / (float)RAND_MAX - 0.5));
			r = v1 * v1 + v2 * v2;
		} while (r > 1.0);
		fac = (float)sqrt((double)(-2 * log(r) / r));
		gstore = v1 * fac;
		gaus = v2 * fac;
		ready = 1;
	}
	else {
		ready = 0;
		gaus = gstore;
	}

	return (gaus * sd);
}
