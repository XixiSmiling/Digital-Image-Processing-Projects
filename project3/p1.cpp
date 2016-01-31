#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "iofile.h"
#include "p1ops.h"

int p1a(void)
{
	// initialize data of input
	const char *filename1 = "Tiffany.raw";
	int SizeW1 = 512;
	int SizeH1 = 512;
	int BytesPerPixel1 = 3;
	unsigned char ImageData1[SizeH1][SizeW1][BytesPerPixel1];
	const char *filename2 = "Components.raw";
	int SizeW2 = 350;
	int SizeH2 = 350;
	int BytesPerPixel2 = 3;
	unsigned char ImageData2[SizeH2][SizeW2][BytesPerPixel2];

	// read input
	readimage(filename1, &ImageData1[0][0][0], SizeH1, SizeW1, BytesPerPixel1);
	readimage(filename2, &ImageData2[0][0][0], SizeH2, SizeW2, BytesPerPixel2);

	// find the center coordinates and the dimension of holes
	FindHoleInfo(&ImageData1[0][0][0], SizeH1, SizeW1);

	// find the center coordinates, the orientation angle and the dimension of the components
	FindPiecesInfo(&ImageData2[0][0][0], SizeH2, SizeW2);

	float mholeH = (60+4), mholeW = (110+4);
	float mholeI = 338, mholeJ = 320;
	float mouthH = 65.069191, mouthW = 127.279221;
	float mouthI = 107.620117, mouthJ = 106.122902;
	float mangle = 0 - 0.785398;
	float eholeH = (35+4), eholeW = (80+4);
	float eholeI = 211.5, eholeJ = 258;
	float eyeH = 52, eyeW = 112.946892;
	float eyeI = 251.533356, eyeJ = 226.533356;
	float eangle = 0.352553 - 3.14159;

	// rotation
	unsigned char rmImage[SizeH2][SizeW2][BytesPerPixel2];
	memset(rmImage, 255, SizeH2*SizeW2*3*(sizeof(unsigned char)));
	rotation(&ImageData2[0][0][0], &rmImage[0][0][0], SizeH2, SizeW2, mouthH, mouthW, mouthI, mouthJ, mangle);
	writeimage("1a rotatedmouth.raw", &rmImage[0][0][0], SizeH2, SizeW2, 3);
	unsigned char reImage[SizeH2][SizeW2][BytesPerPixel2];
	memset(reImage, 255, SizeH2*SizeW2*3*(sizeof(unsigned char)));
	rotation(&ImageData2[0][0][0], &reImage[0][0][0], SizeH2, SizeW2, eyeH, eyeW, eyeI, eyeJ, eangle);
	writeimage("1a rotatedeye.raw", &reImage[0][0][0], SizeH2, SizeW2, 3);

	// scaling
	unsigned char smImage[SizeH2][SizeW2][BytesPerPixel2];
	memset(smImage, 255, SizeH2*SizeW2*3*(sizeof(unsigned char)));
	scaling(&rmImage[0][0][0], &smImage[0][0][0], SizeH2, SizeW2, mouthH, mouthW, mholeH, mholeW, mouthI, mouthJ);
	writeimage("1a scaledmouth.raw", &smImage[0][0][0], SizeH2, SizeW2, 3);
	unsigned char seImage[SizeH2][SizeW2][BytesPerPixel2];
	memset(seImage, 255, SizeH2*SizeW2*3*(sizeof(unsigned char)));
	scaling(&reImage[0][0][0], &seImage[0][0][0], SizeH2, SizeW2, eyeH, eyeW, eholeH, eholeW, eyeI, eyeJ);
	writeimage("1a scaledeye.raw", &seImage[0][0][0], SizeH2, SizeW2, 3);

	// shift
	unsigned char shiftm[SizeH1][SizeW1][BytesPerPixel1];
	memset(shiftm, 255, SizeH1*SizeW1*3*(sizeof(unsigned char)));
	shift(&smImage[0][0][0], &shiftm[0][0][0], SizeW2, SizeW1, mholeH, mholeW, mouthI, mouthJ, mholeI, mholeJ);
	writeimage("1a shiftedmouth.raw", &shiftm[0][0][0], SizeH1, SizeW1, 3);
	unsigned char shifte[SizeH1][SizeW1][BytesPerPixel1];
	memset(shifte, 255, SizeH1*SizeW1*3*(sizeof(unsigned char)));
	shift(&seImage[0][0][0], &shifte[0][0][0], SizeW2, SizeW1, eholeH, eholeW, eyeI, eyeJ, eholeI, eholeJ);
	writeimage("1a shiftedeye.raw", &shifte[0][0][0], SizeH1, SizeW1, 3);

	// combine mouth & eye
	unsigned char MandE[SizeH1][SizeW1][BytesPerPixel1];
	memset(MandE, 255, SizeH1*SizeW1*3*(sizeof(unsigned char)));
	combination(&shifte[0][0][0], &shiftm[0][0][0], &MandE[0][0][0], SizeH1, SizeW1);
	writeimage("1a mouth and eye.raw", &MandE[0][0][0], SizeH1, SizeW1, 3);

	// final combination
	unsigned char fullT[SizeH1][SizeW1][BytesPerPixel1];
	memset(fullT, 255, SizeH1*SizeW1*3*(sizeof(unsigned char)));
	combination(&MandE[0][0][0], &ImageData1[0][0][0], &fullT[0][0][0], SizeH1, SizeW1);
	writeimage("1a fullTiffany.raw", &fullT[0][0][0], SizeH1, SizeW1, 3);

	return 0;
}

