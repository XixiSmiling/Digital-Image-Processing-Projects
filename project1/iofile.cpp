#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iofile.h"
#include "histogram.h"

int readimage(const char *filename, unsigned char *ImageData, int SizeH, int SizeW, int BytesPerPixel)
{
	FILE *file;
	file = fopen(filename, "rb");

	if(file == NULL) {
		printf("Cannot open file: %s\n", filename);
		return 1;
	}

	fread(ImageData, sizeof(unsigned char), SizeH*SizeW*BytesPerPixel, file);
	fclose(file);

	return 0;
}


int writeimage(const char *newfilename, unsigned char *newImageData, int newSizeH, int newSizeW, int newBytesPerPixel)
{
	FILE *newfile;
	newfile=fopen(newfilename,"wb");

	if(newfile == NULL){
		printf("cannot open file: %s\n", newfilename);
		return 1;
	}

	fwrite(newImageData, sizeof(unsigned char), newSizeH*newSizeW*newBytesPerPixel, newfile);
    fclose(newfile);

	return 0;
}


int extendonechannel(const unsigned char *ImageData, int SizeH, int SizeW, unsigned char *extendImageData, int extend)
{
	int newSizeH = SizeH + 2*extend;
	int newSizeW = SizeW + 2*extend;

	int extendSizeH = SizeH+2*extend;
	int extendSizeW = SizeW+2*extend;

	// center
	for(int a=0; a<SizeH; a++)
	{
		for(int b=0; b<SizeW; b++)
		{
			int i = a + extend;
			int j = b + extend;

			extendImageData[i*extendSizeW+j] = ImageData[a*SizeW+b];
		}
	}

	// right and left
	for(int a = 0; a<SizeH; a++)
	{
		for(int b = extend; b > 0; b--)
		{
			int i = a + extend;
			int j = extend - b;

			extendImageData[i*extendSizeW+j] = ImageData[a*SizeW+b];
		}
	}
	for(int a = 0; a < SizeH; a++)
	{
		for(int b = (SizeW-1-extend); (b < (SizeW-1)); b++)
		{
			int i = a + extend;
			int j = 2*SizeW - 2 + extend - b;

			extendImageData[i*extendSizeW+j] = ImageData[a*SizeW+b];
		}
	}

	// top and bottom
	for(int c=extend; (c < (2*extend + 1)); c++)
	{
		for(int d=0; d < extendSizeW; d++)
		{
			int i = 2*extend - c;
			int j = d;

			extendImageData[i*extendSizeW+j] = extendImageData[c*extendSizeW+d];
		}
	}
	for(int c=SizeH-1; (c < (SizeH-1+extend)); c++)
	{
		for(int d=0; d < extendSizeW; d++)
		{
			int i = 2*SizeH - 2 + 2*extend - c;
			int j = d;

			extendImageData[i*extendSizeW+j] = extendImageData[c*extendSizeW+d];
		}
	}

	return 0;
}

