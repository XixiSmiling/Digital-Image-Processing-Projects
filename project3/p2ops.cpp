#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "iofile.h"
#include "p1ops.h"



//int Segment(int SizeH, int SizeW, const unsigned char *inputdata, int *storeridgelen)
//{
//	int extend = 1;
//	unsigned char extendinputdata[SizeH+2*extend][SizeW+2*extend];
//	unsigned char checkdata[SizeH+2*extend][SizeW+2*extend];
//	memset(checkdata, 0, (SizeH+2*extend)*(SizeW+2*extend)*(sizeof(unsigned char)));
//	extendonechannel2(inputdata, SizeH, SizeW, &extendinputdata[0][0], extend);
//
//	for(int i=0; i<(SizeH+2*extend); i++)
//	{
//		for(int j=0; j<(SizeW+2*extend); j++)
//		{
//			if((i==0) || (j==0) || (i==(SizeH+2*extend-1)) || (j==(SizeW+2*extend-1)))
//			{
//				extendinputdata[i][j] = 0;
//			}
//		}
//	}
//
//	int ridgenum = 0;
//    int current_i,current_j;
//    int neighbori[]={-1,-1,-1,0,0,1,1,1};
//    int neighborj[]={-1,0,1,-1,1,-1,0,1};
//
//	for(int i=extend; i<=SizeH; i++)
//	{
//		for(int j=extend; j<=SizeW; j++)
//		{
//			if (extendinputdata[i][j] == 255 && checkdata[i][j] == 0)
//			{
//                current_i = i;
//                current_j = j;
//
//				int partnum = 0;
//                for(int k=0; k<8; k++)
//                 {
//                 	if(extendinputdata[(current_i+neighbori[k])][current_j+neighborj[k]]==255 && checkdata[(current_i+neighbori[k])][current_j+neighborj[k]]==0)
//                 	{
//                 		partnum++;
//                 	}
//                 }
//
//                if(partnum == 0)
//                {
//                	checkdata[i][j] = 255;
////    				storeridgelen[ridgenum] = 1;
////    				ridgenum++;
//                }
//
//                if(partnum == 1 || partnum == 2)
//                {
////    				int newridgelen[2];
////    				memset(newridgelen, 0, 2*(sizeof(int)));
//    				checkdata[i][j] = 255;
//    				int part = 0;
////    				newridgelen[part]++;
//
//                    for(int k=0;k<8;k++)
//                    {
//                        if(extendinputdata[(current_i+neighbori[k])][current_j+neighborj[k]]==255 && checkdata[(current_i+neighbori[k])][current_j+neighborj[k]]==0)
//                        {
//                            current_i = current_i+neighbori[k];
//                            current_j = current_j+neighborj[k];
//                            checkdata[current_i][current_j] = 255;
////                          newridgelen[part]++;
//
//                            int next = 1;
//                            while(next > 0)
//                            {
//                            	next = 0;
//                				int subpartnum = 0;
//                            	for(int m=0; m<8; m++)
//                            	{
//                            		if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
//                            		{
//                            			subpartnum++;
//                            		}
//                            	}
//
//                            	if(subpartnum == 1)
//                            	{
//                                	for(int m=0; m<8; m++)
//                                	{
//                                		if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
//                                		{
//                                        	current_i = current_i+neighbori[m];
//                                        	current_j = current_j+neighborj[m];
//                                        	checkdata[current_i][current_j] = 255;
////                                        	newridgelen[part]++;
//                                        	next = 1;
//                                        	break;
//                                		}
//                                	}
//                            	}
//                            	if(subpartnum == 2)
//                            	{
//                            		next = 1;
////                                	int partridgelen[2];
////                                	memset(partridgelen, 0, 2*(sizeof(int)));
//                                	int subpart = 0;
//                                	int temp_i = current_i;
//                                	int temp_j = current_j;
//
//                                   	for(int m=0; m<8; m++)
//                                    {
//                                   		if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
//                                   		{
//                                           	current_i = current_i+neighbori[m];
//                                           	current_j = current_j+neighborj[m];
//                                          	checkdata[current_i][current_j] = 255;
////                                          partridgelen[subpart]++;
//
//                                           	int go = 1;
//                                           	while(go > 0)
//                                           	{
//                                           		go = 0;
//                                           		for(int r=0; r<8; r++)
//                                           		{
//                                           			if(extendinputdata[current_i+neighbori[r]][current_j+neighborj[r]] == 255 && checkdata[current_i+neighbori[r]][current_j+neighborj[r]] == 0)
//                                           			{
//                                           				current_i = current_i+neighbori[r];
//                                           				current_j = current_j+neighborj[r];
//                                           				checkdata[current_i][current_j] = 255;
////                                           			partridgelen[subpart]++;
//                                           				go = 1;
//                                           				break;
//                                           			}
//                                           		}
//                                           	}
//
//                                           	subpart++;
//
//                                   		}
//
//                                   		current_i = temp_i;
//                                   		current_j = temp_j;
//                                   	}
//
////                                   	if(partridgelen[0] >= partridgelen[1])
////                                   	{
////                                   		newridgelen[part] = newridgelen[part] + partridgelen[0];
////                                   	}
////                                   	else
////                                   	{
////                                   		newridgelen[part] = newridgelen[part] + partridgelen[1];
////                                   	}
//
//
//                            	}
//
//                            }
//
//                             part++;
//                        }
//
//                        current_i = i;
//                        current_j = j;
//                    }
//
////    				storeridgelen[ridgenum] = newridgelen[0] + newridgelen[1];
////    				ridgenum++;
//                }
//                if(partnum == 3)
//                {
////    				int newridgelen[3];
////    				memset(newridgelen, 0, 3*(sizeof(int)));
//    				checkdata[i][j] = 255;
//    				int part = 0;
////    				newridgelen[part]++;
//
//                    for(int k=0;k<8;k++)
//                    {
//                        if(extendinputdata[(current_i+neighbori[k])][current_j+neighborj[k]]==255 && checkdata[(current_i+neighbori[k])][current_j+neighborj[k]]==0)
//                        {
//                        	int partridgelen = 1;
//                            current_i = current_i+neighbori[k];
//                            current_j = current_j+neighborj[k];
//                        	checkdata[current_i][current_j]= 255;
//
//    //                      int partridgelen[3];
//    //                      memset(partridgelen, 0, 3*(sizeof(int)));
//
//                        	int next = 1;
//                            while(next >0)
//                            {
//                            	next = 0;
//                                for(int m=0; m<8; m++)
//                                {
//                                    if(extendinputdata[(current_i+neighbori[m])][current_j+neighborj[m]]==255 && checkdata[(current_i+neighbori[m])][current_j+neighborj[m]]==0)
//                                    {
//                                    	current_i = current_i+neighbori[m];
//                                    	current_j = current_j+neighborj[m];
//                                    	checkdata[current_i][current_j] = 255;
//                                    	partridgelen++;
//                                    	next = 1;
//                                    	break;
//                                    }
//
//                                 }
//
//                             }
//
////                             newridgelen[part] = newridgelen[part] + partridgelen;
//                             part++;
//                        }
//
//                        current_i = i;
//                        current_j = j;
//
//                    }
//
////    				vector<int> ridgelen;
////                    for(int p=0; p<3; p++)
////                    {
////    					ridgelen.push_back(newridgelen[p]);
////                    }
////        			sort(ridgelen.begin(), ridgelen.end());
////
////    				storeridgelen[ridgenum] = ridgelen.at(2) + ridgelen.at(1);
////    				ridgenum++;
////    				ridgelen.clear();
//
//                }
//                if(partnum > 3)
//                {
//                	printf("partnum > 3, there must be something wrong\n");
//                }
//
////                printf("ridgenum = %d, len = %d\n", (ridgenum-1), storeridgelen[ridgenum-1]);
//
//			}
//
//		}
//	}
//
//	int maxlen = 0;
//	for(int w=0; w<ridgenum; w++)
//	{
//		if(storeridgelen[w] > maxlen)
//		{
//			maxlen = storeridgelen[w];
////			printf("enter here\n");
//		}
//	}
//
//	writeimage("2b checkdata.raw", &checkdata[0][0], (SizeH+2*extend), (SizeW+2*extend), 1);
//
//	return maxlen;
//
//}

