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

using namespace std;

int PixelWiseEn(float m0, float v0, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata)
{
	float sum = 0.0, mean;
	float sumv = 0.0, variance;

	for(int i=0; i < SizeH; i++)
	{
		for(int j=0; j < SizeW; j++)
		{
			sum = sum + inputdata[i*SizeW+j];
		}
	}
	mean = sum/(SizeH*SizeW);

	for(int i=0; i < SizeH; i++)
	{
		for(int j=0; j < SizeW; j++)
		{
			sumv = sumv + (inputdata[i*SizeW+j] - mean)*(inputdata[i*SizeW+j] - mean);
		}
	}
	variance = sumv/(SizeH*SizeW - 1);

	for(int i=0; i < SizeH; i++)
	{
		for(int j=0; j < SizeW; j++)
		{
			int x = inputdata[i*SizeW+j];

			if (x > mean)
			{
				outputdata[i*SizeW+j] = m0 + sqrt((x-mean)*(x-mean)*v0/variance);
			}
			else
			{
				outputdata[i*SizeW+j] = m0 - sqrt((x-mean)*(x-mean)*v0/variance);
			}
		}
	}

	return 0;
}

//int PixelWiseEn(int windowsize, float m0, float v0, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata)
//{
//	int extend = (windowsize-1)/2;
//	unsigned char extendimagedata[SizeH+2*extend][SizeW+2*extend];
//
//	extendonechannel2(inputdata, SizeH, SizeW, &extendimagedata[0][0], extend);
//
//	float v = 0;
//
//	for(int i=extend; (i < (SizeH + extend)); i++)
//	{
//		for(int j=extend; (j < (SizeW + extend)); j++)
//		{
//			int x = extendimagedata[i][j];
//			float sum = 0.0, mean;
//			float sumv = 0.0, variance;
//
//			for(int a = (i-extend); (a <= (i+extend)); a++)
//			{
//				for(int b = (j-extend); (b <= (j+extend)); b++)
//				{
//					sum = sum + extendimagedata[a][b];
//				}
//			}
//			mean = sum/(windowsize*windowsize);
//
//			for(int a = (i-extend); (a <= (i+extend)); a++)
//			{
//				for(int b = (j-extend); (b <= (j+extend)); b++)
//				{
//					sumv = sumv + (extendimagedata[a][b] - mean)*(extendimagedata[a][b] - mean);
//				}
//			}
//			variance = sumv/(windowsize*windowsize - 1);
//
//			if (variance > v)
//				v = variance;
//
//			if (x > mean)
//			{
//				outputdata[(i-extend)*SizeW+(j-extend)] = m0 + sqrt((x-mean)*(x-mean)*v0/variance);
//			}
//			else
//			{
//				outputdata[(i-extend)*SizeW+(j-extend)] = m0 - sqrt((x-mean)*(x-mean)*v0/variance);
//			}
//
//		}
//	}
//
//	printf("%f\n", v);
//	return 0;
//}

int gray2binary(int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata)
{
	int neighborsize = 91;
	int extend = (neighborsize-1)/2;
	unsigned char extendimagedata[SizeH+2*extend][SizeW+2*extend];

	extendonechannel2(inputdata, SizeH, SizeW, &extendimagedata[0][0], extend);

	for(int i=extend; (i < (SizeH + extend)); i++)
	{
		for(int j=extend; (j < (SizeW + extend)); j++)
		{
			int next = 0;
			int median = 0;
			vector<int> window;
			int x = extendimagedata[i][j];

			for(int a = (i-extend); (a <= (i+extend)); a++)
			{
				for(int b = (j-extend); (b <= (j+extend)); b++)
				{
					next = extendimagedata[a][b];
					window.push_back(next);
				}
			}

			sort(window.begin(), window.end());
			median = window.at((neighborsize*neighborsize+1)/2);

			if (median > x)
			{
				outputdata[(i-extend)*SizeW+(j-extend)] = 255;
			}
			else
			{
				outputdata[(i-extend)*SizeW+(j-extend)] = 0;
			}

			window.clear();

		}
	}


	return 0;
}

