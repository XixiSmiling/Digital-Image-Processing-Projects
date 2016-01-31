#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "iofile.h"
#include "histogram.h"
#include "p1ops.h"

int TwoPassLable(int SizeH, int SizeW, unsigned char *inputdata, int *labeleddata)
{
	int linked[SizeH][SizeH];
	int linkednum[SizeH];
	int nextlabel = 1;
	int L[4];
	int minL;
	for(int i=1; i<SizeH; i++)
	{
		for(int j=1; j<SizeW; j++)
		{
			if(inputdata[i*SizeW+j] == 255)
			{
				if(inputdata[(i-1)*SizeW+(j-1)] == 0 && inputdata[(i-1)*SizeW+j] == 0 &&
						inputdata[(i-1)*SizeW+(j+1)] == 0 && inputdata[i*SizeW+(j-1)] == 0)
				{
					linked[nextlabel][0] = nextlabel;
					linkednum[nextlabel] = 1;
					labeleddata[i*SizeW+j] = nextlabel;
					nextlabel++;
				}
				else
				{
//					printf("check1\n");
//					int L[4];
					L[0] = labeleddata[(i-1)*SizeW+(j-1)];
					L[1] = labeleddata[(i-1)*SizeW+j];
					L[2] = labeleddata[(i-1)*SizeW+(j+1)];
					L[3] = labeleddata[i*SizeW+(j-1)];
					minL = SizeH*SizeW;
					for(int lx=0; lx<4; lx++)
					{
						if(L[lx] < minL && L[lx] > 0)
						{
							minL = L[lx];
						}
					}
					labeleddata[i*SizeW+j] = minL;
					for(int l=0; l<4; l++)
					{
						for(int num=0; num<4; num++)
						{
							for(int testl=0; testl<(linkednum[L[l]]); testl++)
							{
								if(L[num] == linked[(L[l])][testl])
								{
									break;
								}
								if(testl == (linkednum[(L[l])] - 1) && L[num] != linked[(L[l])][testl])
								{
									linked[(L[l])][linkednum[(L[l])]] = L[num];
									linkednum[(L[l])]++;

									for(int testnum=0; testnum<(linkednum[L[num]]); testnum++)
									{
										if(L[l] == linked[L[num]][testnum])
											break;
										if(testnum == (linkednum[L[num]] - 1) && L[l] != linked[L[num]][testnum])
										{
											linked[L[num]][linkednum[L[num]]] = L[l];
											linkednum[L[num]]++;
										}
									}
								}
							}
						}

					}
				}
			}
		}
	}


	for(int i=1; i<SizeH; i++)
	{
		int min = linked[i][0];
		int com = linked[i][0];
		for(int test=0; test<(linkednum[i]); test++)
		{
			if(linked[i][test] < min && linked[i][test] > 0)
			{
				min = linked[i][test];
			}
		}
		while(min != com)
		{
			com = min;
			for(int test=0; test<(linkednum[min]); test++)
			{
				if(linked[min][test] < min && linked[min][test] > 0)
				{
					min = linked[min][test];
				}
			}
			if(min !=com)
			{
				linked[i][linkednum[i]] = min;
				linkednum[i]++;
			}
		}
	}
	for(int i=1; i<SizeH; i++)
	{
		int min = linked[i][0];
		int com = linked[i][0];
		for(int test=0; test<(linkednum[i]); test++)
		{
			for(int testtest=0; testtest<linkednum[linked[i][test]]; testtest++)
			{
				if(linked[linked[i][test]][testtest] < min && linked[linked[i][test]][testtest] > 0)
				{
					min = linked[linked[i][test]][testtest];
				}
			}
		}
		for(int test=0; test<(linkednum[i]); test++)
		{
			if(min == linked[i][test])
			{
				break;
			}
			else if(test == (linkednum[i] - 1) && min != linked[i][test])
			{
				linked[i][linkednum[i]] = min;
				linkednum[i]++;
			}
		}
	}
	for(int i=SizeH-1; i>=0; i--)
	{
		int min = linked[i][0];
		int com = linked[i][0];
		for(int test=0; test<(linkednum[i]); test++)
		{
			for(int testtest=0; testtest<linkednum[linked[i][test]]; testtest++)
			{
				if(linked[linked[i][test]][testtest] < min && linked[linked[i][test]][testtest] > 0)
				{
					min = linked[linked[i][test]][testtest];
				}
			}
		}
		for(int test=0; test<(linkednum[i]); test++)
		{
			if(min == linked[i][test])
			{
				break;
			}
			else if(test == (linkednum[i] - 1) && min != linked[i][test])
			{
				linked[i][linkednum[i]] = min;
				linkednum[i]++;
			}
		}
	}

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			if(labeleddata[i*SizeW+j] != SizeH*SizeW)
			{
				int minLabel = labeleddata[i*SizeW+j];
				for(int test=0; test<(linkednum[(labeleddata[i*SizeW+j])]); test++)
				{
					if(linked[labeleddata[i*SizeW+j]][test] < minLabel && linked[labeleddata[i*SizeW+j]][test] > 0)
					{
						minLabel = linked[labeleddata[i*SizeW+j]][test];
					}
				}
				labeleddata[i*SizeW+j] = minLabel;
			}
		}
	}

	int count = 0;
	for(int i=1; i<SizeH; i++)
	{
		int min = linked[i][0];
		int self = linked[i][0];
		for(int test=0; test<(linkednum[i]); test++)
		{
			if(linked[i][test] < min && linked[i][test] > 0)
			{
				min = linked[i][test];
			}

		}
		if(min == self)
		{
			count++;
		}
	}

