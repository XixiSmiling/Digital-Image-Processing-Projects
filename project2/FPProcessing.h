
int PixelWiseEn(float m0, float v0, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata);
//int PixelWiseEn(int windowsize, float m0, float v0, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata);
int gray2binary(int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata);
int RemoveErrorDots(int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata);
int HoleFilling(int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata);
int GetM(int type, int condition);
int GetFinalM(int uncondition);
int MorphFilter(int type, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata);

int CountRidgeLenght(int SizeH, int SizeW, const unsigned char *inputdata, int *storeridgelen);

int removedots(int thre, int SizeH, int SizeW, const unsigned char *inputdata, unsigned char *outputdata);