int RemoveErrorDots(int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata)
{
	unsigned char middata[SizeH][SizeW];
	unsigned char tempdata[SizeH][SizeW];

	// remove isolated dots
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			middata[i][j] = inputdata[i*SizeW+j];
		}
	}
	for(int i=0; i<(SizeH-3); i++)
	{
		for(int j=0; j<(SizeW-3); j++)
		{
			if (inputdata[i*SizeW+j] == 0)
			{
				if(inputdata[i*SizeW+(j+1)] == 0 && inputdata[i*SizeW+(j+2)] == 0 && inputdata[i*SizeW+(j+3)] == 0
						&& inputdata[(i+1)*SizeW+j] == 0 && inputdata[(i+1)*SizeW+(j+3)] == 0
						&& inputdata[(i+2)*SizeW+j] == 0 && inputdata[(i+2)*SizeW+(j+3)] == 0
						&& inputdata[(i+3)*SizeW+(j+1)] == 0 && inputdata[(i+3)*SizeW+(j+2)] == 0 && inputdata[(i+3)*SizeW+(j+3)] == 0
						&& inputdata[(i+3)*SizeW+j] == 0)
				{
					middata[i+1][j+1] = 0;
					middata[i+1][j+2] = 0;
					middata[i+2][j+1] = 0;
					middata[i+2][j+2] = 0;
				}
			}
		}
	}

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			tempdata[i][j] = middata[i][j];
		}
	}
	for(int i=1; i<(SizeH-4); i++)
	{
		for(int j=1; j<(SizeW-4); j++)
		{
			if (middata[i][j] == 0)
			{
				if(middata[i][j+1]==0 && middata[i][j+2]==0 && middata[i][j+3]==0 && middata[i][j+4]==0
						&& middata[i+1][j]==0 && middata[i+1][j+4]==0
						&& middata[i+2][j]==0 && middata[i+2][j+4]==0 && middata[i+4][j+4]==0
						&& middata[i+3][j]==0 && middata[i+3][j+4]==0 && middata[i+4][j+3]==0
						&& middata[i+4][j]==0 && middata[i+4][j+1]==0 && middata[i+4][j+2]==0)
				{
					tempdata[i+1][j+1] = 0;
					tempdata[i+1][j+2] = 0;
					tempdata[i+1][j+3] = 0;
					tempdata[i+2][j+1] = 0;
					tempdata[i+2][j+2] = 0;
					tempdata[i+2][j+3] = 0;
					tempdata[i+3][j+1] = 0;
					tempdata[i+3][j+2] = 0;
					tempdata[i+3][j+3] = 0;
				}
			}
		}
	}

	// remove error connected dots
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			outputdata[i*SizeW+j] = tempdata[i][j];
		}
	}



//	for(int i=2; i<(SizeH-2); i++)
//	{
//		for(int j=2; j<(SizeW-2); j++)
//		{
//				int x0 = tempdata[(i-1)][(j-1)];
//				int x1 = tempdata[(i-1)][j];
//				int x2 = tempdata[(i-1)][(j+1)];
//				int x3 = tempdata[i][(j-1)];
//				int x4 = tempdata[i][(j+1)];
//				int x5 = tempdata[(i+1)][(j-1)];
//				int x6 = tempdata[(i+1)][j];
//				int x7 = tempdata[(i+1)][(j+1)];
//				int x8 = tempdata[(i-2)][(j-2)];
//				int x9 = tempdata[(i-2)][(j-1)];
//				int x10 = tempdata[(i-2)][j];
//				int x11 = tempdata[(i-2)][(j+1)];
//				int x12 = tempdata[(i-2)][(j+2)];
//				int x13 = tempdata[(i-1)][(j-2)];
//				int x14 = tempdata[(i-1)][(j+2)];
//				int x15 = tempdata[i][(j-2)];
//				int x16 = tempdata[i][(j+2)];
//				int x17 = tempdata[(i+1)][(j-2)];
//				int x18 = tempdata[(i+1)][(j-1)];
//				int x19 = tempdata[(i+2)][(j-2)];
//				int x20 = tempdata[(i+2)][(j-1)];
//				int x21 = tempdata[(i+2)][j];
//				int x22 = tempdata[(i+2)][(j+1)];
//				int x23 = tempdata[(i+2)][(j+2)];

//				if(x0 == 255 && x1==0 && x2==0 && x3==0 && x4==0 && x5==0 && x6==255 && x7==255 && tempdata[i][j]==255)
//				{
//					printf("there is one dot\n");
//					outputdata[i*SizeW+j] = 0;
//				}
//				if(x10==255 && x9==0 && x8==0 && x2==255 && x13==0
//						&& x15==255 && x16==255
//						&& x18==0 && x5==255 && x23==0 && x22==0 && x21==255)
//				{
//					outputdata[i*SizeW+j] = 0;
//					outputdata[(i-1)*SizeW+j] = 0;
//					outputdata[(i+1)*SizeW+j] = 0;
//					outputdata[i*SizeW+(j-1)] = 0;
//					outputdata[i*SizeW+(j+1)] = 0;
//					outputdata[(i-1)*SizeW+(j-1)] = 0;
//					outputdata[(i+1)*SizeW+(j+1)] = 0;
//				}
//				if(x8==255 && x13==255 && x15==255 && x17==255 && x19==255
//						&& x9==0 && x20==0
//						&& x10==0 && x21==0
//						&& x11==0 && x22==0
//						&& x12==255 && x14==255 && x16==255 && x18==255 && x23==255 )
//				{
//					outputdata[i*SizeW+j] = 0;
//					outputdata[(i-1)*SizeW+j] = 0;
//					outputdata[(i+1)*SizeW+j] = 0;
//					outputdata[i*SizeW+(j-1)] = 0;
//					outputdata[(i-1)*SizeW+(j-1)] = 0;
//					outputdata[(i+1)*SizeW+(j-1)] = 0;
//					outputdata[i*SizeW+(j+1)] = 0;
//					outputdata[(i-1)*SizeW+(j+1)] = 0;
//					outputdata[(i+1)*SizeW+(j+1)] = 0;
//				}
//				if(x8==255 && x9==255 && x10==255 && x11==255 && x12==255
//						&& x13==0 && x14==0
//						&& x15==0 && x16==0
//						&& x17==0 && x18==0
//						&& x19==255 && x20==255 && x21==255 && x22==255 && x23==255 )
//				{
//					outputdata[i*SizeW+j] = 0;
//					outputdata[(i-1)*SizeW+j] = 0;
//					outputdata[(i+1)*SizeW+j] = 0;
//					outputdata[i*SizeW+(j-1)] = 0;
//					outputdata[(i-1)*SizeW+(j-1)] = 0;
//					outputdata[(i+1)*SizeW+(j-1)] = 0;
//					outputdata[i*SizeW+(j+1)] = 0;
//					outputdata[(i-1)*SizeW+(j+1)] = 0;
//					outputdata[(i+1)*SizeW+(j+1)] = 0;
//				}
//		}
//	}

	return 0;
}

