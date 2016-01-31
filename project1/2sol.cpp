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

int Q2a(void)
{
	// initialize data of original file "Girl.raw"
	string filename1 = "Girl.raw";
	int SizeW1 = 256;
	int SizeH1 = 256;
	int BytesPerPixel1 = 3;
	unsigned char ImageData1[SizeH1][SizeW1][BytesPerPixel1];

	// initialize data of original file "desk.raw"
	string filename2 = "desk.raw";
	int SizeW2 = 400;
	int SizeH2 = 300;
	int BytesPerPixel2 = 3;
	unsigned char ImageData2[SizeH2][SizeW2][BytesPerPixel2];

	// initialize data of Histogram Equalized Girl
	string newfilename1a = "2a_1a Girl_ehm_ans.raw";
	unsigned char newImageData1a[SizeH1][SizeW1][BytesPerPixel1];
	string newfilename1b = "2a_1b Girl_echm_ans.raw";
	unsigned char newImageData1b[SizeH1][SizeW1][BytesPerPixel1];

	// initialize data of Histogram Equalized desk
	string newfilename2a = "2a_2a desk_ehm_ans.raw";
	unsigned char newImageData2a[SizeH2][SizeW2][BytesPerPixel2];
	string newfilename2b = "2a_2b desk_echm_ans.raw";
	unsigned char newImageData2b[SizeH2][SizeW2][BytesPerPixel2];

	// read "Girl.raw" and "desk.raw"
	readimage(filename1.c_str(), &ImageData1[0][0][0], SizeH1, SizeW1, BytesPerPixel1);
	readimage(filename2.c_str(), &ImageData2[0][0][0], SizeH2, SizeW2, BytesPerPixel2);

	// Part 1: the equalized histogram method
	int N1 = SizeW1 * SizeH1;  // the number of pixels of "Girl.raw"
	int N2 = SizeW2 * SizeH2;  // the number of pixels of "desk.raw"
	int n1 = (N1/256);
	int n2 = (N2/256) + 1;

	int hg1sb[N1], hg2sb[N2];
	memset(hg1sb, 0, N1*sizeof(int));
	memset(hg2sb, 0, N2*sizeof(int));
	for(int i=0; i<N1; i++)
	{
		hg1sb[i] = i/n1;
	}
	for(int i=0; i<N2; i++)
	{
		hg2sb[i] = i/n2;
	}

	int R1 = 0, G1 = 0, B1 = 0;
	for(int d = 0; d< 256; d++)
	{
		for(int i=0; i<SizeH1; i++)
		{
			for(int j=0; j<SizeW1; j++)
			{
				if(ImageData1[i][j][0] == d)
				{
					newImageData1a[i][j][0] = hg1sb[R1];
					R1++;
				}
				if(ImageData1[i][j][1] == d)
				{
					newImageData1a[i][j][1] = hg1sb[G1];
					G1++;
				}
				if(ImageData1[i][j][2] == d)
				{
					newImageData1a[i][j][2] = hg1sb[B1];
					B1++;
				}
			}
		}
	}

	int R2 = 0, G2 = 0, B2 = 0;
	for(int d = 0; d< 256; d++)
	{
		for(int i=0; i<SizeH2; i++)
		{
			for(int j=0; j<SizeW2; j++)
			{
				if(ImageData2[i][j][0] == d)
				{
					newImageData2a[i][j][0] = hg2sb[R2];
					R2++;
				}
				if(ImageData2[i][j][1] == d)
				{
					newImageData2a[i][j][1] = hg2sb[G2];
					G2++;
				}
				if(ImageData2[i][j][2] == d)
				{
					newImageData2a[i][j][2] = hg2sb[B2];
					B2++;
				}
			}
		}
	}

	// Output "2a_1a Girl_ehm.raw" and "2a_2a desk_ehm.raw"
	writeimage(newfilename1a.c_str(), &newImageData1a[0][0][0], SizeH1, SizeW1, BytesPerPixel1);
	writeimage(newfilename2a.c_str(), &newImageData2a[0][0][0], SizeH2, SizeW2, BytesPerPixel2);

	//Part 2: using the equalized "cumulative" histogram method
	int hgR1[256], hgG1[256], hgB1[256];  // histograms of "Girl.raw"
	int hgR1cpf[256], hgG1cpf[256], hgB1cpf[256];  // cumulative histograms of "Girl.raw"
	int tranR1[256], tranG1[256], tranB1[256];
	int hgR2[256], hgG2[256], hgB2[256];  // histograms of "desk.raw"
	int hgR2cpf[256], hgG2cpf[256], hgB2cpf[256];
	int tranR2[256], tranG2[256], tranB2[256];
	memset(hgR1, 0, 256*sizeof(int));
	memset(hgG1, 0, 256*sizeof(int));
	memset(hgB1, 0, 256*sizeof(int));
	memset(hgR1cpf, 0, 256*sizeof(int));
	memset(hgG1cpf, 0, 256*sizeof(int));
	memset(hgB1cpf, 0, 256*sizeof(int));
	memset(tranR1, 0, 256*sizeof(int));
	memset(tranG1, 0, 256*sizeof(int));
	memset(tranB1, 0, 256*sizeof(int));
	memset(hgR2, 0, 256*sizeof(int));
	memset(hgG2, 0, 256*sizeof(int));
	memset(hgB2, 0, 256*sizeof(int));
	memset(hgR2cpf, 0, 256*sizeof(int));
	memset(hgG2cpf, 0, 256*sizeof(int));
	memset(hgB2cpf, 0, 256*sizeof(int));
	memset(tranR2, 0, 256*sizeof(int));
	memset(tranG2, 0, 256*sizeof(int));
	memset(tranB2, 0, 256*sizeof(int));

	for(int i=0; i<SizeH1; i++)
	{
		for(int j=0; j<SizeW1; j++)
		{
			hgR1[ImageData1[i][j][0]] ++;
			hgG1[ImageData1[i][j][1]] ++;
			hgB1[ImageData1[i][j][2]] ++;
		}
	}
	for(int i=0; i<SizeH2; i++)
	{
		for(int j=0; j<SizeW2; j++)
		{
			hgR2[ImageData2[i][j][0]] ++;
			hgG2[ImageData2[i][j][1]] ++;
			hgB2[ImageData2[i][j][2]] ++;
		}
	}

	Getcpf(hgR1, hgR1cpf);
	Getcpf(hgG1, hgG1cpf);
	Getcpf(hgB1, hgB1cpf);
	Getcpf(hgR2, hgR2cpf);
	Getcpf(hgG2, hgG2cpf);
	Getcpf(hgB2, hgB2cpf);

    for(int i=0; i<256; i++)
    {
        tranR1[i] = 255*hgR1cpf[i]/N1;
        tranG1[i] = 255*hgG1cpf[i]/N1;
        tranB1[i] = 255*hgB1cpf[i]/N1;
        tranR2[i] = 255*hgR2cpf[i]/N2;
        tranG2[i] = 255*hgG2cpf[i]/N2;
        tranB2[i] = 255*hgB2cpf[i]/N2;

    }

    for(int i=0; i<SizeH1; i++)
    {
        for(int j=0; j<SizeW1; j++)
        {
            newImageData1b[i][j][0] = tranR1[ImageData1[i][j][0]];
            newImageData1b[i][j][1] = tranG1[ImageData1[i][j][1]];
            newImageData1b[i][j][2] = tranB1[ImageData1[i][j][2]];
        }
    }

    for(int i=0; i<SizeH2; i++)
    {
        for(int j=0; j<SizeW2; j++)
        {
            newImageData2b[i][j][0] = tranR2[ImageData2[i][j][0]];
            newImageData2b[i][j][1] = tranG2[ImageData2[i][j][1]];
            newImageData2b[i][j][2] = tranB2[ImageData2[i][j][2]];
        }
    }


	// Output "2a_1b Girl_echm.raw" and "2a_2b desk_echm.raw"
	writeimage(newfilename1b.c_str(), &newImageData1b[0][0][0], SizeH1, SizeW1, BytesPerPixel1);
	writeimage(newfilename2b.c_str(), &newImageData2b[0][0][0], SizeH2, SizeW2, BytesPerPixel2);

	// histograms of images for report
	OutputHG("P2 Girl_R_hist.dat", hgR1, 256);
	OutputHG("P2 Girl_G_hist.dat", hgG1, 256);
	OutputHG("P2 Girl_B_hist.dat", hgB1, 256);
	OutputHG("P2 desk_R_hist.dat", hgR2, 256);
	OutputHG("P2 desk_G_hist.dat", hgG2, 256);
	OutputHG("P2 desk_B_hist.dat", hgB2, 256);
	OutputHG("P2 Girl_R_c_hist.dat", hgR1cpf, 256);
	OutputHG("P2 Girl_G_c_hist.dat", hgG1cpf, 256);
	OutputHG("P2 Girl_B_c_hist.dat", hgB1cpf, 256);
	OutputHG("P2 desk_R_c_hist.dat", hgR2cpf, 256);
	OutputHG("P2 desk_G_c_hist.dat", hgG2cpf, 256);
	OutputHG("P2 desk_B_c_hist.dat", hgB2cpf, 256);

	int hgR1a[256], hgG1a[256], hgB1a[256];
	int hgR1b[256], hgG1b[256], hgB1b[256];
	int hgR2a[256], hgG2a[256], hgB2a[256];
	int hgR2b[256], hgG2b[256], hgB2b[256];
	int hgR1bcpf[256], hgG1bcpf[256], hgB1bcpf[256];
	int hgR2bcpf[256], hgG2bcpf[256], hgB2bcpf[256];
	memset(hgR1a, 0, 256*sizeof(int));
	memset(hgG1a, 0, 256*sizeof(int));
	memset(hgB1a, 0, 256*sizeof(int));
	memset(hgR1b, 0, 256*sizeof(int));
	memset(hgG1b, 0, 256*sizeof(int));
	memset(hgB1b, 0, 256*sizeof(int));
	memset(hgR2a, 0, 256*sizeof(int));
	memset(hgG2a, 0, 256*sizeof(int));
	memset(hgB2a, 0, 256*sizeof(int));
	memset(hgR2b, 0, 256*sizeof(int));
	memset(hgG2b, 0, 256*sizeof(int));
	memset(hgB2b, 0, 256*sizeof(int));
	memset(hgR1bcpf, 0, 256*sizeof(int));
	memset(hgG1bcpf, 0, 256*sizeof(int));
	memset(hgB1bcpf, 0, 256*sizeof(int));
	memset(hgR2bcpf, 0, 256*sizeof(int));
	memset(hgG2bcpf, 0, 256*sizeof(int));
	memset(hgB2bcpf, 0, 256*sizeof(int));


	for(int i=0; i<SizeH1; i++)
	{
		for(int j=0; j<SizeW1; j++)
		{
			hgR1a[newImageData1a[i][j][0]] ++;
			hgG1a[newImageData1a[i][j][1]] ++;
			hgB1a[newImageData1a[i][j][2]] ++;
			hgR1b[newImageData1b[i][j][0]] ++;
			hgG1b[newImageData1b[i][j][1]] ++;
			hgB1b[newImageData1b[i][j][2]] ++;
		}
	}

	for(int i=0; i<SizeH2; i++)
	{
		for(int j=0; j<SizeW2; j++)
		{
			hgR2a[newImageData2a[i][j][0]] ++;
			hgG2a[newImageData2a[i][j][1]] ++;
			hgB2a[newImageData2a[i][j][2]] ++;
			hgR2b[newImageData2b[i][j][0]] ++;
			hgG2b[newImageData2b[i][j][1]] ++;
			hgB2b[newImageData2b[i][j][2]] ++;
		}
	}

	Getcpf(hgR1b, hgR1bcpf);
	Getcpf(hgG1b, hgG1bcpf);
	Getcpf(hgB1b, hgB1bcpf);
	Getcpf(hgR2b, hgR2bcpf);
	Getcpf(hgG2b, hgG2bcpf);
	Getcpf(hgB2b, hgB2bcpf);

	OutputHG("P2 Girl_ehm_R_hist.dat", hgR1a, 256);
	OutputHG("P2 Girl_ehm_G_hist.dat", hgG1a, 256);
	OutputHG("P2 Girl_ehm_B_hist.dat", hgB1a, 256);
	OutputHG("P2 Girl_echm_R_hist.dat", hgR1b, 256);
	OutputHG("P2 Girl_echm_G_hist.dat", hgG1b, 256);
	OutputHG("P2 Girl_echm_B_hist.dat", hgB1b, 256);
	OutputHG("P2 desk_ehm_R_hist.dat", hgR2a, 256);
	OutputHG("P2 desk_ehm_G_hist.dat", hgG2a, 256);
	OutputHG("P2 desk_ehm_B_hist.dat", hgB2a, 256);
	OutputHG("P2 desk_echm_R_hist.dat", hgR2b, 256);
	OutputHG("P2 desk_echm_G_hist.dat", hgG2b, 256);
	OutputHG("P2 desk_echm_B_hist.dat", hgB2b, 256);
	OutputHG("P2 Girl_echm_R_c_hist.dat", hgR1bcpf, 256);
	OutputHG("P2 Girl_echm_G_c_hist.dat", hgG1bcpf, 256);
	OutputHG("P2 Girl_echm_B_c_hist.dat", hgB1bcpf, 256);
	OutputHG("P2 desk_echm_R_c_hist.dat", hgR2bcpf, 256);
	OutputHG("P2 desk_echm_G_c_hist.dat", hgG2bcpf, 256);
	OutputHG("P2 desk_echm_B_c_hist.dat", hgB2bcpf, 256);

	return 0;
}

