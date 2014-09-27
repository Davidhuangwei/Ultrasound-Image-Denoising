// Dib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H__47172EBE_159C_499D_91C6_B86A503538C9__INCLUDED_)
#define AFX_DIB_H__47172EBE_159C_499D_91C6_B86A503538C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDib  
{
public:
	CDib();
	virtual ~CDib();

public:
	bool LoadFile(const char *FileName);
    bool Save(const char *pFilename);
	void Draw(CDC *pDC);
	void Huanyuan();

	//������
    void AddPepperSaltNoise();
	void AddRandomNoise();
	double NoiseVariance();
	double Noiseq(unsigned char * Bits,int t);
	double NoiseDensity();
    void SNR();
    void PSNR();

	//����Ӧ�˲�
	void Midchanged();

	//С���任
	void HardThreshold();
	void SoftThreshold();
	void HardThreshold2();
	void SoftThreshold2();
	void Hard(double* coeff, int cHeight, int cWidth, double threshold,
						  int staticHeight, int staticWidth);
	void Soft(double* coeff, int cHeight, int cWidth, double threshold,
						  int staticHeight, int staticWidth);
	void Sym8WaveletTrans(int scale);
	void Sym8WaveletReverse();
	void Sym8WaveletTransDisp(int scale=1);
	void Sym8WaveletReverseDisp();
	void SymHaarWaveletTrans(int scale);
	void SymHaarWaveletReverse();
	int GetSgn(double number);
	int m_nScale;
	double * pCoeff;

	//����������
	void MatrixRegionCopy(unsigned char *matrixRegion, int heightRegion, int widthRegion,
		unsigned char *matrixDest, int heightDest, int widthDest);	
	//�������
	void MatrixSub(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);		
	//�в���
	void MatrixSampleRow(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);	
	//�в���
	void MatrixSampleLine(unsigned char *matrixInput, unsigned char *matrixOutputOdd,
		unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput);
	//һ��С���ֽ�
	void Trans(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);
	//С���任
	void SimpleWaveletTrans(int scale=1);
	//�������
	void MatrixPlus(unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result);	
	//�в��
	void MatrixInsertRow(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);	
	//�в��
	void MatrixInsertLine(unsigned char *matrixInputOdd, unsigned char *matrixInputEven,
		unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput);	
	//һ��С���ع�
	void Reverse(unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData);	
	//С���ع�
	void SimpleWaveletReverse();
	
private:
	unsigned char *m_pDib;
	unsigned char *m_pDibOrig;
	unsigned char *m_pDibBits;
	unsigned char *m_pDibBitstemp;
	unsigned char *m_pDibBitsOrig;

	long m_width;
	long m_height;
	BITMAPINFOHEADER *m_pBIH;
	BITMAPFILEHEADER BFH;
	RGBQUAD *m_pPalette;
	CPalette m_Palette;    //����һ����ɫ�����
	int PaletteEntries;    //��ɫ�����ɫ����
	long linebytes;
	int pixelbyte;
	DWORD dwDibsize;
//Dibͼ����ز���

	int m_WindowLevel;
	int m_WindowWidth;
	unsigned char *m_pDicom;
	unsigned char *m_pName;
	unsigned char *m_pNumber;
	unsigned char *m_pBirthday;
	unsigned char *m_pSex;
	unsigned char *m_pHospitalName;
	unsigned char *m_pHosipitalAddress;
	unsigned char *m_pHeight;
	unsigned char *m_pWidth;
	unsigned char *m_pWindowLevel;
	unsigned char *m_pWindowWidth;
	unsigned char *m_pPixelBit;
	unsigned char *m_pComponentBit;
	unsigned char *m_pEffectiveBit;
	unsigned char *m_pDicomBits;
//dicomͼ����ز���
	BOOL style;

};

#endif // !defined(AFX_DIB_H__47172EBE_159C_499D_91C6_B86A503538C9__INCLUDED_)