//int LabelLinked(int SizeH, int SizeW, unsigned char *inputdata, int *labeleddata)
//{
//	int linked[SizeH][SizeH];
//	int linkednum[SizeH];
//	int nextlabel = 1;
//
//	for(int i=1; i<SizeH; i++)
//	{
//		for(int j=1; j<SizeW; j++)
//		{
//			if(inputdata[i*SizeW+j] == 255)
//			{
//				if(inputdata[(i-1)*SizeW+(j-1)] == 0 && inputdata[(i-1)*SizeW+j] == 0 &&
//						inputdata[(i-1)*SizeW+(j+1)] == 0 && inputdata[i*SizeW+(j-1)] == 0)
//				{
//					linked[nextlabel][0] = nextlabel;
//					linkednum[nextlabel] = 1;
//					labeleddata[i*SizeW+j] = nextlabel;
//					nextlabel++;
//				}
//				else
//				{
//					int L[4];
//					L[0] = labeleddata[(i-1)*SizeW+(j-1)];
//					L[1] = labeleddata[(i-1)*SizeW+j];
//					L[2] = labeleddata[(i-1)*SizeW+(j+1)];
//					L[3] = labeleddata[i*SizeW+(j-1)];
//					int minL = SizeH*SizeW;
//					for(int l=0; l<4; l++)
//					{
//						if(L[l] < minL && L[l] > 0)
//						{
//							minL = L[l];
//						}
//					}
//					labeleddata[i*SizeW+j] = minL;
//
//					for(int l=0; l<4; l++)
//					{
//						for(int num=0; num<4; num++)
//						{
//							for(int testl=0; testl<(linkednum[(L[l])]); testl++)
//							{
//								if(L[num] == linked[(L[l])][testl])
//								{
//									break;
//								}
//								if(testl == (linkednum[(L[l])] - 1) && L[num] != linked[(L[l])][testl])
//								{
//									linked[(L[l])][linkednum[(L[l])]] = L[num];
//									linkednum[(L[l])]++;
//
//									for(int testnum=0; testnum<(linkednum[L[num]]); testnum++)
//									{
//										if(L[l] == linked[L[num]][testnum])
//											break;
//										if(testnum == (linkednum[L[num]] - 1) && L[l] != linked[L[num]][testnum])
//										{
//											linked[L[num]][linkednum[L[num]]] = L[l];
//											linkednum[L[num]]++;
//										}
//									}
//								}
//							}
//						}
//
//					}
//				}
//			}
//		}
//	}
//
//
//	for(int i=1; i<SizeH; i++)
//	{
//		int min = linked[i][0];
//		int com = linked[i][0];
//		for(int test=0; test<(linkednum[i]); test++)
//		{
//			if(linked[i][test] < min && linked[i][test] > 0)
//			{
//				min = linked[i][test];
//			}
//		}
//		while(min != com)
//		{
//			com = min;
//			for(int test=0; test<(linkednum[min]); test++)
//			{
//				if(linked[min][test] < min && linked[min][test] > 0)
//				{
//					min = linked[min][test];
//				}
//			}
//			if(min !=com)
//			{
//				linked[i][linkednum[i]] = min;
//				linkednum[i]++;
//			}
//		}
//	}
//
//
//	for(int i=0; i<SizeH; i++)
//	{
//		for(int j=0; j<SizeW; j++)
//		{
//			if(labeleddata[i*SizeW+j] != SizeH*SizeW)
//			{
//				int minLabel = labeleddata[i*SizeW+j];
//				for(int test=0; test<(linkednum[(labeleddata[i*SizeW+j])]); test++)
//				{
//					if(linked[labeleddata[i*SizeW+j]][test] < minLabel && linked[labeleddata[i*SizeW+j]][test] > 0)
//					{
//						minLabel = linked[labeleddata[i*SizeW+j]][test];
//					}
//				}
//				labeleddata[i*SizeW+j] = minLabel;
//			}
//		}
//	}
//
//
//	int finallink[SizeH];
//	memset(finallink, 0, SizeH*(sizeof(int)));
//	int final = 1;
//	for(int i=1; i<SizeH; i++)
//	{
//		int minLabel = linked[i][0];
//		for(int test=0; test<linkednum[i]; test++)
//		{
//			if(linked[i][test] < minLabel && linked[i][test] > 0)
//			{
//				minLabel = linked[i][test];
//			}
//		}
//		if(minLabel == linked[i][0])
//		{
//			finallink[minLabel] = final;
//			final++;
//		}
//	}
//
//	for(int i=1; i<SizeH; i++)
//	{
//		for(int j=1; j<SizeW; j++)
//		{
//			if(labeleddata[i*SizeW+j] != SizeH*SizeW)
//			{
//				labeleddata[i*SizeW+j] = finallink[labeleddata[i*SizeW+j]];
//			}
//		}
//	}
//
//	for(int i=0; i<SizeH; i++)
//	{
//		printf("%d %d\n ", i, finallink[i]);
//	}
//
//	return 0;
//}


