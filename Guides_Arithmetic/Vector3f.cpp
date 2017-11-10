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
//无参构造函数 
Vector3f::Vector3f() 
{ 
	x=y=z=0; 
}

//有参构造函数 
Vector3f::Vector3f(const float inx,const float iny,const float inz) 
{ 
  //分别将参数赋值给三个成员变量 
  x=inx; 
  y=iny; 
  z=inz;  
}

//析构函数 
Vector3f::~Vector3f()
{

} 


//重载=操作符,实现两向量变量的赋值 
Vector3f& Vector3f::operator=(Vector3f& inVet) 
{ 
  x=inVet.x; 
  y=inVet.y; 
  z=inVet.z; 
  return *this; 
}

//重载+操作符,实现两向量变量的相加 
Vector3f Vector3f::operator+(Vector3f vVector) 
{ 
  //返回相加的结果 
  return Vector3f(vVector.x + x, vVector.y + y, vVector.z + z); 
}

//重载-操作符,实现两向量变量的相减 
Vector3f Vector3f::operator-(Vector3f vVector) 
{ 
  //返回相减的结果 
  return Vector3f(x - vVector.x, y - vVector.y, z - vVector.z); 
}

//重载*操作符,实现一个向量变量和一个浮点数的乘法 
Vector3f Vector3f::operator*(float num) 
{ 
  //返回缩放了的向量 
  return Vector3f(x * num, y * num, z * num); 
}

//重载/操作符,实现一个向量变量和一个浮点数的除法 
Vector3f Vector3f::operator/(float num) 
{ 
  //返回缩放了的向量 
  return Vector3f(x / num, y / num, z / num); 
}
  
//向量绕x轴旋转,参数sintheta为旋转角度的正弦值,参数costheta为旋转角度的余弦值 
void Vector3f::RotateX(float sintheta,float costheta) 
	 { 
	  float sin_beta,cos_bata; 
	  sin_beta=z*costheta +y*sintheta; 
	  cos_bata=y* costheta- z*sintheta; 
	  z=sin_beta; 
	  y=cos_bata; 
	 }

	 //向量绕y轴旋转,参数sintheta为旋转角度的正弦值,参数costheta为旋转角度的余弦值 
void Vector3f::RotateY(float sintheta,float costheta) 
	 { 
	  float sin_beta,cos_bata; 
	  sin_beta=z*costheta +x*sintheta; 
	  cos_bata=x* costheta- z*sintheta; 
	  z=sin_beta; 
	  x=cos_bata; 
	 } 

		//向量绕z轴旋转,参数sintheta为旋转角度的正弦值,参数costheta为旋转角度的余弦值 
void Vector3f::RotateZ(float sintheta,float costheta) 
	 { 
	  float sin_beta,cos_bata; 
	  sin_beta=y*costheta +x*sintheta; 
	  cos_bata=x* costheta- y*sintheta; 
	  y=sin_beta; 
	  x=cos_bata; 
	 }
  
	 //缩放一个向量,参数scale为缩放的比例 
void Vector3f::Zoom(float scale) 
	 { 
	  x*=scale; 
	  y*=scale; 
	  z*=scale; 
	 }
  
	 //平移一个向量 
void Vector3f::Move(Vector3f inVect) 
	 { 
	  x+=inVect.x; 
	  y+=inVect.y; 
	  z+=inVect.z; 
	 }


//得到两向量的叉乘 
/* 有关叉乘的说明(文字出处:http://www.gameres.com/Articles/Program/Visual/Other/shiliang.htm) 
叉乘:Vector1(x1,y1,z1),Vector2(x2,y2,z2): 
其结果是个矢量. 
方向是Vector1,Vector2构成的平面法线.再使用右手定则 
长度是Length(Vector1)*Length(Vector2)*sin(theta) 
theta是Vector1 & Vector2的夹角. 
所以,平行的矢量叉乘结果为0矢量(长为0,方向任意) 
计算结果矢量:(ox,oy,oz) 
ox = (y1 * z2) - (y2 * z1) 
oy = (z1 * x2) - (z2 * x1) 
oz = (x1 * y2) - (x2 * y1) 
用途:计算法向量 
*/ 
Vector3f Vector3f::Cross(Vector3f vVector1, Vector3f vVector2) 
{ 
 //定义一个容纳叉乘结果的向量 
 Vector3f vNormal; 

 //得到此向量在X轴上的投影值 
 vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y)); 

 //得到此向量在Y轴上的投影值 
 vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z)); 

 //得到此向量在Z轴上的投影值 
 vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x)); 

 //返回此向量 
 return vNormal; 
} 

//得到一个向量的绝对长度 
float Vector3f::Magnitude(Vector3f vNormal) 
{ 
 return (float)sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z) ); 
} 

//将一个向量单位化
Vector3f Vector3f::Normalize(Vector3f vNormal) 
{ 
 //得到此向量的绝对长度 
 float magnitude = Magnitude(vNormal); 

 //让每个分量分别除以此长度 
 vNormal.x /= magnitude;  
 vNormal.y /= magnitude;  
 vNormal.z /= magnitude;  

 //返回此向量 
 return vNormal;  
} 

//得到一个三点决定的平面的垂直向量(经过单位化) 
Vector3f Vector3f::Normal(Vector3f vPolygon[]) 
{ 
 //得到两条边的向量 
 Vector3f vVector1 = vPolygon[2] - vPolygon[0]; 
 Vector3f vVector2 = vPolygon[1] - vPolygon[0]; 

 //得到这两向量的叉乘 
 Vector3f vNormal = Cross(vVector1, vVector2); 

 //单位化 
 vNormal.x = Normalize(vNormal).x;   
 vNormal.y = Normalize(vNormal).y; 
 vNormal.z = Normalize(vNormal).z; 
  
 //返回此变量 
 return vNormal;  
} 

//得到两点间的距离 
float Vector3f::Distance(Vector3f vPoint1, Vector3f vPoint2) 
{ 
 double distance = sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) + 
          (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) + 
          (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) ); 

 return (float)distance; 
} 

//得到两向量的点积 
/*有关点积的说明(文字出处:http://www.gameres.com/Articles/Program/Visual/Other/shiliang.htm) 
两个矢量的点积是个标量. 
中学物理的力做功就是矢量点积的例子:W=|F|.|S|.cos(theta) 

二矢量点积: 
Vector1:(x1,y1,z1) Vector2(x2,y2,z2) 
DotProduct=x1*x2+y1*y2+z1*z2 

很重要的应用: 
1.求二矢量余弦: 
由我们最熟悉的力做功: 
cos(theta)=F.S/(|F|.|S|) 
可以判断二矢量的方向情况: cos=1同向,cos=-1相反,cos=0直角 
曲面消隐(Cull face)时判断物体表面是否可见:(法线和视线矢量的方向问题)cos>0不可见,cos<0可见 
OpenGL就是这么做的。 

2.Lambert定理求光照强度也用点积: 
Light=K.I.cos(theta) 
K,I为常数,theta是平面法线与入射光线夹角 
*/ 
float Vector3f::Dot(Vector3f vVector1, Vector3f vVector2) 
{ 
 return ( (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z) ); 
} 

//得到两向量的夹角 
double Vector3f::AngleBetweenVectors(Vector3f Vector1, Vector3f Vector2) 
{ 
 //得到两向量的点积 
 float dotProduct = Dot(Vector1, Vector2); 

 //得到两向量长度的乘积 
 float vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2) ; 

 //得到两向量夹角 
 double angle = acos( dotProduct / vectorsMagnitude ); 

 //返回角度值 
 return( angle ); 
}




  
