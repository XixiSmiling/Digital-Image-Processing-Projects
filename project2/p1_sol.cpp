#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "iofile.h"
#include "DenoisingFilters.h"
#include "EdgeDetectionFilters.h"
#include "colorspace.h"

// ******************  Problem 1: Edge Detection ***********************
int p1a(void)
{
	// initialize data of input
	const char *filename1 = "elaine.raw";
	const char *filename2 = "noisy_elaine.raw";
	int SizeW = 256;
	int SizeH = 256;
	int BytesPerPixel = 1;
	unsigned char ImageData1[SizeH][SizeW][BytesPerPixel];
	unsigned char ImageData2[SizeH][SizeW][BytesPerPixel];

	// initialize data of output
	const char *newfilename1a = "1a elaine_soble_ans.raw";
	const char *newfilename1b = "1a elaine_LoG_ans.raw";
	const char *newfilename2a = "1a noisyelaine_soble_ans.raw";
	const char *newfilename2b = "1a noisyelaine_LoG_ans.raw";
	int newSizeW = SizeW;
	int newSizeH = SizeH;
	int newBytesPerPixel = BytesPerPixel;
	unsigned char newImageData1a[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData1b[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData2a[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData2b[newSizeH][newSizeW][newBytesPerPixel];

	// read input
	readimage(filename1, &ImageData1[0][0][0], SizeH, SizeW, BytesPerPixel);
	readimage(filename2, &ImageData2[0][0][0], SizeH, SizeW, BytesPerPixel);

	// --------------------Sobel Detector------------------------
	int extend = 2;
	int thr1a = 24;
	int thr2a = 37;
	unsigned char Gradient1aR[SizeH][SizeW];
	unsigned char Gradient1aC[SizeH][SizeW];
	unsigned char Gradient1a[SizeH][SizeW];
	const char *gradient1aR = "1a elaine_s_R_grad.raw";
	const char *gradient1aC = "1a elaine_s_C_grad.raw";
	const char *gradient1a = "1a elaine_soble_grad.raw";
	unsigned char Gradient2aR[SizeH][SizeW];
	unsigned char Gradient2aC[SizeH][SizeW];
	unsigned char Gradient2a[SizeH][SizeW];
	const char *gradient2aR = "1a noisingelaine_s_R_grad.raw";
	const char *gradient2aC = "1a noisingelaine_s_C_grad.raw";
	const char *gradient2a = "1a noisingelaine_soble_grad.raw";

	GetSobleGradient(thr1a, &ImageData1[0][0][0], SizeH, SizeW, extend, &Gradient1aR[0][0], &Gradient1aC[0][0], &Gradient1a[0][0], &newImageData1a[0][0][0]);
	GetSobleGradient(thr2a, &ImageData2[0][0][0], SizeH, SizeW, extend, &Gradient2aR[0][0], &Gradient2aC[0][0], &Gradient2a[0][0], &newImageData2a[0][0][0]);

	// output
	writeimage(gradient1aR, &Gradient1aR[0][0], SizeH, SizeW, BytesPerPixel);
	writeimage(gradient1aC, &Gradient1aC[0][0], SizeH, SizeW, BytesPerPixel);
	writeimage(gradient1a, &Gradient1a[0][0], SizeH, SizeW, BytesPerPixel);
	writeimage(newfilename1a, &newImageData1a[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	writeimage(gradient2aR, &Gradient2aR[0][0], SizeH, SizeW, BytesPerPixel);
	writeimage(gradient2aC, &Gradient2aC[0][0], SizeH, SizeW, BytesPerPixel);
	writeimage(gradient2a, &Gradient2a[0][0], SizeH, SizeW, BytesPerPixel);
	writeimage(newfilename2a, &newImageData2a[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	// ----------------------LoG Detector------------------------------
	int Dfiltersize = 9;
	float sigma = 1.0;
	float thr1b = 0.9;
	float thr2b = 1.5;
	unsigned char Gradient1b[SizeH][SizeW];
	unsigned char ternarymap1[SizeH][SizeW];
	unsigned char Gradient2b[SizeH][SizeW];
	unsigned char ternarymap2[SizeH][SizeW];
	const char *gradient1b = "1a elaine_LoG_grad.raw";
	const char *ternary1 = "1a elaine_LoG_ter.raw";
	const char *gradient2b = "1a noisingelaine_LoG_grad.raw";
	const char *ternary2 = "1a noisingelaine_LoG_ter.raw";

	LoG(Dfiltersize, sigma, thr1b, &ImageData1[0][0][0], SizeH, SizeW, &Gradient1b[0][0], &ternarymap1[0][0], &newImageData1b[0][0][0]);
	LoG(Dfiltersize, sigma, thr2b, &ImageData2[0][0][0], SizeH, SizeW, &Gradient2b[0][0], &ternarymap2[0][0], &newImageData2b[0][0][0]);

	writeimage(gradient1b, &Gradient1b[0][0], SizeH, SizeW, BytesPerPixel);
	writeimage(ternary1, &ternarymap1[0][0], SizeH, SizeW, BytesPerPixel);
	writeimage(newfilename1b, &newImageData1b[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	writeimage(gradient2b, &Gradient2b[0][0], SizeH, SizeW, BytesPerPixel);
	writeimage(ternary2, &ternarymap2[0][0], SizeH, SizeW, BytesPerPixel);
	writeimage(newfilename2b, &newImageData2b[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	return 0;
}

int p1b(void)
{
	// initialize data of input
	const char *filename = "Scarlett.raw";
	int SizeW = 400;
	int SizeH = 300;
	int BytesPerPixel = 3;
	unsigned char ImageData[SizeH][SizeW][BytesPerPixel];
	unsigned char R[SizeH][SizeW];
	unsigned char G[SizeH][SizeW];
	unsigned char B[SizeH][SizeW];
	double dImageData[SizeH][SizeW][BytesPerPixel];

	// initialize data of output
	const char *BGfilename = "1b Scarlett_lab_bf.raw";
	const char *BGfilename1 = "1b Scarlett_rgb_bf.raw";
	const char *Exfilename = "1b Scarlett_ex.raw";
	const char *newfilename = "1b Scarlett_ans.raw";
	int newSizeW = SizeW;
	int newSizeH = SizeH;
	int newBytesPerPixel = BytesPerPixel;
	double MBGImageData[newSizeH][newSizeW][newBytesPerPixel];
	double MCImageData[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char BGImageData[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char BGImageData1[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char CBGImageData[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData[newSizeH][newSizeW][newBytesPerPixel];

	// input
	readimage(filename, &ImageData[0][0][0], SizeH, SizeW, BytesPerPixel);

	double L[SizeH][SizeW];
	double a[SizeH][SizeW];
	double b[SizeH][SizeW];

	for(int i = 0; i<SizeH; i++)
	{
		for(int j = 0; j < SizeW; j++)
		{
			R[i][j] = ImageData[i][j][0];
			G[i][j] = ImageData[i][j][1];
			B[i][j] = ImageData[i][j][2];
			dImageData[i][j][0] = ((double) ImageData[i][j][0]/255.0);
			dImageData[i][j][1] = ((double) ImageData[i][j][1]/255.0);
			dImageData[i][j][2] = ((double) ImageData[i][j][2]/255.0);
			Rgb2Lab(&L[i][j], &a[i][j], &b[i][j], dImageData[i][j][0], dImageData[i][j][1], dImageData[i][j][2]);
//			printf("%d, %f = %u, %f = %u, %f = %u\n", i,dImageData[i][j][0], ImageData[i][j][0], dImageData[i][j][1],ImageData[i][j][1], dImageData[i][j][2], ImageData[i][j][2]);
//			printf("%d, %f, %f, %f\n", i,L[i][j], a[i][j],b[i][j]);
		}
	}

	// Bilateral Filter
	double newL[SizeH][SizeW];
	double newa[SizeH][SizeW];
	double newb[SizeH][SizeW];
	double newnewL[SizeH][SizeW];
	double newnewa[SizeH][SizeW];
	double newnewb[SizeH][SizeW];
	unsigned char newR[SizeH][SizeW];
	unsigned char newG[SizeH][SizeW];
	unsigned char newB[SizeH][SizeW];
	unsigned char newnewR[SizeH][SizeW];
	unsigned char newnewG[SizeH][SizeW];
	unsigned char newnewB[SizeH][SizeW];

	int FilterSize = 5;
	int sigmaC = 3;
	int sigmaC1 = 10;
	double sigmaS = 4.25;
	double sigmaS1 = 10.0;

	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &R[0][0], SizeH, SizeW, &newR[0][0]);
	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &G[0][0], SizeH, SizeW, &newG[0][0]);
	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &B[0][0], SizeH, SizeW, &newB[0][0]);
	BFiforLab(FilterSize, sigmaC, sigmaS, &L[0][0], SizeH, SizeW, &newL[0][0]);
	BFiforLab(FilterSize, sigmaC, sigmaS, &a[0][0], SizeH, SizeW, &newa[0][0]);
	BFiforLab(FilterSize, sigmaC, sigmaS, &b[0][0], SizeH, SizeW, &newb[0][0]);

	for(int i = 0; i<SizeH; i++)
	{
		for(int j = 0; j < SizeW; j++)
		{
			Lab2Rgb(&MCImageData[i][j][0], &MCImageData[i][j][1], &MCImageData[i][j][2], newL[i][j], newa[i][j], newb[i][j]);
			CBGImageData[i][j][0] = (int) (MCImageData[i][j][0]*255);
			CBGImageData[i][j][1] = (int) (MCImageData[i][j][1]*255);
			CBGImageData[i][j][2] = (int) (MCImageData[i][j][2]*255);
		}
	}


	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &newR[0][0], SizeH, SizeW, &newnewR[0][0]);
	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &newG[0][0], SizeH, SizeW, &newnewG[0][0]);
	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &newB[0][0], SizeH, SizeW, &newnewB[0][0]);
	BFiforLab(FilterSize, sigmaC, sigmaS, &newL[0][0], SizeH, SizeW, &newnewL[0][0]);
	BFiforLab(FilterSize, sigmaC, sigmaS, &newa[0][0], SizeH, SizeW, &newnewa[0][0]);
	BFiforLab(FilterSize, sigmaC, sigmaS, &newb[0][0], SizeH, SizeW, &newnewb[0][0]);

	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &newnewR[0][0], SizeH, SizeW, &newR[0][0]);
	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &newnewG[0][0], SizeH, SizeW, &newG[0][0]);
	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &newnewB[0][0], SizeH, SizeW, &newB[0][0]);
	BFiforLab(FilterSize, sigmaC, sigmaS, &newnewL[0][0], SizeH, SizeW, &newL[0][0]);
	BFiforLab(FilterSize, sigmaC, sigmaS, &newnewa[0][0], SizeH, SizeW, &newa[0][0]);
	BFiforLab(FilterSize, sigmaC, sigmaS, &newnewb[0][0], SizeH, SizeW, &newb[0][0]);

//	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &newR[0][0], SizeH, SizeW, &newnewR[0][0]);
//	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &newG[0][0], SizeH, SizeW, &newnewG[0][0]);
//	BilateralFilter(FilterSize, sigmaC1, sigmaS1, &newB[0][0], SizeH, SizeW, &newnewB[0][0]);

	for(int i = 0; i<SizeH; i++)
	{
		for(int j = 0; j < SizeW; j++)
		{
			Lab2Rgb(&MBGImageData[i][j][0], &MBGImageData[i][j][1], &MBGImageData[i][j][2], newL[i][j], newa[i][j], newb[i][j]);
			BGImageData[i][j][0] = (int) (MBGImageData[i][j][0]*255);
			BGImageData[i][j][1] = (int) (MBGImageData[i][j][1]*255);
			BGImageData[i][j][2] = (int) (MBGImageData[i][j][2]*255);
			BGImageData1[i][j][0] = newR[i][j];
			BGImageData1[i][j][1] = newG[i][j];
			BGImageData1[i][j][2] = newB[i][j];
//			printf("%d, %f, %f, %f\n", i,newL[i][j], newa[i][j],newb[i][j]);
//			printf("%d, %u = %f, %u = %f, %u = %f\n", i,newImageData[i][j][0], MImageData[i][j][0],newImageData[i][j][1], MImageData[i][j][1], newImageData[i][j][2], MImageData[i][j][2]);
		}
	}

	writeimage(BGfilename, &BGImageData[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	writeimage(BGfilename1, &BGImageData1[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	// XDoG
	// convert to gray-scale image
	unsigned char GrayImageData[newSizeH][newSizeW];
	for(int i = 0; i<SizeH; i++)
	{
		for(int j = 0; j < SizeW; j++)
		{
			float Y = 0.21*CBGImageData[i][j][0] + 0.72*CBGImageData[i][j][1] +0.07*CBGImageData[i][j][2];
			GrayImageData[i][j] = (int) Y;
//			printf("%d %u = %f\n", i, GrayImageData[i][j], Y);
		}
	}

	unsigned char GrayImageData0[newSizeH][newSizeW];
	for(int i = 0; i<SizeH; i++)
	{
		for(int j = 0; j < SizeW; j++)
		{
			float Y = 0.21*ImageData[i][j][0] + 0.72*ImageData[i][j][1] +0.07*ImageData[i][j][2];
			GrayImageData0[i][j] = (int) Y;
		}
	}


	//compute Ex
	int windowsize = 11;
	float sigma = 1.5;
	float k = sqrt(1.6);
	float t = 0.98, e = 0.5, f = 4;
	unsigned char ExImageData[newSizeH][newSizeW];

	XDoG(windowsize, sigma, k, t, e, f, &GrayImageData[0][0], &ExImageData[0][0], SizeH, SizeW);
	writeimage(Exfilename, &ExImageData[0][0], newSizeH, newSizeW, 1);

	// Integration of Portrait Background and Contour
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			newImageData[i][j][0] = (ExImageData[i][j]/255) * BGImageData[i][j][0];
			newImageData[i][j][1] = (ExImageData[i][j]/255) * BGImageData[i][j][1];
			newImageData[i][j][2] = (ExImageData[i][j]/255) * BGImageData[i][j][2];
		}
	}

	writeimage(newfilename, &newImageData[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	// DoG
	unsigned char DoGExImageData[newSizeH][newSizeW];
	unsigned char DoGImageData[newSizeH][newSizeW][newBytesPerPixel];
	DoG(windowsize, sigma, k, e, f, &GrayImageData[0][0], &DoGExImageData[0][0], SizeH, SizeW);
	writeimage("1b Scarlett_DoG_ex.raw", &DoGExImageData[0][0], newSizeH, newSizeW, 1);
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			DoGImageData[i][j][0] = (DoGExImageData[i][j]/255) * BGImageData[i][j][0];
			DoGImageData[i][j][1] = (DoGExImageData[i][j]/255) * BGImageData[i][j][1];
			DoGImageData[i][j][2] = (DoGExImageData[i][j]/255) * BGImageData[i][j][2];
		}
	}
	writeimage("1b Scarlett_DoG_ans.raw", &DoGImageData[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	return 0;
}


int main(void)
{
	p1a();
	p1b();

	return 0;
}
