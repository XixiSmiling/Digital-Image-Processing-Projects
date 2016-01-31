#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "iofile.h"
#include "histogram.h"
#include "FPProcessing.h"
#include "p2ops.h"

using namespace std;

int p2a(void)
{
	// initialize data of input
	const char *filename = "OCR_training.raw";
	int SizeW = 600;
	int SizeH = 300;
	int BytesPerPixel = 1;
	unsigned char ImageData[SizeH][SizeW][BytesPerPixel];

	// read input
	readimage(filename, &ImageData[0][0][0], SizeH, SizeW, BytesPerPixel);

	unsigned char binaryImage[SizeH][SizeW];
	for(int i=0; (i < SizeH); i++)
	{
		for(int j=0; (j < SizeW); j++)
		{
			int x = ImageData[i][j][0];
			if (x < 50)
			{
				binaryImage[i][j] = 255;
			}
			else
			{
				binaryImage[i][j] = 0;
			}
		}
	}
	unsigned char thinbinary[SizeH][SizeW];
	MorphFilter(1, SizeH, SizeW, &binaryImage[0][0], &thinbinary[0][0]);
	writeimage("2a ThinnedBiImage.raw", &thinbinary[0][0], SizeH, SizeW, 1);

	Segment(SizeH, SizeW, &thinbinary[0][0]);

	return 0;
}


int main(void)
{
	p2a();

	return 0;
}
