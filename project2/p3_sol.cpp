#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "iofile.h"
#include "histogram.h"
#include "HalfToning.h"

int p3a(void)
{
	// initialize data of input
	const char *inputname = "boat.raw";
	int SizeW = 512;
	int SizeH = 512;
	int BytesPerPixel = 1;
	unsigned char inputdata[SizeH][SizeW][BytesPerPixel];

	// initialize data of output
	const char *outputname1 = "3a boat_FT_ans.raw";
	const char *outputname2 = "3a boat_RT_ans.raw";
	const char *outputname3a = "3a boat_DM2_ans.raw";
	const char *outputname3b = "3a boat_DM4_ans.raw";
	int newSizeW = SizeW;
	int newSizeH = SizeH;
	int newBytesPerPixel = BytesPerPixel;
	unsigned char outputdata1[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char outputdata2[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char outputdata3a[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char outputdata3b[newSizeH][newSizeW][newBytesPerPixel];

	// read input
	readimage(inputname, &inputdata[0][0][0], SizeH, SizeW, BytesPerPixel);

	//*** Fixed thresholding ***
	float FixedT = 128.0;
	Fixedthre(FixedT, SizeH, SizeW, &inputdata[0][0][0], &outputdata1[0][0][0]);
	writeimage(outputname1, &outputdata1[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	//*** Random thresholding ***
	RandomThre(SizeH, SizeW, &inputdata[0][0][0], &outputdata2[0][0][0]);
	writeimage(outputname2, &outputdata2[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	//*** Dithering Matrix
	Dithering(2, SizeH, SizeW, &inputdata[0][0][0], &outputdata3a[0][0][0]);
	Dithering(4, SizeH, SizeW, &inputdata[0][0][0], &outputdata3b[0][0][0]);
	writeimage(outputname3a, &outputdata3a[0][0][0], newSizeH, newSizeW, newBytesPerPixel);
	writeimage(outputname3b, &outputdata3b[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	return 0;
}


int p3b(void)
{
	// initialize data of input
	const char *inputname = "boat.raw";
	int SizeW = 512;
	int SizeH = 512;
	int BytesPerPixel = 1;
	unsigned char inputdata[SizeH][SizeW][BytesPerPixel];

	// initialize data of output
	const char *outputname = "3b boat_ED_ans.raw";
	int newSizeW = SizeW;
	int newSizeH = SizeH;
	int newBytesPerPixel = BytesPerPixel;
	unsigned char outputdata[newSizeH][newSizeW][newBytesPerPixel];

	// read input
	readimage(inputname, &inputdata[0][0][0], SizeH, SizeW, BytesPerPixel);

	// Error Diffusion
	int color = 255;
	FSErrorDiff(color, SizeH, SizeW, &inputdata[0][0][0], &outputdata[0][0][0]);
	writeimage(outputname, &outputdata[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	return 0;
}


int p3c(void)
{
	// initialize data of input
	const char *inputname = "trees.raw";
	int SizeW = 350;
	int SizeH = 258;
	int BytesPerPixel = 3;
	unsigned char inputdata[SizeH][SizeW][BytesPerPixel];

	// initialize data of output
	const char *outputname1 = "3c tree_SED_ans.raw";
	const char *outputname2 = "3c tree_MBVQ_ans.raw";
	int newSizeW = SizeW;
	int newSizeH = SizeH;
	int newBytesPerPixel = BytesPerPixel;
	unsigned char outputdata1[newSizeH][newSizeW][newBytesPerPixel];

	// read input
	readimage(inputname, &inputdata[0][0][0], SizeH, SizeW, BytesPerPixel);


	double inputC[SizeH][SizeW];
	double inputM[SizeH][SizeW];
	double inputY[SizeH][SizeW];

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			inputC[i][j] = 1 - (inputdata[i][j][0] / 255.0);
			inputM[i][j] = 1 - (inputdata[i][j][1] / 255.0);
			inputY[i][j] = 1 - (inputdata[i][j][2] / 255.0);
//			printf("%d  %f  %f  %f\n", i, inputC[i][j], inputM[i][j], inputY[i][j]);
		}
	}

	// Separable Error Diffusion
	double sedC[SizeH][SizeW];
	double sedM[SizeH][SizeW];
	double sedY[SizeH][SizeW];
	FSErrorDiffFord(SizeH, SizeW, &inputC[0][0], &sedC[0][0]);
	FSErrorDiffFord(SizeH, SizeW, &inputM[0][0], &sedM[0][0]);
	FSErrorDiffFord(SizeH, SizeW, &inputY[0][0], &sedY[0][0]);

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			outputdata1[i][j][0] = 255*(1 - sedC[i][j]);
			outputdata1[i][j][1] = 255*(1 - sedM[i][j]);
			outputdata1[i][j][2] = 255*(1 - sedY[i][j]);

//			printf("%d  %f  %f  %f\n", i, sedC[i][j], sedM[i][j], sedY[i][j]);
		}
	}
	writeimage(outputname1, &outputdata1[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	// MBVQ-based Error diffusion
	MBVQErrorDiff(inputname, outputname2, SizeH, SizeW, BytesPerPixel);

	return 0;
}

int main(void)
{
	p3a();
	p3b();
	p3c();
	return 0;
}
