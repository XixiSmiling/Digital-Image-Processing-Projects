
#include "Filters.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "iofile.h"
#include "histogram.h"

using namespace std;

double FunctionG(int sigma, int k1, int k2)
{
	double G = exp(-((k1*k1+k2*k2)/(2*sigma*sigma)))/(sigma*sqrt(2*3.14159));
	return G;
}

int GaussianLMFilter(int FilterSize, float sigma, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData)
{
	int extend = 30;
	int extendSizeH = SizeH+2*extend;
	int extendSizeW = SizeW+2*extend;
	int edge = (FilterSize-1)/2;

	for(int i=extend; (i < (SizeH + extend)); i++)
	{
		for(int j=extend; (j < (SizeW + extend)); j++)
		{
			double sum = 0.0, z = 0.0;
			double newx;

			for(int a = (i-edge); (a <= (i+edge)); a++)
			{
				for(int b = (j-edge); (b <= (j+edge)); b++)
				{
					double g = FunctionG(sigma, abs(i-a), abs(j-b));
					sum = sum + g*inputImageData[a*extendSizeH+b];
					z = z + g;
				}
			}

			newx =sum/z;

			outputImageData[(i-extend)*SizeW+(j-extend)] = (int) newx;

		}
	}

	return 0;
}

int UniformLMFilter(int FilterSize, float sigma, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData)
{
	int extend = 30;
	int extendSizeH = SizeH+2*extend;
	int extendSizeW = SizeW+2*extend;
	int edge = (FilterSize-1)/2;

	for(int i=extend; (i < (SizeH + extend)); i++)
	{
		for(int j=extend; (j < (SizeW + extend)); j++)
		{
			int sum = 0;
			int next = 0;
			int x = inputImageData[i*extendSizeW+j];
			float newx;

			for(int a = (i-edge); (a <= (i+edge)); a++)
			{
				for(int b = (j-edge); (b <= (j+edge)); b++)
				{
					next = inputImageData[a*extendSizeW+b];
					sum = sum + next;
				}
			}

			sum = sum - x;
			newx =sum/(FilterSize * FilterSize - 1.0);

			if((fabs(x-newx)) > sigma)
			{
				outputImageData[(i-extend)*SizeW+(j-extend)] = (int) newx;
			}
			else
			{
				outputImageData[(i-extend)*SizeW+(j-extend)] = x;
			}
		}
	}

	return 0;
}

int medianFilter(int FilterSize, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData)
{
	int extend = 30;
	int extendSizeH = SizeH+2*extend;
	int extendSizeW = SizeW+2*extend;
	int edge = (FilterSize-1)/2;

	for(int i=extend; (i < (SizeH + extend)); i++)
	{
		for(int j=extend; (j < (SizeW + extend)); j++)
		{
			int next = 0;
			int median = 0;
			vector<int> window;
			int x = inputImageData[i*extendSizeW+j];
			float newx;

			for(int a = (i-edge); (a <= (i+edge)); a++)
			{
				for(int b = (j-edge); (b <= (j+edge)); b++)
				{
					next = inputImageData[a*extendSizeW+b];
					window.push_back(next);
				}
			}

			sort(window.begin(), window.end());

			median = window.at((FilterSize*FilterSize+1)/2);
			outputImageData[(i-extend)*SizeW+(j-extend)] = median;

			window.clear();
		}
	}

	return 0;
}

// Bilateral Filter
double FunctionC(int i,int j,int w,int v, int sigmaC)
{
    return exp(-((w-i)*(w-i)+(v-j)*(v-j))/(2*sigmaC*sigmaC));
}
double FunctionS(int x,int y, int sigmaS)
{
    return exp(-((y-x)*(y-x))/(2*sigmaS*sigmaS));
}
int BilateralFilter(int FilterSize, int sigmaC, int sigmaS, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData)
{
	int extend = 30;
	int extendSizeH = SizeH+2*extend;
	int extendSizeW = SizeW+2*extend;
	int edge = (FilterSize-1)/2;

	for(int i=extend; (i < (SizeH + extend)); i++)
	{
		for(int j=extend; (j < (SizeW + extend)); j++)
		{
			int y = 0;
			double c, s, sum = 0, k = 0;
			int x = inputImageData[i*extendSizeW+j];
			float newx;

			for(int a = (i-edge); (a <= (i+edge)); a++)
			{
				for(int b = (j-edge); (b <= (j+edge)); b++)
				{
					y = inputImageData[a*extendSizeW+b];
					c = FunctionC(i, j, a, b, sigmaC);
					s = FunctionS(x, y, sigmaS);
					sum = (sum + y*c*s);
					k = (k + c*s);
				}
			}

			newx =sum/k;

		    outputImageData[(i-extend)*SizeW+(j-extend)] = (int) newx;

		}
	}

	return 0;
}

//  NLM
int NLMFilter(int FilterSize, int NeighborSize, int sigma, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData)
{
	int extend = 30;
	int extendSizeH = SizeH+2*extend;
	int extendSizeW = SizeW+2*extend;
	int edge = (FilterSize-1)/2;
	int neighbor = (NeighborSize - 1)/2;
	int h2 = 10*sigma;


	for(int i=extend; (i < (SizeH + extend)); i++)
	{
		for(int j=extend; (j < (SizeW + extend)); j++)
		{
			double z = 0.0, sum = 0.0;
			for(int p = (i - edge); (p <= (i + edge)); p++)
			{
				for(int q = (j - edge); (q < (j + edge)); q++)
				{
					double d = 0.0, sumd = 0.0, w = 0.0;

					for(int a = -neighbor; a <= neighbor; a++)
					{
						for(int b = -neighbor; b <= neighbor; b++)
						{
							double g = FunctionG(sigma, a, b);
							d = g*(inputImageData[(i-a)*extendSizeW + (j-b)] - inputImageData[(p-a)*extendSizeW + (q-b)])*(inputImageData[(i-a)*extendSizeW + (j-b)] - inputImageData[(p-a)*extendSizeW + (q-b)]);
							sumd = sumd + d;

						}
					}

					w = exp(-(sumd/(h2)));
					z = z + w;
					sum = sum + (w*inputImageData[p*extendSizeW+q]);

				}

			}
			outputImageData[(i-extend)*SizeW+(j-extend)] = sum/z;
		}
	}

	return 0;
}

double PSNR(const unsigned char *Orignal, const unsigned char *Filterd, int SizeH, int SizeW)
{
	double MSE;
	long sum = 0, next = 0;
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			next = (Filterd[i*SizeW+j]-Orignal[i*SizeW+j])*(Filterd[i*SizeW+j]-Orignal[i*SizeW+j]);
			sum = sum + next;
		}
	}
	MSE = sum/(SizeH*SizeW) + 0.0;

	int max = 255;
	double PSNR = 10*log10(max*max/MSE);
	cout<<"MSE = "<<MSE<<", PSNR = "<<PSNR<<endl;
	cout<<endl;

	return PSNR;

}
