// Dib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "��ҵ���.h"
#include "Dib.h"
#include "��ҵ���Doc.h"
#include "��ҵ���View.h"
// #include "highgui.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <math.h>
#include "WaveletApi.h"
#include "FilterApi.h"
#include "DIBapi.h"

#pragma comment(lib, "wavelet_3_8.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDib::CDib()
{
	m_pDib=NULL;
	m_pDibOrig=NULL;
	m_pDicom = NULL;

	m_pDibBitstemp=NULL;
	pCoeff=NULL;
}

CDib::~CDib()
{
	if (m_pDib!=NULL)
	{
		delete []m_pDib;
		m_pDib=NULL;
		
	}
	if (m_pDibOrig!=NULL)
	{
		delete []m_pDibOrig;
		m_pDibOrig=NULL;	
	}
	if(m_pDicom != NULL)
	{
		delete []m_pDicom;
	}
	if (m_pDibBitstemp!=NULL)
	{
		delete []m_pDibBitstemp;
		m_pDibBitstemp=NULL;	
	}
	if (pCoeff!=NULL)
	{
		delete []pCoeff;
		pCoeff=NULL;	
	}
}

void CDib::Draw( CDC *pDC )
{
						
	 if (m_pDib==NULL)
	{
	       return;
	}else
	{
        ::StretchDIBits(pDC->m_hDC,0,0,m_width,m_height,0,0,m_width,m_height,m_pDibBits,(BITMAPINFO *)m_pBIH,BI_RGB,SRCCOPY);
	    /*��Դ�������е�����ӳ�䵽Ŀ������������ؿ��У�����Ŀ�����ؿ��һ����ɫ��Դ���ص���ɫ�ӽ���
		pDC->SetStretchBltMode(HALFTONE);*/
	}
	if (style==0)
	{
		CString str("BMP��ʽ����Ϣ");
		pDC->TextOut(m_width+100,100,str);
	}
	else
	{
		CString str("����������");
		pDC->TextOut(m_width+100,100,str);
		
		str.Format("��ţ�");
		pDC->TextOut(m_width+100,130,str);
		
		str.Format("�Ա�");
		pDC->TextOut(m_width+100,160,str);
		
		
		str.Format("ҽԺ���ƣ�");
		pDC->TextOut(m_width+100,220,str);
		
		str.Format("��ַ��");
		pDC->TextOut(m_width+100,250,str);
		
		
		str.Format("��λ��");
		pDC->TextOut(m_width+100,340,str);
		
		str.Format("����");
		pDC->TextOut(m_width+100,370,str);
		
		
		
		CString str1(m_pName);
		pDC->TextOut(m_width+200,100,str1);
		CString str2(m_pNumber);
		pDC->TextOut(m_width+200,130,str2);
		
		
		if(*m_pSex == 'F')
		{
			str.Format("Ů");
			pDC->TextOut(m_width+200,160,str);
		}
		else
		{
			str.Format("��");
			pDC->TextOut(m_width+200,160,str);
		}
		
		
		CString str4(m_pHospitalName);
		pDC->TextOut(m_width+200,220,str4);
		
		CString str5(m_pHosipitalAddress);
		pDC->TextOut(m_width+200,250,str5);
		
		
		CString str6(m_pWindowLevel);
		pDC->TextOut(m_width+200,340,str6);
		
		CString str7(m_pWindowWidth);
    	pDC->TextOut(m_width+200,370,str7);
	}
}