int Q2b(void)
{
	// initialize data of original file "scene2_256.raw"
	string filename1 = "scene2_256.raw";
	int SizeW1 = 360;
	int SizeH1 = 360;
	int BytesPerPixel1 = 3;
	unsigned char ImageData1[SizeH1][SizeW1][BytesPerPixel1];

	// initialize data of original file "Trojan_256.raw"
	string filename2 = "Trojan_256.raw";
	int SizeW2 = 384;
	int SizeH2 = 384;
	int BytesPerPixel2 = 3;
	unsigned char ImageData2[SizeH2][SizeW2][BytesPerPixel2];

	// initialize data of "scene2_oil.raw" and "Trojan_oil.raw"
	string newfilename1a = "2b scene2_oil_5_ans.raw";
//	string newfilename1b = "2b scene2_oil_19_ans.raw";
//	string newfilename1c = "2b scene2_oil_25_ans.raw";
//	string newfilename1d = "2b scene2_oil_43_ans.raw";
	unsigned char newImageData1[SizeH1][SizeW1][BytesPerPixel1];
	string newfilename2a = "2b Trojan_oil_5_ans.raw";
//	string newfilename2b = "2b Trojan_oil_19_ans.raw";
//	string newfilename2c = "2b Trojan_oil_25_ans.raw";
//	string newfilename2d = "2b Trojan_oil_43_ans.raw";
	unsigned char newImageData2[SizeH2][SizeW2][BytesPerPixel2];

	// read
	readimage(filename1.c_str(), &ImageData1[0][0][0], SizeH1, SizeW1, BytesPerPixel1);
	readimage(filename2.c_str(), &ImageData2[0][0][0], SizeH2, SizeW2, BytesPerPixel2);

	// creating oil painting effect
	// ****** extend image begin ******
	int extend = 10;
	int extendSizeH1 = SizeH1+2*extend;
	int extendSizeW1 = SizeW1+2*extend;
	unsigned char extendImageData1[extendSizeH1][extendSizeW1][BytesPerPixel1];

	// center
	for(int a=0; a<SizeH1; a++)
	{
		for(int b=0; b<SizeW1; b++)
		{
			int i = a + extend;
			int j = b + extend;

			extendImageData1[i][j][0] = ImageData1[a][b][0];
			extendImageData1[i][j][1] = ImageData1[a][b][1];
			extendImageData1[i][j][2] = ImageData1[a][b][2];
		}
	}

	// right and left
	for(int a = 0; a<SizeH1; a++)
	{
		for(int b = extend; b > 0; b--)
		{
			int i = a + extend;
			int j = extend - b;

			extendImageData1[i][j][0] = ImageData1[a][b][0];
			extendImageData1[i][j][1] = ImageData1[a][b][1];
			extendImageData1[i][j][2] = ImageData1[a][b][2];
		}
	}
	for(int a = 0; a < SizeH1; a++)
	{
		for(int b = (SizeW1-1-extend); (b < (SizeW1-1)); b++)
		{
			int i = a + extend;
			int j = 2*SizeW1 - 2 + extend - b;

			extendImageData1[i][j][0] = ImageData1[a][b][0];
			extendImageData1[i][j][1] = ImageData1[a][b][1];
			extendImageData1[i][j][2] = ImageData1[a][b][2];
		}
	}

	// top and bottom
	for(int c=extend; (c < (2*extend + 1)); c++)
	{
		for(int d=0; d < extendSizeW1; d++)
		{
			int i = 2*extend - c;
			int j = d;

			extendImageData1[i][j][0] = extendImageData1[c][d][0];
			extendImageData1[i][j][1] = extendImageData1[c][d][1];
			extendImageData1[i][j][2] = extendImageData1[c][d][2];
		}
	}
	for(int c=SizeH1-1; (c < (SizeH1-1+extend)); c++)
	{
		for(int d=0; d < extendSizeW1; d++)
		{
			int i = 2*SizeH1 - 2 + 2*extend - c;
			int j = d;

			extendImageData1[i][j][0] = extendImageData1[c][d][0];
			extendImageData1[i][j][1] = extendImageData1[c][d][1];
			extendImageData1[i][j][2] = extendImageData1[c][d][2];
		}
	}

	//********  extend image end **********
	// ****** extend image begin ******
	int extendSizeH2 = SizeH2+2*extend, extendSizeW2 = SizeW2+2*extend;
	unsigned char extendImageData2[extendSizeH2][extendSizeW2][BytesPerPixel2];

	// center
	for(int a=0; a < SizeH2; a++)
	{
		for(int b=0; b < SizeW2; b++)
		{
			int i = a + extend;
			int j = b + extend;

			extendImageData2[i][j][0] = ImageData2[a][b][0];
			extendImageData2[i][j][1] = ImageData2[a][b][1];
			extendImageData2[i][j][2] = ImageData2[a][b][2];
		}
	}

	// right and left
	for(int a = 0; a<SizeH2; a++)
	{
		for(int b = extend; b > 0; b--)
		{
			int i = a + extend;
			int j = extend - b;

			extendImageData2[i][j][0] = ImageData2[a][b][0];
			extendImageData2[i][j][1] = ImageData2[a][b][1];
			extendImageData2[i][j][2] = ImageData2[a][b][2];
		}
	}
	for(int a = 0; a < SizeH2; a++)
	{
		for(int b = (SizeW2-1-extend); (b < (SizeW2-1)); b++)
		{
			int i = a + extend;
			int j = 2*SizeW2 - 2 + extend - b;

			extendImageData2[i][j][0] = ImageData2[a][b][0];
			extendImageData2[i][j][1] = ImageData2[a][b][1];
			extendImageData2[i][j][2] = ImageData2[a][b][2];
		}
	}

	// top and bottom
	for(int c=extend; (c < (2*extend + 1)); c++)
	{
		for(int d=0; d < extendSizeW2; d++)
		{
			int i = 2*extend - c;
			int j = d;

			extendImageData2[i][j][0] = extendImageData2[c][d][0];
			extendImageData2[i][j][1] = extendImageData2[c][d][1];
			extendImageData2[i][j][2] = extendImageData2[c][d][2];
		}
	}
	for(int c=SizeH2-1; (c < (SizeH2-1+extend)); c++)
	{
		for(int d=0; d < extendSizeW2; d++)
		{
			int i = 2*SizeH2 - 2 + 2*extend - c;
			int j = d;

			extendImageData2[i][j][0] = extendImageData2[c][d][0];
			extendImageData2[i][j][1] = extendImageData2[c][d][1];
			extendImageData2[i][j][2] = extendImageData2[c][d][2];
		}
	}

	//********  extend image end **********
//	writeimage("test1.raw", &extendImageData1[0][0][0], extendSizeH1, extendSizeW1, BytesPerPixel1);
//	writeimage("test2.raw", &extendImageData2[0][0][0], extendSizeH2, extendSizeW2, BytesPerPixel2);

	// ******Part 1: scene2_oil*******
		int windowsize=5;
		int edge = (windowsize-1)/2;
		vector< vector<int> > window;
		for(int i=extend; (i<(SizeH1+extend)); i++)
		{
			for(int j=extend; (j<(SizeW1+extend)); j++)
			{
				for(int a = (i-edge); (a<=(i+edge)); a++)
				{
					for(int b=(j-edge); (b<=(j+edge)); b++)
					{
						vector<int> tmp;
						for(int d=0; d<3; d++)
						{
							tmp.push_back(extendImageData1[a][b][d]);
						}
						window.push_back(tmp);
						tmp.clear();
					}
				}

				vector<int> max;
				max = window[0];
//				cout<<i<<"  "<<window[0][0]<<"  "<<window[0][1]<<"  "<<window[0][2]<<endl;
//				cout<<i<<"  "<<max[0]<<"  "<<max[1]<<"  "<<max[2]<<endl;
				int histwindow[(windowsize*windowsize)];
				memset(histwindow, 0, windowsize*windowsize*sizeof(int));
				for(int w = 0; (w < (windowsize*windowsize)); w++)
				{
					for(int v = 0; (v < (windowsize*windowsize)); v++)
					{
						if(window[w] == window[v])
						{
							histwindow[w]++;
						}

					}
				}
				for(int p = 0; p < (windowsize*windowsize -1) ; p++)
				{
					if(histwindow[p+1] >= histwindow[p])
					{
						max = window[p+1];
					}
				}

				newImageData1[i-extend][j-extend][0] = max[0];
				newImageData1[i-extend][j-extend][1] = max[1];
				newImageData1[i-extend][j-extend][2] = max[2];

				window.clear();

			}
		}

	// Output "scene2_oil.raw"
		if(windowsize == 5)
		{
			writeimage(newfilename1a.c_str(), &newImageData1[0][0][0], SizeH1, SizeW1, BytesPerPixel1);
		}
//		else if(windowsize == 19)
//		{
//			writeimage(newfilename1b.c_str(), &newImageData1[0][0][0], SizeH1, SizeW1, BytesPerPixel1);
//		}
//		else if(windowsize == 25)
//		{
//			writeimage(newfilename1c.c_str(), &newImageData1[0][0][0], SizeH1, SizeW1, BytesPerPixel1);
//		}
//		else if(windowsize == 43)
//		{
//			writeimage(newfilename1d.c_str(), &newImageData1[0][0][0], SizeH1, SizeW1, BytesPerPixel1);
//		}
		else
		{
			printf("Problem 2b_scene_oil: error!\n");
		}


	// ******* Part 2: Trojan_oil ****
//		vector< vector<int> > window;
		for(int i=extend; (i<(SizeH2+extend)); i++)
		{
			for(int j=extend; (j<(SizeW2+extend)); j++)
			{
				for(int a = (i-edge); (a<=(i+edge)); a++)
				{
					for(int b=(j-edge); (b<=(j+edge)); b++)
					{
						vector<int> tmp;
						for(int d=0; d<3; d++)
						{
							tmp.push_back(extendImageData2[a][b][d]);
						}
						window.push_back(tmp);
						tmp.clear();
					}
				}

				vector<int> max;
				max = window[0];
//				cout<<i<<"  "<<window[0][0]<<"  "<<window[0][1]<<"  "<<window[0][2]<<endl;
//				cout<<i<<"  "<<max[0]<<"  "<<max[1]<<"  "<<max[2]<<endl;
				int histwindow[(windowsize*windowsize)];
				memset(histwindow, 0, windowsize*windowsize*sizeof(int));
				for(int w = 0; (w < (windowsize*windowsize)); w++)
				{
					for(int v = 0; (v < (windowsize*windowsize)); v++)
					{
						if(window[w] == window[v])
						{
							histwindow[w]++;
						}

					}
				}
				for(int p = 0; p < (windowsize*windowsize -1) ; p++)
				{
					if(histwindow[p+1] >= histwindow[p])
					{
						max = window[p+1];
					}
				}

				newImageData2[i-extend][j-extend][0] = max[0];
				newImageData2[i-extend][j-extend][1] = max[1];
				newImageData2[i-extend][j-extend][2] = max[2];

				window.clear();

			}
		}

	// Output "Trojan_oil.raw"
		if(windowsize == 5)
		{
			writeimage(newfilename2a.c_str(), &newImageData2[0][0][0], SizeH2, SizeW2, BytesPerPixel2);
		}
//		else if(windowsize == 19)
//		{
//			writeimage(newfilename2b.c_str(), &newImageData2[0][0][0], SizeH2, SizeW2, BytesPerPixel2);
//		}
//		else if(windowsize == 25)
//		{
//			writeimage(newfilename2c.c_str(), &newImageData2[0][0][0], SizeH2, SizeW2, BytesPerPixel2);
//		}
//		else if(windowsize == 43)
//		{
//			writeimage(newfilename2d.c_str(), &newImageData2[0][0][0], SizeH2, SizeW2, BytesPerPixel2);
//		}
		else
		{
			printf("Problem 2b_Trojan_oil: error!\n");
		}


	return 0;
}

