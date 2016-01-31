#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include "iofile.h"
#include "histogram.h"
#include "HalfToning.h"

// Fixed thresholding for gray-level image or one channel of color image
int Fixedthre(const float T, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata)
{

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			int pixel = inputdata[i*SizeW+j];

			if((pixel >= 0) && (pixel < T))
			{
				outputdata[i*SizeW+j] = 0;
			}
			else if((pixel >= T) && (pixel < 256))
			{
				outputdata[i*SizeW+j] = 255;
			}
			else
			{
				printf("Fixed thresholding Dithering error: unexpected inputdata\n");
			}
		}
	}

	return 0;
}

// Random thresholding for gray-level image or one channel of color image
int RandomThre(int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata)
{
	srand (time(NULL));

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			int random = rand() % 256;
			int pixel = inputdata[i*SizeW+j];

			if(random < 0 || random >= 256)
			{
				printf("Random thresholding Dithering error: unexpected random threshold\n");
			}
			else
			{
//				printf("%d	%f\n", i, random);
				if(random > pixel)
				{
					outputdata[i*SizeW+j] = 0;
				}
				else if(random <= pixel)
				{
					outputdata[i*SizeW+j] = 255;
				}
			}

		}
	}

	return 0;
}

// Dithering for gray-level image or one channel of color image
int Dithering(int N, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata)
{
	int I2[2][2] = {{1,2},{3,0}};
	int I4[4][4] = {{5,9,6,10}, {13,1,14,2}, {7,11,4,8}, {15,3,12,0}};

	for (int i=0; i<=(SizeH-N); i=i+N)
	{
		for (int j=0; j<=(SizeW-N); j=j+N)
		{
			for (int a=i; a<(i+N); a++)
			{
				for (int b=j; b<(j+N); b++)
				{
					int pixel = inputdata[a*SizeW+b];
					float T;

					if (N == 2)
					{
						T = 255*((I2[a-i][b-j] + 0.5) / (N*N));
					}
					if (N == 4)
					{
						T = 255*((I4[a-i][b-j] + 0.5) / (N*N));
					}

//					printf("%d	%d	%f\n", i, pixel, T);
					if (pixel > T)
					{
						outputdata[a*SizeW+b] = 1*255;
					}
					else
					{
						outputdata[a*SizeW+b] = 0*255;
					}
				}
			}
		}
	}

	return 0;
}

// Floyd-Steinberg Error Diffusion for gray-level image
// need to be modified if SizeH % 2 == 1
int FSErrorDiff(int color, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata)
{
	float inputcopy[SizeH][SizeW];
	copyimage2f(SizeH, SizeW, inputdata, &inputcopy[0][0]);

	for (int i=0; i<SizeH; i++)
	{
		if (i%2 == 0)
		{
			for (int j=0; j<SizeW; j++)
			{
				float pixel = inputcopy[i][j];
				if (pixel <= (0.5*color))
				{
					outputdata[i*SizeW+j] = 0*color;
				}
				else
				{
					outputdata[i*SizeW+j] = 1*color;
				}

				float error;
				error = inputcopy[i][j] - outputdata[i*SizeW+j];

				if(j == 0)
				{
					inputcopy[i][j+1] = inputcopy[i][j+1] + error*7/13;
					inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/13;
					inputcopy[i+1][j+1] = inputcopy[i+1][j+1] + error*1/13;
				}
				else if(j == (SizeW-1))
				{
					inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/6;
					inputcopy[i+1][j-1] = inputcopy[i+1][j-1] + error*1/6;
				}
				else
				{
					inputcopy[i][j+1] = inputcopy[i][j+1] + error*7/16;
					inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/16;
					inputcopy[i+1][j-1] = inputcopy[i+1][j-1] + error*3/16;
					inputcopy[i+1][j+1] = inputcopy[i+1][j+1] + error*1/16;
				}
			}
		}
		else
		{
			for (int j=(SizeW-1); j>=0; j--)
			{
				int pixel = inputcopy[i][j];
				if (pixel <= (0.5*color))
				{
					outputdata[i*SizeW+j] = 0*color;
				}
				else
				{
					outputdata[i*SizeW+j] = 1*color;
				}

				float error;
				error = inputcopy[i][j] - outputdata[i*SizeW+j];

				if(i == (SizeH - 1))
				{
					if(!(j == 0))
					{
						inputcopy[i][j-1] = inputcopy[i][j-1] + error;
					}
				}
				else
				{
					if(j == (SizeW-1))
					{
						inputcopy[i][j-1] = inputcopy[i][j-1] + error*7/13;
						inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/13;
						inputcopy[i+1][j-1] = inputcopy[i+1][j-1] + error*1/13;
					}
					else if(j == 0)
					{
						inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/6;
						inputcopy[i+1][j+1] = inputcopy[i+1][j+1] + error*1/6;
					}
					else
					{
						inputcopy[i][j-1] = inputcopy[i][j-1] + error*7/16;
						inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/16;
						inputcopy[i+1][j+1] = inputcopy[i+1][j+1] + error*3/16;
						inputcopy[i+1][j-1] = inputcopy[i+1][j-1] + error*1/16;
					}
				}
			}
		}
	}

	return 0;
}

