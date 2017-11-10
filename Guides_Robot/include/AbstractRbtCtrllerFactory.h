#pragma once

#include "AbstractRobotController.h"

class EXT_CLASS CAbstractRbtCtrllerFactory
{
public:
	CAbstractRbtCtrllerFactory(void);
	virtual ~CAbstractRbtCtrllerFactory(void);

public:
	virtual CAbstractRobotController *createRbtCtrller(int type) = 0; 

};