int Q2c(void)
{
	// initialize data of "chat.raw"
	string filename = "chat.raw";
	int SizeW = 481;
	int SizeH = 321;
	int BytesPerPixel = 3;
	unsigned char ImageData[SizeH][SizeW][BytesPerPixel];

	// initialize data of "chat_film.raw"
	string newfilename = "2c chat_film_ans.raw";
	int newSizeW = 481;
	int newSizeH = 321;
	int newBytesPerPixel = 3;
	unsigned char newImageData1[newSizeH][newSizeW][newBytesPerPixel];
	unsigned char newImageData2[newSizeH][newSizeW][newBytesPerPixel];

	// read "chat.raw"
	readimage(filename.c_str(), &ImageData[0][0][0], SizeH, SizeW, BytesPerPixel);

	// Create film special effect
	int R, G, B;
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			R = (255 - ImageData[i][j][0]);
			G = (255 - ImageData[i][j][1]);
			B = (255 - ImageData[i][j][2]);
			newImageData1[i][j][0] = (95 + 160*R/255);
			newImageData1[i][j][1] = (31 + 128*G/255);
			newImageData1[i][j][2] = (31 + 128*B/255);
		}
	}

	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			newImageData2[i][(480-j)][0] = newImageData1[i][j][0];
			newImageData2[i][(480-j)][1] = newImageData1[i][j][1];
			newImageData2[i][(480-j)][2] = newImageData1[i][j][2];
		}
	}


	// output "chat_film.raw"
	writeimage(newfilename.c_str(), &newImageData2[0][0][0], newSizeH, newSizeW, newBytesPerPixel);


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
			newhgR[newImageData2[i][j][0]] ++;
			newhgG[newImageData2[i][j][1]] ++;
			newhgB[newImageData2[i][j][2]] ++;
		}
	}

	OutputHG("P2 chat_R_hist.dat", hgR, 256);
	OutputHG("P2 chat_G_hist.dat", hgG, 256);
	OutputHG("P2 chat_B_hist.dat", hgB, 256);
	OutputHG("P2 chat_film_R_hist.dat", newhgR, 256);
	OutputHG("P2 chat_film_G_hist.dat", newhgG, 256);
	OutputHG("P2 chat_film_B_hist.dat", newhgB, 256);

	return 0;
}


int main(void)
{
	Q2a();
	Q2b();
	Q2c();

	return 0;
}