int HoleFilling(int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata)
{
	unsigned char middata[SizeH][SizeW];

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			middata[i][j] = inputdata[i*SizeW+j];
		}
	}

	for(int i=1; i<(SizeH-3); i++)
	{
		for(int j=1; j<(SizeW-3); j++)
		{
			if (inputdata[i*SizeW+j] == 255)
			{
				if(inputdata[i*SizeW+(j+1)] == 255 && inputdata[(i+1)*SizeW+j] == 255 && inputdata[(i+2)*SizeW+j] == 255
						&& inputdata[(i+3)*SizeW+(j+1)] == 255 && inputdata[(i+3)*SizeW+j] == 255)
				{
					middata[i+1][j+1] = 255;
					middata[i+2][j+1] = 255;
				}
				if(inputdata[i*SizeW+(j+1)] == 255 && inputdata[i*SizeW+(j+2)] == 255 && inputdata[i*SizeW+(j+3)] == 255
						&& inputdata[(i+1)*SizeW+j] == 255 && inputdata[(i+1)*SizeW+(j+3)] == 255)
				{
					middata[i+1][j+1] = 255;
					middata[i+1][j+2] = 255;
				}
			}
		}
	}
	for(int i=(SizeH-4); i>0; i--)
	{
		for(int j=(SizeW-4); j>0; j--)
		{
			if (inputdata[i*SizeW+j] == 255)
			{
				if(inputdata[i*SizeW+(j-1)] == 255 && inputdata[(i-1)*SizeW+j] == 255 && inputdata[(i-2)*SizeW+j] == 255
						&& inputdata[(i-3)*SizeW+(j-1)] == 255 && inputdata[(i-3)*SizeW+j] == 255)
				{
					middata[i-1][j-1] = 255;
					middata[i-2][j-1] = 255;
				}
				if(inputdata[i*SizeW+(j-1)] == 255 && inputdata[i*SizeW+(j-2)] == 255 && inputdata[i*SizeW+(j-3)] == 255
						&& inputdata[(i-1)*SizeW+j] == 255 && inputdata[(i-1)*SizeW+(j-3)] == 255)
				{
					middata[i-1][j-1] = 255;
					middata[i-1][j-2] = 255;
				}
			}
		}
	}
	for(int i=1; i<(SizeH-3); i++)
	{
		for(int j=1; j<(SizeW-3); j++)
		{
			if (inputdata[i*SizeW+j] == 255)
			{
				if(inputdata[i*SizeW+(j+1)] == 255 && inputdata[i*SizeW+(j+2)] == 255 && inputdata[i*SizeW+(j+3)] == 255
						&& inputdata[(i+1)*SizeW+j] == 255 && inputdata[(i+1)*SizeW+(j+3)] == 255
						&& inputdata[(i+2)*SizeW+j] == 255 && inputdata[(i+2)*SizeW+(j+3)] == 255
						&& inputdata[(i+3)*SizeW+(j+1)] == 255 && inputdata[(i+3)*SizeW+(j+2)] == 255 && inputdata[(i+3)*SizeW+(j+3)] == 255
						&& inputdata[(i+3)*SizeW+j] == 255)
				{
					middata[i+1][j+1] = 255;
					middata[i+1][j+2] = 255;
					middata[i+2][j+1] = 255;
					middata[i+2][j+2] = 255;
				}
			}
		}
	}

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			outputdata[i*SizeW+j] = middata[i][j];
		}
	}
	for(int i=1; i<(SizeH-4); i++)
	{
		for(int j=1; j<(SizeW-4); j++)
		{
			if (middata[i][j] == 255)
			{
				if(middata[i][j+1]==255 && middata[i][j+2]==255 && middata[i][j+3]==255 && middata[i][j+4]==255
						&& middata[i+1][j]==255 && middata[i+1][j+4]==255
						&& middata[i+2][j]==255 && middata[i+2][j+4]==255 && middata[i+4][j+4]==255
						&& middata[i+3][j]==255 && middata[i+3][j+4]==255 && middata[i+4][j+3]==255
						&& middata[i+4][j]==255 && middata[i+4][j+1]==255 && middata[i+4][j+2]==255)
				{
					outputdata[(i+1)*SizeW+(j+1)] = 255;
					outputdata[(i+1)*SizeW+(j+2)] = 255;
					outputdata[(i+1)*SizeW+(j+3)] = 255;
					outputdata[(i+2)*SizeW+(j+1)] = 255;
					outputdata[(i+2)*SizeW+(j+2)] = 255;
					outputdata[(i+2)*SizeW+(j+3)] = 255;
					outputdata[(i+3)*SizeW+(j+1)] = 255;
					outputdata[(i+3)*SizeW+(j+2)] = 255;
					outputdata[(i+3)*SizeW+(j+3)] = 255;
				}

//				if(middata[i][j+1]==255 && middata[i][j+2]==255 && middata[i][j+3]==255 && middata[i][j+4]==255
//						&& middata[i+1][j]==255 && middata[i+1][j+1]==255 && middata[i+1][j+2]==255
//						&& middata[i+2][j]==255 && middata[i+2][j+1]==255 && middata[i+2][j+3]==255 && middata[i+2][j+4]==255
//						&& middata[i+3][j]==255 && middata[i+3][j+2]==255 && middata[i+3][j+3]==255 && middata[i+3][j+4]==255
//						&& middata[i+4][j]==255 && middata[i+4][j+1]==255 && middata[i+4][j+2]==255 && middata[i+4][j+3]==255 && middata[i+4][j+4]==255)
//				{
////					outputdata[(i+1)*SizeW+(j+1)] = 255;
////					outputdata[(i+1)*SizeW+(j+2)] = 255;
//					outputdata[(i+1)*SizeW+(j+3)] = 255;
//					outputdata[(i+1)*SizeW+(j+4)] = 255;
//					outputdata[(i+2)*SizeW+(j+1)] = 255;
//					outputdata[(i+2)*SizeW+(j+2)] = 255;
//					outputdata[(i+2)*SizeW+(j+3)] = 255;
//					outputdata[(i+3)*SizeW+(j+1)] = 255;
////					outputdata[(i+3)*SizeW+(j+2)] = 255;
////					outputdata[(i+3)*SizeW+(j+3)] = 255;
//
//				}

			}
		}
	}

	return 0;
}

