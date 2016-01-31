#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "iofile.h"
#include "histogram.h"


// filename should be ***.dat
void Getcpf(int *hist, int *histcpf)
{
	 histcpf[0] = hist[0];
	 for(int i=1; i<256; i++)
	 {
	     histcpf[i] = histcpf[i-1] + hist[i];
	 }

	return;
}

void OutputHG(const char *filename, int *histogram, int Size)
{
	FILE *hist;
	hist = fopen(filename,"wb");

    for(int i=0; i<Size; i++)
    {
        fprintf(hist,"%d	", histogram[i]);
    }
    fclose(hist);

	return;
}