//	for(int i=0; i<SizeH; i++)
//	{
//		printf("i = %d ", i);
//		for(int test=0; test<(linkednum[i]); test++)
//		{
//			printf("%d ", linked[i][test]);
//		}
//		printf("\n");
//	}
	return count;
}

int FindHoleInfo(unsigned char *ImageData, int SizeH, int SizeW)
{
	unsigned char holemap[SizeH][SizeW];

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			if(ImageData[i*SizeW*3+j*3+0] > 235 && ImageData[i*SizeW*3+j*3+1] > 225 && ImageData[i*SizeW*3+j*3+2] > 218)
			{
				holemap[i][j] = 255;
			}
			else
				holemap[i][j] = 0;
		}
	}
	writeimage("1a holemap.raw", &holemap[0][0], SizeH, SizeW, 1);

	int labeledmap[SizeH][SizeW];
	memset(labeledmap, 0, SizeH*SizeW*(sizeof(int)));
	TwoPassLable(SizeH, SizeW, &holemap[0][0], &labeledmap[0][0]);

	unsigned char fortest[SizeH][SizeW];
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			fortest[i][j] = labeledmap[i][j]*50;
		}
	}
	writeimage("1a labeledmap.raw", &fortest[0][0], SizeH, SizeW, 1);

	unsigned char finalmap[SizeH][SizeW];
	memset(finalmap, 0, SizeH*SizeW*(sizeof(unsigned char)));
