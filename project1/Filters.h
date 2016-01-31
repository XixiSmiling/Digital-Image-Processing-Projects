double FunctionG(int sigma, int k1, int k2);
int GaussianLMFilter(int FilterSize, float sigma, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData);
int UniformLMFilter(int FilterSize, float sigma, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData);
int medianFilter(int FilterSize, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData);
double FunctionC(int i,int j,int w,int v, int sigmaC);
double FunctionS(int x,int y, int sigmaS);
int BilateralFilter(int FilterSize, int sigmaC, int sigmaS, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData);
double PSNR(const unsigned char *Orignal, const unsigned char *Filterd, int SizeH, int SizeW);
int NLMFilter(int FilterSize, int NeighborSize, int sigma, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData);