bool CDib::LoadFile( const char *FileName )
{
	
	CFile File;
	File.Open(FileName,CFile::modeRead|CFile::shareDenyRead,NULL);

	DWORD dwDibsize=File.GetLength()-sizeof(BITMAPFILEHEADER);
    File.Read(&BFH,sizeof(BITMAPFILEHEADER));
    if(BFH.bfType!= 'MB')
	{

	unsigned char *temp;
	int a=128-sizeof(BITMAPFILEHEADER);
	temp=new unsigned char[a];
	File.Read(temp,a);
	delete []temp;

	DWORD Type;
	File.Read(&Type,sizeof(4));
	if(Type != 'MCID')
	{
		AfxMessageBox("Error :not BMP or Dicom file",MB_OK);
		return false;
	}
	style=1;
	int m_FileSize = File.GetLength()-132;
	m_pDicom = new unsigned char[m_FileSize];
	File.Read(m_pDicom,m_FileSize);
	DWORD m_Tag;
	DWORD m_VL;
	int i;
	int m_Flag=0;
	unsigned int j;
	for(i=0;i<m_FileSize&&m_Flag==0;i++)
	{
		m_Tag = *((DWORD *)(m_pDicom+i));
		m_VL = *((DWORD *)(m_pDicom+i+4));
		switch(m_Tag)
		{
 			case 0x00100010:								// ��������
				m_pName = new unsigned char[m_VL];
			    m_pName = m_pDicom+i+8;
				break;
			case 0x00200010:								// ���˱��
				m_pNumber = new unsigned char[m_VL];
			    m_pNumber = m_pDicom+i+8;
				break;
			case 0x00400010:								// �Ա�
				m_pSex = new unsigned char[m_VL];
				for(j=0;j<m_VL;j++)
				{
					*(m_pSex+j) = *(m_pDicom+i+8+j);
				}
				break;
			case 0x00800008:								// ҽԺ����
				m_pHospitalName = new unsigned char[m_VL];
			    m_pHospitalName = m_pDicom+i+8;
				break;
			case 0x00810008:								// ҽԺ��ַ
				m_pHosipitalAddress = new unsigned char[m_VL];
			    m_pHosipitalAddress = m_pDicom+i+8;
				break;
			case 0x00100028:								// ͼ�������
				m_pHeight = new unsigned char[m_VL];
				for(j=0;j<m_VL;j++)
				{
					//*(m_pHeight+j) = *(m_pDicom+i+8+j);
					int Heightlen = (*(m_pDicom+i+6))*65536+(*(m_pDicom+i+5))*256+(*(m_pDicom+i+4));
					m_pHeight=m_pDicom+i+8;
				}
				m_height = 0;
				for(j=m_VL;j>0;j--)
				{
					m_height=m_height*256;
					m_height=m_height+m_pHeight[j-1];
				}
				break;
			case 0x00110028:								// ͼ�������				
				m_pWidth = new unsigned char[m_VL];
				for(j=0;j<m_VL;j++)
				{
					*(m_pWidth+j) = *(m_pDicom+i+8+j);
				}
				m_width = 0;
				for(j=m_VL;j>0;j--)
				{
					m_width=m_width*256;
					m_width=m_width+m_pWidth[j-1];
				}
				break;				
			case 0x10500028:								// ͼ��Ĵ�λ
				m_pWindowLevel = new unsigned char[m_VL];
			    m_pWindowLevel=m_pDicom+i+8;
				m_WindowLevel = 0;
				for(j=0;j<m_VL-1;j++)
				{
					m_WindowLevel = m_WindowLevel*10;
					m_WindowLevel = m_WindowLevel+m_pWindowLevel[j]-48;
				}
				break;
			case 0x10510028:								// ͼ��Ĵ���
				m_pWindowWidth = new unsigned char[m_VL];
				m_pWindowWidth=m_pDicom+i+8;
				m_WindowWidth = 0;
				for(j=0;j<m_VL-1;j++)
				{
					m_WindowWidth = m_WindowWidth*10;
					m_WindowWidth = m_WindowWidth+m_pWindowWidth[j]-48;
				}
				break;
			case 0x01000028:								// ÿ�����صı���λ��
				m_pPixelBit = new unsigned char[m_VL];
				for(j=0;j<m_VL;j++)
				{
					*(m_pPixelBit+j) = *(m_pDicom+i+8+j);
				}
				break;
			case 0x01010028:								// ����ÿ�������ı���λ��
				m_pComponentBit = new unsigned char[m_VL];
				for(j=0;j<m_VL;j++)
				{
					*(m_pComponentBit+j) = *(m_pDicom+i+8+j);
				}
				break;
			case 0x01020028:								// ��ʾ������Чλ�������λ��λ��
				m_pEffectiveBit = new unsigned char[m_VL];
				for(j=0;j<m_VL;j++)
				{
					*(m_pEffectiveBit+j) = *(m_pDicom+i+8+j);
				}
				break;
			case 0x00107fe0:								// ͼ���������ݣ�ǰ���Ǹ����ݵĳ��ȣ���������ݾ��Ǿ����ͼ����������
				m_pDicomBits = new unsigned char[m_VL];
				for(j=0;j<m_VL;j++)
				{
					*(m_pDicomBits+j) = *(m_pDicom+i+8+j);
				}
				m_Flag = 1;
				break;
		}
	}
/*	&BFH = (BITMAPFILEHEADER *)m_pDib;*/


	m_pDib = new unsigned char[40+4*256+m_width*m_height];
	m_pDibOrig = new unsigned char[40+4*256+m_width*m_height];
	m_pBIH = (BITMAPINFOHEADER *)m_pDib;
	m_pBIH->biSize = 40;
	m_pBIH->biWidth = m_width;
	m_pBIH->biHeight = m_height;
	m_pBIH->biPlanes = 1;
	m_pBIH->biBitCount = 8;
	m_pBIH->biCompression = 0;
	m_pBIH->biSizeImage = 0;
	m_pBIH->biXPelsPerMeter = 0;
	m_pBIH->biYPelsPerMeter = 0;
	m_pBIH->biClrUsed = 256;
	PaletteEntries=m_pBIH->biClrUsed;
	m_pBIH->biClrImportant = 256;
	for (i=0;i<256;i++)
	{
		m_pPalette = (RGBQUAD *)(m_pDib+40+i*4);
		m_pPalette->rgbBlue = i;
		m_pPalette->rgbGreen = i;
		m_pPalette->rgbRed = i;
		m_pPalette->rgbReserved = 0;
	}
	m_pDibBits = m_pDib+40+4*256;
    m_pDibBitsOrig=m_pDibOrig+40+4*256;
	int k=0;
	for(i=m_height;i>0;i--)
	{
		for (int j=0;j<m_width;j++)
		{	
			WORD m_Buffer;
			m_Buffer = *((WORD *)(m_pDicomBits+k*2));
			if(m_Buffer>=(m_WindowLevel-m_WindowWidth/2)&&m_Buffer<=(m_WindowLevel+m_WindowWidth/2))
			{
				m_Buffer = (m_Buffer-(m_WindowLevel-m_WindowWidth/2))*(4096/m_WindowWidth);
 				*(m_pDibBits+i*m_width+j) = m_Buffer*256/4096;
				k++;
// 				*(m_pDibBits+k) = m_Buffer*256/4096;
			}
			else if(m_Buffer<(m_WindowLevel-m_WindowWidth/2))
			{
 				*(m_pDibBits+i*m_width+j) = 1;
				k++;
// 				*(m_pDibBits+k) = 1;
			}
			else
			{
 				*(m_pDibBits+i*m_width+j) = 254;
				k++;
// 				*(m_pDibBits+k) = 1;
			}
		}
	}
	linebytes=(m_width*m_pBIH->biBitCount+31)/32*4;
	pixelbyte=m_pBIH->biBitCount/8;
	memcpy(m_pDibBitsOrig,m_pDibBits,linebytes*m_height);
    
	BFH.bfType='MB';
    BFH.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+linebytes*m_height+PaletteEntries*sizeof(RGBQUAD);
    BFH.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+PaletteEntries*sizeof(RGBQUAD);
	return true;

	}else
	{
	style=0;
	m_pDib=new unsigned char[dwDibsize];
	m_pDibOrig=new unsigned char[dwDibsize];
	File.Read(m_pDib,dwDibsize);
	m_pBIH=(BITMAPINFOHEADER *)m_pDib;
	m_width=m_pBIH->biWidth;
	m_height=m_pBIH->biHeight;
	linebytes=(m_width*m_pBIH->biBitCount+31)/32*4;
	pixelbyte=m_pBIH->biBitCount/8;
	
	if (m_pBIH->biBitCount>8)
	{
	     PaletteEntries=0;
	}else
	{
	     PaletteEntries=m_pBIH->biClrUsed;	
	}
	m_pDibBits=m_pDib+sizeof(BITMAPINFOHEADER)+PaletteEntries*sizeof(RGBQUAD);
	m_pDibBitsOrig=m_pDibOrig+sizeof(BITMAPINFOHEADER)+PaletteEntries*sizeof(RGBQUAD);
	memcpy(m_pDibBitsOrig,m_pDibBits,linebytes*m_height);

    m_pPalette=(RGBQUAD *)(m_pDib+sizeof(BITMAPINFOHEADER));

    return true;
	}

}

