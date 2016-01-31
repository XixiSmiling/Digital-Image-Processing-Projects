#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "iofile.h"
#include "histogram.h"
#include "DenoisingFilters.h"
#include "EdgeDetectionFilters.h"


// for gray-scale image or one channel of color image
int GetSobleGradient(int thr, unsigned char *inputImageData, int SizeH, int SizeW, int extend, unsigned char *outputGradientR, unsigned char *outputGradientC, unsigned char *outputGradient, unsigned char *outputImageData)
{
	// extend image
	int exH = SizeH+2*extend;  // extendSizeH
	int exW = SizeW+2*extend;  //extendSizeW
	unsigned char ext[exH][exW];  // extend image data

	extendonechannel(inputImageData, SizeH, SizeW, &ext[0][0], extend);

	// Gradient Generation & Gradient Combination
	float GradientDataR[SizeH][SizeW];
	float GradientDataC[SizeH][SizeW];
	int GradientData[SizeH][SizeW];
	int minR = 256, maxR = -256;
	int minC = 256, maxC = -256;
	int min = 999, max = 0;

	for(int i = extend; i < (SizeH + extend); i++)
	{
		for(int j = extend; j < (SizeW + extend); j++)
		{
			int a = i - extend;
			int b = j - extend;
			int A0 = ext[i-1][j-1];
			int A1 = ext[i-1][j];
			int A2 = ext[i-1][j+1];
			int A3 = ext[i][j+1];
			int A4 = ext[i+1][j+1];
			int A5 = ext[i+1][j];
			int A6 = ext[i+1][j-1];
			int A7 = ext[i][j-1];

			GradientDataR[a][b] = (A2 + 2*A3 + A4 - A0 - 2*A7 - A6) / 4;
			GradientDataC[a][b] = (A0 + 2*A1 + A2 - A6 - 2*A5 - A4) / 4;
			GradientData[a][b] = sqrt(GradientDataR[a][b]*GradientDataR[a][b] + GradientDataC[a][b]*GradientDataC[a][b]);

            if(GradientData[a][b] > thr)
            {
                outputImageData[a*SizeW+b] = 0;
            }
            else
            {
                outputImageData[a*SizeW+b] = 255;
            }

            if(GradientDataR[a][b] > maxR)
                maxR = GradientDataR[a][b];
            if(GradientDataR[a][b] < minR)
                minR = GradientDataR[a][b];
            if(GradientDataC[a][b] > maxC)
                maxC = GradientDataC[a][b];
            if(GradientDataC[a][b] < minC)
                minC = GradientDataC[a][b];
            if(GradientData[a][b] > max)
                max = GradientData[a][b];
            if(GradientData[a][b] < min)
                min = GradientData[a][b];
		}
	}

	// Normalize the x-gradient and the y-gradient value to 0-255
	for(int a=0; a<SizeH; a++)
	{
	    for(int b=0; b<SizeW; b++)
	    {
	    	outputGradient[a*SizeW+b] = 255.0*(GradientData[a][b]-min)/max;
	    	outputGradientR[a*SizeW+b] = 255.0*(GradientDataR[a][b]-minR)/(maxR-minR);
	    	outputGradientC[a*SizeW+b] = 255.0*(GradientDataC[a][b]-minC)/(maxC-minC);
	    }
	}

	// get Gradient for tuning threshold
	int SizeHG = 361;
	float hist[SizeHG];
	float histcpf[SizeHG];
	memset(hist, 0, SizeHG*sizeof(float));
	memset(histcpf, 0, SizeHG*sizeof(float));

	GetHG(&GradientData[0][0], hist, SizeH, SizeW, SizeHG);
	OutputHG("sgrad.txt", hist, SizeHG);
	Getcpf(hist, histcpf, SizeHG, SizeH, SizeW);
	OutputHG("sgradcpf.txt", histcpf, SizeHG);

	return 0;
}

