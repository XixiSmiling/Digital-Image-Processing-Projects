

int GetSobleGradient(int thr, unsigned char *inputImageData, int SizeH, int SizeW, int extend, unsigned char *outputGradientR, unsigned char *outputGradientC, unsigned char *outputGradient, unsigned char *outputImageData);
int LoG(int Dfiltersize, float sigma, float thr, unsigned char *inputImageData, int SizeH, int SizeW, unsigned char *outputGradient, unsigned char *ternarymap, unsigned char *outputImageData);
int XDoG(int windowsize, float sigma, float k, float t, float e, float f, const unsigned char *inputImageData, unsigned char *outputImageData, int SizeH, int SizeW);
int DoG(int windowsize, float sigma, float k, float e, float f, const unsigned char *inputImageData, unsigned char *outputImageData, int SizeH, int SizeW);