void CDib::Midchanged()
{
	BYTE * F_temp= new BYTE [linebytes * m_height];
	memcpy (F_temp , m_pDibBits , linebytes * m_height);
	int min=3,maxx=7,a=3;
	int i,j,s=a*a;
	int middle_temp[49],midp,minp=0,maxp=255;
	int xx, yy, m;
	int chg,medi;
	for (i = 1; i < m_height; i++)//��
	{
		for(j = 1; j < m_width; j++)//��
		{
			for (int k=0;k<pixelbyte;k++)
			{


				for(a=3; a<=maxx; a=a+2)
				{
				m=0;
				for(xx= i-(a-1)/2; xx<= i+(a-1)/2; xx++)
					for(yy=j-(a-1)/2; yy<=j+(a-1)/2; yy++)
					{
						middle_temp[m]=m_pDibBits [xx*linebytes+yy*pixelbyte+k];
						m++;
					}//��
					do{
						chg=0;
						for(m=0 ; m<s-1 ;m++)
						{
							if (middle_temp[m]<minp)
							{
								minp=middle_temp[m];
							}
							if (middle_temp[m]>maxp)
							{
								maxp=middle_temp[m];
							}
							if(middle_temp[m]<middle_temp[m+1])
							{
								midp=middle_temp[m];
								middle_temp[m]=middle_temp[m+1];
								middle_temp[m+1]=midp;
								chg=1;
							}
						}
					}while(chg==1);
					medi = middle_temp[(s-1)/2];
					if ((medi<maxp) && (medi>minp))
					{
                         if ((m_pDibBits [i*linebytes+j*pixelbyte+k]<maxp) && (m_pDibBits [i*linebytes+j*pixelbyte+k]>minp))
                         {
							 *(F_temp+i*linebytes+j*pixelbyte+k)=m_pDibBits [i*linebytes+j*pixelbyte+k];
							  break;
                         }
                         *(F_temp+i*linebytes+j*pixelbyte+k)=medi;
						 break;
					}
				}


			}		
		}
	}
	memcpy(m_pDibBits ,F_temp,linebytes*m_height);
	delete  []F_temp;
}

void CDib::AddPepperSaltNoise()
{
	BYTE * F_temp= new BYTE [linebytes * m_height];
	memcpy (F_temp , m_pDibBits , linebytes * m_height);
	int bytecount,i;
	bytecount = linebytes*m_height; //��ȡλͼ���������ֽ���
	
	if (PaletteEntries!=0)      //����Ҷ�ͼ��
	{  
        for (i=0;i<bytecount;i++)
        {
			if(rand()>25000)  F_temp[i]=0;
			if(rand()<600)    F_temp[i]=255;
        }
	}
	else	                 //24λ���ɫͼ��
	{
        for (i=0;i<bytecount;i=i+3)
        {
			int num=rand();
            if (num>25000) 
			{
				F_temp[i]=(rand())%255;        //����ÿһ�����ص�RGBֵ
				F_temp[i+1]=(rand())%255;
			    F_temp[i+2]=(rand())%255;
			}
			if (num<600) 
			{
				F_temp[i]=(rand())%255;
				F_temp[i+1]=(rand())%255;
				F_temp[i+2]=(rand())%255;
			}
        }
	}
	//�����µ�DIBλͼ
	for(i=0;i<m_height;i++)
	{
		for(int j=0;j<linebytes;j++)
			*(m_pDibBits+i*linebytes+j)= *(F_temp+i*linebytes+j);
	}
	delete  []F_temp;
}

bool CDib::Save( const char* pFileName )
{
	if (m_pDib ==NULL) 
	{
		AfxMessageBox("This isn't a BMP File!",MB_OK);
		return false;
	}
	
	CFile file(pFileName, CFile::modeCreate|CFile::modeWrite);
	file.Write(&BFH,sizeof(BITMAPFILEHEADER));
	file.Write(m_pDib,linebytes*m_height+PaletteEntries*sizeof(RGBQUAD)+sizeof(BITMAPINFOHEADER));
	
	file.Close();
	
	return true;
	
}

void CDib::AddRandomNoise()
{
	BYTE * F_temp= new BYTE [linebytes * m_height];
	memcpy (F_temp , m_pDibBits , linebytes * m_height);
	int bytecount,i;
	bytecount = linebytes*m_height; //��ȡλͼ���������ֽ���
    for( i=0;i<bytecount;i++)
	{
		LONG Temp=rand();
		Temp = F_temp[i]*224/256+Temp/512;
		F_temp[i] = Temp >= 255 ? 255 : (unsigned char) Temp;     //if Temp>=255;F_temp[i] = 255;else F_temp[i] = Temp
    }
	//�����µ�DIBλͼ
	for(i=0;i<m_height;i++)
	{
		for(int j=0;j<linebytes;j++)
			*(m_pDibBits+i*linebytes+j)= *(F_temp+i*linebytes+j);
	}
	delete  []F_temp;
}