int FSErrorDiffFord(int SizeH, int SizeW, double *inputdata, double *outputdata)
{
	double inputcopy[SizeH][SizeW];
	copyimaged2d(SizeH, SizeW, inputdata, &inputcopy[0][0]);

	for (int i=0; i<SizeH; i++)
	{
		if (i%2 == 0)
		{
			for (int j=0; j<SizeW; j++)
			{
				double pixel = inputcopy[i][j];

				if (pixel <= (0.5))
				{
					outputdata[i*SizeW+j] = 0;
				}
				else
				{
					outputdata[i*SizeW+j] = 1;
				}

				double error;
				error = pixel - outputdata[i*SizeW+j];

				if(j == 0)
				{
					if(((inputcopy[i][j+1] + error*7/13) >=0) && ((inputcopy[i][j+1] + error*7/13) <= 1))
					{
						inputcopy[i][j+1] = inputcopy[i][j+1] + error*7/13;
					}

					if(((inputcopy[i+1][j] + error*5/13) >= 0) && ((inputcopy[i+1][j] + error*5/13) <= 1))
					{
						inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/13;
					}

					if(((inputcopy[i+1][j+1] + error*1/13) >= 0) && ((inputcopy[i+1][j+1] + error*1/13) <= 1))
					{
						inputcopy[i+1][j+1] = inputcopy[i+1][j+1] + error*1/13;
					}

				}
				else if(j == (SizeW-1))
				{
					if(((inputcopy[i+1][j] + error*5/6) >= 0) && ((inputcopy[i+1][j] + error*5/6) <= 1))
					{
						inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/6;
					}

					if(((inputcopy[i+1][j-1] + error*1/6) >= 0) && ((inputcopy[i+1][j-1] + error*1/6) <= 1))
					{
						inputcopy[i+1][j-1] = inputcopy[i+1][j-1] + error*1/6;
					}
				}
				else
				{
					if(((inputcopy[i][j+1] + error*7/16) >= 0) && ((inputcopy[i][j+1] + error*7/16) <= 1))
					{
						inputcopy[i][j+1] = inputcopy[i][j+1] + error*7/16;
					}

					if(((inputcopy[i+1][j] + error*5/16) >= 0) && ((inputcopy[i+1][j] + error*5/16) <= 1))
					{
						inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/16;
					}

					if(((inputcopy[i+1][j-1] + error*3/16) >= 0) && ((inputcopy[i+1][j-1] + error*3/16) <= 1))
					{
						inputcopy[i+1][j-1] = inputcopy[i+1][j-1] + error*3/16;
					}

					if(((inputcopy[i+1][j+1] + error*1/16) >= 0) && ((inputcopy[i+1][j+1] + error*1/16) <= 1))
					{
						inputcopy[i+1][j+1] = inputcopy[i+1][j+1] + error*1/16;
					}

				}
			}
		}
		else
		{
			for (int j=(SizeW-1); j>=0; j--)
			{
				double pixel = inputcopy[i][j];

				if (pixel <= (0.5))
				{
					outputdata[i*SizeW+j] = 0;
				}
				else
				{
					outputdata[i*SizeW+j] = 1;
				}

				double error;
				error = pixel - outputdata[i*SizeW+j];

				if(i == (SizeH - 1))
				{
					if(!(j == 0))
					{
						if(((inputcopy[i][j-1] + error) >= 0) && ((inputcopy[i][j-1] + error) <= 1))
						{
							inputcopy[i][j-1] = inputcopy[i][j-1] + error;
						}
					}
				}
				else
				{
					if(j == (SizeW-1))
					{
						if(((inputcopy[i][j-1] + error*7/13) >= 0) && ((inputcopy[i][j-1] + error*7/13) <= 1))
						{
							inputcopy[i][j-1] = inputcopy[i][j-1] + error*7/13;
						}

						if(((inputcopy[i+1][j] + error*5/13) >= 0) && ((inputcopy[i+1][j] + error*5/13) <= 1))
						{
							inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/13;
						}

						if (((inputcopy[i+1][j-1] + error*1/13) >= 0) && ((inputcopy[i+1][j-1] + error*1/13) <= 1))
						{
							inputcopy[i+1][j-1] = inputcopy[i+1][j-1] + error*1/13;
						}
					}
					else if(j == 0)
					{
						if (((inputcopy[i+1][j] + error*5/6) >= 0) && ((inputcopy[i+1][j] + error*5/6) <= 1))
						{
							inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/6;
						}

						if (((inputcopy[i+1][j+1] + error*1/6) >= 0) && ((inputcopy[i+1][j+1] + error*1/6) <= 1))
						{
							inputcopy[i+1][j+1] = inputcopy[i+1][j+1] + error*1/6;
						}

					}
					else
					{
						if (((inputcopy[i][j-1] + error*7/16) >= 0) && ((inputcopy[i][j-1] + error*7/16) <= 1))
						{
							inputcopy[i][j-1] = inputcopy[i][j-1] + error*7/16;
						}

						if (((inputcopy[i+1][j] + error*5/16) >= 0) && ((inputcopy[i+1][j] + error*5/16) <= 1))
						{
							inputcopy[i+1][j] = inputcopy[i+1][j] + error*5/16;
						}

						if (((inputcopy[i+1][j+1] + error*3/16) >= 0) && ((inputcopy[i+1][j+1] + error*3/16) <= 1))
						{
							inputcopy[i+1][j+1] = inputcopy[i+1][j+1] + error*3/16;
						}

						if (((inputcopy[i+1][j-1] + error*1/16) >= 0) && ((inputcopy[i+1][j-1] + error*1/16) <= 1))
						{
							inputcopy[i+1][j-1] = inputcopy[i+1][j-1] + error*1/16;
						}

					}
				}
			}
		}
	}

	return 0;
}


