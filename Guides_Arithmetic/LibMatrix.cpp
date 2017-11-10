///////////////////////////////////////////////////////////////
//	文 件 名 :LibMatrix.cpp
//	文件功能 :
//	作    者 : wrj
//	创建时间 : 2006年6月24日
//	项目名称 ：CANs系统
//	操作系统 : WinXP
//	备    注 :
//	历史记录： : 
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LibMatrix.h"
#include "stdlib.h"
#include "math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLibMatrix::CLibMatrix(void)
{

}

CLibMatrix::~CLibMatrix(void)
{

}


///////////////////////////////////////////////////////////////
//	函 数 名 : couUnit
//	所属类名 : CLibMatrix
//	函数功能 : 得到单位矩阵
//	作    者 : wrj
//	时    间 : 2006年6月24日
//	返 回 值 : void
//	参数说明 : double A[],  单位阵
//				 int len	方阵的阶
///////////////////////////////////////////////////////////////
void CLibMatrix::couUnit(double A[], int len)
{
	int i,j,k;
    for(i=0; i<len; i++)
	 for(j=0; j<len; j++)
	 {
	    k=i*len+j;
	    *(A+k)=0.0;
	    if(i==j) *(A+k)=1.0;
	 }
}

///////////////////////////////////////////////////////////////
//	函 数 名 : couArray
//	所属类名 : CLibMatrix
//	函数功能 : 矩阵相乘
//	作    者 : wrj
//	时    间 : 2006年6月24日
//	返 回 值 : void
//	参数说明 : double A[],		相乘矩阵 NxL
//				 double B[],	相乘矩阵 LxM
//				 double C[],	相乘结果 NxM
//				 int N,			
//				 int L,
//				 int M
///////////////////////////////////////////////////////////////
void CLibMatrix::couArray(double A[], double B[], double C[], int N, int L, int M)
{
	int i,j,k,ka,kb,kt;
	for(i=0; i<N; i++)
	{
	    for(j=0; j<M; j++)
		{
	        kt=i*M+j;
			*(C+kt)=0.0;
			for(k=0; k<L; k++)
			{
			  ka=i*L+k;
			  kb=k*M+j;
//**********&&&&&&&&&&&&&&&&&&&&&&&&&******************
			  *(C+kt)=(*(A+ka))*(*(B+kb))+(*(C+kt));  
//**********&&&&&&&&&&&&&&&&&&&&&&&&&******************
			}
	    }
	}
	
}

///////////////////////////////////////////////////////////////
//	函 数 名 : couInv
//	所属类名 : CLibMatrix
//	函数功能 : 矩阵求逆
//	处理过程 : 
//	备    注 : 满足二阶三阶求逆
//	作    者 : wrj
//	时    间 : 2006年6月24日
//	返 回 值 : double *		逆矩阵数组指针
//	参数说明 : double A[],	求逆矩阵
//				 int m,		矩阵行
//				 int n		矩阵列
///////////////////////////////////////////////////////////////
double * CLibMatrix::couInv(double A[], int m, int n)  
{
     int i,j,x,y,k; 
     double *SP=NULL,*AB=NULL,*B=NULL,X,*C; 
     SP=(double *)malloc(m*n*sizeof(double)); 
     AB=(double *)malloc(m*n*sizeof(double)); 
     B=(double *)malloc(m*n*sizeof(double)); 
     
     X=couSur(A,m,n); 
     X=1/X; 
     
     for(i=0;i<m;i++) 
     for(j=0;j<n;j++) 
     {for(k=0;k<m*n;k++) 
     B[k]=A[k]; 
     {for(x=0;x<n;x++) 
     B[i*n+x]=0; 
     for(y=0;y<m;y++) 
     B[m*y+j]=0; 
     B[i*n+j]=1; 
     SP[i*n+j]=couSur(B,m,n); 
     AB[i*n+j]=X*SP[i*n+j]; 
     } 
     } 
     C=couRev(AB,m,n);

	 delete SP;	delete AB;	delete B;
     
     return C; 


}

