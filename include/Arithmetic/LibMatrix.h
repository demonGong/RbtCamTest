///////////////////////////////////////////////////////////////
//	文 件 名 :LibMatrix.h
//	文件功能 :
//	作    者 : wrj
//	创建时间 : 2006年6月24日
//	项目名称 ：CANs系统
//	操作系统 : WinXP
//	备    注 :
//	历史记录： : 
///////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIXOP_H__946687A2_15DE_11D8_8BF5_00055DFE509B__INCLUDED_)
#define AFX_MATRIXOP_H__946687A2_15DE_11D8_8BF5_00055DFE509B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#ifdef NAVARITHMETIC_EXPORTS
//#define NAVARITHMETIC_API __declspec(dllexport)
//#else
//#define NAVARITHMETIC_API __declspec(dllimport)
//#endif

class  AFX_EXT_CLASS CLibMatrix  
{
public:
	double couSur(double A[], int m, int n);
	double * couRev(double A[], int m, int n);
	double * couInv(double A[],int m,int n);  //3x3求逆
	bool couInv4x4(double A[4][4]);
	void couArray(double A[], double B[], double C[], int N, int L, int M);
	void couUnit(double A[], int len);
	
	void SetElement(double A[][4], int i, int j, double val);

	CLibMatrix(void);
	virtual ~CLibMatrix(void);

};

#endif // !defined(AFX_MATRIXOP_H__946687A2_15DE_11D8_8BF5_00055DFE509B__INCLUDED_)
