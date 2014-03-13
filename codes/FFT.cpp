#include  "stdafx.h"
#include  "stdio.h"
#include  "stdlib.h"

#include "fft.h"

HGLOBAL WINAPI CopyHandle (HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	DWORD dwLen = ::GlobalSize((HGLOBAL) h);
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);

	if (hCopy != NULL)
	{
		void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
		void* lp     = ::GlobalLock((HGLOBAL) h);
		memcpy(lpCopy, lp, dwLen);
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}

void WINAPI FFT(float *R,float *I,int len,int judge)
{

/*  Allocating memory for bit reversal LUT.  */
    int m=(int)(log10((float)len)/log10((float)2.0));
		 unsigned int*L=(unsigned int *)malloc(len*sizeof(unsigned int));
	    
	 Bit_Reversal(L,m,len);
/*  Allocating memory for FFT arrays.        */
     float *xr=(float *)GlobalAlloc(GPTR,len*sizeof(float));
     float *xi=(float *)GlobalAlloc(GPTR,len*sizeof(float));
/*  Initialize xr and xi.                    */
     
	 	 
	 for( int i=0;i<len;i++)
	 {
	     int k=L[i];
	     xr[k]=R[i];
	     xi[k]=I[i];
	 }
     int  n2=(len>>1);
     float *wr=(float *)GlobalAlloc(GPTR,n2*sizeof(float));
     float *wi=(float *)GlobalAlloc(GPTR,n2*sizeof(float));

     GLT(wr,wi,len,judge);
	 CFFT(xr,xi,wr,wi,m,len);
	 for(i=0;i<len;i++)
	 {
		 R[i]=xr[i];
		 I[i]=xi[i];
	 }
	// GlobalFree(wr);
	 //GlobalFree(wi);
	 //GlobalFree(xr);
	 //GlobalFree(xi);
}

/***************************************************************************/
/*       Name:  Bit_Reversal().                                            */
/*   Function:  Generating LUT for bit reversal.                           */
/* Parameters:  L-array stores LUT.                                        */
/*              N-LUT size(=2 to the pow of m).                            */
/***************************************************************************/

void Bit_Reversal(unsigned int *L,int m1,int N)
{
  unsigned int MASK,C,A,j;
  int k,i;
    for(k=0;k<N;k++) {
		       MASK=1;
		       C=0;
		       for(i=0,j=m1-1;i<m1;i++,j--) {
			A=(k&MASK)>>i;
			A<<=j;
			C|=A;
			MASK=MASK<<1;
		       }
	  L[k]=C;
    }
}

/**************************************************************************
	  Name:GLT().
      Function:Generating LUT for twinddle factors.
    Parameters:wr,wi-arrays store cos and sin look-up table.
		   N-LUT size(=2 to the pow of m).
		sign-sign=-1 for FFT ,and sign=1 for IFFT.
***************************************************************************/

void GLT(float *wr,float *wi,int N,int sign)
{
 int n2,i;
 float theta;
	 n2=(N>>1)-1;
	 /* Generating LUT for twinddle factors.  */
	 theta=(float)(2.0*pi/((float)N));
	 for(i=0;i<=n2;i++)  {
		    wr[i]=(float)cos((float)((i+1)*theta));
		    wi[i]=(float)sin((float)((i+1)*theta));
		    if (sign==(int)(-1))  wi[i]=-wi[i];
	 }
 }

 /************************************************************************
	Name:FFT()
    Function:FFT using decimation-in-time algrithm.
  Parameters: xr,xi--FFT arrays for store real and image. The input arrays
		     are assumed to be rearranged in bit_revers order.
	      wr,wi--array store the twinddle factor.
		  N--LUT size(=2 to the pow of m).
**************************************************************************/

void CFFT(float *xr,float *xi,float *wr,float *wi,int m1,int N)
{
 int ip,k,kk,l,incr,iter,j,i;
 float Tr,Ti;
       ip=1;
       kk=(N>>1);
       incr=2;

       for(iter=0;iter<m1;iter++) {
	  for(j=0;j<N;j+=incr) {
		 i=j+ip;
		 Tr=xr[i];
		 Ti=xi[i];
		 xr[i]=xr[j]-Tr;
		 xi[i]=xi[j]-Ti;

		 xr[j]=xr[j]+Tr;
		 xi[j]=xi[j]+Ti;
	  }
	  if(iter!=0)  {
	      for (k=1;k<ip;k++) {
		       l=k*kk-1;
		   for(j=k;j<N;j+=incr) {
			      i=j+ip;
			      Tr=xr[i]*wr[l]-xi[i]*wi[l];
			      Ti=xr[i]*wi[l]+xi[i]*wr[l];

			      xr[i]=xr[j]-Tr;
			      xi[i]=xi[j]-Ti;

			      xr[j]=xr[j]+Tr;
			      xi[j]=xi[j]+Ti;
		   }
	      }
        
       }
       kk>>=1;
       ip<<=1;

       incr<<=1;
      }
}

