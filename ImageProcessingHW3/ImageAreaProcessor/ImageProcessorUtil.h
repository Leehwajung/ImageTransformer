// ImageProcessorUtil.h : CImageProcessorUtil 클래스의 인터페이스입니다.
//

#pragma once

#include <algorithm>

#define HTGSIZE			256	// histogram 데이터 너비
#define INTENSITYMAX	255	// intensity 최댓값
#define INTENSITYMIN	0	// intensity 최솟값

// CImageProcessorUtil 명령 대상입니다.

class Mask;

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
	// Get gaussian
	static double gaussian(IN const double sd);
	// Mask
	static void mask(OUT BYTE pixelData[], IN const UINT pixelDataWidth,IN const UINT pixelDataHeight, IN Mask& mask);
	// Quick Sort
	template<class Num>
	static void quickSort(OUT Num array[], IN const UINT arraySize);
private:
	// Quick Sort
	template<class Num>
	static void quickSort(OUT Num array[], IN const int left, IN const int right);
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

template<class Num>
inline void CImageProcessorUtil::quickSort(OUT Num array[], IN const UINT arraySize)
{
	quickSort<Num>(array, 0, arraySize);
}

template<class Num>
inline void CImageProcessorUtil::quickSort(OUT Num array[], IN const int left, IN const int right)
{
	if (left >= right) {
		return;
	}

	int last = left;
	std::swap(array[left], array[(left + right) / 2]);

	for (int i = left + 1; i <= right; i++) {
		if (array[i] < array[left]) {
			std::swap(array[++last], array[i]);
		}
	}

	std::swap(array[left], array[last]);

	quickSort(array, left, last - 1);
	quickSort(array, last + 1, right);
}