int LoG(int Dfiltersize, float sigma, float thr, unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputGradient, unsigned char *ternarymap, unsigned char *outputImageData)
{
	// extend image
	int extend = (Dfiltersize-1)/2;
	int exH = SizeH+2*extend;  // extendSizeH
	int exW = SizeW+2*extend;  //extendSizeW
	unsigned char ext[exH][exW];  // extend image data

	// Gaussian filtering
	unsigned char denoisingimage[SizeH][SizeW];
	GaussianLMFilter(Dfiltersize, sigma, inputImageData, SizeH, SizeW, &denoisingimage[0][0]);

	// extend image
	extendonechannel(&denoisingimage[0][0], SizeH, SizeW, &ext[0][0], extend);

	// 5-point Laplacian
	float GradientData[SizeH][SizeW];
	memset(GradientData, 0, SizeH*SizeW*sizeof(float));
	int Z[SizeH][SizeW];
	int min = 256, max = -256;

	for(int i = extend; i < (SizeH + extend); i++)
	{
		for(int j = extend; j < (SizeW + extend); j++)
		{
			int a = i - extend;
			int b = j - extend;
			int A1 = ext[i-1][j];
			int A3 = ext[i][j+1];
			int A5 = ext[i+1][j];
			int A7 = ext[i][j-1];

			GradientData[a][b] = (4.0*ext[i][j] - A1 - A3 -A5 -A7) / 4.0;

	        if(GradientData[a][b] > max)
	            max = GradientData[a][b];
	        if(GradientData[a][b] < min)
	            min = GradientData[a][b];

	        if(GradientData[a][b] < -thr)
	        {
	        	Z[a][b] = -1;
	        	ternarymap[a*SizeW+b] = 64;
	        }
	        else if(GradientData[a][b] > thr)
	        {
	            Z[a][b] = 1;
	            ternarymap[a*SizeW+b] = 192;
	        }
	        else
	        {
	        	Z[a][b] = 0;
	        	ternarymap[a*SizeW+b] = 128;
	        }

		}
	}

	// get Gradient for find threshold
	int SizeHG = max*10;
	int Gradient[SizeH][SizeW];
	float hist[SizeHG];
	memset(Gradient, 0, SizeH*SizeW*sizeof(int));
	memset(hist, 0, SizeHG*sizeof(float));

	for(int a=0; a<SizeH; a++)
	{
		for(int b=0; b<SizeW; b++)
		{
			Gradient[a][b] = GradientData[a][b]*10.0;
		}
	}


	GetHG(&Gradient[0][0], hist, SizeH, SizeW, SizeHG);
	OutputHG("LoG_grad.txt", hist, SizeHG);


	// Normalize the LoG response to 0-255
	for(int a=0; a<SizeH; a++)
	{
	    for(int b=0; b<SizeW; b++)
	    {
	    	outputGradient[a*SizeW+b] = 255.0*(GradientData[a][b]-min)/(max-min);

	    }
	}

	// Zero-Crossing check
	int extendZ[exH][exW];
	ExtendMap(&Z[0][0], SizeH, SizeW, &extendZ[0][0], extend);
	for (int i = extend; i < (SizeH+extend); i++)
	{
		for (int j = extend; j < (SizeW+extend); j++)
		{
			int a = i - extend;
			int b = j - extend;
			int A0 = extendZ[i-1][j-1];
			int A1 = extendZ[i-1][j];
			int A2 = extendZ[i-1][j+1];
			int A3 = extendZ[i][j+1];
			int A4 = extendZ[i+1][j+1];
			int A5 = extendZ[i+1][j];
			int A6 = extendZ[i+1][j-1];
			int A7 = extendZ[i][j-1];

			if ((A0*A4 == (-1)) || (A1*A5 == (-1)) || (A2*A6 == (-1)) || (A3*A7 == (-1)))
			{
				outputImageData[a*SizeW+b] = 0;
			}
			else
			{
				outputImageData[a*SizeW+b] = 255;
			}
		}
	}

	return 0;
}

int XDoG(int windowsize, float sigma, float k, float t, float e, float f, const unsigned char *inputImageData, unsigned char *outputImageData, int SizeH, int SizeW)
{

	unsigned char G1ImageData[SizeH][SizeW];
	unsigned char G2ImageData[SizeH][SizeW];
	double DxImageData[SizeH][SizeW];

	GaussianLMFilter(windowsize, sigma, inputImageData, SizeH, SizeW, &G1ImageData[0][0]);
	GaussianLMFilter(windowsize, (sigma*k), inputImageData, SizeH, SizeW, &G2ImageData[0][0]);

	for(int i = 0; i<SizeH; i++)
	{
		for(int j = 0; j < SizeW; j++)
		{
			DxImageData[i][j] = (double) (G1ImageData[i][j] - t * G2ImageData[i][j]) ;
//			printf("%d %f = %u -  %u\n", i, DxImageData[i][j], G1ImageData[i][j],G2ImageData[i][j]);


			if(DxImageData[i][j] > e)
			{
				outputImageData[i*SizeW+j] = 1*255;
			}
			else
			{
				outputImageData[i*SizeW+j] =(( 1 + tanh(f*(DxImageData[i][j] - e)) ) * 255);
			}
		}
	}
	writeimage("1b G1_ans.raw", &G1ImageData[0][0], SizeH, SizeW, 1);
	writeimage("1b G2_ans.raw", &G2ImageData[0][0], SizeH, SizeW, 1);
//	writeimage("1b Dx.raw", &Dx[0][0], SizeH, SizeW, 1);

	return 0;
}

int DoG(int windowsize, float sigma, float k, float e, float f, const unsigned char *inputImageData, unsigned char *outputImageData, int SizeH, int SizeW)
{

	unsigned char G1ImageData[SizeH][SizeW];
	unsigned char G2ImageData[SizeH][SizeW];
	double DxImageData[SizeH][SizeW];

	GaussianLMFilter(windowsize, sigma, inputImageData, SizeH, SizeW, &G1ImageData[0][0]);
	GaussianLMFilter(windowsize, (sigma*k), inputImageData, SizeH, SizeW, &G2ImageData[0][0]);

	for(int i = 0; i<SizeH; i++)
	{
		for(int j = 0; j < SizeW; j++)
		{
			DxImageData[i][j] = (double) (G1ImageData[i][j] - G2ImageData[i][j]) ;
//			printf("%d %f = %u -  %u\n", i, DxImageData[i][j], G1ImageData[i][j],G2ImageData[i][j]);


			if(DxImageData[i][j] > e)
			{
				outputImageData[i*SizeW+j] = 1*255;
			}
			else
			{
				outputImageData[i*SizeW+j] =(( 1 + tanh(f*(DxImageData[i][j] - e)) ) * 255);
			}
		}
	}
//	writeimage("1b G1_ans.raw", &G1ImageData[0][0], SizeH, SizeW, 1);
//	writeimage("1b G2_ans.raw", &G2ImageData[0][0], SizeH, SizeW, 1);
//	writeimage("1b Dx.raw", &Dx[0][0], SizeH, SizeW, 1);

	return 0;
}

