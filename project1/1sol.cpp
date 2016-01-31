#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "iofile.h"
#include "histogram.h"


int P1a(void)
{
	// initialize data of "Pepper.raw"
	const char *filename = "Pepper.raw";
	int SizeW = 512;
	int SizeH = 512;
	int BytesPerPixel = 3;
	unsigned char ImageData[SizeH][SizeW][BytesPerPixel];

	// initialize data of Gray-scale Pepper
	const char *newfilename = "1a Gray-scale Pepper_ans.raw";
	int newSizeW = 512;
	int newSizeH = 512;
	int newBytesPerPixel = 1;
	unsigned char newImageData[newSizeH][newSizeW][newBytesPerPixel];

	// read "Pepper.raw"
	readimage(filename, &ImageData[0][0][0], SizeH, SizeW, BytesPerPixel);

	// color-to-grayscale conversion
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			int R = ImageData[i][j][0];
			int G = ImageData[i][j][1];
			int B = ImageData[i][j][2];
			float Y = 0.21*R + 0.72*G +0.07*B;
			newImageData[i][j][0] = (int)Y;
		}
	}

	// output Gray-scale Pepper.raw
	writeimage(newfilename, &newImageData[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	// histograms of images for report
	int hgR[256], hgG[256], hgB[256];
	int hgY[256];
	memset(hgR, 0, 256*sizeof(int));
	memset(hgG, 0, 256*sizeof(int));
	memset(hgB, 0, 256*sizeof(int));
	memset(hgY, 0, 256*sizeof(int));

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			hgR[ImageData[i][j][0]] ++;
			hgG[ImageData[i][j][1]] ++;
			hgB[ImageData[i][j][2]] ++;
			hgY[newImageData[i][j][0]] ++;
		}
	}
	OutputHG("P1 Pepper_R_hist.dat", hgR, 256);
	OutputHG("P1 Pepper_G_hist.dat", hgG, 256);
	OutputHG("P1 Pepper_B_hist.dat", hgB, 256);
	OutputHG("P1 Pepper_Gray_Y_hist.dat", hgY, 256);

	return 0;
}