//	for(int i=1; i<(SizeH-3); i++)
//	{
//		for(int j=1; j<(SizeW-3); j++)
//		{
//			if(labeledmap[i][j] != 0)
//			{
//				if(labeledmap[i-1][j-1]==0 && labeledmap[i-1][j]==0 && labeledmap[i-1][j+1]==0 &&
//				labeledmap[i-1][j+2]==0 && labeledmap[i-1][j+3]==0 && labeledmap[i][j-1]==0 &&
//				labeledmap[i+1][j-1]==0 && labeledmap[i+2][j-1]==0 && labeledmap[i+3][j-1]==0 &&
//				labeledmap[i][j+1]!=0 && labeledmap[i][j+2]!=0 && labeledmap[i][j+3]!=0 &&
//				labeledmap[i+1][j]!=0 && labeledmap[i+2][j]!=0 && labeledmap[i+3][j]!=0)
//				{
//					for(int u=0; u<SizeH; u++)
//					{
//						for(int v=0; v<SizeW; v++)
//						{
//							if(labeledmap[u][v] == labeledmap[i][j])
//								finalmap[u][v] = labeledmap[i][j];
//						}
//					}
//				}
//			}
//		}
//	}
	int mark = 1;
	for(int i=(SizeH-2); i>=3; i--)
	{
		for(int j=(SizeW-2); j>=3; j--)
		{
			if(labeledmap[i][j] != 0)
			{
				if(labeledmap[i-3][j+1]==0 && labeledmap[i-2][j+1]==0 && labeledmap[i-1][j+1]==0 &&
				labeledmap[i][j+1]==0 && labeledmap[i+1][j+1]==0 && labeledmap[i+1][j]==0 &&
				labeledmap[i+1][j-3]==0 && labeledmap[i+1][j-2]==0 && labeledmap[i+1][j-1]==0 &&
				labeledmap[i][j-1]!=0 && labeledmap[i][j-2]!=0 && labeledmap[i][j-3]!=0 &&
				labeledmap[i-1][j]!=0 && labeledmap[i-2][j]!=0 && labeledmap[i-3][j]!=0)
				{
					for(int u=0; u<SizeH; u++)
					{
						for(int v=0; v<SizeW; v++)
						{
							if(labeledmap[u][v] == labeledmap[i][j])
								finalmap[u][v] = mark*100;
						}
					}
//					printf("%d\n", mark);
					mark++;
				}
			}
		}
	}
	writeimage("1a finalmap.raw", &finalmap[0][0], SizeH, SizeW, 1);

	int mouthH=0, mouthW=0, eyeH=0, eyeW=0;
	float mouthI, mouthJ, eyeI, eyeJ;
	for(int i=0; i<SizeH; i++)
	{
		int check = 0;
		for(int j=0; j<SizeW; j++)
		{
			if(finalmap[i][j] == 1*100)
			{
				for(int u=0; u<SizeW; u++)
				{
					if(finalmap[i][u] == 1*100)
					{
						mouthW++;
					}
				}
				for(int u=0; u<SizeH; u++)
				{
					if(finalmap[u][j] == 1*100)
					{
						mouthH++;
					}
				}
				mouthI = (i-1)+(mouthH/2.0);
				mouthJ = (j-1)+(mouthW/2.0);
				check = 1;
				break;
			}
		}
		if(check == 1)
			break;
	}

	for(int i=(SizeH-1); i>=0; i--)
	{
		int check = 0;
		for(int j=(SizeW-1); j>=0; j--)
		{
			if(finalmap[i][j] == 2*100)
			{
				for(int u=0; u<SizeW; u++)
				{
					if(finalmap[i][u] == 2*100)
					{
						eyeW++;
					}
				}
				for(int u=0; u<SizeH; u++)
				{
					if(finalmap[u][j] == 2*100)
					{
						eyeH++;
					}
				}
				eyeI = i-(eyeH/2.0);
				eyeJ = j-(eyeW/2.0);
				check = 1;
				break;
			}
		}
		if(check == 1)
			break;
	}

	printf(" height of hole of mouth = %d,\n width of hole of mouth = %d,\n center coordinate of hole of mouth (i, j)= (%f, %f);\n", mouthH, mouthW, mouthI, mouthJ);
	printf(" height of hole of eye = %d,\n width of hole of eye = %d,\n center coordinate of hole of eye (i, j)= (%f, %f).\n\n", eyeH, eyeW, eyeI, eyeJ);

	return 0;
}

int FindPiecesInfo(unsigned char *ImageData, int SizeH, int SizeW)
{
	unsigned char piecemap[SizeH][SizeW];

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			if((ImageData[i*SizeW*3+j*3+0] <= 220) || (ImageData[i*SizeW*3+j*3+1] <= 220) || (ImageData[i*SizeW*3+j*3+2] <= 220))
			{
				piecemap[i][j] = 255;
			}
			else
				piecemap[i][j] = 0;
		}
	}
	writeimage("1a piecemap.raw", &piecemap[0][0], SizeH, SizeW, 1);

	int labeledmap[SizeH][SizeW];