///////////////////////////////////////////////////////////////
//	函 数 名 : couRev
//	所属类名 : CLibMatrix
//	函数功能 : 矩阵转置
//	处理过程 : 
//	备    注 : 
//	作    者 : wrj
//	时    间 : 2006年6月24日
//	返 回 值 : double *		转置矩阵数组指针
//	参数说明 : double A[],  转置矩阵
//				 int m,		矩阵行		
//				 int n		矩阵列
///////////////////////////////////////////////////////////////
double * CLibMatrix::couRev(double A[], int m, int n) 
{
	 int i,j; 
     double *B=NULL; 
     B=(double *)malloc(m*n*sizeof(double)); 
     
     for(i=0;i<n;i++) 
     for(j=0;j<m;j++) 
     B[i*m+j]=A[j*n+i]; 
     
     return B; 

}


///////////////////////////////////////////////////////////////
//	函 数 名 : couSur
//	所属类名 : CLibMatrix
//	函数功能 : 求矩阵行列式
//	处理过程 : 
//	备    注 : 
//	作    者 : wrj
//	时    间 : 2006年6月24日
//	返 回 值 : double		行列式
//	参数说明 : double A[],	矩阵
//				 int m,		矩阵行
//				 int n		矩阵列
///////////////////////////////////////////////////////////////
double CLibMatrix::couSur(double A[], int m, int n)
{
	int i,j,k,p,r; 
	double X,temp=1,temp1=1,s=0,s1=0; 

	if(n==2) 
	{
		for(i=0;i<m;i++) 
			for(j=0;j<n;j++) 
		if((i+j)%2)	temp1*=A[i*n+j]; 
		else temp*=A[i*n+j]; 
		X=temp-temp1;
	} 
	else{ 
		for(k=0;k<n;k++) 
		{
			for(i=0,j=k;i<m,j<n;i++,j++) 
			temp*=A[i*n+j]; 
			if(m-i) 
				{for(p=m-i,r=m-1;p>0;p--,r--) 
			temp*=A[r*n+p-1];} 
			s+=temp; 
			temp=1; 
		} 

		for(k=n-1;k>=0;k--) 
		{
			for(i=0,j=k;i<m,j>=0;i++,j--) 
				temp1*=A[i*n+j]; 
			if(m-i) 
			{
				for(p=m-1,r=i;r<m;p--,r++) 
					temp1*=A[r*n+p];
			} 
			s1+=temp1; 
			temp1=1; 
		} 
		X=s-s1;

	} 

	return X; 

}




///////////////////////////////////////////////////////////////
//	函 数 名 : couInv4x4
//	所属类名 : CLibMatrix
//	函数功能 : 四阶矩阵求逆
//	作    者 : wrj
//	时    间 : 2006年6月24日
//	返 回 值 : void
//	参数说明 : double A[][4],		求逆矩阵（如计算成功保存求逆结果） 
///////////////////////////////////////////////////////////////
bool CLibMatrix::couInv4x4(double A[][4])
{
  int i,j,t;
  double g[4][8],k;

  for(i=0;i<4;i++)
   for(j=0;j<8;j++)
     if(j<4) g[i][j]=A[i][j];
        else if((j-4)==i) g[i][j]=1.0;
               else g[i][j]=0.0;

  for(i=0;i<4;i++)
   { j=i;
     while((j<4)&&(fabs(g[j][i])<0.001))  j++;
     if (j>=4) return FALSE;
     if (j!=i)
        for(t=0;t<8;t++)
          { k=g[i][t]; g[i][t]=g[j][t]; g[j][t]=k; }
     k=g[i][i];
     for(t=0;t<8;t++) g[i][t]=g[i][t]/k;
     for(j=0;j<4;j++)
       if(j!=i)
        { k=g[j][i];
          for(t=0;t<8;t++)
             g[j][t]=g[j][t]-g[i][t]*k;
        }
   }

  for(i=0;i<4;i++)
     for(j=0;j<4;j++)
		A[i][j]=g[i][j+4];

  return TRUE;
}

void CLibMatrix::SetElement(double A[][4],int i, int j, double val)
{
	if (j >= 4)
		return;
	A[i][j] = val;
}