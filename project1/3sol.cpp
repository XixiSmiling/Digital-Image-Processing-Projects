#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "iofile.h"
#include "histogram.h"
#include "Filters.h"

int Q3a(void)
{
	// initialize data of "Lena_mixed.raw"
	const char *filename = "Lena_mixed.raw";
	int SizeW = 512;
	int SizeH = 512;
	int BytesPerPixel = 3;
	unsigned char ImageData[SizeH][SizeW][BytesPerPixel];
	unsigned char Red[SizeH][SizeW];
	unsigned char Green[SizeH][SizeW];
	unsigned char Blue[SizeH][SizeW];

	// initialize data of "Lena.raw"
//	const char *anotherfilename = "Lena_mixed.raw";
//	unsigned char anotherImageData[SizeH][SizeW][BytesPerPixel];

	// initialize data of Denoising Lena
	const char *newfilename1 = "3a Lena_GaussionFirst_ans.raw";
	const char *newfilename2 = "3a Lena_medianFirst_ans.raw";
	const char *newfilename3 = "3a Lena_uniform_ans.raw";
	int newSizeW = 512;
	int newSizeH = 512;
	int newBytesPerPixel = 3;
	unsigned char newImageData1[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData2[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData3[newSizeH][newSizeW][newBytesPerPixel];

	// read
	readimage(filename, &ImageData[0][0][0], SizeH, SizeW, BytesPerPixel);
//	readimage(anotherfilename, &anotherImageData[0][0][0], SizeH, SizeW, BytesPerPixel);

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			Red[i][j] = ImageData[i][j][0];
			Green[i][j] = ImageData[i][j][1];
			Blue[i][j] = ImageData[i][j][2];
		}
	}

	// get histograms of each channal for report
	int hgR[256], hgG[256], hgB[256];
//	int anotherhgR[256], anotherhgG[256], anotherhgB[256];
	memset(hgR, 0, 256*sizeof(int));
	memset(hgG, 0, 256*sizeof(int));
	memset(hgB, 0, 256*sizeof(int));
//	memset(anotherhgR, 0, 256*sizeof(int));
//	memset(anotherhgG, 0, 256*sizeof(int));
//	memset(anotherhgB, 0, 256*sizeof(int));

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			hgR[ImageData[i][j][0]] ++;
			hgG[ImageData[i][j][1]] ++;
			hgB[ImageData[i][j][2]] ++;
//			anotherhgR[anotherImageData[i][j][0]] ++;
//			anotherhgG[anotherImageData[i][j][1]] ++;
//			anotherhgB[anotherImageData[i][j][2]] ++;
		}
	}
	OutputHG("P3 Lena_mixed_R_hist.dat", hgR, 256);
	OutputHG("P3 Lena_mixed_G_hist.dat", hgG, 256);
	OutputHG("P3 Lena_mixed_B_hist.dat", hgB, 256);
//	OutputHG("P3 Lena_R_hist.dat", anotherhgR, 256);
//	OutputHG("P3 Lena_G_hist.dat", anotherhgG, 256);
//	OutputHG("P3 Lena_B_hist.dat", anotherhgB, 256);

	int extend = 30;
	int extendSizeH = SizeH+2*extend;
	int extendSizeW = SizeW+2*extend;
	unsigned char extendR[extendSizeH][extendSizeW];
	unsigned char extendG[extendSizeH][extendSizeW];
	unsigned char extendB[extendSizeH][extendSizeW];
	extendonechannel(&Red[0][0], SizeH, SizeW, &extendR[0][0], extend);
	extendonechannel(&Green[0][0], SizeH, SizeW, &extendG[0][0], extend);
	extendonechannel(&Blue[0][0], SizeH, SizeW, &extendB[0][0], extend);

