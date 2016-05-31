// ImageProcessorUtil.h : CImageProcessorUtil 클래스의 인터페이스입니다.
//

#pragma once

#include <algorithm>

#define HTGSIZE			256	// histogram 데이터 너비
#define INTENSITYMAX	255	// intensity 최댓값
#define INTENSITYMIN	0	// intensity 최솟값

#define B_size 8

// CImageProcessorUtil 명령 대상입니다.

class Mask;

class CImageProcessorUtil : public CObject
{
public:
	// 픽셀 데이터에 대한 histogram 생성
	static void generateHistogram(IN const BYTE pixelData[], IN const UINT pixelDataSize, OUT UINT histogramData[HTGSIZE]);
	// Basic Contrast Stretching
	template<class Num>
	static void stretchContrast(OUT Num pixelData[], IN const UINT dataSize);
	// Ends-in Contrast Stretching
	template<class Num>
	static void stretchContrast(OUT Num data[], IN const UINT dataSize, IN const Num low, IN const Num high);
	// Get Variance of Pixels
	static double getImagePower(IN const BYTE data[], IN const UINT pixelDataSize);
	// Get Standard Deviation of Noise
	static double getStandardDeviationOfNoise(IN const BYTE pixelData[], IN const UINT pixelDataSize, IN const double snr);
	// Add Gaussian Noise
	static void addGaussianNoise(OUT BYTE pixelData[], IN const UINT pixelDataSize, IN const double sigma);
	// Get gaussian
	static double gaussian(IN const double sd);
	// Mask
	static void mask(OUT BYTE pixelData[], IN const UINT pixelDataWidth,IN const UINT pixelDataHeight, IN Mask& mask);
	// Low-pass Filtering
	static void filterLowPass(OUT BYTE pixelData[], IN const UINT pixelDataWidth, IN const UINT pixelDataHeight, IN const UINT filterWidth);
	// Median Filtering
	static void filterMedian(OUT BYTE pixelData[], IN const UINT pixelDataWidth, IN const UINT pixelDataHeight, IN UINT windowWidth);
	// Get Mean Square Error
	static double obtainMeanSquareError(IN BYTE srcPixelData[], IN BYTE dstPixelData[], IN const UINT pixelDataSize);
	// Forward Discrete Cosine Transform (8 * 8)
	static void dct8x8(int ix[][B_size]);
	// Inverse Discrete Cosine Transform (8 * 8)
	static void idct8x8(int ix[][B_size]);
	// Quick Sort
	template<class Num>
	static void quickSort(OUT Num array[], IN const UINT arraySize);
	// Compare for Quick Sort
	template<class Num>
	static int compare(IN const void* src, IN const void* dst);
};


class Mask
{
public:
	enum Type {
		Roberts,
		Sobel,
		Prewitt,
		StochasticGradient
	};

	Mask();
	Mask(Type type);
	const int getLength();
	double* getMaskX();
	double* getMaskY();

private:
	int m_Length;
	double *m_MaskX;
	double *m_MaskY;

private:
	// Roberts Mask
	static const int RobertsLength = 3;
	static const double RobertsX[RobertsLength][RobertsLength];
	static const double RobertsY[RobertsLength][RobertsLength];

	// Sobel Mask
	static const int SobelLength = 3;
	static const double SobelX[SobelLength][SobelLength];
	static const double SobelY[SobelLength][SobelLength];

	// Prewitt Mask
	static const int PrewittLength = 3;
	static const double PrewittX[PrewittLength][PrewittLength];
	static const double PrewittY[PrewittLength][PrewittLength];

	// 5*5 Stochastic Gradient Mask
	static const int StochasticGradientLength = 5;
	static const double StochasticGradientX[StochasticGradientLength][StochasticGradientLength];
	static const double StochasticGradientY[StochasticGradientLength][StochasticGradientLength];
};
