double FunctionG(float sigma, int k1, int k2);
int GaussianLMFilter(int FilterSize, float sigma, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData);
int GaussianFilter(int FilterSize, float sigma, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData);
int UniformLMFilter(int FilterSize, float sigma, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData);
int medianFilter(int FilterSize, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData);
double FunctionC(int i,int j,int w,int v, int sigmaC);
double FunctionS(int x,int y, double sigmaS);
int BilateralFilter(int FilterSize, int sigmaC, double sigmaS, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData);
double PSNR(const unsigned char *Orignal, const unsigned char *Filterd, int SizeH, int SizeW);
int NLMFilter(int FilterSize, int NeighborSize, int sigma, const unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputImageData);
double FSLab(double x,double y, double sigmaS);
int BFiforLab(int FilterSize, int sigmaC, double sigmaS, double *inputImageData, int SizeH, int SizeW, double *outputImageData);
