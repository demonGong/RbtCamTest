// Vector3f.cpp : implementation file
//
#include "stdafx.h"
#include "Vector3f.h"
//#include "global.h" 
#include "math.h" 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Vector3f
//�޲ι��캯�� 
Vector3f::Vector3f() 
{ 
	x=y=z=0; 
}

//�вι��캯�� 
Vector3f::Vector3f(const float inx,const float iny,const float inz) 
{ 
  //�ֱ𽫲�����ֵ��������Ա���� 
  x=inx; 
  y=iny; 
  z=inz;  
}

//�������� 
Vector3f::~Vector3f()
{

} 


//����=������,ʵ�������������ĸ�ֵ 
Vector3f& Vector3f::operator=(Vector3f& inVet) 
{ 
  x=inVet.x; 
  y=inVet.y; 
  z=inVet.z; 
  return *this; 
}

//����+������,ʵ����������������� 
Vector3f Vector3f::operator+(Vector3f vVector) 
{ 
  //������ӵĽ�� 
  return Vector3f(vVector.x + x, vVector.y + y, vVector.z + z); 
}

//����-������,ʵ����������������� 
Vector3f Vector3f::operator-(Vector3f vVector) 
{ 
  //��������Ľ�� 
  return Vector3f(x - vVector.x, y - vVector.y, z - vVector.z); 
}

//����*������,ʵ��һ������������һ���������ĳ˷� 
Vector3f Vector3f::operator*(float num) 
{ 
  //���������˵����� 
  return Vector3f(x * num, y * num, z * num); 
}

//����/������,ʵ��һ������������һ���������ĳ��� 
Vector3f Vector3f::operator/(float num) 
{ 
  //���������˵����� 
  return Vector3f(x / num, y / num, z / num); 
}
  
//������x����ת,����sinthetaΪ��ת�Ƕȵ�����ֵ,����costhetaΪ��ת�Ƕȵ�����ֵ 
void Vector3f::RotateX(float sintheta,float costheta) 
	 { 
	  float sin_beta,cos_bata; 
	  sin_beta=z*costheta +y*sintheta; 
	  cos_bata=y* costheta- z*sintheta; 
	  z=sin_beta; 
	  y=cos_bata; 
	 }

	 //������y����ת,����sinthetaΪ��ת�Ƕȵ�����ֵ,����costhetaΪ��ת�Ƕȵ�����ֵ 
void Vector3f::RotateY(float sintheta,float costheta) 
	 { 
	  float sin_beta,cos_bata; 
	  sin_beta=z*costheta +x*sintheta; 
	  cos_bata=x* costheta- z*sintheta; 
	  z=sin_beta; 
	  x=cos_bata; 
	 } 

		//������z����ת,����sinthetaΪ��ת�Ƕȵ�����ֵ,����costhetaΪ��ת�Ƕȵ�����ֵ 
void Vector3f::RotateZ(float sintheta,float costheta) 
	 { 
	  float sin_beta,cos_bata; 
	  sin_beta=y*costheta +x*sintheta; 
	  cos_bata=x* costheta- y*sintheta; 
	  y=sin_beta; 
	  x=cos_bata; 
	 }
  
	 //����һ������,����scaleΪ���ŵı��� 
void Vector3f::Zoom(float scale) 
	 { 
	  x*=scale; 
	  y*=scale; 
	  z*=scale; 
	 }
  
	 //ƽ��һ������ 
void Vector3f::Move(Vector3f inVect) 
	 { 
	  x+=inVect.x; 
	  y+=inVect.y; 
	  z+=inVect.z; 
	 }


