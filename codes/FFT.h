#ifndef _FFT_H
#define _FFT_H
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "StdAfx.h"
#include "CCImageDoc.h"

/* Handle to a DIB */
//DECLARE_HANDLE(HDIB);
void Bit_Reversal(unsigned  int *,int,int);
void GLT(float *,float *,int,int);
void CFFT(float *,float *,float *,float *,int,int);
void Normalize(float *,int len,float max);
void WINAPI FFT(float *R,float *I,int len,int judge);
BOOL ImageFFT(HDIB hDIB,HDIB *hFFTAbs,HDIB *hFFTPhs);
void ImageNormalize(float  *data,DWORD len,UINT max);

#endif
