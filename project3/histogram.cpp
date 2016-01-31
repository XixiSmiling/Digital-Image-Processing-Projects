#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "iofile.h"
#include "histogram.h"


// filename should be ***.txt
void Getcpf(float *hist, float *histcpf, int SizeHG, int SizeH, int SizeW)
{
	 histcpf[0] = hist[0];
	 for(int i=1; i<SizeHG; i++)
	 {
	     histcpf[i] = histcpf[i-1] + hist[i];
	 }

	return;
}

void OutputHG(const char *filename, float *histogram, int Size)
{
	FILE *hist;
	hist = fopen(filename,"wb");

    for(int i=0; i<Size; i++)
    {
        fprintf(hist,"%d	%f\n", i, histogram[i]);
    }
    fclose(hist);

	return;
}

void GetHG(int *inputData, float *hist, int SizeH, int SizeW, int SizeHG)
{
	for(int i=0; i<SizeH; i++)
		{
			for(int j=0; j<SizeW; j++)
			{
				if(inputData[i*SizeW+j] >= 0)
					hist[inputData[i*SizeW+j]] ++;
			}
		}
//	for(int i=0; i<SizeHG; i++)
//	{
//		hist[i] = hist[i] / (SizeH*SizeW);
//	}
}
