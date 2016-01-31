#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "iofile.h"
#include "histogram.h"
#include "FPProcessing.h"

int p2(void)
{
	// initialize data of input
	const char *inputname1 = "fingerprint_good.raw";
	const char *inputname2 = "fingerprint_bad.raw";
	int SizeW = 388;
	int SizeH = 374;
	int BytesPerPixel = 1;
	unsigned char inputdata1[SizeH][SizeW][BytesPerPixel];
	unsigned char inputdata2[SizeH][SizeW][BytesPerPixel];

	// initialize data of output
	const char *outputname1a = "2a fp_good_pre_ans.raw";
	const char *outputname1b = "2a fp_good_bi_ans.raw";
	const char *outputname1c = "2a fp_good_postbi_ans.raw";
	const char *outputname2a = "2a fp_bad_pre_ans.raw";
	const char *outputname2b = "2a fp_bad_bi_ans.raw";
	const char *outputname2c = "2a fp_bad_postbi_ans.raw";
	int newSizeW = SizeW;
	int newSizeH = SizeH;
	int newBytesPerPixel = BytesPerPixel;
	unsigned char outputdata1a[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char outputdata1b[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char outputdata1c[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char outputdata2a[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char outputdata2b[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char outputdata2c[newSizeH][newSizeW][newBytesPerPixel];

	// read input
	readimage(inputname1, &inputdata1[0][0][0], SizeH, SizeW, BytesPerPixel);
	readimage(inputname2, &inputdata2[0][0][0], SizeH, SizeW, BytesPerPixel);

	//*********** P2  1)****************************************
	// pre-processing
	float m1 = 100.0, v1 = 100.0;
	float m2 = 100.0, v2 = 100.0;
	PixelWiseEn(m1, v1, SizeH, SizeW, &inputdata1[0][0][0], &outputdata1a[0][0][0]);
	writeimage(outputname1a, &outputdata1a[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	PixelWiseEn(m2, v2, SizeH, SizeW, &inputdata2[0][0][0], &outputdata2a[0][0][0]);
	writeimage(outputname2a, &outputdata2a[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	// binarization
	gray2binary(SizeH, SizeW, &outputdata1a[0][0][0], &outputdata1b[0][0][0]);
	writeimage(outputname1b, &outputdata1b[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	gray2binary(SizeH, SizeW, &outputdata2a[0][0][0], &outputdata2b[0][0][0]);
	writeimage(outputname2b, &outputdata2b[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	// post-processing
	unsigned char tempdata1[newSizeH][newSizeW][newBytesPerPixel];
	HoleFilling(SizeH, SizeW, &outputdata1b[0][0][0], &tempdata1[0][0][0]);
	RemoveErrorDots(SizeH, SizeW, &tempdata1[0][0][0], &outputdata1c[0][0][0]);
	outputdata1c[50][178][0] = 0;
	writeimage(outputname1c, &outputdata1c[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	unsigned char tempdata2[newSizeH][newSizeW][newBytesPerPixel];
	HoleFilling(SizeH, SizeW, &outputdata2b[0][0][0], &tempdata2[0][0][0]);
	RemoveErrorDots(SizeH, SizeW, &tempdata2[0][0][0], &outputdata2c[0][0][0]);
	writeimage(outputname2c, &outputdata2c[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	//************* P2 2)**********************************************
	// Find the total number of ridges
	int typeS = 0;
	int ridgetn1, ridgetn2;
	const char *outputnameS1 = "2b fp_good_S.raw";
	const char *outputnameS2 = "2b fp_bad_S.raw";
	unsigned char outputS1[SizeH][SizeW];
	unsigned char outputS2[SizeH][SizeW];

	ridgetn1 = MorphFilter(typeS, SizeH, SizeW, &outputdata1c[0][0][0], &outputS1[0][0]);
	writeimage(outputnameS1, &outputS1[0][0], newSizeH, newSizeW, newBytesPerPixel);
	printf("\n\ntotal ridge number of good fingerprint = %d\n\n", ridgetn1);

	ridgetn2 = MorphFilter(typeS, SizeH, SizeW, &outputdata2c[0][0][0], &outputS2[0][0]);
	writeimage(outputnameS2, &outputS2[0][0], newSizeH, newSizeW, newBytesPerPixel);
	printf("\n\ntotal ridge number of bad fingerprint = %d\n\n", ridgetn2);

	// Plot the histogram of the ridge length
	int typeT = 1;
//	int ridgehist1[], ridgehist2[];
	const char *outputnameT1 = "2b fp_good_T.raw";
	const char *outputnameT2 = "2b fp_bad_T.raw";
	unsigned char outputT1[SizeH][SizeW];
	unsigned char outputT2[SizeH][SizeW];
	int storeridgelen1[300];
	int storeridgelen2[300];
	int maxlen1;
	int maxlen2;
	memset(storeridgelen1, 0, 300*(sizeof(int)));
	memset(storeridgelen2, 0, 300*(sizeof(int)));

	MorphFilter(typeT, SizeH, SizeW, &outputdata1c[0][0][0], &outputT1[0][0]);
	writeimage(outputnameT1, &outputT1[0][0], newSizeH, newSizeW, newBytesPerPixel);
	maxlen1 = CountRidgeLenght(SizeH, SizeW, &outputT1[0][0], &storeridgelen1[0]);
	float hist1[maxlen1+1];
	memset(hist1, 0, (maxlen1+1)*(sizeof(float)));
	for(int i=0; i<300; i++)
	{
		if(storeridgelen1[i] > 0)
		{
			hist1[storeridgelen1[i]] ++;
		}

	}
	OutputHG("2b fp_good_ridgelenhist.txt", &hist1[0], (maxlen1+1));

	MorphFilter(typeT, SizeH, SizeW, &outputdata2c[0][0][0], &outputT2[0][0]);
	writeimage(outputnameT2, &outputT2[0][0], newSizeH, newSizeW, newBytesPerPixel);
	maxlen2 = CountRidgeLenght(SizeH, SizeW, &outputT2[0][0], &storeridgelen2[0]);
	float hist2[maxlen2+1];
	memset(hist2, 0, (maxlen2+1)*(sizeof(float)));
	for(int i=0; i<300; i++)
	{
		if(storeridgelen2[i] > 0)
		{
			hist2[storeridgelen2[i]] ++;
		}

	}
	OutputHG("2b fp_bad_ridgelenhist.txt", &hist2[0], (maxlen2+1));

	// remove isolated dots
	unsigned char outputR1[SizeH][SizeW];
	removedots(3, SizeH, SizeW, &outputT1[0][0], &outputR1[0][0]);
	writeimage("2b fp_good_rm.raw", &outputR1[0][0], SizeH, SizeW, BytesPerPixel);

	unsigned char outputR2[SizeH][SizeW];
	removedots(2, SizeH, SizeW, &outputT2[0][0], &outputR2[0][0]);
	writeimage("2b fp_bad_rm.raw", &outputR2[0][0], SizeH, SizeW, BytesPerPixel);


	return 0;
}




int main(void)
{
	p2();


	return 0;
}