//	float hist[256];
	memset(labeledmap, 0, SizeH*SizeW*(sizeof(int)));
	TwoPassLable(SizeH, SizeW, &piecemap[0][0], &labeledmap[0][0]);
//	GetHG(&labeledmap[0][0], &hist[0], SizeH, SizeW, 256);
//	OutputHG("1a piecelabeledmap_hist.txt", &hist[0], 256);

	unsigned char fortest[SizeH][SizeW];
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			fortest[i][j] = labeledmap[i][j]*50;
		}
	}
	writeimage("1a piecelabeledmap.raw", &fortest[0][0], SizeH, SizeW, 1);

	int x1, y1, x2, y2, x3, y3, x4, y4;
	int a1, b1, a2, b2, a3, b3, a4, b4;
	float x, y, a, b, mW, mH, eW, eH, mAngle, eAngle;
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			if(labeledmap[i][j] == 1)
			{
				x2 = j;
				y2 = SizeH - (i+1);
//				y2 = y2 - 1;
				break;
			}
		}
	}
	for(int j=0; j<SizeW; j++)
	{
		for(int i=0; i<SizeH; i++)
		{
			if(labeledmap[i][j] == 1)
			{
				x3 = j;
//				x3 = x3 + 1;
				y3 = SizeH - (i+1);
				break;
			}
		}
	}
	for(int i=(SizeH-1); i>=0; i--)
	{
		for(int j=(SizeW-1); j>=0; j--)
		{
			if(labeledmap[i][j] == 1)
			{
				x1 = j;
				y1 = SizeH - (i+1);
//				y1 = y1 + 1;
				break;
			}
		}
	}
	for(int j=(SizeW-1); j>=0; j--)
	{
		for(int i=(SizeH-1); i>=0; i--)
		{
			if(labeledmap[i][j] == 1)
			{
				x4 = j;
//				x4 = x4 - 1;
				y4 = SizeH - (i+1);
				break;
			}
		}
	}

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			if(labeledmap[i][j] != 0 && labeledmap[i][j] != 1)
			{
				a2 = j;
				b2 = SizeH - (i+1);
//				b2 = b2 - 1;
				break;
			}
		}
	}
	for(int j=0; j<SizeW; j++)
	{
		for(int i=0; i<SizeH; i++)
		{
			if(labeledmap[i][j] != 0 && labeledmap[i][j] != 1)
			{
				a3 = j;
//				a3 = a3 + 1;
				b3 = SizeH - (i+1);
				break;
			}
		}
	}
	for(int i=(SizeH-1); i>=0; i--)
	{
		for(int j=(SizeW-1); j>=0; j--)
		{
			if(labeledmap[i][j] != 0 && labeledmap[i][j] != 1)
			{
				a1 = j;
				b1 = SizeH - (i+1);
//				b1 = b1 + 1;
				break;
			}
		}
	}
	for(int j=(SizeW-1); j>=0; j--)
	{
		for(int i=(SizeH-1); i>=0; i--)
		{
			if(labeledmap[i][j] != 0 && labeledmap[i][j] != 1)
			{
				a4 = j;
//				a4 = a4 - 1;
				b4 = SizeH - (i+1);
				break;
			}
		}
	}