int GetM(int type, int condition)
{
	int hitmask[] = { 0b001010000, 0b100010000, 0b000010100, 0b000010001, // S 1
					  0b000011000, 0b010010000, 0b000110000, 0b000010010, // S 2
					  0b001011000, 0b011010000, 0b110010000, 0b100110000, // S 3
					  0b000110100, 0b000010110, 0b000010011, 0b000011001, // S 3
					  0b110011000, 0b010011001, 0b011110000, 0b001011010, // ST 5
					  0b011011000, 0b110110000, 0b000110110, 0b000011011, // ST 5
					  0b110011001, 0b011110100, // ST 6
					  0b001011001, 0b111010000, 0b100110100, 0b000010111, // STK 4
					  0b111011000, 0b011011001, 0b111110000, 0b110110100, // STK 6
					  0b100110110, 0b000110111, 0b000011111, 0b001011011, // STK 6
					  0b111011001, 0b111110100, 0b100110111, 0b001011111, // STK 7
					  0b011011011, 0b111111000, 0b110110110, 0b000111111, // STK 8
					  0b111011011, 0b011011111, 0b111111100, 0b111111001, // STK 9
					  0b111110110, 0b110110111, 0b100111111, 0b001111111, // STK 9
					  0b111011111, 0b111111101, 0b111110111, 0b101111111, // STK 10
					  0b010011000, 0b010110000, 0b000110010, 0b000011010, // TK 4
					  0b111111011, 0b111111110, 0b110111111, 0b011111111  // K 11
	};

//	for(int i=0; i<62; i++)
//	{
//		printf("%d	", hitmask[i]);
//	}


	if (type == 0)
	{
//		printf("\n type = S,  %d \n", condition);
		for (int h=0; h<58; h++)
		{
//			printf("%d	", hitmask[h]);
			if(condition == hitmask[h])
			{
				return 1;
			}
		}
	}
	if (type == 1)
	{
		for (int i=16; i<62; i++)
		{
			if(condition == hitmask[i])
			{
				return 1;
			}

		}
	}
	if (type == 2)
	{
		for (int i=26; i<66; i++)
		{
			if(condition == hitmask[i])
			{
				return 1;
			}

		}
	}

	return 0;

}

int GetFinalM(int uncondition)
{
	int hitmask[] = { 0b001010000, 0b100010000, // Spur
					  0b000010010, 0b000011000, // Single 4-connection
					  0b001011000, 0b011010000, 0b110010000, 0b100110000, // L Cluster
					  0b000110100, 0b000010110, 0b000010011, 0b000011001,
					  0b011110000, 0b110011000, 0b010011001, 0b001011010, // 4-connected offset
					  0b011010100, 0b100110001, 0b001110100, 0b100010011,  // Spur corner Cluster  A = 1, B = 0
					  0b001011100, 0b110010001, 0b001010110, 0b100011001,  // A = 0, B = 1
					  0b011011100, 0b110110001, 0b001110110, 0b100011011,  // A = 1, B = 1
	};

	int hitmaskD[] = {
			  0b110110000, // Corner Cluster
			  0b010111000, 0b010111000, 0b000111010, 0b000111010,   // Tee Branch D = 0
			  0b010110010, 0b010110010, 0b010011010, 0b010011010,
			  0b010011100, 0b010110001, 0b001110010, 0b100011010,    // Diagonal Branch D = 0
			  0b101010111, 0b101011101, 0b111010101, 0b101110101,    // A = 1, B = 1, C = 1, D = 0 Vee Branch
			  0b101010011, 0b101011100, 0b110010101, 0b001110101,    // A = 0, B = 1, C = 1, D = 0
			  0b101010101, 0b101010101, 0b101010101, 0b101010101,    // A = 1, B = 0, C = 1, D = 0
			  0b101010110, 0b100011101, 0b011010101, 0b101110001,    // A = 1, B = 1, C = 0, D = 0
			  0b101010100, 0b100010101, 0b001010101, 0b101010001,    // A = 1, B = 0, C = 0, D = 0;
			  0b101010010, 0b100011100, 0b010010101, 0b001110001,    // A = 0, B = 1, C = 0, D = 0
			  0b101010001, 0b101010100, 0b100010101, 0b001010101,    // A = 0, B = 0, C = 1, D = 0

	};

	int maskDhit[] = {
			0b110110000, // Corner Cluster
			0b011111011, 0b110111110, 0b110111110, 0b011111011,    // Tee Branch D = 0
			0b010111111, 0b111111010, 0b111111010, 0b010111111,
			0b011111110, 0b110111011, 0b011111110, 0b110111011,    // Diagonal Branch
			0b101010111, 0b101011101, 0b111010101, 0b101110101,    // Vee Branch
			0b101010111, 0b101011101, 0b111010101, 0b101110101,
			0b101010111, 0b101011101, 0b111010101, 0b101110101,
			0b101010111, 0b101011101, 0b111010101, 0b101110101,
			0b101010111, 0b101011101, 0b111010101, 0b101110101,
			0b101010111, 0b101011101, 0b111010101, 0b101110101,
			0b101010111, 0b101011101, 0b111010101, 0b101110101,

	};


	for (int i=0; i<28; i++)
	{
		if(uncondition == hitmask[i])
		{
			return 1;
		}
	}
	for (int i=0; i<41; i++)
	{
		int temp = uncondition ^ hitmaskD[i];
		if(((~temp) & maskDhit[i]) == maskDhit[i])
		{
			return 1;
		}

	}

	return 0;
}

