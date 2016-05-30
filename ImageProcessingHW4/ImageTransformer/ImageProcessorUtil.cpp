// ImageProcessorUtil.cpp : CImageProcessorUtil Ŭ������ ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageProcessorUtil.h"


// CImageProcessorUtil

#define THRESH	150

// CImageProcessorUtil ��� �Լ�

// �ȼ� �����Ϳ� ���� histogram ����
void CImageProcessorUtil::generateHistogram(IN const BYTE pixelData[], IN const UINT pixelDataSize, OUT UINT histogramData[HTGSIZE])
{
	for (UINT i = 0; i < HTGSIZE; i++) {
		histogramData[i] = 0;					// histogram �迭 �ʱ�ȭ
	}

	for (UINT i = 0; i < pixelDataSize; i++) {
		if (pixelData[i] >= 0 && pixelData[i] < HTGSIZE) {
			histogramData[pixelData[i]]++;	// ��Ⱚ �� ����
		}
		else {	// �߸��� �ȼ� �������̸� ����
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
	double pixelSquare = 0, wholeSquare = 0;

	for (UINT i = 0; i < pixelDataSize; i++) {
		pixelSquare += pow(pixelData[i], 2.0);
		wholeSquare += pixelData[i];
	}

	pixelSquare /= pixelDataSize;
	wholeSquare = pow(wholeSquare / pixelDataSize, 2.0);

	return pixelSquare - wholeSquare;
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

// Get gaussian
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

// Mask
void CImageProcessorUtil::mask(OUT BYTE pixelData[], 
	IN const UINT pixelDataWidth, IN const UINT pixelDataHeight, IN Mask& mask) {
	
	UINT pixelDataSize = pixelDataWidth * pixelDataHeight;

	// Magnitude Gradient
	double *g = new double[pixelDataSize];

	// Get Magnitude Gradient
	int length = mask.getLength();	// ����ũ�� ���ο� ���� ����
	int indicator = length / 2;		// ����ũ �߾��� ã�� ���� ����

	for (UINT n = 0; n < pixelDataHeight; n++) {		// ���� ���� ���� ���� (Image Abscissa)
		for (UINT m = 0; m < pixelDataWidth; m++) {		// ���� ���� ���� ���� (Image Ordinate)
			double gx = 0;								// �� �����
			double gy = 0;								// �� �����
			for (int mr = 0; mr < length; mr++) {		// ����ũ ���� ���� ���� (Mask Row)
				for (int mc = 0; mc < length; mc++) {	// ����ũ ���� ���� ���� (Mask Column)
					int ir = n + mr - indicator;			// ����� ���� �ȼ� ���� ��ġ (Image Row)
					int ic = m + mc - indicator;			// ����� ���� �ȼ� ���� ��ġ (Image Column)

					// ���κ� ó��
					//	��踦 �Ѿ�� ���� �ȼ� ��ġ�� ���� ����� �ȼ��� �����Ͽ�,
					//	������ ��� �κ��� ������ Resolution�� ������Ű�� �Ͱ� ���� ȿ���� ����
					ir = ir >= (int)pixelDataHeight ? (int)pixelDataHeight - 1 : ir < 0 ? 0 : ir;
					ic = ic >= (int)pixelDataWidth ? (int)pixelDataWidth - 1 : ic < 0 ? 0 : ic;

					// ���� �ȼ��� ����ũ ���� ��
					UINT ip = ir * pixelDataWidth + ic;
					UINT mp = mr * length + mc;
					double currPixelValue = (double)pixelData[ip];
					gx += mask.getMaskX()[mp] * currPixelValue;
					gy += mask.getMaskY()[mp] * currPixelValue;
				}
			}

			// ���� �ű״�Ʃ�� ���
			g[n * pixelDataWidth + m] = sqrt(pow(gx, 2.0) + pow(gy, 2.0));
		}
	}

	// ����ȭ�� ���� ���� �۰ų� ū ���� ����
	double min = INT_MAX;
	double max = INT_MIN;
	for (UINT i = 1; i < pixelDataSize; i++) {
		if (g[i] < min) {
			min = g[i];
		}
		else if (g[i] > max) {
			max = g[i];
		}
	}

	// [min, max] ������ [0, 255]������ ��ȯ
	double scaleFactor = (double)INTENSITYMAX / (max - min);
	double translator = -(double)INTENSITYMAX * min / (max - min);
	for (UINT i = 0; i < pixelDataSize; i++) {
		// ����ȭ
		double normalized = scaleFactor * g[i] + translator + 0.5;

		// �Ӱ谪 ����
		//if (normalized > THRESH) {
		//	pixelData[i] = INTENSITYMAX;
		//}
		//else {
		//	pixelData[i] = INTENSITYMIN;
		//}

		// �Ӱ谪 ������
		pixelData[i] = (BYTE)normalized;
	}

	// ���� �Ҵ� �޸� ����
	delete[] g;
}

// Low-pass Filtering
void CImageProcessorUtil::filterLowPass(OUT BYTE pixelData[], 
	IN const UINT pixelDataWidth, IN const UINT pixelDataHeight, IN const UINT filterWidth)
{
	UINT pixelDataSize = pixelDataWidth * pixelDataHeight;

	// Low-pass Filtering
	double *result = new double[pixelDataSize];						// ���͸� ��� �ӽ� ���� �迭
	const double denominator = (double)(filterWidth * filterWidth);	// ������ ��� ������ �и�
	const int indicator = filterWidth / 2;							// ���� �߾��� ã�� ���� ����

	for (UINT n = 0; n < pixelDataHeight; n++) {					// ���� ���� ���� ���� (Image Abscissa)
		for (UINT m = 0; m < pixelDataWidth; m++) {					// ���� ���� ���� ���� (Image Ordinate)
			double *newPixel = &result[n * pixelDataWidth + m];		// ���͸� �� �ȼ��� �ű� ��
			*newPixel = 0;											// �ű� �� �ʱ�ȭ
			for (UINT mr = 0; mr < filterWidth; mr++) {				// ���� ���� ���� ���� (Filter Row)
				for (UINT mc = 0; mc < filterWidth; mc++) {			// ���� ���� ���� ���� (Filter Column)
					int ir = n + mr - indicator;						// ����� ���� �ȼ� ���� ��ġ (Image Row)
					int ic = m + mc - indicator;						// ����� ���� �ȼ� ���� ��ġ (Image Column)

					// ���κ� ó��
					//	��踦 �Ѿ�� ���� �ȼ� ��ġ�� ���� ����� �ȼ��� �����Ͽ�,
					//	������ ��� �κ��� ������ Resolution�� ������Ű�� �Ͱ� ���� ȿ���� ����
					ir = ir >= (int)pixelDataHeight ? (int)pixelDataHeight - 1 : ir < 0 ? 0 : ir;
					ic = ic >= (int)pixelDataWidth ? (int)pixelDataWidth - 1 : ic < 0 ? 0 : ic;

					// �ű� ���� ���� �ȼ��� ���� ���� ��
					UINT ip = ir * pixelDataWidth + ic;
					*newPixel += (1.0 / denominator) * (double)pixelData[ip];
				}
			}
		}
	}

	// ��� �迭���� ���� �迭�� ����
	for (UINT i = 0; i < pixelDataSize; i++) {
		pixelData[i] = (BYTE)(result[i] + 0.5);
	}

	// ���� �Ҵ� �޸� ����
	delete[] result;
}

// Median Filtering
void CImageProcessorUtil::filterMedian(OUT BYTE pixelData[], 
	IN const UINT pixelDataWidth, IN const UINT pixelDataHeight, IN UINT windowWidth)
{
	UINT pixelDataSize = pixelDataWidth * pixelDataHeight;

	// windowWidth�� ¦���� ��� Ȧ���� �ٲ���
	if (windowWidth % 2 == 0) {
		windowWidth++;
	}

	// Median Filtering
	BYTE *result = new BYTE[pixelDataSize];				// ���͸� ��� �ӽ� ���� �迭
	BYTE *sample = new BYTE[windowWidth * windowWidth];	// �����쿡 ���Ե� �ȼ� �� ���� �迭
	const int indicator = windowWidth / 2;				// ������ �߾��� ã�� ���� ����

	for (UINT n = 0; n < pixelDataHeight; n++) {		// ���� ���� ���� ���� (Image Abscissa)
		for (UINT m = 0; m < pixelDataWidth; m++) {		// ���� ���� ���� ���� (Image Ordinate)

			// ���κ� ó��
			//	�̹��� �ȼ� ���� ���� �ȼ� ������ �����ϰ� ������ ���� �� �߰����� ���,
			//	ù ��° ���ð� ������ ������ �����Ͽ� �� ���� ä��� �Ͱ� ���� ȿ���� ����
			UINT pixelNum = 0;										// �����쿡 ���Ե� �ȼ� �� (���� ��)
			for (UINT mr = 0; mr < windowWidth; mr++) {				// ������ ���� ���� ���� (Window Row)
				int ir = n + mr - indicator;							// ����� ���� �ȼ� ���� ��ġ (Image Row)
				if (ir < (int)pixelDataHeight && ir >= 0) {			// ���� ���� �̹��� �ȼ� ���� ��
					for (UINT mc = 0; mc < windowWidth; mc++) {		// ������ ���� ���� ���� (Window Column)
						int ic = m + mc - indicator;					// ����� ���� �ȼ� ���� ��ġ (Image Column)
						if (ic < (int)pixelDataWidth && ic >= 0) {	// ���� ���� �̹��� �ȼ� ���� ��
																	// ���� ����
							UINT ip = ir * pixelDataWidth + ic;
							sample[pixelNum++] = pixelData[ip];
						}
					}
				}
			}

			// ���� ����

			CImageProcessorUtil::quickSort<BYTE>(sample, pixelNum);

			// �߰��� ��������
			result[n * pixelDataWidth + m] = sample[pixelNum / 2];
		}
	}
	
	// ��� �迭���� ���� �迭�� ����
	for (UINT i = 0; i < pixelDataSize; i++) {
		pixelData[i] = result[i];
	}

	// ���� �Ҵ� �޸� ����
	delete[] result;
	delete[] sample;
}

// Get Mean Square Error
double CImageProcessorUtil::obtainMeanSquareError(IN BYTE srcPixelData[], IN BYTE dstPixelData[], IN const UINT pixelDataSize)
{
	double sum = 0;

	for (UINT i = 0; i < pixelDataSize; i++) {
		sum += pow(srcPixelData[i] - dstPixelData[i], 2);
	}

	return sum / (double)pixelDataSize;
}


Mask::Mask()
	:Mask(Roberts)
{
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