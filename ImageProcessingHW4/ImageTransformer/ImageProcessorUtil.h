// ImageProcessorUtil.h : CImageProcessorUtil Ŭ������ �������̽��Դϴ�.
//

#pragma once

#include <algorithm>

#define HTGSIZE			256	// histogram ������ �ʺ�
#define INTENSITYMAX	255	// intensity �ִ�
#define INTENSITYMIN	0	// intensity �ּڰ�

#define B_size 8

// CImageProcessorUtil ��� ����Դϴ�.

class Mask;

class CImageProcessorUtil : public CObject
{
public:
	// �ȼ� �����Ϳ� ���� histogram ����
	static void generateHistogram(IN const BYTE pixelData[], IN const UINT pixelDataSize, OUT UINT histogramData[HTGSIZE]);
	// Basic Contrast Stretching
	template<class NUM1, class NUM2 = NUM1>
	static void stretchContrast(OUT NUM1 pixelData[], IN const UINT dataSize, OUT NUM2* outputPixelData = NULL);
	// Ends-in Contrast Stretching
	template<class NUM1, class NUM2 = NUM1>
	static void stretchContrast(OUT NUM1 data[], IN const UINT dataSize, IN const NUM1 low, IN const NUM1 high, OUT NUM2* outputPixelData = NULL);
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
	template<class NUM1>
	static void quickSort(OUT NUM1 array[], IN const UINT arraySize);
	// Compare for Quick Sort
	template<class NUM1>
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