int Segment(int SizeH, int SizeW, unsigned char *inputdata)
{
	int labeleddata[SizeH][SizeW];
	int count;
	memset(labeleddata, 0, SizeH*SizeW*(sizeof(int)));
	count = TwoPassLable(SizeH, SizeW, inputdata, &labeleddata[0][0]);

	unsigned char fortest[SizeH][SizeW];
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			fortest[i][j] = labeleddata[i][j]*50;
		}
	}
	writeimage("2a labeledmap.raw", &fortest[0][0], SizeH, SizeW, 1);

	int label = 1;
	int check = 0;
	int top, botton, left, right;
	for(int i=0; i<SizeH; i++)
	{
		for(int j=0; j<SizeW; j++)
		{
			if(labeleddata[i][j] == label)
			{
//				printf("top = %d  ", i);
				top = i;
				check = 1;
				break;
			}
		}
		if(check == 1)
		{
			check = 0;
			break;
		}
	}
	for(int j=0; j<SizeW; j++)
	{
		for(int i=0; i<SizeH; i++)
		{
			if(labeleddata[i][j] == label)
			{
//				printf("left = %d  ", j);
				left = j;
				check = 1;
				break;
			}
		}
		if(check == 1)
		{
			check = 0;
			break;
		}
	}
	for(int i=SizeH-1; i>=0; i--)
	{
		for(int j=SizeW-1; j>=0; j--)
		{
			if(labeleddata[i][j] == label)
			{
//				printf("botton = %d  ", i);
				botton = i;
				check = 1;
				break;
			}
		}
		if(check == 1)
		{
			check = 0;
			break;
		}
	}
	for(int j=SizeW-1; j>=0; j--)
	{
		for(int i=0; i<SizeH; i++)
		{
			if(labeleddata[i][j] == label)
			{
//				printf("right = %d  \n", j);
				right = j;
				check = 1;
				break;
			}
		}
		if(check == 1)
		{
			check = 0;
			break;
		}
	}
	int testH, testW;
	testH = botton - top + 1;
	testW = right - left + 1;

	int mayH, mayW;
	while(label < SizeH)
	{
		for(int i=0; i<SizeH; i++)
		{
			for(int j=0; j<SizeW; j++)
			{
				if(labeleddata[i][j] == label)
				{
//					printf("top = %d  ", i);
					top = i;
					check = 1;
					break;
				}
			}
			if(check == 1)
			{
				check = 0;
				break;
			}
		}
		for(int j=0; j<SizeW; j++)
		{
			for(int i=0; i<SizeH; i++)
			{
				if(labeleddata[i][j] == label)
				{
//					printf("left = %d  ", j);
					left = j;
					check = 1;
					break;
				}
			}
			if(check == 1)
			{
				check = 0;
				break;
			}
		}
		for(int i=SizeH-1; i>=0; i--)
		{
			for(int j=SizeW-1; j>=0; j--)
			{
				if(labeleddata[i][j] == label)
				{
//					printf("botton = %d  ", i);
					botton = i;
					check = 1;
					break;
				}
			}
			if(check == 1)
			{
				check = 0;
				break;
			}
		}
		for(int j=SizeW-1; j>=0; j--)
		{
			for(int i=0; i<SizeH; i++)
			{
				if(labeleddata[i][j] == label)
				{
//					printf("right = %d  \n", j);
					right = j;
					check = 1;
					break;
				}
			}
			if(check == 1)
			{
				check = 0;
				break;
			}
		}
		mayH = botton - top + 1;
		mayW = right - left + 1;

		if(abs(mayH - testH)<=2 && abs(mayW - testW)<= 2)
		{
			printf("")
		}

		label++;
	}


	return 0;
}
