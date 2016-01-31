
int TwoPassLable(int SizeH, int SizeW, unsigned char *inputdata, int *labeleddata);
int FindHoleInfo(unsigned char *ImageData, int SizeH, int SizeW);
int FindPiecesInfo(unsigned char *ImageData, int SizeH, int SizeW);
int rotation(unsigned char *input, unsigned char *output, int imageSizeH, int imageSizeW, float rSizeH, float rSizeW, float rI, float rJ, float rAngle);
int scaling(unsigned char *input, unsigned char *output, int imageSizeH, int imageSizeW, float oldH, float oldW, float newH, float newW, float sI, float sJ);
int shift(unsigned char *input, unsigned char *output, int inputSizeW, int outputSizeW, float H, float W, float oldI, float oldJ, float newI, float newJ);
int combination(unsigned char *input1, unsigned char *input2, unsigned char *output, int SizeH, int SizeW);
int resizeimage(unsigned char *input, unsigned char *output, int inputH, int inputW, int outputH, int outputW);
