#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iofile.h"


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

//int readlabimage(const char *filename, double *ImageData, int SizeH, int SizeW, int BytesPerPixel)
//{
//	FILE *file;
//	file = fopen(filename, "rb");
//
//	if(file == NULL) {
//		printf("Cannot open file: %s\n", filename);
//		return 1;
//	}
//
//	fread(ImageData, sizeof(double), SizeH*SizeW*BytesPerPixel, file);
//	fclose(file);
//
//	return 0;
//}


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

//int writelabimage(const char *newfilename, double *newImageData, int newSizeH, int newSizeW, int newBytesPerPixel)
//{
//	FILE *newfile;
//	newfile=fopen(newfilename,"wb");
//
//	if(newfile == NULL){
//		printf("cannot open file: %s\n", newfilename);
//		return 1;
//	}
//
//	fwrite(newImageData, sizeof(double), newSizeH*newSizeW*newBytesPerPixel, newfile);
//    fclose(newfile);
//
//	return 0;
//}

int copyimage2f(int SizeH, int SizeW, const unsigned char *inputdata, float *copydata)
{
	for (int i=0; i<SizeH; i++)
	{
		for (int j=0; j<SizeW; j++)
		{
			copydata[i*SizeW+j] = inputdata[i*SizeW+j];
		}
	}

	return 0;
}

int copyimaged2d(int SizeH, int SizeW, double *inputdata, double *copydata)
{
	for (int i=0; i<SizeH; i++)
	{
		for (int j=0; j<SizeW; j++)
		{
			copydata[i*SizeW+j] = inputdata[i*SizeW+j];
		}
	}

	return 0;
}

// extend lines won't include the boundary lines.
int extendonechannel(const unsigned char *ImageData, int SizeH, int SizeW, unsigned char *extendImageData, int extend)
{
//	int extendSizeH = SizeH+2*extend;
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

// extend lines will include the boundary lines.
int extendonechannel2(const unsigned char *ImageData, int SizeH, int SizeW, unsigned char *extendImageData, int extend)
{
//	int extendSizeH = SizeH+2*extend;
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
		for(int b = extend-1; b >= 0; b--)
		{
			int i = a + extend;
			int j = (extend-1) - b;

			extendImageData[i*extendSizeW+j] = ImageData[a*SizeW+b];
		}
	}
	for(int a = 0; a < SizeH; a++)
	{
		for(int b = (SizeW-extend); (b <= (SizeW-1)); b++)
		{
			int i = a + extend;
			int j = 2*SizeW - 1 + extend - b;

			extendImageData[i*extendSizeW+j] = ImageData[a*SizeW+b];
		}
	}

	// top and bottom
	for(int c=extend; (c < (2*extend - 1)); c++)
	{
		for(int d=0; d < extendSizeW; d++)
		{
			int i = 2*extend - 1 - c;
			int j = d;

			extendImageData[i*extendSizeW+j] = extendImageData[c*extendSizeW+d];
		}
	}
	for(int c=SizeH+extend-1; (c >= SizeH); c--)
	{
		for(int d=0; d < extendSizeW; d++)
		{
			int i = 2*SizeH - 1 + 2*extend - c;
			int j = d;

			extendImageData[i*extendSizeW+j] = extendImageData[c*extendSizeW+d];
		}
	}

	return 0;
}