//	printf("%d, %d, %d, %d, %d, %d, %d, %d\n", a1, b1, a2, b2, a3, b3, a4, b4);

	float mtemp1, mtemp2, etemp1, etemp2;
	mtemp1 = 1.0*(y2 - y1)/(x2 - x1);
	mtemp2 = 1.0*(y4 - y3)/(x4 - x3);
	etemp1 = 1.0*(b2 - b1)/(a2 - a1);
	etemp2 = 1.0*(b4 - b3)/(a4 - a3);

	x = (y3 - mtemp2*x3 + mtemp1*x1 - y1)/(mtemp1 - mtemp2);
	y = mtemp1*(x - x1) + y1;
	a = (b3 - etemp2*a3 + etemp1*a1 - b1)/(etemp1 - etemp2);
	b = etemp1*(a - a1) + b1;
	mW = sqrt((x2 - x3)*(x2 - x3) + (y2 - y3)*(y2 - y3));
	mH = sqrt((x4 - x2)*(x4 - x2) + (y2 - y4)*(y2 - y4));
	eH = sqrt((a2 - a3)*(a2 - a3) + (b2 - b3)*(b2 - b3));
	eW = sqrt((a4 - a2)*(a4 - a2) + (b2 - b4)*(b2 - b4));
	mAngle = atan(1.0*(y4-y1)/(x4-x1));
	eAngle = atan(1.0*(b4-b2)/(a4-a2));

	printf(" mouth height = %f,\n mouth width = %f,\n center coordinate of mouth (i, j) = (%f, %f),\n the orientation angle = %f;\n", mH, mW, (SizeH-y-1), x, mAngle);
	printf(" eye height = %f,\n eye width = %f,\n center coordinate of eye (i, j) = (%f, %f),\n the orientation angle = %f.\n", eH, eW, (SizeH-b-1), a, eAngle);

	return 0;
}

int rotation(unsigned char *input, unsigned char *output, int imageSizeH, int imageSizeW, float rSizeH, float rSizeW, float rI, float rJ, float rAngle)
{
	int iStart, iEnd, jStart, jEnd;
	iStart = rI - rSizeH/2.0 + 1;
	iEnd = rI + rSizeH/2.0;
	jStart = rJ - rSizeW/2.0 + 1;
	jEnd = rJ + rSizeW/2.0;
//	if(((rI - rSizeH/2) - (int)(rI - rSizeH/2)) == 0)
//	{
//		iStart = rI - rSizeH/2.0;
//	}
//	else
//		iStart = rI - rSizeH/2.0 - 1;
//	if(((rI + rSizeH/2) - (int)(rI + rSizeH/2)) == 0)
//	{
//		iEnd = rI + rSizeH/2.0;
//	}
//	else
//		iEnd = rI + rSizeH/2.0 + 1;
//	if(((rJ - rSizeW/2) - (int)(rJ - rSizeW/2)) == 0)
//	{
//		jStart = rJ - rSizeW/2.0;
//	}
//	else
//		jStart = rJ - rSizeW/2.0 - 1;
//	if(((rJ + rSizeW/2) - (int)(rJ + rSizeW/2)) == 0)
//	{
//		jEnd = rJ + rSizeW/2.0;
//	}
//	else
//		jEnd = rJ + rSizeW/2.0 + 1;

	printf("\ncenter i = %f, center j =%f\n", rI, rJ);
	printf("i Start = %d, i End = %d, j Start = %d, j End = %d\n", iStart, iEnd, jStart, jEnd);

	float newx, newy;
	float oldx, oldy, oldj, oldi, diffj, diffi;
	int oldi0, oldj0, oldi1, oldj1, oldi2, oldj2, oldi3, oldj3;
	int R0, G0, B0, R1, G1, B1, R2, G2, B2, R3, G3, B3;
	for(int i=iStart; i<= iEnd; i++)
	{
		for(int j=jStart; j<= jEnd; j++)
		{
			newx = j - rJ;
			newy = -(i-rI);

			oldx = cos(rAngle)*newx + sin(rAngle)*newy;
			oldy = -sin(rAngle)*newx + cos(rAngle)*newy;

			oldj = oldx+rJ;
			oldi = 0 - oldy + rI;
			diffj = oldj - ((int)oldj);
			diffi = oldi - ((int)oldi);

//			printf("i=%d, j=%d, oldi=%f, oldj=%f\n", i, j, oldi, oldj);

			oldi0 = oldi; oldj0 = oldj;
			oldi1 = oldi; oldj1 = oldj+1;
			oldi2 = oldi+1; oldj2 = oldj;
			oldi3 = oldi+1; oldj3 = oldj+1;

			R0 = input[oldi0*imageSizeW*3+oldj0*3+0];
			G0 = input[oldi0*imageSizeW*3+oldj0*3+1];
			B0 = input[oldi0*imageSizeW*3+oldj0*3+2];
			R1 = input[oldi1*imageSizeW*3+oldj1*3+0];
			G1 = input[oldi1*imageSizeW*3+oldj1*3+1];
			B1 = input[oldi1*imageSizeW*3+oldj1*3+2];
			R2 = input[oldi2*imageSizeW*3+oldj2*3+0];
			G2 = input[oldi2*imageSizeW*3+oldj2*3+1];
			B2 = input[oldi2*imageSizeW*3+oldj2*3+2];
			R3 = input[oldi3*imageSizeW*3+oldj3*3+0];
			G3 = input[oldi3*imageSizeW*3+oldj3*3+1];
			B3 = input[oldi3*imageSizeW*3+oldj3*3+2];

			output[i*imageSizeW*3+j*3+0] = (int)((1.0-diffj)*(1-diffi)*R0 + diffj*(1-diffi)*R1 + diffi*(1-diffj)*R2 + diffj*diffi*R3);
			output[i*imageSizeW*3+j*3+1] = (int)((1.0-diffj)*(1-diffi)*G0 + diffj*(1-diffi)*G1 + diffi*(1-diffj)*G2 + diffj*diffi*G3);
			output[i*imageSizeW*3+j*3+2] = (int)((1.0-diffj)*(1-diffi)*B0 + diffj*(1-diffi)*B1 + diffi*(1-diffj)*B2 + diffj*diffi*B3);
		}
	}

	return 0;
}

