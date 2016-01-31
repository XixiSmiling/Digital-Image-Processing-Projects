

int Fixedthre(const float T, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata);
int RandomThre(int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata);
int Dithering(int N, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata);
int FSErrorDiff(int color, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata);
int FSErrorDiffFord(int SizeH, int SizeW, double *inputdata, double *outputdata);
int FindVertex(int position, double c0, double m0, double y0);
int ErrorPass(int i, int j, double result, int SizeH, int SizeW, double *inputcopy);
int MBVQErrorDiff(const char *inputfilename, const char *outputfilename, int SizeH, int SizeW, int BytesPerPixel);
