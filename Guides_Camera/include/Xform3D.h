/**************************************************************
*
*     Micron Tracker: Example C++ wrapper and Multi-platform demo
*   
*     Written by: 
*			Shahram Izadyar, Robarts Research Institute - London- Ontario , www.robarts.ca
*			Claudio Gatti, Claron Technology - Toronto -Ontario, www.clarontech.com
*
*	  Modified by:
*			Gregory Bootsma, Princess Margaret Hospital - Toronto - Ontario
*
*     Copyright Claron Technology 2000-2013
*
***************************************************************/
#ifndef __XFORM3D_H__
#define __XFORM3D_H__

#include "MTC.h"

class Xform3D
{

public:
	Xform3D(mtHandle h=0);
	~Xform3D();

	inline mtHandle getHandle() {return m_handle;};
	Xform3D* concatenate( Xform3D* subsequentXform);
	Xform3D* inverse();
	Xform3D* inBetween(Xform3D* secondXf, double secondFract0To1);

	double getShift(int index);
	void getShiftVector(double* s);
	void setShiftVector(double *input);
	void getRotateVector(double* resultVector, double* inVector, bool scaleIt = false);
	double	getQuaternion( int index );
	void	getQuaternionVector(double *q);
	mtMeasurementHazardCode		getHazardState();
	void getRotateMat(double *result);
	void setRotateMat(double *input);
private:
	mtHandle m_handle;
	bool ownedByMe;

};
#endif