int FindVertex(int position, double c0, double m0, double y0)
{
	double error;
	int vertex = -1;
	double W = sqrt((c0-0)*(c0-0) + (m0-0)*(m0-0) + (y0-0)*(y0-0));
	double M = sqrt((c0-0)*(c0-0) + (m0-1)*(m0-1) + (y0-0)*(y0-0));
	double C = sqrt((c0-1)*(c0-1) + (m0-0)*(m0-0) + (y0-0)*(y0-0));
	double Y = sqrt((c0-0)*(c0-0) + (m0-0)*(m0-0) + (y0-1)*(y0-1));
	double B = sqrt((c0-1)*(c0-1) + (m0-1)*(m0-1) + (y0-0)*(y0-0));
	double R = sqrt((c0-0)*(c0-0) + (m0-1)*(m0-1) + (y0-1)*(y0-1));
	double G = sqrt((c0-1)*(c0-1) + (m0-0)*(m0-0) + (y0-1)*(y0-1));
	double K = sqrt((c0-1)*(c0-1) + (m0-1)*(m0-1) + (y0-1)*(y0-1));

	switch (position)
	{

	case 1 :
		error = W;
		vertex = 0;
		if(M < error)
		{
			error = M;
			vertex = 1;
		}
		if(C < error)
		{
			error = C;
			vertex = 2;
		}
		if(Y < error)
		{
			error = Y;
			vertex = 3;
		}
		break;
	case 2 :
		error = M;
		vertex = 1;
		if(C < error)
		{
			error = C;
			vertex = 2;
		}
		if(Y < error)
		{
			error = Y;
			vertex = 3;
		}
		if(G < error)
		{
			error = G;
			vertex = 6;
		}
		break;
	case 3 :
		error = M;
		vertex = 1;
		if(Y < error)
		{
			error = Y;
			vertex = 3;
		}
		if(R < error)
		{
			error = R;
			vertex = 5;
		}
		if(G < error)
		{
			error = G;
			vertex = 6;
		}
		break;
	case 4 :
		error = B;
		vertex = 4;
		if(R < error)
		{
			error = R;
			vertex = 5;
		}
		if(G < error)
		{
			error = G;
			vertex = 6;
		}
		if(K < error)
		{
			error = K;
			vertex = 7;
		}
		break;
	case 5 :
		error = M;
		vertex = 1;
		if(B < error)
		{
			error = B;
			vertex = 4;
		}
		if(R < error)
		{
			error = R;
			vertex = 5;
		}
		if(G < error)
		{
			error = G;
			vertex = 6;
		}
		break;
	case 6 :
		error = M;
		vertex = 1;
		if(C < error)
		{
			error = C;
			vertex = 2;
		}
		if(B < error)
		{
			error = B;
			vertex = 4;
		}
		if(G < error)
		{
			error = G;
			vertex = 6;
		}
		break;
	}

	return vertex;
}