void CDib::HardThreshold()
{
	Sym8WaveletTrans(2);
	int N= m_width;	//���
	int M= m_height;	//����
	int k,scale =0,t=1,start =0;
	double Threshold;
	for(scale =1;scale<=m_nScale;scale++,start +=N*(M/t))
	{
		t =1<<scale;
		double* thresholdTemp = new double[M/t*N/t];
		k =start+N/t;
		::Copy(pCoeff+k,M,N,thresholdTemp,M/t,N/t);
		Threshold =::GetThreshold(thresholdTemp, M/t, N/t, 1);
		Threshold += 10;
		
		delete[] thresholdTemp;
		//
		::ReverseDIBBits(pCoeff,M,N);
		
		Hard(pCoeff + N/t,M/t,N/t,Threshold,M,N);
		Hard(pCoeff + N*(M/t),M/t,N/t*2,Threshold,M,N);
		
		::ReverseDIBBits(pCoeff,M,N);
	}
	Sym8WaveletReverse();
}

void CDib::SoftThreshold()
{
	Sym8WaveletTrans(2);
	int N= m_width;	//���
	int M= m_height;	//����
	int k,scale =0,t=1,start =0;
	double Threshold;
	for(scale =1;scale<=m_nScale;scale++,start +=N*(M/t))
	{
		t =1<<scale;
		double* thresholdTemp = new double[M/t*N/t];
		k =start+N/t;
		::Copy(pCoeff+k,M,N,thresholdTemp,M/t,N/t);
		Threshold =::GetThreshold(thresholdTemp, M/t, N/t, 1);
		//		Threshold += 10;
//		Threshold=200;
		delete[] thresholdTemp;
		//
		::ReverseDIBBits(pCoeff,M,N);
		
		Soft(pCoeff + N/t,M/t,N/t,Threshold,M,N);
		Soft(pCoeff + N*(M/t),M/t,N/t*2,Threshold,M,N);
		
		::ReverseDIBBits(pCoeff,M,N);
	}
	Sym8WaveletReverse();
}

void CDib::Hard( double* coeff, int cHeight, int cWidth, double threshold, int staticHeight, int staticWidth )
{
	int i,j;
	for(i = 0; i < cHeight; i++)
	{         
		for(j = 0; j < cWidth; j++)
		{
			if((*(coeff + i * staticWidth + j)>=0)&&(*(coeff + i * staticWidth + j) <= threshold) ||
				(*(coeff + i * staticWidth + j)<0)&&(*(coeff + i * staticWidth + j)*(-1) <= threshold))
			{
				*(coeff + i * staticWidth + j) =0;
			}
		}
	}
}

void CDib::Soft( double* coeff, int cHeight, int cWidth, double threshold, int staticHeight, int staticWidth )
{
	int i,j;
	double temp;
	for(i = 0; i < cHeight; i++)
	{         
		for(j = 0; j < cWidth; j++)
		{
			if((*(coeff + i * staticWidth + j)>=0)&&(*(coeff + i * staticWidth + j) <= threshold) ||
				(*(coeff + i * staticWidth + j)<0)&&(*(coeff + i * staticWidth + j)*(-1) <= threshold))
			{
				*(coeff + i * staticWidth + j) =0;
			}
			else
			{
				temp =*(coeff + i * staticWidth + j);
				if(temp<0)
				{
					temp *=(-1);
				}
				temp -=threshold;
				*(coeff + i * staticWidth + j) = GetSgn(*(coeff + i * staticWidth + j))*temp;
			}
		}
	}
}

void CDib::Sym8WaveletTrans( int scale )
{
	if (scale<1 || scale>5)
	{
		return;
	}

// 	if(m_pDibBitstemp!=NULL)
// 	{
// 		delete[] m_pDibBitstemp;
// 		m_pDibBitstemp=NULL;
// 	}

	this->m_nScale = scale;
	int lineByte=(m_width*pixelbyte+3)/4*4;
//	m_pDibBitstemp=new unsigned char[lineByte*m_height];

	int N= m_width;
	int M= m_height; 
	
	if (pCoeff != NULL)
	{
		delete[] pCoeff;
	}
	pCoeff = new double[M*N];
	double* coeff1 = new double[M*N];
	
	::Copy(m_pDibBits, M, N, coeff1, M, N);              //��m_pDibBits����coeff1
	::ReverseDIBBits(coeff1, M, N);
	
	if(Wavelet(coeff1, M, N, scale, 4, pCoeff, M, N))    //coeff1ΪҪ�ֽ�Ķ�ά����pCoeffΪ�ֽ��Ķ�ά���󣬷ֽ�ɹ�����true
	{
		::ReverseDIBBits(pCoeff, M, N);
	}
	delete[] coeff1;
}

void CDib::Sym8WaveletReverse()
{
	if (m_nScale <= 0)
	{
		return;
	}
// 	if (m_pDibBitstemp==NULL)
// 	{
// 		return;
// 	}
//  	if(m_pDibBitstemp!=NULL)
// 	{
//  		delete[] m_pDibBitstemp;
//  		m_pDibBitstemp=NULL;
//  	}
	if (pCoeff == NULL)
	{
		return;
	}
	int lineByte=(m_width*pixelbyte+3)/4*4;
	m_pDibBitstemp=new unsigned char[lineByte*m_height];

	int N= m_width;
	int M= m_height;
	::ReverseDIBBits(pCoeff, M, N);
	if(InverseWavelet(pCoeff, M, N, m_nScale, 4, M, N))
	{
		::ReverseDIBBits(pCoeff, M, N);
		::CopyPositive(pCoeff,M, N,m_pDibBits,M, N);
	}
//	memcpy(m_pDibBits,m_pDibBitstemp,linebytes*m_height);
}