int p1b(void)
{
	// initialize data of input
	const char *filename = "jennifer.raw";
	int SizeW = 512;
	int SizeH = 512;
	int BytesPerPixel = 3;
	unsigned char ImageData[SizeH][SizeW][BytesPerPixel];

	// read input
	readimage(filename, &ImageData[0][0][0], SizeH, SizeW, BytesPerPixel);

//	int scaleW1 = 512 - (128*(576-512)/512);
//	int scaleH1 = 512;
//	unsigned char scaledImage1[scaleH1][scaleW1][3];
//	resizeimage(&ImageData[0][0][0], &scaledImage1[0][0][0], SizeH, SizeW, scaleH1, scaleW1);
//	int scaleH2 = 512 - (128*(576-512)/512);
//	int scaleW2 = 512;
//	unsigned char scaledImage2[scaleH2][scaleW2][3];
//	resizeimage(&ImageData[0][0][0], &scaledImage2[0][0][0], SizeH, SizeW, scaleH2, scaleW2);
//	writeimage("1b horzscaledJ.raw", &scaledImage1[0][0][0], scaleH1, scaleW1, 3);

	int horzH = 512, horzW = 576;
	unsigned char horzoutput[horzH][horzW][BytesPerPixel];
	memset(horzoutput, 0, horzH*horzW*3*(sizeof(unsigned char)));
	int vertH = 576, vertW = 512;
	unsigned char vertoutput[vertH][vertW][BytesPerPixel];
	memset(vertoutput, 0, vertH*vertW*3*(sizeof(unsigned char)));

	for(int i=0; i<128; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			int s = (576-512)*(i+1)/128.0;
			horzoutput[i][j+s][0] = ImageData[i][j][0];
			horzoutput[i][j+s][1] = ImageData[i][j][1];
			horzoutput[i][j+s][2] = ImageData[i][j][2];
		}
	}
	for(int i=128; i<256; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			int s = (576-512)*((255-i)+1)/128.0;
			horzoutput[i][j+s][0] = ImageData[i][j][0];
			horzoutput[i][j+s][1] = ImageData[i][j][1];
			horzoutput[i][j+s][2] = ImageData[i][j][2];
		}
	}
	for(int i=256; i<384; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			int s = (576-512)*((i-256)+1)/128.0;
			horzoutput[i][j+s][0] = ImageData[i][j][0];
			horzoutput[i][j+s][1] = ImageData[i][j][1];
			horzoutput[i][j+s][2] = ImageData[i][j][2];
		}
	}
	for(int i=384; i<512; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			int s = (576-512)*((511-i)+1)/128.0;
			horzoutput[i][j+s][0] = ImageData[i][j][0];
			horzoutput[i][j+s][1] = ImageData[i][j][1];
			horzoutput[i][j+s][2] = ImageData[i][j][2];
		}
	}
	writeimage("1b horizontal shear J.raw", &horzoutput[0][0][0], horzH, horzW, 3);

	for(int j=0; j<128; j++)
	{
		for(int i=0; i<SizeW; i++)
		{
			int s = (576-512)*(j+1)/128.0;
			vertoutput[i+s][j][0] = ImageData[i][j][0];
			vertoutput[i+s][j][1] = ImageData[i][j][1];
			vertoutput[i+s][j][2] = ImageData[i][j][2];
		}
	}
	for(int j=128; j<256; j++)
	{
		for(int i=0; i<SizeW; i++)
		{
			int s = (576-512)*((255-j)+1)/128.0;
			vertoutput[i+s][j][0] = ImageData[i][j][0];
			vertoutput[i+s][j][1] = ImageData[i][j][1];
			vertoutput[i+s][j][2] = ImageData[i][j][2];
		}
	}
	for(int j=256; j<384; j++)
	{
		for(int i=0; i<SizeW; i++)
		{
			int s = (576-512)*((j-256)+1)/128.0;
			vertoutput[i+s][j][0] = ImageData[i][j][0];
			vertoutput[i+s][j][1] = ImageData[i][j][1];
			vertoutput[i+s][j][2] = ImageData[i][j][2];
		}
	}
	for(int j=384; j<512; j++)
	{
		for(int i=0; i<SizeW; i++)
		{
			int s = (576-512)*((511-j)+1)/128.0;
			vertoutput[i+s][j][0] = ImageData[i][j][0];
			vertoutput[i+s][j][1] = ImageData[i][j][1];
			vertoutput[i+s][j][2] = ImageData[i][j][2];
		}
	}
	writeimage("1b vertical shear J.raw", &vertoutput[0][0][0], vertH, vertW, 3);


	return 0;
}