int ErrorPass(int i, int j, double result, int SizeH, int SizeW, double *inputcopy)
{
	double error = inputcopy[i*SizeW+j] - result;

	if (i%2 == 0)
	{
			if(j == 0)
			{
				if(((inputcopy[i*SizeW+(j+1)] + error*7/13) >=0) && ((inputcopy[i*SizeW+(j+1)] + error*7/13) <= 1))
				{
					inputcopy[i*SizeW+(j+1)] = inputcopy[i*SizeW+(j+1)] + error*7/13;
				}
				if(((inputcopy[(i+1)*SizeW+j] + error*5/13) >= 0) && ((inputcopy[(i+1)*SizeW+j] + error*5/13) <= 1))
				{
					inputcopy[(i+1)*SizeW+j] = inputcopy[(i+1)*SizeW+j] + error*5/13;
				}
				if(((inputcopy[(i+1)*SizeW+(j+1)] + error*1/13) >= 0) && ((inputcopy[(i+1)*SizeW+(j+1)] + error*1/13) <= 1))
				{
					inputcopy[(i+1)*SizeW+(j+1)] = inputcopy[(i+1)*SizeW+(j+1)] + error*1/13;
				}
			}
			else if(j == (SizeW-1))
			{
				if(((inputcopy[(i+1)*SizeW+j] + error*5/6) >= 0) && ((inputcopy[(i+1)*SizeW+j] + error*5/6) <= 1))
				{
					inputcopy[(i+1)*SizeW+j] = inputcopy[(i+1)*SizeW+j] + error*5/6;
				}
				if(((inputcopy[(i+1)*SizeW+(j-1)] + error*1/6) >= 0) && ((inputcopy[(i+1)*SizeW+(j-1)] + error*1/6) <= 1))
				{
					inputcopy[(i+1)*SizeW+(j-1)] = inputcopy[(i+1)*SizeW+(j-1)] + error*1/6;
				}
			}
			else
			{
				if(((inputcopy[i*SizeW+(j+1)] + error*7/16) >= 0) && ((inputcopy[i*SizeW+(j+1)] + error*7/16) <= 1))
				{
					inputcopy[i*SizeW+(j+1)] = inputcopy[i*SizeW+(j+1)] + error*7/16;
				}
				if(((inputcopy[(i+1)*SizeW+j] + error*5/16) >= 0) && ((inputcopy[(i+1)*SizeW+j] + error*5/16) <= 1))
				{
					inputcopy[(i+1)*SizeW+j] = inputcopy[(i+1)*SizeW+j] + error*5/16;
				}
				if(((inputcopy[(i+1)*SizeW+(j-1)] + error*3/16) >= 0) && ((inputcopy[(i+1)*SizeW+(j-1)] + error*3/16) <= 1))
				{
					inputcopy[(i+1)*SizeW+(j-1)] = inputcopy[(i+1)*SizeW+(j-1)] + error*3/16;
				}
				if(((inputcopy[(i+1)*SizeW+(j+1)] + error*1/16) >= 0) && ((inputcopy[(i+1)*SizeW+(j+1)] + error*1/16) <= 1))
				{
					inputcopy[(i+1)*SizeW+(j+1)] = inputcopy[(i+1)*SizeW+(j+1)] + error*1/16;
				}
			}
	}
	else
	{
			if(i == (SizeH - 1))
			{
				if(!(j == 0))
				{
					if(((inputcopy[i*SizeW+(j-1)] + error) >= 0) && ((inputcopy[i*SizeW+(j-1)] + error) <= 1))
					{
						inputcopy[i*SizeW+(j-1)] = inputcopy[i*SizeW+(j-1)] + error;
					}
				}
			}
			else
			{
				if(j == (SizeW-1))
				{
					if(((inputcopy[i*SizeW+(j-1)] + error*7/13) >= 0) && ((inputcopy[i*SizeW+(j-1)] + error*7/13) <= 1))
					{
						inputcopy[i*SizeW+(j-1)] = inputcopy[i*SizeW+(j-1)] + error*7/13;
					}
					if(((inputcopy[(i+1)*SizeW+j] + error*5/13) >= 0) && ((inputcopy[(i+1)*SizeW+j] + error*5/13) <= 1))
					{
						inputcopy[(i+1)*SizeW+j] = inputcopy[(i+1)*SizeW+j] + error*5/13;
					}
					if (((inputcopy[(i+1)*SizeW+(j-1)] + error*1/13) >= 0) && ((inputcopy[(i+1)*SizeW+(j-1)] + error*1/13) <= 1))
					{
						inputcopy[(i+1)*SizeW+(j-1)] = inputcopy[(i+1)*SizeW+(j-1)] + error*1/13;
					}
				}
				else if(j == 0)
				{
					if (((inputcopy[(i+1)*SizeW+j] + error*5/6) >= 0) && ((inputcopy[(i+1)*SizeW+j] + error*5/6) <= 1))
					{
						inputcopy[(i+1)*SizeW+j] = inputcopy[(i+1)*SizeW+j] + error*5/6;
					}
					if (((inputcopy[(i+1)*SizeW+(j+1)] + error*1/6) >= 0) && ((inputcopy[(i+1)*SizeW+(j+1)] + error*1/6) <= 1))
					{
						inputcopy[(i+1)*SizeW+(j+1)] = inputcopy[(i+1)*SizeW+(j+1)] + error*1/6;
					}
				}
				else
				{
					if (((inputcopy[i*SizeW+(j-1)] + error*7/16) >= 0) && ((inputcopy[i*SizeW+(j-1)] + error*7/16) <= 1))
					{
						inputcopy[i*SizeW+(j-1)] = inputcopy[i*SizeW+(j-1)] + error*7/16;
					}
					if (((inputcopy[(i+1)*SizeW+j] + error*5/16) >= 0) && ((inputcopy[(i+1)*SizeW+j] + error*5/16) <= 1))
					{
						inputcopy[(i+1)*SizeW+j] = inputcopy[(i+1)*SizeW+j] + error*5/16;
					}
					if (((inputcopy[(i+1)*SizeW+(j+1)] + error*3/16) >= 0) && ((inputcopy[(i+1)*SizeW+(j+1)] + error*3/16) <= 1))
					{
						inputcopy[(i+1)*SizeW+(j+1)] = inputcopy[(i+1)*SizeW+(j+1)] + error*3/16;
					}
					if (((inputcopy[(i+1)*SizeW+(j-1)] + error*1/16) >= 0) && ((inputcopy[(i+1)*SizeW+(j-1)] + error*1/16) <= 1))
					{
						inputcopy[(i+1)*SizeW+(j-1)] = inputcopy[(i+1)*SizeW+(j-1)] + error*1/16;
					}
				}
			}
	}

	return 0;
}