int CDib::GetSgn( double number )
{
	if(number ==0)
	{
		return 0;
	}
	else if(number >0)
	{
		return 1;
	}
	else 
	{
		return (-1);
	}
}


void CDib::MatrixRegionCopy( unsigned char *matrixRegion, int heightRegion, int widthRegion, unsigned char *matrixDest, int heightDest, int widthDest )
{
	//��������߶ȺͿ��
	int heightMin = heightRegion>heightDest ? heightDest : heightRegion;
	int widthMin = widthRegion>widthDest ? widthDest : widthRegion;
	int h, w;
	for (h=0; h<heightMin; h++)
	{
		for (w=0; w<widthMin; w++)
		{
			matrixDest[h * widthDest + w] = matrixRegion[h * widthRegion + w];
		}
	}
}

void CDib::MatrixSub( unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result )
{
	unsigned char temp;
	int h, w;
	for (h=0; h<height; h++)
	{
		for (w=0; w<width; w++)
		{
			temp = matrixA[h * width + w] - matrixB[h * width + w] + 128;
			result[h * width + w] = temp;
		}
	}
}

void CDib::MatrixSampleRow( unsigned char *matrixInput, unsigned char *matrixOutputOdd, unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput )
{
	int h, w;
	for (h=0; h<heightOutput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			matrixOutputEven[h * widthOutput + w] = matrixInput[h * 2 * widthInput + w];
			matrixOutputOdd[h * widthOutput + w] = matrixInput[(h * 2 + 1) * widthInput + w];
		}
	}
}

void CDib::MatrixSampleLine( unsigned char *matrixInput, unsigned char *matrixOutputOdd, unsigned char *matrixOutputEven, int heightOutput, int widthOutput, int widthInput )
{
	int h, w;
	for (h=0; h<heightOutput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			matrixOutputEven[h * widthOutput + w] = matrixInput[h * widthInput + w * 2];
			matrixOutputOdd[h * widthOutput + w] = matrixInput[h * widthInput + w * 2 + 1];
		}
	}
}

