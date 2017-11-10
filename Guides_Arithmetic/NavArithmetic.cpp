#include "StdAfx.h"
#include "NavArithmetic.h"

#include "Vector3f.h"
#include "LibMatrix.h"


CNavArithmetic::CNavArithmetic(void)
{
}


CNavArithmetic::~CNavArithmetic(void)
{
}


void CNavArithmetic::MMM_VS1toVS2(float in_VS1[][3], float in_VS2[][3], float out_Mateix[][4])
{
	Vector3f  v1,v2,v3;	//仿射矢量
	Vector3f  ex,ey,ez;	//单位方向量

	CLibMatrix  mtx;

	//S1 --> VS1
	v1.x=in_VS1[1][0]-in_VS1[0][0];	v1.y=in_VS1[1][1]-in_VS1[0][1];	v1.z=in_VS1[1][2]-in_VS1[0][2];
	v2.x=in_VS1[2][0]-in_VS1[0][0];	v2.y=in_VS1[2][1]-in_VS1[0][1];	v2.z=in_VS1[2][2]-in_VS1[0][2];
	v3.x=in_VS1[3][0]-in_VS1[0][0];	v3.y=in_VS1[3][1]-in_VS1[0][1];	v3.z=in_VS1[3][2]-in_VS1[0][2];

	ex =v1.Normalize(v1);

	ey =ey.Cross(v1,v2);
	ey =ey.Normalize(ey);

	ez =ez.Cross(ex,ey);
	ez =ez.Normalize(ez);

	double T1[4][4];

	T1[0][0]=ex.x;		T1[0][1]=ey.x;		T1[0][2]=ez.x;		T1[0][3]=in_VS1[0][0];
	T1[1][0]=ex.y;		T1[1][1]=ey.y;		T1[1][2]=ez.y;		T1[1][3]=in_VS1[0][1];
	T1[2][0]=ex.z;		T1[2][1]=ey.z;		T1[2][2]=ez.z;		T1[2][3]=in_VS1[0][2];
	T1[3][0]=0;			T1[3][1]=0;			T1[3][2]=0;			T1[3][3]=1;


	//得到转换矩阵T1
	mtx.couInv4x4(T1);  //对矩阵T1求逆


	//VS2 --> S2
	v1.x=in_VS2[1][0]-in_VS2[0][0];	v1.y=in_VS2[1][1]-in_VS2[0][1];	v1.z=in_VS2[1][2]-in_VS2[0][2];
	v2.x=in_VS2[2][0]-in_VS2[0][0];	v2.y=in_VS2[2][1]-in_VS2[0][1];	v2.z=in_VS2[2][2]-in_VS2[0][2];
	v3.x=in_VS2[3][0]-in_VS2[0][0];	v3.y=in_VS2[3][1]-in_VS2[0][1];	v3.z=in_VS2[3][2]-in_VS2[0][2];

	ex =v1.Normalize(v1);

	ey =ey.Cross(v1,v2);
	ey =ey.Normalize(ey);

	ez =ez.Cross(ex,ey);
	ez =ez.Normalize(ez);


	double T2[4][4];

	//得到转换矩阵T2

	T2[0][0]=ex.x;		T2[0][1]=ey.x;		T2[0][2]=ez.x;		T2[0][3]=in_VS2[0][0];
	T2[1][0]=ex.y;		T2[1][1]=ey.y;		T2[1][2]=ez.y;		T2[1][3]=in_VS2[0][1];
	T2[2][0]=ex.z;		T2[2][1]=ey.z;		T2[2][2]=ez.z;		T2[2][3]=in_VS2[0][2];
	T2[3][0]=0;			T2[3][1]=0;			T2[3][2]=0;			T2[3][3]=1;


	//S1 --> S2  转换矩阵
	double A[16], B[16], C[16];

	int i,j;
	for ( i=0; i<4; i++ )
		for ( j=0;j<4;j++ )
		{
			A[i*4+j] = T1[i][j];
			B[i*4+j] = T2[i][j];
		}
		mtx.couArray(B,A,C,4,4,4);

		for ( i=0; i<4; i++ )
			for (j=0;j<4;j++)
			{
				out_Mateix[i][j] =(float)C[i*4+j];
			}

}