//�õ��������Ĳ�� 
/* �йز�˵�˵��(���ֳ���:http://www.gameres.com/Articles/Program/Visual/Other/shiliang.htm) 
���:Vector1(x1,y1,z1),Vector2(x2,y2,z2): 
�����Ǹ�ʸ��. 
������Vector1,Vector2���ɵ�ƽ�淨��.��ʹ�����ֶ��� 
������Length(Vector1)*Length(Vector2)*sin(theta) 
theta��Vector1 & Vector2�ļн�. 
����,ƽ�е�ʸ����˽��Ϊ0ʸ��(��Ϊ0,��������) 
������ʸ��:(ox,oy,oz) 
ox = (y1 * z2) - (y2 * z1) 
oy = (z1 * x2) - (z2 * x1) 
oz = (x1 * y2) - (x2 * y1) 
��;:���㷨���� 
*/ 
Vector3f Vector3f::Cross(Vector3f vVector1, Vector3f vVector2) 
{ 
 //����һ�����ɲ�˽�������� 
 Vector3f vNormal; 

 //�õ���������X���ϵ�ͶӰֵ 
 vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y)); 

 //�õ���������Y���ϵ�ͶӰֵ 
 vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z)); 

 //�õ���������Z���ϵ�ͶӰֵ 
 vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x)); 

 //���ش����� 
 return vNormal; 
} 

//�õ�һ�������ľ��Գ��� 
float Vector3f::Magnitude(Vector3f vNormal) 
{ 
 return (float)sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z) ); 
} 

//��һ��������λ��
Vector3f Vector3f::Normalize(Vector3f vNormal) 
{ 
 //�õ��������ľ��Գ��� 
 float magnitude = Magnitude(vNormal); 

 //��ÿ�������ֱ���Դ˳��� 
 vNormal.x /= magnitude;  
 vNormal.y /= magnitude;  
 vNormal.z /= magnitude;  

 //���ش����� 
 return vNormal;  
} 

//�õ�һ�����������ƽ��Ĵ�ֱ����(������λ��) 
Vector3f Vector3f::Normal(Vector3f vPolygon[]) 
{ 
 //�õ������ߵ����� 
 Vector3f vVector1 = vPolygon[2] - vPolygon[0]; 
 Vector3f vVector2 = vPolygon[1] - vPolygon[0]; 

 //�õ����������Ĳ�� 
 Vector3f vNormal = Cross(vVector1, vVector2); 

 //��λ�� 
 vNormal.x = Normalize(vNormal).x;   
 vNormal.y = Normalize(vNormal).y; 
 vNormal.z = Normalize(vNormal).z; 
  
 //���ش˱��� 
 return vNormal;  
} 

//�õ������ľ��� 
float Vector3f::Distance(Vector3f vPoint1, Vector3f vPoint2) 
{ 
 double distance = sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) + 
          (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) + 
          (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) ); 

 return (float)distance; 
} 

//�õ��������ĵ�� 
/*�йص����˵��(���ֳ���:http://www.gameres.com/Articles/Program/Visual/Other/shiliang.htm) 
����ʸ���ĵ���Ǹ�����. 
��ѧ���������������ʸ�����������:W=|F|.|S|.cos(theta) 

��ʸ�����: 
Vector1:(x1,y1,z1) Vector2(x2,y2,z2) 
DotProduct=x1*x2+y1*y2+z1*z2 

����Ҫ��Ӧ��: 
1.���ʸ������: 
����������Ϥ��������: 
cos(theta)=F.S/(|F|.|S|) 
�����ж϶�ʸ���ķ������: cos=1ͬ��,cos=-1�෴,cos=0ֱ�� 
��������(Cull face)ʱ�ж���������Ƿ�ɼ�:(���ߺ�����ʸ���ķ�������)cos>0���ɼ�,cos<0�ɼ� 
OpenGL������ô���ġ� 

2.Lambert���������ǿ��Ҳ�õ��: 
Light=K.I.cos(theta) 
K,IΪ����,theta��ƽ�淨����������߼н� 
*/ 
float Vector3f::Dot(Vector3f vVector1, Vector3f vVector2) 
{ 
 return ( (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z) ); 
} 

//�õ��������ļн� 
double Vector3f::AngleBetweenVectors(Vector3f Vector1, Vector3f Vector2) 
{ 
 //�õ��������ĵ�� 
 float dotProduct = Dot(Vector1, Vector2); 

 //�õ����������ȵĳ˻� 
 float vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2) ; 

 //�õ��������н� 
 double angle = acos( dotProduct / vectorsMagnitude ); 

 //���ؽǶ�ֵ 
 return( angle ); 
}




  