void CDib::Trans( unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData )
{
	unsigned char *buff = new unsigned char[lineByte*m_height];
	
	int imageHeight = inputHeight;
	int imageWidth = inputWidth;
	
	unsigned char *temp1 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	//��ͼ���������в���
	this->MatrixSampleRow(inputData, temp1, temp2, (imageHeight/2), imageWidth, lineByte);
	//�в�֣��õ���Ƶ���Ƶ����
	this->MatrixSub(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//�Ե�Ƶ���ݽ����в���
	this->MatrixSampleLine(temp2, temp01, temp00, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�ֵõ�LL��LH
	this->MatrixSub(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	//�Ը�Ƶ���ݽ����в���
	this->MatrixSampleLine(temp1, temp11, temp10, (imageHeight/2), (imageWidth/2), imageWidth);
	//�в�֣��õ�HL��HH
	this->MatrixSub(temp11, temp10, (imageHeight/2), (imageWidth/2), temp11);
	
	this->MatrixRegionCopy(temp01, (imageHeight/2), (imageWidth/2),
		buff+lineByte*(int)(imageHeight/2)+(int)(imageWidth/2), imageHeight, lineByte);
	this->MatrixRegionCopy(temp10, (imageHeight/2), (imageWidth/2), buff, imageHeight, lineByte);
	this->MatrixRegionCopy(temp11, (imageHeight/2), (imageWidth/2), buff+(int)(imageWidth/2), imageHeight, lineByte);
	//�ͷſռ�
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;
	
	scale--;
	//������LL���еݹ�ֽ�
	if (scale > 0)
	{
		this->Trans(temp00, imageHeight/2, imageWidth/2, imageWidth/2, scale, temp00);
	}
	this->MatrixRegionCopy(temp00, (imageHeight/2), (imageWidth/2), buff+lineByte*(int)(imageHeight/2),
		imageHeight, lineByte);
	delete[] temp00;
	
	//�����Ľ��������m_pImgDataOut
	memcpy(outputData, buff,  lineByte * inputHeight);
	delete[] buff;
}

void CDib::SimpleWaveletTrans( int scale )
{
	if (scale<1 || scale>5)
	{
		return;
	}
	
	if(m_pDibBitstemp!=NULL)
	{
		delete[] m_pDibBitstemp;
		m_pDibBitstemp=NULL;
	}
	this->m_nScale = scale;
	int lineByte=(m_width*pixelbyte+3)/4*4;
	m_pDibBitstemp=new unsigned char[linebytes*m_height];

	this->Trans(m_pDibBits, m_height, m_width, lineByte, scale, m_pDibBitstemp);
     	memcpy(m_pDibBits,m_pDibBitstemp,lineByte*m_height);
}

void CDib::MatrixPlus( unsigned char *matrixA, unsigned char *matrixB, int height, int width, unsigned char *result )
{
	unsigned char temp;
	int h, w;
	for (h=0; h<height; h++)
	{
		for (w=0; w<width; w++)
		{
			temp = matrixA[h * width + w] + matrixB[h * width + w] - 128;
			result[h * width + w] = temp;
		}
	}
}

void CDib::MatrixInsertRow( unsigned char *matrixInputOdd, unsigned char *matrixInputEven, unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput )
{
	int h, w;
	for (h=0; h<heightInput*2; h++)
	{
		for (w=0; w<widthInput; w++)
		{
			if (h%2 == 0)
			{
				matrixOutput[h * widthOutput + w] = matrixInputEven[(int)(h/2) * widthInput + w];
			} 
			else
			{
				matrixOutput[h * widthOutput + w] = matrixInputOdd[(int)(h/2) * widthInput + w];
			}		
		}
	}
}

void CDib::MatrixInsertLine( unsigned char *matrixInputOdd, unsigned char *matrixInputEven, unsigned char *matrixOutput, int heightInput, int widthInput, int widthOutput )
{
	int h, w;
	for (h=0; h<heightInput; h++)
	{
		for (w=0; w<widthOutput; w++)
		{
			if (w%2 == 0)
			{
				matrixOutput[h * widthOutput + w] = matrixInputEven[h * widthInput + w/2];
			} 
			else
			{
				matrixOutput[h * widthOutput + w] = matrixInputOdd[h * widthInput + w/2];
			}		
		}
	}
}

void CDib::Reverse( unsigned char *inputData, int inputHeight, int inputWidth, int lineByte, int scale, unsigned char *outputData )
{
	unsigned char *buff = new unsigned char[lineByte*m_height];
	
	int imageHeight = inputHeight;
	int imageWidth = inputWidth;
	
	
	unsigned char *temp10 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp11 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	this->MatrixRegionCopy(inputData, imageHeight, lineByte, temp10, (imageHeight/2), (imageWidth/2));
	this->MatrixRegionCopy(inputData+(int)(imageWidth/2), imageHeight, lineByte, temp11, (imageHeight/2), (imageWidth/2));
	this->MatrixPlus(temp11, temp10, (imageHeight/2), (imageWidth/2), temp11);
	unsigned char *temp1 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	this->MatrixInsertLine(temp11, temp10, temp1, (imageHeight/2), (imageWidth/2), imageWidth);
	
	unsigned char *temp00 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	unsigned char *temp01 = new unsigned char[(imageWidth/2) * (imageHeight/2)];
	this->MatrixRegionCopy(inputData+lineByte*(int)(imageHeight/2), imageHeight, lineByte, temp00, (imageHeight/2), (imageWidth/2));
	this->MatrixRegionCopy(inputData+lineByte*(int)(imageHeight/2)+(int)(imageWidth/2),
		imageHeight, lineByte, temp01, (imageHeight/2), (imageWidth/2));
	
	scale--;
	//�ݹ��LL���ݽ����ع�
	if (scale > 0)
	{
		this->Reverse(temp00, imageHeight/2, imageWidth/2, imageWidth/2, scale, temp00);
	}
	this->MatrixPlus(temp01, temp00, (imageHeight/2), (imageWidth/2), temp01);
	unsigned char *temp2 = new unsigned char[imageWidth * (int)(imageHeight/2)];
	this->MatrixInsertLine(temp01, temp00, temp2, (imageHeight/2), (imageWidth/2), imageWidth);
	
	this->MatrixPlus(temp1, temp2, (imageHeight/2), imageWidth, temp1);
	this->MatrixInsertRow(temp1, temp2, buff, (imageHeight/2), imageWidth, lineByte);
	
	//�ͷſռ�
	delete[] temp1;
	delete[] temp2;
	delete[] temp01;
	delete[] temp10;
	delete[] temp11;
	delete[] temp00;
	
	//�����Ľ��������m_pImgDataOut
	memcpy(outputData, buff,  lineByte * inputHeight);
	delete[] buff;
}

void CDib::SimpleWaveletReverse()
{
	if (m_nScale <= 0)
	{
		return;
	}
	if (m_pDibBitstemp==NULL)
	{
		return;
	}
	int lineByte=(m_width*pixelbyte+3)/4*4;
	
	this->Reverse(m_pDibBits, m_height, m_width, lineByte, m_nScale, m_pDibBitstemp);
	memcpy(m_pDibBits,m_pDibBitstemp,lineByte*m_height);
}

void CDib::SNR()
{	
	//����ͼ�������
	double  temp1=0.0;
	double  temp2=0.0;
	//����8λ�Ҷ�ͼ��
	if (pixelbyte==1)
	{
		//��������DIBλͼ���ֽ���

		for(int i=0;i<m_height;i++)
		{
			for(int j=0;j<linebytes;j++)
			{
				temp1=temp1+(double)(*(m_pDibBitsOrig+i*linebytes+j) * *(m_pDibBitsOrig+i*linebytes+j));
		    	temp2=temp2+(double)((*(m_pDibBitsOrig+i*linebytes+j)-*(m_pDibBits+i*linebytes+j)) * (*(m_pDibBitsOrig+i*linebytes+j)-*(m_pDibBits+i*linebytes+j)));
			}
		}
	}
	else
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
	}
	double  SNR=temp1/temp2;
	SNR=10*log10(SNR);    
	
	CString StrSNR;
	StrSNR.Format("%f",SNR);
	::MessageBox(NULL,StrSNR,"�����",MB_OK);
}

void CDib::PSNR()
{	
	//����ͼ�������
	double  temp1=0.0;
	double  temp2=0.0;
	//����8λ�Ҷ�ͼ��
	if (pixelbyte==1)
	{
		//��������DIBλͼ���ֽ���
		
		for(int i=0;i<m_height;i++)
		{
			for(int j=0;j<linebytes;j++)
			{
				temp1=temp1+(double)*(m_pDibBits+i*linebytes+j) * *(m_pDibBits+i*linebytes+j);
				temp2=temp2+(double)(*(m_pDibBits+i*linebytes+j)-*(m_pDibBitsOrig+i*linebytes+j)) * (*(m_pDibBits+i*linebytes+j)-*(m_pDibBitsOrig+i*linebytes+j));
			}
		}
	}
	else
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
	}
	double   PSNR=m_height*m_width/temp2;
	PSNR=10*log10(255*255*PSNR);       //PSNR��Ϊ��ֵ�����

	CString StrPSNR;
	StrPSNR.Format("%f",PSNR);
	::MessageBox(NULL,StrPSNR,"��ֵ�����",MB_OK);
}

void CDib::Huanyuan()
{
	//memcpy(m_pDibBits,m_pDibBitsOrig,linebytes*m_height);
	for(int i=0;i<m_height;i++)
	{
		for(int j=0;j<linebytes;j++)
			*(m_pDibBits+i*linebytes+j)= *(m_pDibBitsOrig+i*linebytes+j);
	}
}

void CDib::HardThreshold2()
{
	SymHaarWaveletTrans(1);
	int N= m_width;	//���
	int M= m_height;	//����
	int scale =0,t=1,start =0;
	double Threshold;
//	double Variance=NoiseVariance();
	for(scale =1;scale<=m_nScale;scale++,start +=N*(M/t))
	{
		t =1<<scale;
		unsigned char* thresholdTemp = new unsigned char[M*N];
//		k =start+N/t;
		::Copy(pCoeff,M,N,thresholdTemp,M,N);
		double q=Noiseq(thresholdTemp,1);
		Threshold =0.3*sqrt(2*log(M/t*N/t))*q;
	//	Threshold =::GetThreshold(thresholdTemp, M/t, N/t, 1);
		Threshold += 10;
		
		delete[] thresholdTemp;
		//
		::ReverseDIBBits(pCoeff,M,N);
		
		Hard(pCoeff ,M/t,N/t,Threshold,M,N);
	//	Hard(pCoeff + N*(M/t),M/t,N/t*2,Threshold,M,N);
		
		::ReverseDIBBits(pCoeff,M,N);
	}
	SymHaarWaveletReverse();
}

void CDib::SoftThreshold2()
{
	SymHaarWaveletTrans(1);
	int N= m_width;	//���
	int M= m_height;	//����
	int scale =0,t=1,start =0;
	double Threshold;
//	double Variance=NoiseVariance();
	for(scale =1;scale<=m_nScale;scale++,start +=N*(M/t))
	{
		t =1<<scale;
		unsigned char* thresholdTemp = new unsigned char [M*N];
//		k =start+N/t;
		::Copy(pCoeff,M,N,thresholdTemp,M,N);
		double q=Noiseq(thresholdTemp,1);
		Threshold =0.3*sqrt(2*log(M/t*N/t))*q;     //����ֵ̫�����˲���ͼ��ʧ��

		//		Threshold += 10;
//		Threshold=26.363678668004123;
		delete[] thresholdTemp;
		
		::ReverseDIBBits(pCoeff,M,N);
		
		Soft(pCoeff ,M/t,N/t,Threshold,M,N);
//		Soft(pCoeff + N*(M/t),M/t,N/t*2,Threshold,M,N);
		
		::ReverseDIBBits(pCoeff,M,N);
	}
	SymHaarWaveletReverse();
}

void CDib::SymHaarWaveletTrans( int scale )
{
	if (scale<1 || scale>5)
	{
		return;
	}
	
// 	if(m_pDibBitstemp!=NULL)
// 	{
// 		delete[] m_pDibBitstemp;
// 		m_pDibBitstemp=NULL;
// 	}
	this->m_nScale = scale;
	int lineByte=(m_width*pixelbyte+3)/4*4;
	m_pDibBitstemp=new unsigned char[linebytes*m_height];

 	int N= m_width;
 	int M= m_height;
	if (pCoeff != NULL)
	{
		delete[] pCoeff;
	}
 
	pCoeff = new double[M*N];
//	double* coeff1 = new double[M*N];
//	::Copy(m_pDibBits, M, N, coeff1, M, N);
//	::ReverseDIBBits(coeff1, M, N);

	this->Trans(m_pDibBits, m_height, m_width, lineByte, scale, m_pDibBitstemp);
	::ReverseDIBBits(m_pDibBitstemp, M, N);
	::Copy(m_pDibBitstemp, M, N, pCoeff, M, N);
	::ReverseDIBBits(pCoeff, M, N);
//	::CopyPositive(pCoeff,M, N,m_pDibBitstemp,M, N);
//	this->Trans(m_pDibBits, m_height, m_width, lineByte, scale, pCoeff);
//	memcpy(m_pDibBits,m_pDibBitstemp,lineByte*m_height);
//	delete[] coeff1;
// 	delete[] m_pDibBitstemp;
}

void CDib::SymHaarWaveletReverse()
{
	if (m_nScale <= 0)
	{
		return;
	}
// 	if (m_pDibBitstemp!=NULL)
// 	{
// 		return;
// 	}
	if (pCoeff == NULL)
	{
		return;
	}
// 	if (m_pDibBitstemp != NULL)
// 	{
// 		delete[] m_pDibBitstemp;
// 		m_pDibBitstemp=NULL;
// 	}
	int lineByte=(m_width*pixelbyte+3)/4*4;
 //	 m_pDibBitstemp=new unsigned char[linebytes*m_height];
	unsigned char * F_temp= new unsigned char [linebytes * m_height];
 	int N= m_width;
 	int M= m_height;

 	::ReverseDIBBits(pCoeff, M, N);
 	::CopyPositive(pCoeff,M, N,F_temp,M, N);
	::ReverseDIBBits(F_temp, M, N);
	this->Reverse(F_temp, m_height, m_width, lineByte, m_nScale, m_pDibBits);
// 	::ReverseDIBBits(m_pDibBitstemp, M, N);
// 	memcpy(m_pDibBits,m_pDibBitstemp,linebytes*m_width);
// 	memcpy(pCoeff,m_pDibBitstemp,lineByte*m_height);
// 
//  	::ReverseDIBBits(m_pDibBitstemp, M, N);
//  	::CopyPositive(pCoeff,M, N,m_pDibBitstemp,M, N);

}

void CDib::Sym8WaveletTransDisp(int scale)
{
	if (scale<1 || scale>5)
	{
		return;
	}
	
	if(m_pDibBitstemp!=NULL)
	{
		delete[] m_pDibBitstemp;
		m_pDibBitstemp=NULL;
	}
	this->m_nScale = scale;
	int lineByte=(m_width*pixelbyte+3)/4*4;
	m_pDibBitstemp=new unsigned char[linebytes*m_height];
	
	int N= m_width;
	int M= m_height;
	if (pCoeff != NULL)
	{
		delete[] pCoeff;
	}
	pCoeff = new double[M*N];
	double* coeff1 = new double[M*N];
	::Copy(m_pDibBits, M, N, coeff1, M, N);              //��m_pDibBits����coeff1
	::ReverseDIBBits(coeff1, M, N);
	if(Wavelet(coeff1, m_height,m_width, scale, 4,pCoeff, m_height, m_width))
	{
		::ReverseDIBBits(pCoeff, M, N);
		::CopyPositive(pCoeff,M, N,m_pDibBitstemp,M, N);
		memcpy(m_pDibBits,m_pDibBitstemp,lineByte*m_height);
	}
}

void CDib::Sym8WaveletReverseDisp()
{
	if (m_nScale <= 0)
	{
		return;
	}
	int lineByte=(m_width*pixelbyte+3)/4*4;

	int N= m_width;
	int M= m_height;

	if (pCoeff == NULL)
	{
		return;
	}
	::ReverseDIBBits(pCoeff, M, N);

//	double* coeff1 = new double[M*N];
//	::Copy(m_pDibBits, M, N, coeff1, M, N);              //��m_pDibBits����coeff1
//	::ReverseDIBBits(coeff1, M, N);                           //����coeff1�ķ���ͼ��ʧ�棬����pCoeff�ķ�������ʧ��
	if(InverseWavelet(pCoeff, m_height,m_width, m_nScale, 4,m_height,m_width))
	{
		::ReverseDIBBits(pCoeff, M, N);
		::CopyPositive(pCoeff,M, N,m_pDibBits,M, N);
//		memcpy(m_pDibBits,m_pDibBitstemp,lineByte*m_height);
	}
}

double CDib::NoiseVariance()
{
	//����ͼ����������
	double * F_temp= new double [linebytes * m_height];
	double  temp1=0.0;
	double  sum=0.0,mean;
	//����8λ�Ҷ�ͼ��
	if (pixelbyte==1)
	{
		//��������DIBλͼ���ֽ���		
		for(int i=0;i<m_height;i++)
		{
			for(int j=0;j<m_width;j++)
			{
				F_temp[i*m_width+j]=(double)(*(m_pDibBits+i*m_width+j)-*(m_pDibBitsOrig+i*m_width+j));
				sum=sum+(double)(*(m_pDibBits+i*m_width+j)-*(m_pDibBitsOrig+i*m_width+j));
			}
		}
		mean=sum/(m_height*m_width);
		for(i=0;i<m_height;i++)
		{
			for(int j=0;j<m_width;j++)
			{
				temp1=temp1+(F_temp[i*m_width+j]-mean) * (F_temp[i*m_width+j]-mean);
			}
		}
	}
	else
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
	}
	double Variance=sqrt(temp1/(m_height*m_width));
	delete  []F_temp;
// 	CString StrVariance;
// 	StrVariance.Format("%f",Variance);
// 	::MessageBox(NULL,StrVariance,"���������׼��",MB_OK);
	return Variance;
}

