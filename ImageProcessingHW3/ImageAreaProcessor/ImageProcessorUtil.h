// ImageProcessorUtil.h : CImageProcessorUtil 클래스의 인터페이스입니다.
//

#pragma once

#define HTGSIZE			256	// histogram 데이터 너비
#define INTENSITYMAX	255	// intensity 최댓값
#define INTENSITYMIN	0	// intensity 최솟값


// CImageProcessorUtil 명령 대상입니다.

class CImageProcessorUtil : public CObject
{
public:
	// 픽셀 데이터에 대한 histogram 생성
	static void generateHistogram(IN const BYTE pixelData[], IN const UINT pixelDataSize, OUT UINT histogramData[HTGSIZE]);
	// Ends-in Contrast Stretching
	static void stretchContrast(OUT BYTE pixelData[], IN const UINT pixelDataSize, IN const BYTE low, IN const BYTE high);
	// Get Variance of Pixels
	static double getImagePower(IN const BYTE pixelData[], IN const UINT pixelDataSize);
	// Get Standard Deviation of Noise
	static double getStandardDeviationOfNoise(IN const BYTE pixelData[], IN const UINT pixelDataSize, IN const double snr);
	// Add Gaussian Noise
	static void addGaussianNoise(OUT BYTE pixelData[], IN const UINT pixelDataSize, IN const double sigma);
	// get gaussian
	static double gaussian(IN const double sd);
};