int MorphFilter(int type, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata)
{
	// S = 0;  T = 1; K = 2;

	int extend = 1;
	unsigned char extendinputdata[SizeH+2*extend][SizeW+2*extend];
	extendonechannel2(inputdata, SizeH, SizeW, &extendinputdata[0][0], extend);

	for(int i=0; i<(SizeH+2*extend); i++)
	{
		for(int j=0; j<(SizeW+2*extend); j++)
		{
			if((i==0) || (j==0) || (i==(SizeH+2*extend-1)) || (j==(SizeW+2*extend-1)))
			{
				extendinputdata[i][j] = 0;
			}
			else
			{
				extendinputdata[i][j] = (extendinputdata[i][j]/255);
			}
		}
	}

	int moreerode = 1;
	while(moreerode > 0) {

	moreerode = 0;
	unsigned char candidate[SizeH+2*extend][SizeW+2*extend];
	memset(candidate, 0, (SizeH+2*extend)*(SizeW+2*extend)*(sizeof(unsigned char)));

	for(int i=extend; i<(SizeH+extend); i++)
	{
		for(int j=extend; j<(SizeW+extend); j++)
		{
			if(extendinputdata[i][j] == 1)
			{

				int condition = 0;
				int n = 0;

				for(int a=(i+1); a>=(i-1); a--)
				{
					for(int b=(j+1); b>=(j-1); b--)
					{

						if(extendinputdata[a][b] == 1)
						{
							condition = (condition | (int)(pow(2, n)));
						}
						n++;
					}
				}
//				printf("%d, %d, condition = %d \n", (i-1), (j-1), condition);
				int Mvalue = GetM(type, condition);
//				printf("%d, M = %d\n", i, Mvalue);
				candidate[i][j] = Mvalue;
			}

		}
	}

//	unsigned char candidatecopy[SizeH+2*extend][SizeW+2*extend];
//	for(int i=0; i<(SizeH+2*extend); i++)
//	{
//		for(int j=0; j<(SizeW+2*extend); j++)
//		{
//			candidatecopy[i][j] = candidate[i][j] * 255;
//		}
//	}
//	writeimage("2a candidate.raw", &candidatecopy[0][0], (SizeH+2*extend), (SizeW+2*extend), 1);



	for(int i=extend; i<(SizeH+extend); i++)
	{
		for(int j=extend; j<(SizeW+extend); j++)
		{
			if(candidate[i][j] == 1)
			{
				int uncondition = 0;
				int n = 0;

				for(int a=(i+1); a>=(i-1); a--)
				{
					for(int b=(j+1); b>=(j-1); b--)
					{
						if(candidate[a][b] == 1)
						{
							uncondition = (uncondition | (int)(pow(2, n)));
						}
						n++;
					}
				}

				int Mfinal = 0;
				Mfinal = GetFinalM(uncondition);
				if (Mfinal == 1)
				{
					outputdata[(i-extend)*SizeW+(j-extend)] = extendinputdata[i][j]*255;
//					candidatecopy[i][j] = 0;
				}
				else
				{
					moreerode = 1;
					outputdata[(i-extend)*SizeW+(j-extend)] = 0;

				}
			}
			else
			{
				outputdata[(i-extend)*SizeW+(j-extend)] = extendinputdata[i][j]*255;
			}

		}
	}

	extendonechannel2(outputdata, SizeH, SizeW, &extendinputdata[0][0], extend);
//	writeimage("2a extend_bfS.raw", &extendinputdata[0][0], (SizeH+2*extend), (SizeW+2*extend), 1);

	for(int i=0; i<(SizeH+2*extend); i++)
	{
		for(int j=0; j<(SizeW+2*extend); j++)
		{
			if((i==0) || (j==0) || (i==(SizeH+2*extend-1)) || (j==(SizeW+2*extend-1)))
			{
				extendinputdata[i][j] = 0;
			}
			else
			{
				extendinputdata[i][j] = (extendinputdata[i][j]/255);
			}
		}
	}

	}

//	writeimage("2a candidate_final.raw", &candidatecopy[0][0], (SizeH+2*extend), (SizeW+2*extend), 1);

	int remaindots = 0;
	for(int i = 0; i<SizeH; i++)
	{
		for(int j = 0; j<SizeW; j++)
		{
			if(outputdata[i*SizeW+j] == 255)
			{
				remaindots ++;
			}
		}
	}

	return remaindots;
}