//get extendImageData
//	unsigned char extendImageData[extendSizeH][extendSizeW][BytesPerPixel];
//	for(int i=0; i<extendSizeH; i++)
//	{
//		for(int j=0; j<extendSizeW; j++)
//		{
//			extendImageData[i][j][0] = extendR[i][j];
//			extendImageData[i][j][1] = extendG[i][j];
//			extendImageData[i][j][2] = extendB[i][j];
//		}
//	}

	//************** remove noise begin:  ********************

	int filtersize1 = 3;
	int filtersize2 = 5;
	float sigma = 5.0;
	unsigned char newR[SizeH][SizeW];
	unsigned char newG[SizeH][SizeW];
	unsigned char newB[SizeH][SizeW];


	// denoising blue channel with Gaussian filter
	GaussianLMFilter(filtersize1, sigma, &extendB[0][0], SizeH, SizeW, &newB[0][0]);

	// denoising green channel with median filter
	medianFilter(filtersize2, &extendG[0][0], SizeH, SizeW, &newG[0][0]);

	// denoising red channel with Gaussian filter first then median filter
	GaussianLMFilter(filtersize1, sigma, &extendR[0][0], SizeH, SizeW, &newR[0][0]);

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			newImageData1[i][j][0] = newR[i][j];
			newImageData1[i][j][1] = newG[i][j];
			newImageData1[i][j][2] = newB[i][j];
		}
	}

	unsigned char extendnewR[extendSizeH][extendSizeW];
