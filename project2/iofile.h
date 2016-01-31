

int readimage(const char *filename, unsigned char *ImageData, int SizeH, int SizeW, int BytesPerPixel);
int writeimage(const char *newfilename, unsigned char *newImageData, int newSizeH, int newSizeW, int newBytesPerPixel);
int copyimage2f(int SizeH, int SizeW, const unsigned char *inputdata, float *copydata);
int extendonechannel(const unsigned char *ImageData, int SizeH, int SizeW, unsigned char *extendImageData, int extend);
int extendonechannel2(const unsigned char *ImageData, int SizeH, int SizeW, unsigned char *extendImageData, int extend);
int ExtendMap(int *ImageData, int SizeH, int SizeW, int *extendImageData, int extend);
void OutputData(const char *filename, float *Data, int Size);
void OutputData2D(const char *filename, float *Data, int SizeH, int SizeW);
//int readlabimage(const char *filename, double *ImageData, int SizeH, int SizeW, int BytesPerPixel);
//int writelabimage(const char *newfilename, double *newImageData, int newSizeH, int newSizeW, int newBytesPerPixel);
int extendlab(double *ImageData, int SizeH, int SizeW, double *extendImageData, int extend);
int copyimaged2d(int SizeH, int SizeW, double *inputdata, double *copydata);
int DetMBVQ(unsigned char R, unsigned char G, unsigned char B);
