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
	//�޲ι��캯�� 
	Vector3f(); 
	//�вι��캯�� 
	Vector3f(const float inx,const float iny,const float inz); 
	//�������� 
	virtual ~Vector3f(); 


	 //����=������,ʵ�������������ĸ�ֵ 
	 Vector3f& operator=(Vector3f& inVet);

	 //����+������,ʵ����������������� 
	 Vector3f operator+(Vector3f vVector);

	 //����-������,ʵ����������������� 
	 Vector3f operator-(Vector3f vVector);

	 //����*������,ʵ��һ������������һ���������ĳ˷� 
	 Vector3f operator*(float num);

	 //����/������,ʵ��һ������������һ���������ĳ��� 
	 Vector3f operator/(float num);
  
	 //������x����ת,����sinthetaΪ��ת�Ƕȵ�����ֵ,����costhetaΪ��ת�Ƕȵ�����ֵ 
	 void RotateX(float sintheta,float costheta);

	 //������y����ת,����sinthetaΪ��ת�Ƕȵ�����ֵ,����costhetaΪ��ת�Ƕȵ�����ֵ 
	 void RotateY(float sintheta,float costheta);

		//������z����ת,����sinthetaΪ��ת�Ƕȵ�����ֵ,����costhetaΪ��ת�Ƕȵ�����ֵ 
	 void RotateZ(float sintheta,float costheta);

  
	 //����һ������,����scaleΪ���ŵı��� 
	 void Zoom(float scale);
  
	 //ƽ��һ������ 
	 void Move(Vector3f inVect);

public:  
	float x;//��Ա����x,������x���ϵķ��� 
	float y;//��Ա����y,������y���ϵķ��� 
	float z;//��Ա����z,������z���ϵķ��� 

	//�õ��������Ĳ�� 
	Vector3f Cross(Vector3f vVector1, Vector3f vVector2); 

	//�õ�һ�������ľ��Գ��� 
	float Magnitude(Vector3f vNormal); 

	//��һ��������λ�� 
	Vector3f Normalize(Vector3f vNormal); 

	//�õ�һ�����������ƽ��Ĵ�ֱ����(������λ��) 
	Vector3f Normal(Vector3f vPolygon[]); 

	//�õ��ռ�������ľ��� 
	float Distance(Vector3f vPoint1, Vector3f vPoint2); 

	//�õ��������ĵ�� 
	float Dot(Vector3f vVector1, Vector3f vVector2); 

	//�õ��������ļн� 
	double AngleBetweenVectors(Vector3f Vector1, Vector3f Vector2); 


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VECTOR3F_H__BE3EBCFF_9506_4551_A30D_F11B7D3027A0__INCLUDED_)