//	unsigned char extendnewG[extendSizeH][extendSizeW];
	unsigned char extendnewB[extendSizeH][extendSizeW];
	extendonechannel(&newR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
//	extendonechannel(&newG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&newB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	unsigned char newnewR[SizeH][SizeW];
//	unsigned char newnewG[SizeH][SizeW];
	unsigned char newnewB[SizeH][SizeW];
	medianFilter(filtersize2, &extendnewR[0][0], SizeH, SizeW, &newnewR[0][0]);
//	medianFilter(filtersize, &extendnewG[0][0], SizeH, SizeW, &newnewG[0][0]);
	medianFilter(filtersize2, &extendnewB[0][0], SizeH, SizeW, &newnewB[0][0]);

	for(int i=0; i<SizeH; i++)
	{
			for(int j=0; j<SizeW; j++)
			{
				newImageData1[i][j][0] = newnewR[i][j];
				newImageData1[i][j][1] = newG[i][j];
				newImageData1[i][j][2] = newnewB[i][j];
			}
	}

	// output
	writeimage(newfilename1, &newImageData1[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	printf("Red channel with Gaussian filter first:\n");
	PSNR(&Red[0][0], &newnewR[0][0], SizeH, SizeW);
	printf("Green channel:\n");
	PSNR(&Green[0][0], &newG[0][0], SizeH, SizeW);
	printf("Blue channel with Gaussian filter first:\n");
	PSNR(&Blue[0][0], &newnewB[0][0], SizeH, SizeW);

	// denoising blue channel with median filter first then Gaussian filter
	medianFilter(filtersize2, &extendR[0][0], SizeH, SizeW, &newR[0][0]);
//	medianFilter(filtersize, &extendG[0][0], SizeH, SizeW, &newG[0][0]);
	medianFilter(filtersize2, &extendB[0][0], SizeH, SizeW, &newB[0][0]);

	extendonechannel(&newR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
//	extendonechannel(&newG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&newB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	GaussianLMFilter(filtersize1, sigma, &extendnewR[0][0], SizeH, SizeW, &newnewR[0][0]);
//	UniformLMFilter(filtersize, sigma, &extendnewG[0][0], SizeH, SizeW, &newnewG[0][0]);
	GaussianLMFilter(filtersize1, sigma, &extendnewB[0][0], SizeH, SizeW, &newnewB[0][0]);

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			newImageData2[i][j][0] = newnewR[i][j];
			newImageData2[i][j][1] = newG[i][j];
			newImageData2[i][j][2] = newnewB[i][j];
		}
	}

	// output
	writeimage(newfilename2, &newImageData2[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	// calculate PSNR and MSE
//	PSNR(&Red[0][0], &newnewR[0][0], SizeH, SizeW);
//	PSNR(&Green[0][0], &newnewG[0][0], SizeH, SizeW);
	printf("Red channel with median filter first:\n");
	PSNR(&Red[0][0], &newnewR[0][0], SizeH, SizeW);
	printf("Blue channel with median filter first:\n");
	PSNR(&Blue[0][0], &newnewB[0][0], SizeH, SizeW);

	// try uniform local mean filter
//	float sigma_o = 0.5;
//	UniformLMFilter(filtersize, sigma_o, &extendB[0][0], SizeH, SizeW, &newB[0][0]);
//	for(int i=0; i<SizeH; i++)
//		{
//			for(int j=0; j<SizeW; j++)
//			{
//				newImageData3[i][j][0] = newnewR[i][j];
//				newImageData3[i][j][1] = newG[i][j];
//				newImageData3[i][j][2] = newB[i][j];
//			}
//		}
//
//	// output
//	writeimage(newfilename3, &newImageData3[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
//	printf("Blue channel with uniform local mean filter first:\n");
//	PSNR(&Blue[0][0], &newB[0][0], SizeH, SizeW);




	return 0;
}

int Q3b(void)
{
	// initialize data of "Lena_mixed.raw"
	const char *filename = "Lena_mixed.raw";
	const char *filename0 = "Lena.raw";
	int SizeW = 512;
	int SizeH = 512;
	int BytesPerPixel = 3;
	unsigned char ImageData[SizeH][SizeW][BytesPerPixel];
	unsigned char ImageData0[SizeH][SizeW][BytesPerPixel];
	unsigned char Red[SizeH][SizeW];
	unsigned char Green[SizeH][SizeW];
	unsigned char Blue[SizeH][SizeW];
	unsigned char Red0[SizeH][SizeW];
	unsigned char Green0[SizeH][SizeW];
	unsigned char Blue0[SizeH][SizeW];

	// initialize data of Denoising Lena
	const char *newfilename1 = "3b Lena_BilateralFilter_ans.raw";
	const char *newfilename1R = "3b Lena_BF_R_ans.raw";
	const char *newfilename1G = "3b Lena_BF_G_ans.raw";
	const char *newfilename1B = "3b Lena_BF_B_ans.raw";
	const char *newfilename2 = "3b Lena_cartoon_BF_ans.raw";
	const char *newfilename2R = "3b Lena_cartoon_R_ans.raw";
	const char *newfilename2G = "3b Lena_cartoon_G_ans.raw";
	const char *newfilename2B = "3b Lena_cartoon_B_ans.raw";
	int newSizeW = 512;
	int newSizeH = 512;
	int newBytesPerPixel = 3;
	unsigned char newImageData1[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData1R[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData1G[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData1B[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData2[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData2R[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData2G[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData2B[newSizeH][newSizeW][newBytesPerPixel];

	// read
	readimage(filename, &ImageData[0][0][0], SizeH, SizeW, BytesPerPixel);
	readimage(filename0, &ImageData0[0][0][0], SizeH, SizeW, BytesPerPixel);

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			Red[i][j] = ImageData[i][j][0];
			Green[i][j] = ImageData[i][j][1];
			Blue[i][j] = ImageData[i][j][2];
			Red0[i][j] = ImageData0[i][j][0];
			Green0[i][j] = ImageData0[i][j][1];
			Blue0[i][j] = ImageData0[i][j][2];
		}
	}

	int extend = 30;
	int extendSizeH = SizeH+2*extend;
	int extendSizeW = SizeW+2*extend;
	unsigned char extendR[extendSizeH][extendSizeW];
	unsigned char extendG[extendSizeH][extendSizeW];
	unsigned char extendB[extendSizeH][extendSizeW];
	extendonechannel(&Red[0][0], SizeH, SizeW, &extendR[0][0], extend);
	extendonechannel(&Green[0][0], SizeH, SizeW, &extendG[0][0], extend);
	extendonechannel(&Blue[0][0], SizeH, SizeW, &extendB[0][0], extend);

	//***** median filter first*********
	int filtersize1 = 5;
	unsigned char newR[SizeH][SizeW];
	unsigned char newG[SizeH][SizeW];
	unsigned char newB[SizeH][SizeW];

//	medianFilter(filtersize1, &extendR[0][0], SizeH, SizeW, &newR[0][0]);
//	medianFilter(filtersize1, &extendG[0][0], SizeH, SizeW, &newG[0][0]);
//	medianFilter(filtersize1, &extendB[0][0], SizeH, SizeW, &newB[0][0]);

	unsigned char extendnewR[extendSizeH][extendSizeW];
	unsigned char extendnewG[extendSizeH][extendSizeW];
	unsigned char extendnewB[extendSizeH][extendSizeW];

//	extendonechannel(&newR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
//	extendonechannel(&newG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
//	extendonechannel(&newB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);


	// Perform Bilateral Filter
	int FilterSize = 5;
	int sigmaC = 3;
	int sigmaS = 100;
	unsigned char newnewR[SizeH][SizeW];
	unsigned char newnewG[SizeH][SizeW];
	unsigned char newnewB[SizeH][SizeW];

	BilateralFilter(FilterSize, sigmaC, sigmaS, &extendR[0][0], SizeH, SizeW, &newR[0][0]);
	BilateralFilter(FilterSize, sigmaC, sigmaS, &extendG[0][0], SizeH, SizeW, &newG[0][0]);
	BilateralFilter(FilterSize, sigmaC, sigmaS, &extendB[0][0], SizeH, SizeW, &newB[0][0]);

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			newImageData1[i][j][0] = newR[i][j];
			newImageData1[i][j][1] = newG[i][j];
			newImageData1[i][j][2] = newB[i][j];
			newImageData1R[i][j][0] = newR[i][j];
			newImageData1R[i][j][1] = 0;
			newImageData1R[i][j][2] = 0;
			newImageData1G[i][j][0] = 0;
			newImageData1G[i][j][1] = newG[i][j];
			newImageData1G[i][j][2] = 0;
			newImageData1B[i][j][0] = 0;
			newImageData1B[i][j][1] = 0;
			newImageData1B[i][j][2] = newB[i][j];
		}
	}

	// output
	writeimage(newfilename1, &newImageData1[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	writeimage(newfilename1R, &newImageData1R[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	writeimage(newfilename1G, &newImageData1G[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	writeimage(newfilename1B, &newImageData1B[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	// cartoon effect

	int sigmaC0 = 5;
	int sigmaS0 = 10;

	extendonechannel(&Red0[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
	extendonechannel(&Green0[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&Blue0[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newR[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newG[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newB[0][0]);

	extendonechannel(&newR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
	extendonechannel(&newG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&newB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newnewR[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newnewG[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newnewB[0][0]);

	extendonechannel(&newnewR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
	extendonechannel(&newnewG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&newnewB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newR[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newG[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newB[0][0]);

	extendonechannel(&newR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
	extendonechannel(&newG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&newB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newnewR[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newnewG[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newnewB[0][0]);

	extendonechannel(&newnewR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
	extendonechannel(&newnewG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&newnewB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newR[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newG[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newB[0][0]);

	extendonechannel(&newR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
	extendonechannel(&newG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&newB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newnewR[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newnewG[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newnewB[0][0]);

	extendonechannel(&newnewR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
	extendonechannel(&newnewG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&newnewB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newR[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newG[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newB[0][0]);

	extendonechannel(&newR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
	extendonechannel(&newG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&newB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newnewR[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newnewG[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newnewB[0][0]);

	extendonechannel(&newnewR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
	extendonechannel(&newnewG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&newnewB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newR[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newG[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newB[0][0]);

	extendonechannel(&newR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
	extendonechannel(&newG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
	extendonechannel(&newB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);

	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newnewR[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newnewG[0][0]);
	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newnewB[0][0]);

//	extendonechannel(&newnewR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
//	extendonechannel(&newnewG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
//	extendonechannel(&newnewB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);
//
//	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newR[0][0]);
//	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newG[0][0]);
//	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newB[0][0]);
//
//	extendonechannel(&newR[0][0], SizeH, SizeW, &extendnewR[0][0], extend);
//	extendonechannel(&newG[0][0], SizeH, SizeW, &extendnewG[0][0], extend);
//	extendonechannel(&newB[0][0], SizeH, SizeW, &extendnewB[0][0], extend);
//
//	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewR[0][0], SizeH, SizeW, &newnewR[0][0]);
//	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewG[0][0], SizeH, SizeW, &newnewG[0][0]);
//	BilateralFilter(FilterSize, sigmaC0, sigmaS0, &extendnewB[0][0], SizeH, SizeW, &newnewB[0][0]);



	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			newImageData2[i][j][0] = newnewR[i][j];
			newImageData2[i][j][1] = newnewG[i][j];
			newImageData2[i][j][2] = newnewB[i][j];
			newImageData2R[i][j][0] = newnewR[i][j];
			newImageData2R[i][j][1] = 0;
			newImageData2R[i][j][2] = 0;
			newImageData2G[i][j][0] = 0;
			newImageData2G[i][j][1] = newnewG[i][j];
			newImageData2G[i][j][2] = 0;
			newImageData2B[i][j][0] = 0;
			newImageData2B[i][j][1] = 0;
			newImageData2B[i][j][2] = newnewB[i][j];
		}
	}

	// output
	writeimage(newfilename2, &newImageData2[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	writeimage(newfilename2R, &newImageData2R[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	writeimage(newfilename2G, &newImageData2G[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	writeimage(newfilename2B, &newImageData2B[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	return 0;
}

int Q3c(void)
{
	// initialize data of "Lena_mixed.raw"
	const char *filename = "Lena_mixed.raw";
	int SizeW = 512;
	int SizeH = 512;
	int BytesPerPixel = 3;
	unsigned char ImageData[SizeH][SizeW][BytesPerPixel];
	unsigned char Red[SizeH][SizeW];
	unsigned char Green[SizeH][SizeW];
	unsigned char Blue[SizeH][SizeW];

	// initialize data of Denoising Lena
	const char *newfilename = "3c Lena_NLM_ans.raw";
	int newSizeW = 512;
	int newSizeH = 512;
	int newBytesPerPixel = 3;
	unsigned char newImageData[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newR[SizeH][SizeW];
	unsigned char newG[SizeH][SizeW];
	unsigned char newB[SizeH][SizeW];

	// read
	readimage(filename, &ImageData[0][0][0], SizeH, SizeW, BytesPerPixel);

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			Red[i][j] = ImageData[i][j][0];
			Green[i][j] = ImageData[i][j][1];
			Blue[i][j] = ImageData[i][j][2];
		}
	}

	int extend = 30;
	int extendSizeH = SizeH+2*extend;
	int extendSizeW = SizeW+2*extend;
	unsigned char extendR[extendSizeH][extendSizeW];
	unsigned char extendG[extendSizeH][extendSizeW];
	unsigned char extendB[extendSizeH][extendSizeW];
	extendonechannel(&Red[0][0], SizeH, SizeW, &extendR[0][0], extend);
	extendonechannel(&Green[0][0], SizeH, SizeW, &extendG[0][0], extend);
	extendonechannel(&Blue[0][0], SizeH, SizeW, &extendB[0][0], extend);


	// NML Filter
	int FilterSize = 30, neighborsize = 3, sigma = 20;
	printf("please wait patiently ^_^\n");
	NLMFilter(FilterSize, neighborsize, sigma, &extendR[0][0], SizeH, SizeW, &newR[0][0]);
	NLMFilter(FilterSize, neighborsize, sigma, &extendG[0][0], SizeH, SizeW, &newG[0][0]);
	NLMFilter(FilterSize, neighborsize, sigma, &extendB[0][0], SizeH, SizeW, &newB[0][0]);

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			newImageData[i][j][0] = newR[i][j];
			newImageData[i][j][1] = newG[i][j];
			newImageData[i][j][2] = newB[i][j];
		}
	}

	// output
	writeimage(newfilename, &newImageData[0][0][0], newSizeH, newSizeW, newBytesPerPixel);



	return 0;
}



int main(void)
{
	Q3a();
	Q3b();
	Q3c();

	return 0;
}