int scaling(unsigned char *input, unsigned char *output, int imageSizeH, int imageSizeW, float oldH, float oldW, float newH, float newW, float sI, float sJ)
{
	int iStart, iEnd, jStart, jEnd;
	iStart = sI - newH/2.0 + 1;
	iEnd = sI + newH/2.0;
	jStart = sJ - newW/2.0 + 1;
	jEnd = sJ + newW/2.0;
//	if(((sI - newH/2) - (int)(sI - newH/2)) == 0)
//	{
//		iStart = sI - newH/2.0;
//	}
//	else
//		iStart = sI - newH/2.0 - 1;
//	if(((sI + newH/2) - (int)(sI + newH/2)) == 0)
//	{
//		iEnd = sI + newH/2.0;
//	}
//	else
//		iEnd = sI + newH/2.0 + 1;
//	if(((sJ - newW/2) - (int)(sJ - newW/2)) == 0)
//	{
//		jStart = sJ - newW/2.0;
//	}
//	else
//		jStart = sJ - newW/2.0 - 1;
//	if(((sJ + newW/2) - (int)(sJ + newW/2)) == 0)
//	{
//		jEnd = sJ + newW/2.0;
//	}
//	else
//		jEnd = sJ + newW/2.0 + 1;

	printf("\ncenter i = %f, center j =%f\n", sI, sJ);
	printf("i Start = %d, i End = %d, j Start = %d, j End = %d\n", iStart, iEnd, jStart, jEnd);

	float newx, newy;
	float oldx, oldy, oldj, oldi, diffj, diffi;
	int oldi0, oldj0, oldi1, oldj1, oldi2, oldj2, oldi3, oldj3;
	int R0, G0, B0, R1, G1, B1, R2, G2, B2, R3, G3, B3;
	for(int i=iStart; i<= iEnd; i++)
	{
		for(int j=jStart; j<= jEnd; j++)
		{
			newx = j - sJ;
			newy = -(i-sI);

			oldx = newx*oldW/newW;
			oldy = newy*oldH/newH;

			oldj = oldx+sJ;
			oldi = 0 - oldy + sI;
			diffj = oldj - ((int)oldj);
			diffi = oldi - ((int)oldi);

//			printf("newx=%f, newy=%f, oldx=%f, oldy=%f\n", newx, newy, oldx, oldy);

			oldi0 = oldi; oldj0 = oldj;
			oldi1 = oldi; oldj1 = oldj+1;
			oldi2 = oldi+1; oldj2 = oldj;
			oldi3 = oldi+1; oldj3 = oldj+1;

			R0 = input[oldi0*imageSizeW*3+oldj0*3+0];
			G0 = input[oldi0*imageSizeW*3+oldj0*3+1];
			B0 = input[oldi0*imageSizeW*3+oldj0*3+2];
			R1 = input[oldi1*imageSizeW*3+oldj1*3+0];
			G1 = input[oldi1*imageSizeW*3+oldj1*3+1];
			B1 = input[oldi1*imageSizeW*3+oldj1*3+2];
			R2 = input[oldi2*imageSizeW*3+oldj2*3+0];
			G2 = input[oldi2*imageSizeW*3+oldj2*3+1];
			B2 = input[oldi2*imageSizeW*3+oldj2*3+2];
			R3 = input[oldi3*imageSizeW*3+oldj3*3+0];
			G3 = input[oldi3*imageSizeW*3+oldj3*3+1];
			B3 = input[oldi3*imageSizeW*3+oldj3*3+2];

			output[i*imageSizeW*3+j*3+0] = (int)((1.0-diffj)*(1-diffi)*R0 + diffj*(1-diffi)*R1 + diffi*(1-diffj)*R2 + diffj*diffi*R3);
			output[i*imageSizeW*3+j*3+1] = (int)((1.0-diffj)*(1-diffi)*G0 + diffj*(1-diffi)*G1 + diffi*(1-diffj)*G2 + diffj*diffi*G3);
			output[i*imageSizeW*3+j*3+2] = (int)((1.0-diffj)*(1-diffi)*B0 + diffj*(1-diffi)*B1 + diffi*(1-diffj)*B2 + diffj*diffi*B3);
		}
	}


	return 0;
}

