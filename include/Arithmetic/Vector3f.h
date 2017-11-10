#if !defined(AFX_VECTOR3F_H__BE3EBCFF_9506_4551_A30D_F11B7D3027A0__INCLUDED_)
#define AFX_VECTOR3F_H__BE3EBCFF_9506_4551_A30D_F11B7D3027A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Vector3f.h : header file
//

//#ifdef NAVARITHMETIC_EXPORTS
//#define NAVARITHMETIC_API __declspec(dllexport)
//#else
//#define NAVARITHMETIC_API __declspec(dllimport)
//#endif
/////////////////////////////////////////////////////////////////////////////
// Vector3f window

class AFX_EXT_CLASS Vector3f 
{
public: 
	//无参构造函数 
	Vector3f(); 
	//有参构造函数 
	Vector3f(const float inx,const float iny,const float inz); 
	//析构函数 
	virtual ~Vector3f(); 


	 //重载=操作符,实现两向量变量的赋值 
	 Vector3f& operator=(Vector3f& inVet);

	 //重载+操作符,实现两向量变量的相加 
	 Vector3f operator+(Vector3f vVector);

	 //重载-操作符,实现两向量变量的相减 
	 Vector3f operator-(Vector3f vVector);

	 //重载*操作符,实现一个向量变量和一个浮点数的乘法 
	 Vector3f operator*(float num);

	 //重载/操作符,实现一个向量变量和一个浮点数的除法 
	 Vector3f operator/(float num);
  
	 //向量绕x轴旋转,参数sintheta为旋转角度的正弦值,参数costheta为旋转角度的余弦值 
	 void RotateX(float sintheta,float costheta);

	 //向量绕y轴旋转,参数sintheta为旋转角度的正弦值,参数costheta为旋转角度的余弦值 
	 void RotateY(float sintheta,float costheta);

		//向量绕z轴旋转,参数sintheta为旋转角度的正弦值,参数costheta为旋转角度的余弦值 
	 void RotateZ(float sintheta,float costheta);

  
	 //缩放一个向量,参数scale为缩放的比例 
	 void Zoom(float scale);
  
	 //平移一个向量 
	 void Move(Vector3f inVect);

public:  
	float x;//成员变量x,向量在x轴上的分量 
	float y;//成员变量y,向量在y轴上的分量 
	float z;//成员变量z,向量在z轴上的分量 

	//得到两向量的叉乘 
	Vector3f Cross(Vector3f vVector1, Vector3f vVector2); 

	//得到一个向量的绝对长度 
	float Magnitude(Vector3f vNormal); 

	//将一个向量单位化 
	Vector3f Normalize(Vector3f vNormal); 

	//得到一个三点决定的平面的垂直向量(经过单位化) 
	Vector3f Normal(Vector3f vPolygon[]); 

	//得到空间中两点的距离 
	float Distance(Vector3f vPoint1, Vector3f vPoint2); 

	//得到两向量的点积 
	float Dot(Vector3f vVector1, Vector3f vVector2); 

	//得到两向量的夹角 
	double AngleBetweenVectors(Vector3f Vector1, Vector3f Vector2); 


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VECTOR3F_H__BE3EBCFF_9506_4551_A30D_F11B7D3027A0__INCLUDED_)