int CountRidgeLenght(int SizeH, int SizeW, const unsigned char *inputdata, int *storeridgelen)
{
	int extend = 1;
	unsigned char extendinputdata[SizeH+2*extend][SizeW+2*extend];
	unsigned char checkdata[SizeH+2*extend][SizeW+2*extend];
	memset(checkdata, 0, (SizeH+2*extend)*(SizeW+2*extend)*(sizeof(unsigned char)));
	extendonechannel2(inputdata, SizeH, SizeW, &extendinputdata[0][0], extend);

	for(int i=0; i<(SizeH+2*extend); i++)
	{
		for(int j=0; j<(SizeW+2*extend); j++)
		{
			if((i==0) || (j==0) || (i==(SizeH+2*extend-1)) || (j==(SizeW+2*extend-1)))
			{
				extendinputdata[i][j] = 0;
			}
		}
	}

	int ridgenum = 0;
    int current_i,current_j;
    int neighbori[]={-1,-1,-1,0,0,1,1,1};
    int neighborj[]={-1,0,1,-1,1,-1,0,1};

	for(int i=extend; i<=SizeH; i++)
	{
		for(int j=extend; j<=SizeW; j++)
		{
			if (extendinputdata[i][j] == 255 && checkdata[i][j] == 0)
			{
//				printf("%d %d\n", i, j);
                current_i = i;
                current_j = j;

				int partnum = 0;
                for(int k=0; k<8; k++)
                 {
                 	if(extendinputdata[(current_i+neighbori[k])][current_j+neighborj[k]]==255 && checkdata[(current_i+neighbori[k])][current_j+neighborj[k]]==0)
                 	{
                 		partnum++;
                 	}
                 }
//                printf("partnum = %d\n", partnum);

                if(partnum == 0)
                {
                	checkdata[i][j] = 255;
    				storeridgelen[ridgenum] = 1;
    				ridgenum++;
                }

                if(partnum == 1 || partnum == 2)
                {
    				int newridgelen[2];
    				memset(newridgelen, 0, 2*(sizeof(int)));
    				checkdata[i][j] = 255;
    				int part = 0;
    				newridgelen[part]++;

                    for(int k=0;k<8;k++)
                    {
                        if(extendinputdata[(current_i+neighbori[k])][current_j+neighborj[k]]==255 && checkdata[(current_i+neighbori[k])][current_j+neighborj[k]]==0)
                        {
                            current_i = current_i+neighbori[k];
                            current_j = current_j+neighborj[k];
                            checkdata[current_i][current_j] = 255;
                            newridgelen[part]++;

                            int next = 1;
                            while(next > 0)
                            {
                            	next = 0;
                				int subpartnum = 0;
                            	for(int m=0; m<8; m++)
                            	{
                            		if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
                            		{
                            			subpartnum++;
                            		}
                            	}

                            	if(subpartnum == 1)
                            	{
                                	for(int m=0; m<8; m++)
                                	{
                                		if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
                                		{
                                        	current_i = current_i+neighbori[m];
                                        	current_j = current_j+neighborj[m];
                                        	checkdata[current_i][current_j] = 255;
                                        	newridgelen[part]++;
                                        	next = 1;
                                        	break;
                                		}
                                	}
                            	}
                            	if(subpartnum == 2)
                            	{
                            		next = 1;
                                	int partridgelen[2];
                                	memset(partridgelen, 0, 2*(sizeof(int)));
                                	int subpart = 0;
                                	int temp_i = current_i;
                                	int temp_j = current_j;

                                   	for(int m=0; m<8; m++)
                                    {
                                   		if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
                                   		{
                                           	current_i = current_i+neighbori[m];
                                           	current_j = current_j+neighborj[m];
                                          	checkdata[current_i][current_j] = 255;
                                           	partridgelen[subpart]++;

                                           	int go = 1;
                                           	while(go > 0)
                                           	{
                                           		go = 0;
                                           		for(int r=0; r<8; r++)
                                           		{
                                           			if(extendinputdata[current_i+neighbori[r]][current_j+neighborj[r]] == 255 && checkdata[current_i+neighbori[r]][current_j+neighborj[r]] == 0)
                                           			{
                                           				current_i = current_i+neighbori[r];
                                           				current_j = current_j+neighborj[r];
                                           				checkdata[current_i][current_j] = 255;
                                           				partridgelen[subpart]++;
                                           				go = 1;
                                           				break;
                                           			}
                                           		}
                                           	}

                                           	subpart++;

                                   		}

                                   		current_i = temp_i;
                                   		current_j = temp_j;
                                   	}

                                   	if(partridgelen[0] >= partridgelen[1])
                                   	{
                                   		newridgelen[part] = newridgelen[part] + partridgelen[0];
                                   	}
                                   	else
                                   	{
                                   		newridgelen[part] = newridgelen[part] + partridgelen[1];
                                   	}


                            	}

                            }

                             part++;
                        }

                        current_i = i;
                        current_j = j;
                    }

    				storeridgelen[ridgenum] = newridgelen[0] + newridgelen[1];
    				ridgenum++;
                }
                if(partnum == 3)
                {
    				int newridgelen[3];
    				memset(newridgelen, 0, 3*(sizeof(int)));
    				checkdata[i][j] = 255;
    				int part = 0;
    				newridgelen[part]++;

                    for(int k=0;k<8;k++)
                    {
                        if(extendinputdata[(current_i+neighbori[k])][current_j+neighborj[k]]==255 && checkdata[(current_i+neighbori[k])][current_j+neighborj[k]]==0)
                        {
                        	int partridgelen = 1;
                            current_i = current_i+neighbori[k];
                            current_j = current_j+neighborj[k];
                        	checkdata[current_i][current_j]= 255;

    //                      int partridgelen[3];
    //                      memset(partridgelen, 0, 3*(sizeof(int)));

                        	int next = 1;
                            while(next >0)
                            {
                            	next = 0;
                                for(int m=0; m<8; m++)
                                {
                                    if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
                                    {
                                    	current_i = current_i+neighbori[m];
                                    	current_j = current_j+neighborj[m];
                                    	checkdata[current_i][current_j] = 255;
                                    	partridgelen++;
                                    	next = 1;
                                    	break;
                                    }

                                 }

                             }

                             newridgelen[part] = newridgelen[part] + partridgelen;
                             part++;
                        }

                        current_i = i;
                        current_j = j;

                    }

    				vector<int> ridgelen;
                    for(int p=0; p<3; p++)
                    {
    					ridgelen.push_back(newridgelen[p]);
                    }
        			sort(ridgelen.begin(), ridgelen.end());

    				storeridgelen[ridgenum] = ridgelen.at(2) + ridgelen.at(1);
    				ridgenum++;
    				ridgelen.clear();

                }
                if(partnum > 3)
                {
                	printf("partnum > 3, there must be something wrong\n");
                }

                printf("ridgenum = %d, len = %d\n", (ridgenum-1), storeridgelen[ridgenum-1]);

			}

		}
	}

	int maxlen = 0;
	for(int w=0; w<ridgenum; w++)
	{
		if(storeridgelen[w] > maxlen)
		{
			maxlen = storeridgelen[w];
//			printf("enter here\n");
		}
	}

	writeimage("2b checkdata.raw", &checkdata[0][0], (SizeH+2*extend), (SizeW+2*extend), 1);

	return maxlen;

}