int shift(unsigned char *input, unsigned char *output, int inputSizeW, int outputSizeW, float H, float W, float oldI, float oldJ, float newI, float newJ)
{
	int iStart, iEnd, jStart, jEnd;
	iStart = newI - H/2.0 + 2;
	iEnd = newI + H/2.0 - 2;
	jStart = newJ - W/2.0 + 2;
	jEnd = newJ + W/2.0 - 2;

	printf("\ncenter i = %f, center j =%f\n", newI, newJ);
	printf("i Start = %d, i End = %d, j Start = %d, j End = %d\n", iStart, iEnd, jStart, jEnd);

	float oldj, oldi, diffj, diffi;
	int oldi0, oldj0, oldi1, oldj1, oldi2, oldj2, oldi3, oldj3;
	int R0, G0, B0, R1, G1, B1, R2, G2, B2, R3, G3, B3;
	for(int i=iStart; i<= iEnd; i++)
	{
		for(int j=jStart; j<= jEnd; j++)
		{
			oldj = j-(newJ-oldJ);
			oldi = i-(newI-oldI);
			diffj = oldj - ((int)oldj);
			diffi = oldi - ((int)oldi);

//			printf("i=%d, j=%d, oldi=%f, oldj=%f\n", i, j, oldi, oldj);

			oldi0 = oldi; oldj0 = oldj;
			oldi1 = oldi; oldj1 = oldj+1;
			oldi2 = oldi+1; oldj2 = oldj;
			oldi3 = oldi+1; oldj3 = oldj+1;

			R0 = input[oldi0*inputSizeW*3+oldj0*3+0];
			G0 = input[oldi0*inputSizeW*3+oldj0*3+1];
			B0 = input[oldi0*inputSizeW*3+oldj0*3+2];
			R1 = input[oldi1*inputSizeW*3+oldj1*3+0];
			G1 = input[oldi1*inputSizeW*3+oldj1*3+1];
			B1 = input[oldi1*inputSizeW*3+oldj1*3+2];
			R2 = input[oldi2*inputSizeW*3+oldj2*3+0];
			G2 = input[oldi2*inputSizeW*3+oldj2*3+1];
			B2 = input[oldi2*inputSizeW*3+oldj2*3+2];
			R3 = input[oldi3*inputSizeW*3+oldj3*3+0];
			G3 = input[oldi3*inputSizeW*3+oldj3*3+1];
			B3 = input[oldi3*inputSizeW*3+oldj3*3+2];

			output[i*outputSizeW*3+j*3+0] = (int)((1.0-diffj)*(1-diffi)*R0 + diffj*(1-diffi)*R1 + diffi*(1-diffj)*R2 + diffj*diffi*R3);
			output[i*outputSizeW*3+j*3+1] = (int)((1.0-diffj)*(1-diffi)*G0 + diffj*(1-diffi)*G1 + diffi*(1-diffj)*G2 + diffj*diffi*G3);
			output[i*outputSizeW*3+j*3+2] = (int)((1.0-diffj)*(1-diffi)*B0 + diffj*(1-diffi)*B1 + diffi*(1-diffj)*B2 + diffj*diffi*B3);
		}
	}

	return 0;
}