int ExtendColorI(const unsigned char *ImageData, int SizeH, int SizeW, unsigned char *extendImageData, int extend)
{
//	int extendSizeH = SizeH+2*extend;
	int extendSizeW = SizeW+2*extend;

	// center
	for(int a=0; a<SizeH; a++)
	{
		for(int b=0; b<SizeW; b++)
		{
			int i = a + extend;
			int j = b + extend;

			extendImageData[i*extendSizeW*3+j*3+0] = ImageData[a*SizeW*3+b*3+0];
			extendImageData[i*extendSizeW*3+j*3+1] = ImageData[a*SizeW*3+b*3+1];
			extendImageData[i*extendSizeW*3+j*3+2] = ImageData[a*SizeW*3+b*3+2];
		}
	}

	// right and left
	for(int a = 0; a<SizeH; a++)
	{
		for(int b = extend-1; b >= 0; b--)
		{
			int i = a + extend;
			int j = (extend-1) - b;

			extendImageData[i*extendSizeW*3+j*3+0] = ImageData[a*SizeW*3+b*3+0];
			extendImageData[i*extendSizeW*3+j*3+1] = ImageData[a*SizeW*3+b*3+1];
			extendImageData[i*extendSizeW*3+j*3+2] = ImageData[a*SizeW*3+b*3+2];
		}
	}
	for(int a = 0; a < SizeH; a++)
	{
		for(int b = (SizeW-extend); (b <= (SizeW-1)); b++)
		{
			int i = a + extend;
			int j = 2*SizeW - 1 + extend - b;

			extendImageData[i*extendSizeW*3+j*3+0] = ImageData[a*SizeW*3+b*3+0];
			extendImageData[i*extendSizeW*3+j*3+1] = ImageData[a*SizeW*3+b*3+1];
			extendImageData[i*extendSizeW*3+j*3+2] = ImageData[a*SizeW*3+b*3+2];
		}
	}

	// top and bottom
	for(int c=extend; (c < (2*extend - 1)); c++)
	{
		for(int d=0; d < extendSizeW; d++)
		{
			int i = 2*extend - 1 - c;
			int j = d;

			extendImageData[i*extendSizeW*3+j*3+0] = extendImageData[c*extendSizeW*3+d*3+0];
			extendImageData[i*extendSizeW*3+j*3+1] = extendImageData[c*extendSizeW*3+d*3+1];
			extendImageData[i*extendSizeW*3+j*3+2] = extendImageData[c*extendSizeW*3+d*3+2];
		}
	}
	for(int c=SizeH+extend-1; (c >= SizeH); c--)
	{
		for(int d=0; d < extendSizeW; d++)
		{
			int i = 2*SizeH - 1 + 2*extend - c;
			int j = d;

			extendImageData[i*extendSizeW*3+j*3+0] = extendImageData[c*extendSizeW*3+d*3+0];
			extendImageData[i*extendSizeW*3+j*3+1] = extendImageData[c*extendSizeW*3+d*3+1];
			extendImageData[i*extendSizeW*3+j*3+2] = extendImageData[c*extendSizeW*3+d*3+2];

		}
	}

	return 0;
}


int ExtendMap(int *ImageData, int SizeH, int SizeW, int *extendImageData, int extend)
{
//	int extendSizeH = SizeH+2*extend;
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

int extendlab(double *ImageData, int SizeH, int SizeW, double *extendImageData, int extend)
{
//	int extendSizeH = SizeH+2*extend;
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

void OutputData(const char *filename, float *Data, int Size)
{
	FILE *hist;
	hist = fopen(filename,"wb");

    for(int i=0; i<Size; i++)
    {
        fprintf(hist,"%f	", Data[i]);
    }
    fclose(hist);

	return;
}

void OutputData2D(const char *filename, float *Data, int SizeH, int SizeW)
{
	FILE *hist;
	hist = fopen(filename,"wb");

    for(int i=0; i<SizeH; i++)
    {
    	for (int j=0; j<SizeW; j++)
    	{
    		fprintf(hist,"%d	%d	%f\n", i,j,Data[i*SizeW+j]);
    	}
    }
    fclose(hist);

	return;
}

int DetMBVQ(unsigned char R, unsigned char G, unsigned char B)
{
	const int CMYW = 1, MYGC = 2, RGMY = 3, KRGB = 4, RGBM = 5, CMGB = 6;

	if((R+G) > 255)
	{
		if((G+B) > 255)
		{
			if((R+G+B) > 510)
			{
				return CMYW;
			}
			else
			{
				return MYGC;
			}
		}
		else
		{
			return RGMY;
		}
	}
	else
	{
		if(!((G+B) > 255))
		{
			if(!((R+G+B) > 255))
			{
				return KRGB;
			}
			else
			{
				return RGBM;
			}
		}
		else
		{
			return CMGB;
		}
	}


}