double CDib::NoiseDensity()
{
	//����ͼ�������ܶ�
	double  sum=0.0;
//	double mean;
	//����8λ�Ҷ�ͼ��
	if (pixelbyte==1)
	{
		for(int i=0;i<m_height;i++)
		{
			for(int j=0;j<m_width;j++)
			{
				if (*(m_pDibBits+i*m_width+j)!=*(m_pDibBitsOrig+i*m_width+j))
				{
					sum=sum+1;
				}
			}
		}
	}
	else
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
	}
	double   Density=sum/(m_height*m_width);     //sum�趨���double�ͣ��������ͳ������͵õ�0
    return Density;
}

double CDib::Noiseq(unsigned char * Bits,int t)
{
	//����ͼ����������
	unsigned char * m_pDibBitstemp1=new unsigned char[linebytes * m_height];
	this->Reverse(Bits, m_height/t, m_width/t, linebytes, m_nScale, m_pDibBitstemp1);
//	SymHaarWaveletReverse();
	double * F_temp= new double [linebytes * m_height];
//	unsigned char * F_temp2=new unsigned char[linebytes * m_height];
//	::Copy(Bits,M,N,F_temp2,m_height,m_width);
	double  temp1=0.0;
	double  sum=0.0,mean;
	//����8λ�Ҷ�ͼ��
	if (pixelbyte==1)
	{
		//��������DIBλͼ���ֽ���		
		for(int i=0;i<m_height;i++)
		{
			for(int j=0;j<m_width;j++)
			{
				F_temp[i*m_width+j]=(double)(*(m_pDibBitstemp1+i*m_width+j)-*(m_pDibBitsOrig+i*m_width+j));
				sum=sum+(double)(*(m_pDibBitstemp1+i*m_width+j)-*(m_pDibBitsOrig+i*m_width+j));
			}
		}
		mean=sum/(m_height*m_width);
		for(i=0;i<m_height;i++)
		{
			for(int j=0;j<m_width;j++)
			{
				temp1=temp1+(F_temp[i*m_width+j]-mean) * (F_temp[i*m_width+j]-mean);
			}
		}
	}
	else
	{
		AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
	}
	double q=sqrt(temp1/(m_height*m_width));
	delete  []F_temp;
	delete  []m_pDibBitstemp1;
//	delete  []F_temp2;
//	SymHaarWaveletTrans(scale);
	return q;
}