int MBVQErrorDiff(const char *inputfilename, const char *outputfilename, int SizeH, int SizeW, int BytesPerPixel)
{

	unsigned char inputdata[SizeH][SizeW][BytesPerPixel];
	unsigned char outputdata[SizeH][SizeW][BytesPerPixel];

	readimage(inputfilename, &inputdata[0][0][0], SizeH, SizeW, BytesPerPixel);

	double inputC[SizeH][SizeW];
	double inputM[SizeH][SizeW];
	double inputY[SizeH][SizeW];
	double outputC[SizeH][SizeW];
	double outputM[SizeH][SizeW];
	double outputY[SizeH][SizeW];
	double copyC[SizeH][SizeW];
	double copyM[SizeH][SizeW];
	double copyY[SizeH][SizeW];

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			inputC[i][j] = 1 - (inputdata[i][j][0] / 255.0);
			inputM[i][j] = 1 - (inputdata[i][j][1] / 255.0);
			inputY[i][j] = 1 - (inputdata[i][j][2] / 255.0);
			copyC[i][j] = 1 - (inputdata[i][j][0] / 255.0);
			copyM[i][j] = 1 - (inputdata[i][j][1] / 255.0);
			copyY[i][j] = 1 - (inputdata[i][j][2] / 255.0);
		}
	}

	for (int i=0; i<SizeH; i++)
	{
		if (i%2 == 0)
		{
			for (int j=0; j<SizeW; j++)
			{


				unsigned char copyR = 255*(1.0 - copyC[i][j]);
				unsigned char copyG = 255*(1.0 - copyM[i][j]);
				unsigned char copyB = 255*(1.0 - copyY[i][j]);

				int position = DetMBVQ(copyR, copyG, copyB); //CMYW = 1, MYGC = 2, RGMY = 3, KRGB = 4, RGBM = 5, CMGB = 6;
				int vertex = FindVertex(position, copyC[i][j], copyM[i][j], copyY[i][j]);  // W=0, M=1, C=2, Y=3, B=4, R=5, G=6, K=7
//				printf("%d %d\n", i, vertex);

				switch (vertex)
				{
				case 0 : //W
					outputC[i][j] = 0;
					outputM[i][j] = 0;
					outputY[i][j] = 0;
					break;
				case 1 :  //M
					outputC[i][j] = 0;
					outputM[i][j] = 1;
					outputY[i][j] = 0;
					break;
				case 2 :  // C
					outputC[i][j] = 1;
					outputM[i][j] = 0;
					outputY[i][j] = 0;
					break;
				case 3 :  // Y
					outputC[i][j] = 0;
					outputM[i][j] = 0;
					outputY[i][j] = 1;
					break;
				case 4 :  // B
					outputC[i][j] = 1;
					outputM[i][j] = 1;
					outputY[i][j] = 0;
					break;
				case 5 :  // R
					outputC[i][j] = 0;
					outputM[i][j] = 1;
					outputY[i][j] = 1;
					break;
				case 6 :  // G
					outputC[i][j] = 1;
					outputM[i][j] = 0;
					outputY[i][j] = 1;
					break;
				case 7 :  // K
					outputC[i][j] = 1;
					outputM[i][j] = 1;
					outputY[i][j] = 1;
					break;
				}

				ErrorPass(i, j, outputC[i][j], SizeH, SizeW, &copyC[0][0]);
				ErrorPass(i, j, outputM[i][j], SizeH, SizeW, &copyM[0][0]);
				ErrorPass(i, j, outputY[i][j], SizeH, SizeW, &copyY[0][0]);

			}

		}
		else
		{
			for (int j=(SizeW-1); j>=0; j--)
			{


				unsigned char copyR = 255*(1 - copyC[i][j]);
				unsigned char copyG = 255*(1 - copyM[i][j]);
				unsigned char copyB = 255*(1 - copyY[i][j]);

				int position = DetMBVQ(copyR, copyG, copyB); //CMYW = 1, MYGC = 2, RGMY = 3, KRGB = 4, RGBM = 5, CMGB = 6;
				int vertex = FindVertex(position, copyC[i][j], copyM[i][j], copyY[i][j]); // W=0, M=1, C=2, Y=3, B=4, R=5, G=6, K=7
//				printf("%d %d\n", i, vertex);

				switch (vertex)
				{
				case 0 :
					outputC[i][j] = 0;
					outputM[i][j] = 0;
					outputY[i][j] = 0;
					break;
				case 1 :
					outputC[i][j] = 0;
					outputM[i][j] = 1;
					outputY[i][j] = 0;
					break;
				case 2 :
					outputC[i][j] = 1;
					outputM[i][j] = 0;
					outputY[i][j] = 0;
					break;
				case 3 :
					outputC[i][j] = 0;
					outputM[i][j] = 0;
					outputY[i][j] = 1;
					break;
				case 4 :
					outputC[i][j] = 1;
					outputM[i][j] = 1;
					outputY[i][j] = 0;
					break;
				case 5 :
					outputC[i][j] = 0;
					outputM[i][j] = 1;
					outputY[i][j] = 1;
					break;
				case 6 :
					outputC[i][j] = 1;
					outputM[i][j] = 0;
					outputY[i][j] = 1;
					break;
				case 7 :
					outputC[i][j] = 1;
					outputM[i][j] = 1;
					outputY[i][j] = 1;
					break;
				}

				ErrorPass(i, j, outputC[i][j], SizeH, SizeW, &copyC[0][0]);
				ErrorPass(i, j, outputM[i][j], SizeH, SizeW, &copyM[0][0]);
				ErrorPass(i, j, outputY[i][j], SizeH, SizeW, &copyY[0][0]);

			}
		}
	}

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			outputdata[i][j][0] = 255*(1 - outputC[i][j]);
			outputdata[i][j][1] = 255*(1 - outputM[i][j]);
			outputdata[i][j][2] = 255*(1 - outputY[i][j]);
		}
	}

	writeimage(outputfilename, &outputdata[0][0][0], SizeH, SizeW, BytesPerPixel);


	return 0;
}
