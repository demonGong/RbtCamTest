#pragma once

#include "AbstractRbtCtrllerFactory.h"
#include "RobotControllerCB2.h"
#include "RobotControllerCB3.h"
#include "RobotControllerCB3X2.h"


class EXT_CLASS CSimpleRbtCtrllerFactory :public CAbstractRbtCtrllerFactory
{
public:
	CSimpleRbtCtrllerFactory(void);
	virtual ~CSimpleRbtCtrllerFactory(void);

public:
	CAbstractRobotController *createRbtCtrller(int type);
};