int removedots(int thre, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata)
{
	int extend = 1;
	unsigned char extendinputdata[SizeH+2*extend][SizeW+2*extend];
	unsigned char checkdata[SizeH+2*extend][SizeW+2*extend];
	unsigned char flagdata[SizeH+2*extend][SizeW+2*extend];
	memset(checkdata, 0, (SizeH+2*extend)*(SizeW+2*extend)*(sizeof(unsigned char)));
	extendonechannel2(inputdata, SizeH, SizeW, &extendinputdata[0][0], extend);

	for(int i=0; i<(SizeH+2*extend); i++)
	{
		for(int j=0; j<(SizeW+2*extend); j++)
		{
			if((i==0) || (j==0) || (i==(SizeH+2*extend-1)) || (j==(SizeW+2*extend-1)))
			{
				extendinputdata[i][j] = 0;
			}
		}
	}
//	printf("extend success\n");

	int ridgenum = 0;
	int storeridgelen[300];
    int current_i,current_j;
    int neighbori[]={-1,-1,-1,0,0,1,1,1};
    int neighborj[]={-1,0,1,-1,1,-1,0,1};

	for(int i=extend; i<=SizeH; i++)
	{
		for(int j=extend; j<=SizeW; j++)
		{

			if (extendinputdata[i][j] == 255 && checkdata[i][j] == 0)
			{
//				printf("%d %d\n", i, j);
                current_i = i;
                current_j = j;

				int partnum = 0;
                for(int k=0; k<8; k++)
                 {
                 	if(extendinputdata[(current_i+neighbori[k])][current_j+neighborj[k]]==255 && checkdata[(current_i+neighbori[k])][current_j+neighborj[k]]==0)
                 	{
                 		partnum++;
                 	}
                 }
//                printf("partnum = %d\n", partnum);

                if(partnum == 0)
                {
                	checkdata[i][j] = 255;
                	flagdata[i][j] = ridgenum;
                	storeridgelen[ridgenum] = 1;
    				ridgenum++;
                }

                if(partnum == 1 || partnum == 2)
                {
    				int newridgelen[2];
    				memset(newridgelen, 0, 2*(sizeof(int)));
    				checkdata[i][j] = 255;
    				flagdata[i][j] = ridgenum;
    				int part = 0;
    				newridgelen[part]++;

                    for(int k=0;k<8;k++)
                    {
                        if(extendinputdata[(current_i+neighbori[k])][current_j+neighborj[k]]==255 && checkdata[(current_i+neighbori[k])][current_j+neighborj[k]]==0)
                        {
                            current_i = current_i+neighbori[k];
                            current_j = current_j+neighborj[k];
                            checkdata[current_i][current_j] = 255;
                            flagdata[current_i][current_j] = ridgenum;
                            newridgelen[part]++;

                            int next = 1;
                            while(next > 0)
                            {
                            	next = 0;
                				int subpartnum = 0;
                            	for(int m=0; m<8; m++)
                            	{
                            		if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
                            		{
                            			subpartnum++;
                            		}
                            	}

                            	if(subpartnum == 1)
                            	{
                                	for(int m=0; m<8; m++)
                                	{
                                		if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
                                		{
                                        	current_i = current_i+neighbori[m];
                                        	current_j = current_j+neighborj[m];
                                        	checkdata[current_i][current_j] = 255;
                                        	flagdata[current_i][current_j] = ridgenum;
                                        	newridgelen[part]++;
                                        	next = 1;
                                        	break;
                                		}
                                	}
                            	}
                            	if(subpartnum == 2)
                            	{
                            		next = 1;
                                	int partridgelen[2];
                                	memset(partridgelen, 0, 2*(sizeof(int)));
                                	int subpart = 0;
                                	int temp_i = current_i;
                                	int temp_j = current_j;

                                   	for(int m=0; m<8; m++)
                                    {
                                   		if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
                                   		{
                                           	current_i = current_i+neighbori[m];
                                           	current_j = current_j+neighborj[m];
                                          	checkdata[current_i][current_j] = 255;
                                          	flagdata[current_i][current_j] = ridgenum;
                                           	partridgelen[subpart]++;

                                           	int go = 1;
                                           	while(go > 0)
                                           	{
                                           		go = 0;
                                           		for(int r=0; r<8; r++)
                                           		{
                                           			if(extendinputdata[current_i+neighbori[r]][current_j+neighborj[r]] == 255 && checkdata[current_i+neighbori[r]][current_j+neighborj[r]] == 0)
                                           			{
                                           				current_i = current_i+neighbori[r];
                                           				current_j = current_j+neighborj[r];
                                           				checkdata[current_i][current_j] = 255;
                                           				flagdata[current_i][current_j] = ridgenum;
                                           				partridgelen[subpart]++;
                                           				go = 1;
                                           				break;
                                           			}
                                           		}
                                           	}

                                           	subpart++;

                                   		}

                                   		current_i = temp_i;
                                   		current_j = temp_j;
                                   	}

                                   	if(partridgelen[0] >= partridgelen[1])
                                   	{
                                   		newridgelen[part] = newridgelen[part] + partridgelen[0];
                                   	}
                                   	else
                                   	{
                                   		newridgelen[part] = newridgelen[part] + partridgelen[1];
                                   	}


                            	}

                            }

                             part++;
                        }

                        current_i = i;
                        current_j = j;
                    }

                    storeridgelen[ridgenum] = newridgelen[0] + newridgelen[1];
    				ridgenum++;
                }
                if(partnum == 3)
                {
    				int newridgelen[3];
    				memset(newridgelen, 0, 3*(sizeof(int)));
    				checkdata[i][j] = 255;
    				flagdata[i][j] = ridgenum;
    				int part = 0;
    				newridgelen[part]++;

                    for(int k=0;k<8;k++)
                    {
                        if(extendinputdata[(current_i+neighbori[k])][current_j+neighborj[k]]==255 && checkdata[(current_i+neighbori[k])][current_j+neighborj[k]]==0)
                        {
                        	int partridgelen = 1;
                            current_i = current_i+neighbori[k];
                            current_j = current_j+neighborj[k];
                        	checkdata[current_i][current_j]= 255;
                        	flagdata[current_i][current_j] = ridgenum;

    //                      int partridgelen[3];
    //                      memset(partridgelen, 0, 3*(sizeof(int)));

                        	int next = 1;
                            while(next >0)
                            {
                            	next = 0;
                                for(int m=0; m<8; m++)
                                {
                                    if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
                                    {
                                    	current_i = current_i+neighbori[m];
                                    	current_j = current_j+neighborj[m];
                                    	checkdata[current_i][current_j] = 255;
                                    	flagdata[current_i][current_j] = ridgenum;
                                    	partridgelen++;
                                    	next = 1;
                                    	break;
                                    }

                                 }

                             }

                             newridgelen[part] = newridgelen[part] + partridgelen;
                             part++;
                        }

                        current_i = i;
                        current_j = j;

                    }

    				vector<int> ridgelen;
                    for(int p=0; p<3; p++)
                    {
    					ridgelen.push_back(newridgelen[p]);
                    }
        			sort(ridgelen.begin(), ridgelen.end());

        			storeridgelen[ridgenum] = ridgelen.at(2) + ridgelen.at(1);
    				ridgenum++;
    				ridgelen.clear();

                }
                if(partnum > 3)
                {
                	printf("partnum > 3, there must be something wrong\n");
                }

//                printf("ridgenum = %d, len = %d\n", (ridgenum-1), storeridgelen[ridgenum-1]);
			}

		}
	}

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			outputdata[i*SizeW+j]=inputdata[i*SizeW+j];
		}
	}

	for(int w=0; w<ridgenum; w++)
	{
		if(storeridgelen[w] <= thre)
		{
			for(int i=extend; i<=SizeH; i++)
			{
				for(int j=extend; j<=SizeW; j++)
				{
					if(flagdata[i][j] == w)
					{
						outputdata[(i-extend)*SizeW+(j-extend)] = 0;
					}
				}
			}


		}
	}

	return 0;

}
