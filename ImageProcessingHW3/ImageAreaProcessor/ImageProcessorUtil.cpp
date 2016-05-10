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
	float scaleFactor = (FLOAT)INTENSITYMAX / (FLOAT)(high - low);
	for (UINT i = 0; i < pixelDataSize; i++) {
		if (pixelData[i] <= low) {
			pixelData[i] = (BYTE)INTENSITYMIN;
		}
		else if (pixelData[i] > high) {
			pixelData[i] = (BYTE)INTENSITYMAX;
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
		pixelData[i] = (BYTE)(s > INTENSITYMAX ? INTENSITYMAX : s < INTENSITYMIN ? INTENSITYMIN : s);
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

// mask
void CImageProcessorUtil::mask(OUT BYTE pixelData[], IN const UINT pixelDataWidth, IN const UINT pixelDataHeight, IN Mask& mask) {
	
	UINT pixelDataSize = pixelDataWidth * pixelDataHeight;

	// Magnitude Gradient
	double *g = new double[pixelDataSize];

	// Get Magnitude Gradient
	int length = mask.getLength();	// 마스크의 가로와 세로 길이
	int indicator = length / 2;		// 마스크 중앙을 찾기 위한 보정
	for (UINT n = 0; n < pixelDataHeight; n++) {		// 영상 세로 방향 루프 (Image Abscissa)
		for (UINT m = 0; m < pixelDataWidth; m++) {		// 영상 가로 방향 루프 (Image Ordinate)
			double gx = 0;								// 행 검출기
			double gy = 0;								// 열 검출기
			for (int mr = 0; mr < length; mr++) {		// 마스크 세로 방향 루프 (Mask Row)
				for (int mc = 0; mc < length; mc++) {	// 마스크 가로 방향 루프 (Mask Column)
					int ir = n + mr - indicator;			// 계산할 영상 픽셀 세로 위치 (Image Row)
					int ic = m + mc - indicator;			// 계산할 영상 픽셀 가로 위치 (Image Column)

					// 경계부분 처리
					//	경계를 넘어가는 영상 픽셀 위치는 가장 가까운 픽셀로 변경하여,
					//	영상의 경계 부분을 복사해 Resolution을 증가시키는 것과 같은 효과를 얻음
					ir = ir >= (int)pixelDataHeight ? (int)pixelDataHeight - 1 : ir < 0 ? 0 : ir;
					ic = ic >= (int)pixelDataWidth ? (int)pixelDataWidth - 1 : ic < 0 ? 0 : ic;

					// 영상 픽셀과 마스크 셀의 곱
					UINT ip = ir * pixelDataWidth + ic;
					UINT mp = mr * length + mc;
					double currPixelValue = (double)pixelData[ip];
					gx += mask.getMaskX()[mp] * currPixelValue;
					gy += mask.getMaskY()[mp] * currPixelValue;
				}
			}

			// 셀별 매그니튜드 계산
			g[n * pixelDataWidth + m] = sqrt(pow(gx, 2.0) + pow(gy, 2.0));
		}
	}

	// 정규화를 위해 가장 작거나 큰 값을 구함
	//double min = INT_MAX;
	//double max = INT_MIN;
	//for (UINT i = 1; i < pixelDataSize; i++) {
	//	if (g[i] < min) {
	//		min = g[i];
	//	}
	//	else if (g[i] > max) {
	//		max = g[i];
	//	}
	//}

	// [min, max] 구간을 [0, 255]값으로 변환
	//double scaleFactor = (double)INTENSITYMAX / (max - min);
	//double translator = -(double)INTENSITYMAX * min / (max - min);
	for (UINT i = 0; i < pixelDataSize; i++) {
		//double a = scaleFactor * g[i] + translator + 0.5;
		//pixelData[i] = (BYTE)(scaleFactor * g[i] + translator + 0.5);
		if (g[i] > 50) {
			pixelData[i] = INTENSITYMAX;
		}
		else {
			pixelData[i] = INTENSITYMIN;
		}
	}

	// 동적 할당 메모리 해제
	delete[] g;
}


Mask::Mask()
{
	Mask(Roberts);
}

Mask::Mask(Type type)
{
	switch (type)
	{
	case Mask::Roberts:
		m_Length = RobertsLength;
		m_MaskX = (double*)RobertsX;
		m_MaskY = (double*)RobertsY;
		break;
	case Mask::Sobel:
		m_Length = SobelLength;
		m_MaskX = (double*)SobelX;
		m_MaskY = (double*)SobelY;
		break;
	case Mask::Prewitt:
		m_Length = PrewittLength;
		m_MaskX = (double*)PrewittX;
		m_MaskY = (double*)PrewittY;
		break;
	case Mask::StochasticGradient:
		m_Length = StochasticGradientLength;
		m_MaskX = (double*)StochasticGradientX;
		m_MaskY = (double*)StochasticGradientY;
		break;
	}
}

const int Mask::getLength()
{
	return m_Length;
}

double* Mask::getMaskX()
{
	return m_MaskX;
}

double* Mask::getMaskY()
{
	return m_MaskY;
}


// Roberts Mask
const double Mask::RobertsX[RobertsLength][RobertsLength] = {
	{  0,  0, -1 },
	{  0,  1,  0 },
	{  0,  0,  0 }
};

const double Mask::RobertsY[RobertsLength][RobertsLength] = {
	{ -1,  0,  0 },
	{  0,  1,  0 },
	{  0,  0,  0 }
};

// Sobel Mask
const double Mask::SobelX[SobelLength][SobelLength] = {
	{  1,  0, -1 },
	{  2,  0, -2 },
	{  1,  0, -1 }
};

const double Mask::SobelY[SobelLength][SobelLength] = {
	{ -1, -2, -1 },
	{  0,  0,  0 },
	{  1,  2,  1 }
};

// Prewitt Mask
const double Mask::PrewittX[PrewittLength][PrewittLength] = {
	{  1,  0, -1 },
	{  1,  0, -1 },
	{  1,  0, -1 }
};

const double Mask::PrewittY[PrewittLength][PrewittLength] = {
	{ -1, -1, -1 },
	{  0,  0,  0 },
	{  1,  1,  1 }
};

// 5*5 Stochastic Gradient Mask
const double Mask::StochasticGradientX[StochasticGradientLength][StochasticGradientLength] = {
	{  0.267,  0.364,  0.000, -0.364, -0.267 },
	{  0.373,  0.562,  0.000, -0.562, -0.373 },
	{  0.463,  1.000,  0.000, -1.000, -0.463 },
	{  0.373,  0.562,  0.000, -0.562, -0.373 },
	{  0.267,  0.364,  0.000, -0.364, -0.267 }
};

const double Mask::StochasticGradientY[StochasticGradientLength][StochasticGradientLength] = {
	{  0.267,  0.373,  0.463,  0.373,  0.267 },
	{  0.364,  0.562,  1.000,  0.562,  0.364 },
	{  0.000,  0.000,  0.000,  0.000,  0.000 },
	{ -0.267, -0.373, -0.463, -0.373, -0.267 },
	{ -0.364, -0.562, -1.000, -0.562, -0.364 }
};