// MakeMappedMatrix：  Space 1 --> Space 2   仿射变换
void CNavArithmetic::MDD_VS1toVS2(float in_VS1[][3], float in_VS2[][3], float out_Mateix[][4])
{
	Vector3f  v0,v1,v2,v3;	//仿射矢量
	Vector3f  ex,ey,ez;	//单位方向量

	CLibMatrix  mtx;

	//S1 --> VS1
	v1.x=in_VS1[1][0]-in_VS1[0][0];	v1.y=in_VS1[1][1]-in_VS1[0][1];	v1.z=in_VS1[1][2]-in_VS1[0][2];
	v2.x=in_VS1[2][0]-in_VS1[0][0];	v2.y=in_VS1[2][1]-in_VS1[0][1];	v2.z=in_VS1[2][2]-in_VS1[0][2];
	v3.x=in_VS1[3][0]-in_VS1[0][0];	v3.y=in_VS1[3][1]-in_VS1[0][1];	v3.z=in_VS1[3][2]-in_VS1[0][2];
	v0.x = in_VS1[0][0];   v0.y = in_VS1[0][1];   v0.z = in_VS1[0][2];  

	double T1[4][4];

	T1[0][0]=v1.x;		T1[0][1]=v2.x;		T1[0][2]=v3.x;		T1[0][3]=v0.x;
	T1[1][0]=v1.y;		T1[1][1]=v2.y;		T1[1][2]=v3.y;		T1[1][3]=v0.y;
	T1[2][0]=v1.z;		T1[2][1]=v2.z;		T1[2][2]=v3.z;		T1[2][3]=v0.z;
	T1[3][0]=0;			T1[3][1]=0;			T1[3][2]=0;			T1[3][3]=1;


	//得到转换矩阵T1
	mtx.couInv4x4(T1);  //对矩阵T1求逆


	//VS2 --> S2
	v1.x=in_VS2[1][0]-in_VS2[0][0];	v1.y=in_VS2[1][1]-in_VS2[0][1];	v1.z=in_VS2[1][2]-in_VS2[0][2];
	v2.x=in_VS2[2][0]-in_VS2[0][0];	v2.y=in_VS2[2][1]-in_VS2[0][1];	v2.z=in_VS2[2][2]-in_VS2[0][2];
	v3.x=in_VS2[3][0]-in_VS2[0][0];	v3.y=in_VS2[3][1]-in_VS2[0][1];	v3.z=in_VS2[3][2]-in_VS2[0][2];
	v0.x = in_VS2[0][0];   v0.y = in_VS2[0][1];   v0.z = in_VS2[0][2];  


	double T2[4][4];

	//得到转换矩阵T2
	T2[0][0]=v1.x;		T2[0][1]=v2.x;		T2[0][2]=v3.x;		T2[0][3]=v0.x;
	T2[1][0]=v1.y;		T2[1][1]=v2.y;		T2[1][2]=v3.y;		T2[1][3]=v0.y;
	T2[2][0]=v1.z;		T2[2][1]=v2.z;		T2[2][2]=v3.z;		T2[2][3]=v0.z;
	T2[3][0]=0;			T2[3][1]=0;			T2[3][2]=0;			T2[3][3]=1;


	//S1 --> S2  转换矩阵
	double A[16], B[16], C[16];

	int i,j;
	for ( i=0; i<4; i++ )
		for ( j=0;j<4;j++ )
		{
			A[i*4+j] = T1[i][j];
			B[i*4+j] = T2[i][j];
		}
		mtx.couArray(B,A,C,4,4,4);

		for ( i=0; i<4; i++ )
			for (j=0;j<4;j++)
			{
				out_Mateix[i][j] =(float)C[i*4+j];
			}

}