int P1b(void)
{
	// initialize data of "mandril.raw"
	const char *filename = "mandril.raw";
	int SizeW = 512;
	int SizeH = 512;
	int BytesPerPixel = 3;
	unsigned char ImageData[SizeH][SizeW][BytesPerPixel];
//	memset(ImageData, 0, SizeH*SizeW*BytesPerPixel*sizeof(int));

	// initialize data of "mandril_scale.raw"
	const char *newfilename = "1b mandril_scale_ans.raw";
	int newSizeW = 700;
	int newSizeH = 700;
	int newBytesPerPixel = 3;
	float scaleR, scaleG, scaleB;
	unsigned char newImageData[newSizeH][newSizeW][newBytesPerPixel];
//	memset(newImageData, 0, newSizeH*newSizeW*newBytesPerPixel*sizeof(int));

	// read "mandril.raw"
	readimage(filename, &ImageData[0][0][0], SizeH, SizeW, BytesPerPixel);

	// resize image
	int neww, newh;
	float midw, midh;
	float a, b;
	int midw0, midh0, midw1, midh1, midw2, midh2, midw3, midh3;
	int R0, R1, R2, R3, G0, G1, G2, G3, B0, B1, B2, B3;

	for(newh=0; newh < 700; newh++)
	{
		for(neww=0; neww < newSizeW; neww++)
		{

			midw = 511.0*(neww-1)/699.0 + 1;
			midh = 511.0*(newh-1)/699.0 + 1;
			a = midw - ((int)midw);
			b = midh - ((int)midh);

			midw0 = midw;
			midh0 = midh;
			midw1 = midw0 + 1;
			midh1 = midh0;
			midw2 = midw0;
			midh2 = midh0 + 1;
			midw3 = midw0 + 1;
			midh3 = midh0 + 1;

			if((midw < 511) && (midh < 511))
			{
				R0 = ImageData[midh0][midw0][0];
				G0 = ImageData[midh0][midw0][1];
				B0 = ImageData[midh0][midw0][2];
				R1 = ImageData[midh1][midw1][0];
				G1 = ImageData[midh1][midw1][1];
				B1 = ImageData[midh1][midw1][2];
				R2 = ImageData[midh2][midw2][0];
				G2 = ImageData[midh2][midw2][1];
				B2 = ImageData[midh2][midw2][2];
				R3 = ImageData[midh3][midw3][0];
				G3 = ImageData[midh3][midw3][1];
				B3 = ImageData[midh3][midw3][2];

				scaleR = (1.0-a)*(1-b)*R0 + a*(1-b)*R1 + b*(1-a)*R2 + a*b*R3;
				scaleG = (1.0-a)*(1-b)*G0 + a*(1-b)*G1 + b*(1-a)*G2 + a*b*G3;
				scaleB = (1.0-a)*(1-b)*B0 + a*(1-b)*B1 + b*(1-a)*B2 + a*b*B3;

				newImageData[newh][neww][0] = (int)scaleR;
				newImageData[newh][neww][1] = (int)scaleG;
				newImageData[newh][neww][2] = (int)scaleB;
			}

			// edge of picture
			if((midw >= 511) && (midh < 511))
			{
				R0 = ImageData[midh0][midw0][0];
				G0 = ImageData[midh0][midw0][1];
				B0 = ImageData[midh0][midw0][2];
				R2 = ImageData[midh2][midw2][0];
				G2 = ImageData[midh2][midw2][1];
				B2 = ImageData[midh2][midw2][2];

				scaleR = b*R2 + (1.0-b)*R0;
				scaleG = b*G2 + (1.0-b)*G0;
				scaleB = b*B2 + (1.0-b)*B0;
			}

			// edge of picture
			if((midh >= 511) && (midw < 511))
			{
				R0 = ImageData[midh0][midw0][0];
				G0 = ImageData[midh0][midw0][1];
				B0 = ImageData[midh0][midw0][2];
				R1 = ImageData[midh1][midw1][0];
				G1 = ImageData[midh1][midw1][1];
				B1 = ImageData[midh1][midw1][2];

				scaleR = a*R1 + (1.0-a)*R0;
				scaleG = a*G1 + (1.0-a)*G0;
				scaleB = a*B1 + (1.0-a)*B0;
			}

			// edge of picture
			if((midw >= 511) && (midh >= 511))
			{
				R0 = ImageData[midh0][midw0][0];
				G0 = ImageData[midh0][midw0][1];
				B0 = ImageData[midh0][midw0][2];

				scaleR = R0;
				scaleG = G0;
				scaleB = B0;
			}

			if((midw >= 512) || (midh >= 512))
			{
				printf("there must be something wrong :(");
				return 1;
			}

			newImageData[newh][neww][0] = (int)scaleR;
			newImageData[newh][neww][1] = (int)scaleG;
			newImageData[newh][neww][2] = (int)scaleB;

//			printf("%d, %d, R = %d  ", newh, neww, (int)scaleR);
//			printf("G = %d  ", (int)scaleG);
//			printf("B = %d\n", (int)scaleB);

		}

	}

	// output "mandril_scale.raw"
	writeimage(newfilename, &newImageData[0][0][0], newSizeH, newSizeW, newBytesPerPixel);

	// histograms of images for report
	int hgR[256], hgG[256], hgB[256];
	int newhgR[256], newhgG[256], newhgB[256];
	memset(hgR, 0, 256*sizeof(int));
	memset(hgG, 0, 256*sizeof(int));
	memset(hgB, 0, 256*sizeof(int));
	memset(newhgR, 0, 256*sizeof(int));
	memset(newhgG, 0, 256*sizeof(int));
	memset(newhgB, 0, 256*sizeof(int));


	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			hgR[ImageData[i][j][0]] ++;
			hgG[ImageData[i][j][1]] ++;
			hgB[ImageData[i][j][2]] ++;
		}
	}

	for(int i=0; i<newSizeH; i++)
	{
		for(int j=0; j<newSizeW; j++)
		{
			newhgR[newImageData[i][j][0]] ++;
			newhgG[newImageData[i][j][1]] ++;
			newhgB[newImageData[i][j][2]] ++;
		}
	}

	OutputHG("P1 mandril_R_hist.dat", hgR, 256);
	OutputHG("P1 mandril_G_hist.dat", hgG, 256);
	OutputHG("P1 mandril_B_hist.dat", hgB, 256);
	OutputHG("P1 mandril-scale_R_hist.dat", newhgR, 256);
	OutputHG("P1 mandril-scale_G_hist.dat", newhgG, 256);
	OutputHG("P1 mandril-scale_B_hist.dat", newhgB, 256);

	return 0;
}

int main(void)
{
	P1a();
	P1b();

	return 0;
}