void Normalize(float *data,int len,float max)
{
	float fMax;
	float fMin;
	float fCol;
	fMax=0;
	for(int i=0;i<len;i++)
	{
		if(fMax<data[i])
			fMax=data[i];
	}
	fMin=fMax;
	for(i=0;i<len;i++)
	{
		if(fMin>data[i])
			fMin=data[i];
	}
	
	fCol=max/(fMax-fMin);
	for(i=0;i<len;i++)
	{
		data[i]=(data[i]-fMin)*fCol;
		if(data[i]>max)
			data[i]=max;
	}
}

BOOL ImageFFT(HDIB hDIB,HDIB *hnFFTAbs,HDIB *hnFFTPhs)
{
	HGLOBAL hData;

	HDIB hFFTAbs,hFFTPhs;
	HDIB hFFTDIB;
	if (hDIB == NULL)
		return FALSE;

	hFFTDIB=(HDIB)::CopyHandle(hDIB);
	unsigned char* ptrDIB=(unsigned char*)GlobalLock((HGLOBAL)hFFTDIB);

	LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
	
	DWORD dwDIBSize;

	lpBI = (LPBITMAPINFOHEADER) ptrDIB;
	if (lpBI == NULL)
		return FALSE;

	
	dwDIBSize = *(LPDWORD)lpBI + 256*sizeof(RGBQUAD);

	DWORD dwImageWidth,dwImageHeight;

	dwImageWidth=lpBI->biWidth;
	dwImageHeight=lpBI->biHeight;	

	//fft width should be pow(2,n)
	DWORD fftWidth;
	if(dwImageWidth>dwImageHeight)
		fftWidth=dwImageWidth;
	else fftWidth=dwImageHeight;
    int nPower=1;
    while(nPower<fftWidth)
		nPower*=2;
	fftWidth=nPower;

    

	DWORD dwActImageWidth;
	if(lpBI->biWidth%4!=0)
		dwActImageWidth=lpBI->biWidth+4-lpBI->biWidth%4;
	else
		dwActImageWidth=lpBI->biWidth;

	//if((hData=::GlobalAlloc(GHND,dwActImageWidth*dwImageHeight*sizeof(float)))==NULL)
	//	return FALSE;

	// ptrData  中间数据,补零
	if((hData=::GlobalAlloc(GHND,fftWidth*fftWidth*sizeof(float)))==NULL)
		return FALSE;
	float* ptrData;
	float* fRData;
	float* fIData;

	
	ptrData=(float *)::GlobalLock(hData);
	for(DWORD i=0;i<dwImageHeight;i++)
		for(DWORD j=0;j<dwImageWidth;j++)
		{
			ptrData[j+i*fftWidth]=(float)ptrDIB[dwDIBSize+j+i*dwActImageWidth];
		}


	HGLOBAL hRData;
	HGLOBAL hIData;
	float* ptrRData;
	float* ptrIData;
/*
	if((hRData=::GlobalAlloc(GHND,dwImageWidth*dwImageHeight*sizeof(float)))==NULL)
		return FALSE;
	if((hIData=::GlobalAlloc(GHND,dwImageWidth*dwImageHeight*sizeof(float)))==NULL)
		return FALSE;

*/
	 hFFTAbs=(HDIB)::GlobalAlloc(GHND,fftWidth*fftWidth+dwDIBSize);
//FFT计算时的实部与虚部数据
	if((hRData=::GlobalAlloc(GHND,fftWidth*fftWidth*sizeof(float)))==NULL)
		return FALSE;
	if((hIData=::GlobalAlloc(GHND,fftWidth*fftWidth*sizeof(float)))==NULL)
		return FALSE;

	ptrRData=(float *)::GlobalLock(hRData);
	ptrIData=(float *)::GlobalLock(hIData);

	//每行的实部与虚部数据
	fRData=new float[fftWidth];//[dwImageWidth];
	fIData=new float[fftWidth];//[dwImageWidth];

	
//计算行FFT
	for(i=0;i<dwImageHeight;i++)
	{
		for(int fi=0;fi<fftWidth;fi++)
		{
			fRData[fi]=0;
			fIData[fi]=0;
		}
		for(DWORD j=0;j<dwImageWidth;j++)
		{
			if((i+j)%2==0)
				fRData[j]=ptrData[j+i*fftWidth]; //每行宽度为fftWidth，但有效数据仅为dwImageWidth
			else
				fRData[j]=-ptrData[j+i*fftWidth];  //??

			//fIData[j]=0;
		}
		::FFT(fRData,fIData,fftWidth,-1);

	
		for(j=0;j<fftWidth;j++)
		{
			ptrRData[j+i*fftWidth]=fRData[j];
			ptrIData[j+i*fftWidth]=fIData[j];
		}
	}
	
//转置
	
	
//列 FFT
	for(i=0;i<fftWidth;i++)
	{
		for(int fi=0;fi<fftWidth;fi++)
		{
			fRData[fi]=0;
			fIData[fi]=0;
		}
		for(DWORD j=0;j<fftWidth;j++)
		{
			fRData[j]=ptrRData[i+j*fftWidth];
			fIData[j]=ptrIData[i+j*fftWidth];
		}
		::FFT(fRData,fIData,fftWidth,-1);
		for(j=0;j<fftWidth;j++)
		{
			ptrRData[i+j*fftWidth]=fRData[j];
			ptrIData[i+j*fftWidth]=fIData[j];

			float fValue=(float)(atan2(fIData[j],fRData[j])+pi);
			if(fValue<0)
				fValue=0;
			ptrData[i+j*fftWidth]=fValue;

		}
	}
	ImageNormalize(ptrData,fftWidth*fftWidth,255);

	GlobalUnlock(hFFTDIB);
	HDIB hReFFTDIB=(HDIB)GlobalReAlloc(hFFTDIB,dwDIBSize+fftWidth*fftWidth,GMEM_ZEROINIT);
	
	if(hReFFTDIB==NULL)
		return NULL;
	ptrDIB=(unsigned char *)GlobalLock(hReFFTDIB);
		lpBI = (LPBITMAPINFOHEADER) ptrDIB;
		lpBI->biHeight=fftWidth;
		lpBI->biWidth=fftWidth;
		lpBI->biSizeImage=fftWidth*fftWidth;

	for(i=0;i<fftWidth;i++)
	{
		for(DWORD j=0;j<fftWidth;j++)
		{
			ptrDIB[dwDIBSize+j+i*fftWidth]=(unsigned char)ptrData[j+i*fftWidth];
		}
	}

    hFFTPhs=(HDIB)GlobalAlloc(GHND,fftWidth*fftWidth+dwDIBSize);

	LPSTR lpPhs=(LPSTR)GlobalLock(hFFTPhs);
	::memcpy(lpPhs,ptrDIB,::GlobalSize(hReFFTDIB));

	for(i=0;i<fftWidth;i++)
	{
		for(DWORD j=0;j<fftWidth;j++)
		{
			float fValue=(float)sqrt(ptrRData[j+i*fftWidth]*ptrRData[j+i*fftWidth]+
				ptrIData[j+i*fftWidth]*ptrIData[j+i*fftWidth]);
			if(fValue<1)
			{
				fValue=1;
			}
			ptrData[j+i*fftWidth]=(float)log10(1+fabs(fValue));
		}
	}
	ImageNormalize(ptrData,fftWidth*fftWidth,255);
	for(i=0;i<fftWidth;i++)
	{
		for(DWORD j=0;j<fftWidth;j++)
		{
			ptrDIB[dwDIBSize+j+i*fftWidth]=(unsigned char)ptrData[j+i*fftWidth];
		}
	}
	
	


   // HDIB hnFFTAbs=(HDIB)GlobalAlloc(GHND,fftWidth*fftWidth+dwDIBSize);

	LPSTR lpAbs=(LPSTR)GlobalLock(hFFTAbs);
	::memcpy(lpAbs,ptrDIB,::GlobalSize(hReFFTDIB));

	::GlobalUnlock((HGLOBAL)hDIB);
	::GlobalUnlock(hReFFTDIB);
	::GlobalFree(hReFFTDIB);
	
	::GlobalUnlock(hData);
	::GlobalFree(hData);
	::GlobalUnlock(hRData);
	::GlobalFree(hRData);
	::GlobalUnlock(hIData);
	::GlobalFree(hIData);

	::GlobalUnlock(hFFTPhs);
	::GlobalUnlock(hFFTAbs);
     *hnFFTPhs=hFFTPhs;
	 *hnFFTAbs=hFFTAbs;
	return TRUE;	
}

void ImageNormalize(float far *data,DWORD len,UINT max)
{
	float fMax;
	float fMin;
	float fCol;
	float fmax=(float)max;
	fMax=0;
	for(DWORD i=0;i<len;i++)
	{
		if(fMax<data[i])
			fMax=data[i];
	}
	fMin=fMax;
	for(i=0;i<len;i++)
	{
		if(fMin>data[i])
			fMin=data[i];
	}
	
	fCol=fmax/(fMax-fMin);
	for(i=0;i<len;i++)
	{
		float fData=data[i];
		fData=(float)fabs((fData-fMin)*fCol);
		if(fData>fmax)
			fData=fmax;
		data[i]=fData;
	}
}