int p1c(void)
{
	// initialize data of input
	const char *filename = "fruits.raw";
	int SizeW = 512;
	int SizeH = 512;
	int BytesPerPixel = 3;
	unsigned char ImageData[SizeH][SizeW][BytesPerPixel];

	// read input
	readimage(filename, &ImageData[0][0][0], SizeH, SizeW, BytesPerPixel);

	unsigned char output[SizeH][SizeW][BytesPerPixel];
	memset(output, 0, SizeH*SizeW*3*(sizeof(unsigned char)));
	float oldx, oldy, midi, midj;
	float diffi, diffj;
	int midi0, midj0, midi1, midj1, midi2, midj2, midi3, midj3;
	int R0, G0, B0, R1, G1, B1, R2, G2, B2, R3, G3, B3;
	for(int i=0; i<256; i++)
	{
		for(int j=256; (j <= 511-i) && (j <= 255 + i/0.4142); j++)
		{
			float x = j - 255;
			float y = 256 -i;

			oldx = 1.4142*x;
			oldy = 0.4142*x + y;
			midj = oldx + 255;
			midi = 256 - oldy;
			diffi = midi - ((int)midi);
			diffj = midj - ((int)midj);

			midi0 = midi;
			midj0 = midj;
			midi1 = midi0 + 1;
			midj1 = midj0;
			midi2 = midi0;
			midj2 = midj0 + 1;
			midi3 = midi0 + 1;
			midj3 = midj0 + 1;

			R0 = ImageData[midi0][midj0][0];
			G0 = ImageData[midi0][midj0][1];
			B0 = ImageData[midi0][midj0][2];
			R1 = ImageData[midi1][midj1][0];
			G1 = ImageData[midi1][midj1][1];
			B1 = ImageData[midi1][midj1][2];
			R2 = ImageData[midi2][midj2][0];
			G2 = ImageData[midi2][midj2][1];
			B2 = ImageData[midi2][midj2][2];
			R3 = ImageData[midi3][midj3][0];
			G3 = ImageData[midi3][midj3][1];
			B3 = ImageData[midi3][midj3][2];

			output[i][j][0] = (int)((1.0-diffj)*(1-diffi)*R0 + diffj*(1-diffi)*R1 + diffi*(1-diffj)*R2 + diffj*diffi*R3);
			output[i][j][1] = (int)((1.0-diffj)*(1-diffi)*G0 + diffj*(1-diffi)*G1 + diffi*(1-diffj)*G2 + diffj*diffi*G3);
			output[i][j][2] = (int)((1.0-diffj)*(1-diffi)*B0 + diffj*(1-diffi)*B1 + diffi*(1-diffj)*B2 + diffj*diffi*B3);
		}
	}
	for(int j=256; j<512; j++)
	{
		for(int i=255; (i >= 2.4142*j - 977.6593) && (i >= 511-j); i--)
		{
			float x = j - 255;
			float y = 256 -i;

//			printf("i = %d , j = %d, x = %f, y = %f\n", i, j, x, y);
			oldx = x + 0.4142*y;
			oldy = 1.4142*y;
			midj = oldx + 255;
			midi = 256 - oldy;
			diffi = midi - ((int)midi);
			diffj = midj - ((int)midj);

			midi0 = midi;
			midj0 = midj;
			midi1 = midi0 + 1;
			midj1 = midj0;
			midi2 = midi0;
			midj2 = midj0 + 1;
			midi3 = midi0 + 1;
			midj3 = midj0 + 1;

			R0 = ImageData[midi0][midj0][0];
			G0 = ImageData[midi0][midj0][1];
			B0 = ImageData[midi0][midj0][2];
			R1 = ImageData[midi1][midj1][0];
			G1 = ImageData[midi1][midj1][1];
			B1 = ImageData[midi1][midj1][2];
			R2 = ImageData[midi2][midj2][0];
			G2 = ImageData[midi2][midj2][1];
			B2 = ImageData[midi2][midj2][2];
			R3 = ImageData[midi3][midj3][0];
			G3 = ImageData[midi3][midj3][1];
			B3 = ImageData[midi3][midj3][2];

			output[i][j][0] = (int)((1.0-diffj)*(1-diffi)*R0 + diffj*(1-diffi)*R1 + diffi*(1-diffj)*R2 + diffj*diffi*R3);
			output[i][j][1] = (int)((1.0-diffj)*(1-diffi)*G0 + diffj*(1-diffi)*G1 + diffi*(1-diffj)*G2 + diffj*diffi*G3);
			output[i][j][2] = (int)((1.0-diffj)*(1-diffi)*B0 + diffj*(1-diffi)*B1 + diffi*(1-diffj)*B2 + diffj*diffi*B3);

		}
	}
	for(int j=256; j<512; j++)
	{
		for(int i=255; (i <= j) && (i <= 1488.6593 - 2.4142*j); i++)
		{
			float x = j - 255;
			float y = 255 -i;
			oldx = x - 0.4142*y;
			oldy = 1.4142*y;
			midj = oldx + 255;
			midi = 255 - oldy;

//			printf("i = %d , j = %d, x = %f, y = %f, midi = %f, midj = %f\n", i, j, x, y, midi, midj);
			diffi = midi - ((int)midi);
			diffj = midj - ((int)midj);

			midi0 = midi;
			midj0 = midj;
			midi1 = midi0 + 1;
			midj1 = midj0;
			midi2 = midi0;
			midj2 = midj0 + 1;
			midi3 = midi0 + 1;
			midj3 = midj0 + 1;

			R0 = ImageData[midi0][midj0][0];
			G0 = ImageData[midi0][midj0][1];
			B0 = ImageData[midi0][midj0][2];
			R1 = ImageData[midi1][midj1][0];
			G1 = ImageData[midi1][midj1][1];
			B1 = ImageData[midi1][midj1][2];
			R2 = ImageData[midi2][midj2][0];
			G2 = ImageData[midi2][midj2][1];
			B2 = ImageData[midi2][midj2][2];
			R3 = ImageData[midi3][midj3][0];
			G3 = ImageData[midi3][midj3][1];
			B3 = ImageData[midi3][midj3][2];

			output[i][j][0] = (int)((1.0-diffj)*(1-diffi)*R0 + diffj*(1-diffi)*R1 + diffi*(1-diffj)*R2 + diffj*diffi*R3);
			output[i][j][1] = (int)((1.0-diffj)*(1-diffi)*G0 + diffj*(1-diffi)*G1 + diffi*(1-diffj)*G2 + diffj*diffi*G3);
			output[i][j][2] = (int)((1.0-diffj)*(1-diffi)*B0 + diffj*(1-diffi)*B1 + diffi*(1-diffj)*B2 + diffj*diffi*B3);

		}
	}
	for(int i=256; i<512; i++)
	{
		for(int j=256; (j <= i) && (j <= 255 + (511-i)/0.4142); j++)
		{
			float x = j - 255;
			float y = 255 -i;

			oldx = 1.4142*x;
			oldy = y - 0.4142*x;
			midj = oldx + 255;
			midi = 255 - oldy;
			diffi = midi - ((int)midi);
			diffj = midj - ((int)midj);

			midi0 = midi;
			midj0 = midj;
			midi1 = midi0 + 1;
			midj1 = midj0;
			midi2 = midi0;
			midj2 = midj0 + 1;
			midi3 = midi0 + 1;
			midj3 = midj0 + 1;

			R0 = ImageData[midi0][midj0][0];
			G0 = ImageData[midi0][midj0][1];
			B0 = ImageData[midi0][midj0][2];
			R1 = ImageData[midi1][midj1][0];
			G1 = ImageData[midi1][midj1][1];
			B1 = ImageData[midi1][midj1][2];
			R2 = ImageData[midi2][midj2][0];
			G2 = ImageData[midi2][midj2][1];
			B2 = ImageData[midi2][midj2][2];
			R3 = ImageData[midi3][midj3][0];
			G3 = ImageData[midi3][midj3][1];
			B3 = ImageData[midi3][midj3][2];

			output[i][j][0] = (int)((1.0-diffj)*(1-diffi)*R0 + diffj*(1-diffi)*R1 + diffi*(1-diffj)*R2 + diffj*diffi*R3);
			output[i][j][1] = (int)((1.0-diffj)*(1-diffi)*G0 + diffj*(1-diffi)*G1 + diffi*(1-diffj)*G2 + diffj*diffi*G3);
			output[i][j][2] = (int)((1.0-diffj)*(1-diffi)*B0 + diffj*(1-diffi)*B1 + diffi*(1-diffj)*B2 + diffj*diffi*B3);
		}
	}
	for(int i=256; i<512; i++)
	{
		for(int j=255; (j >= 511-i) && (j >= 256 - (511-i)/0.4142); j--)
		{
			float x = j - 256;
			float y = 255 -i;

			oldx = 1.4142*x;
			oldy = 0.4142*x + y;
			midj = oldx + 256;
			midi = 255 - oldy;
			diffi = midi - ((int)midi);
			diffj = midj - ((int)midj);

			midi0 = midi;
			midj0 = midj;
			midi1 = midi0 + 1;
			midj1 = midj0;
			midi2 = midi0;
			midj2 = midj0 + 1;
			midi3 = midi0 + 1;
			midj3 = midj0 + 1;

			R0 = ImageData[midi0][midj0][0];
			G0 = ImageData[midi0][midj0][1];
			B0 = ImageData[midi0][midj0][2];
			R1 = ImageData[midi1][midj1][0];
			G1 = ImageData[midi1][midj1][1];
			B1 = ImageData[midi1][midj1][2];
			R2 = ImageData[midi2][midj2][0];
			G2 = ImageData[midi2][midj2][1];
			B2 = ImageData[midi2][midj2][2];
			R3 = ImageData[midi3][midj3][0];
			G3 = ImageData[midi3][midj3][1];
			B3 = ImageData[midi3][midj3][2];

			output[i][j][0] = (int)((1.0-diffj)*(1-diffi)*R0 + diffj*(1-diffi)*R1 + diffi*(1-diffj)*R2 + diffj*diffi*R3);
			output[i][j][1] = (int)((1.0-diffj)*(1-diffi)*G0 + diffj*(1-diffi)*G1 + diffi*(1-diffj)*G2 + diffj*diffi*G3);
			output[i][j][2] = (int)((1.0-diffj)*(1-diffi)*B0 + diffj*(1-diffi)*B1 + diffi*(1-diffj)*B2 + diffj*diffi*B3);
		}
	}
	for(int j=0; j<256; j++)
	{
		for(int i=256; (i <= 511-j) && (i <= 2.4142*j+255.0031); i++)
		{
			float x = j - 256;
			float y = 255 -i;

//			printf("i = %d , j = %d, x = %f, y = %f\n", i, j, x, y);
			oldx = x + 0.4142*y;
			oldy = 1.4142*y;
			midj = oldx + 256;
			midi = 255 - oldy;
			diffi = midi - ((int)midi);
			diffj = midj - ((int)midj);

			midi0 = midi;
			midj0 = midj;
			midi1 = midi0 + 1;
			midj1 = midj0;
			midi2 = midi0;
			midj2 = midj0 + 1;
			midi3 = midi0 + 1;
			midj3 = midj0 + 1;

			R0 = ImageData[midi0][midj0][0];
			G0 = ImageData[midi0][midj0][1];
			B0 = ImageData[midi0][midj0][2];
			R1 = ImageData[midi1][midj1][0];
			G1 = ImageData[midi1][midj1][1];
			B1 = ImageData[midi1][midj1][2];
			R2 = ImageData[midi2][midj2][0];
			G2 = ImageData[midi2][midj2][1];
			B2 = ImageData[midi2][midj2][2];
			R3 = ImageData[midi3][midj3][0];
			G3 = ImageData[midi3][midj3][1];
			B3 = ImageData[midi3][midj3][2];

			output[i][j][0] = (int)((1.0-diffj)*(1-diffi)*R0 + diffj*(1-diffi)*R1 + diffi*(1-diffj)*R2 + diffj*diffi*R3);
			output[i][j][1] = (int)((1.0-diffj)*(1-diffi)*G0 + diffj*(1-diffi)*G1 + diffi*(1-diffj)*G2 + diffj*diffi*G3);
			output[i][j][2] = (int)((1.0-diffj)*(1-diffi)*B0 + diffj*(1-diffi)*B1 + diffi*(1-diffj)*B2 + diffj*diffi*B3);

		}
	}
	for(int j=0; j<256; j++)
	{
		for(int i=255; (i >= j) && (i >= 255.9969 - 2.4142*j); i--)
		{
			float x = j - 256;
			float y = 256 -i;
			oldx = x - 0.4142*y;
			oldy = 1.4142*y;
			midj = oldx + 256;
			midi = 256 - oldy;

//			printf("i = %d , j = %d, x = %f, y = %f, midi = %f, midj = %f\n", i, j, x, y, midi, midj);
			diffi = midi - ((int)midi);
			diffj = midj - ((int)midj);

			midi0 = midi;
			midj0 = midj;
			midi1 = midi0 + 1;
			midj1 = midj0;
			midi2 = midi0;
			midj2 = midj0 + 1;
			midi3 = midi0 + 1;
			midj3 = midj0 + 1;

			R0 = ImageData[midi0][midj0][0];
			G0 = ImageData[midi0][midj0][1];
			B0 = ImageData[midi0][midj0][2];
			R1 = ImageData[midi1][midj1][0];
			G1 = ImageData[midi1][midj1][1];
			B1 = ImageData[midi1][midj1][2];
			R2 = ImageData[midi2][midj2][0];
			G2 = ImageData[midi2][midj2][1];
			B2 = ImageData[midi2][midj2][2];
			R3 = ImageData[midi3][midj3][0];
			G3 = ImageData[midi3][midj3][1];
			B3 = ImageData[midi3][midj3][2];

			output[i][j][0] = (int)((1.0-diffj)*(1-diffi)*R0 + diffj*(1-diffi)*R1 + diffi*(1-diffj)*R2 + diffj*diffi*R3);
			output[i][j][1] = (int)((1.0-diffj)*(1-diffi)*G0 + diffj*(1-diffi)*G1 + diffi*(1-diffj)*G2 + diffj*diffi*G3);
			output[i][j][2] = (int)((1.0-diffj)*(1-diffi)*B0 + diffj*(1-diffi)*B1 + diffi*(1-diffj)*B2 + diffj*diffi*B3);

		}
	}
	for(int i=0; i<256; i++)
	{
		for(int j=255; (j >= i) && (j >= 256 - i/0.4142); j--)
		{
			float x = j - 256;
			float y = 256 -i;

			oldx = 1.4142*x;
			oldy = y - 0.4142*x;
			midj = oldx + 256;
			midi = 256 - oldy;
			diffi = midi - ((int)midi);
			diffj = midj - ((int)midj);

			midi0 = midi;
			midj0 = midj;
			midi1 = midi0 + 1;
			midj1 = midj0;
			midi2 = midi0;
			midj2 = midj0 + 1;
			midi3 = midi0 + 1;
			midj3 = midj0 + 1;

			R0 = ImageData[midi0][midj0][0];
			G0 = ImageData[midi0][midj0][1];
			B0 = ImageData[midi0][midj0][2];
			R1 = ImageData[midi1][midj1][0];
			G1 = ImageData[midi1][midj1][1];
			B1 = ImageData[midi1][midj1][2];
			R2 = ImageData[midi2][midj2][0];
			G2 = ImageData[midi2][midj2][1];
			B2 = ImageData[midi2][midj2][2];
			R3 = ImageData[midi3][midj3][0];
			G3 = ImageData[midi3][midj3][1];
			B3 = ImageData[midi3][midj3][2];

			output[i][j][0] = (int)((1.0-diffj)*(1-diffi)*R0 + diffj*(1-diffi)*R1 + diffi*(1-diffj)*R2 + diffj*diffi*R3);
			output[i][j][1] = (int)((1.0-diffj)*(1-diffi)*G0 + diffj*(1-diffi)*G1 + diffi*(1-diffj)*G2 + diffj*diffi*G3);
			output[i][j][2] = (int)((1.0-diffj)*(1-diffi)*B0 + diffj*(1-diffi)*B1 + diffi*(1-diffj)*B2 + diffj*diffi*B3);
		}
	}

	writeimage("1c warped fruits.raw", &output[0][0][0], SizeH, SizeW, 3);

	return 0;
}


int main(void)
{
//	p1a();
//	p1b();
//	p1c();

	return 0;
}