int combination(unsigned char *input1, unsigned char *input2, unsigned char *output, int SizeH, int SizeW)
{
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			if((input1[i*SizeW*3+j*3+0] != 255) || (input1[i*SizeW*3+j*3+1] != 255) || (input1[i*SizeW*3+j*3+2] != 255))
			{
				output[i*SizeW*3+j*3+0] = input1[i*SizeW*3+j*3+0];
				output[i*SizeW*3+j*3+1] = input1[i*SizeW*3+j*3+1];
				output[i*SizeW*3+j*3+2] = input1[i*SizeW*3+j*3+2];
			}
			else
			{
				output[i*SizeW*3+j*3+0] = input2[i*SizeW*3+j*3+0];
				output[i*SizeW*3+j*3+1] = input2[i*SizeW*3+j*3+1];
				output[i*SizeW*3+j*3+2] = input2[i*SizeW*3+j*3+2];
			}
		}
	}

	return 0;
}

int resizeimage(unsigned char *input, unsigned char *output, int inputH, int inputW, int outputH, int outputW)
{
	int extend = 3;
	int extendH = inputH + 2*extend;
	int extendW = inputW + 2*extend;
	unsigned char extendImage[extendH][extendW][3];
	ExtendColorI(input, inputH, inputW, &extendImage[0][0][0], extend);

	float midw, midh;
	float diffw, diffh;
	int midw0, midh0, midw1, midh1, midw2, midh2, midw3, midh3;
	int R0, G0, B0, R1, G1, B1, R2, G2, B2, R3, G3, B3;
	for(int i=0; i<outputH; i++)
	{
		for(int j=0; j<outputW; j++)
		{
			midh = (inputW-1)*(i-1)/(outputW-1) + 1 + extend;
			midw = (inputH-1)*(j-1)/(outputH-1) +  extend;
			diffw = midw - ((int)midw);
			diffh = midh - ((int)midh);

			midw0 = midw;
			midh0 = midh;
			midw1 = midw0 + 1;
			midh1 = midh0;
			midw2 = midw0;
			midh2 = midh0 + 1;
			midw3 = midw0 + 1;
			midh3 = midh0 + 1;

			R0 = extendImage[midh0][midw0][0];
			G0 = extendImage[midh0][midw0][1];
			B0 = extendImage[midh0][midw0][2];
			R1 = extendImage[midh1][midw1][0];
			G1 = extendImage[midh1][midw1][1];
			B1 = extendImage[midh1][midw1][2];
			R2 = extendImage[midh2][midw2][0];
			G2 = extendImage[midh2][midw2][1];
			B2 = extendImage[midh2][midw2][2];
			R3 = extendImage[midh3][midw3][0];
			G3 = extendImage[midh3][midw3][1];
			B3 = extendImage[midh3][midw3][2];

			output[i*outputW*3+j*3+0] = (1.0-diffw)*(1-diffh)*R0 + diffw*(1-diffh)*R1 + diffh*(1-diffw)*R2 + diffw*diffh*R3;
			output[i*outputW*3+j*3+1] = (1.0-diffw)*(1-diffh)*G0 + diffw*(1-diffh)*G1 + diffh*(1-diffw)*G2 + diffw*diffh*G3;
			output[i*outputW*3+j*3+2] = (1.0-diffw)*(1-diffh)*B0 + diffw*(1-diffh)*B1 + diffh*(1-diffw)*B2 + diffw*diffh*B3;
		}
	}

	return 0;
}
