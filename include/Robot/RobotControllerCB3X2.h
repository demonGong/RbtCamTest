#pragma once
#include "robotcontrollercb3.h"

class EXT_CLASS CRobotControllerCB3X2 : public CRobotControllerCB3
{
public:
	CRobotControllerCB3X2(void);
	virtual ~CRobotControllerCB3X2(void);

public:
	void PackAnalysis();    //解析数据包
};

