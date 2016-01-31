

int readimage(const char *filename, unsigned char *ImageData, int SizeH, int SizeW, int BytesPerPixel);
int writeimage(const char *newfilename, unsigned char *newImageData, int newSizeH, int newSizeW, int newBytesPerPixel);
int extendonechannel(const unsigned char *ImageData, int SizeH, int SizeW, unsigned char *extendImageData, int extend);

