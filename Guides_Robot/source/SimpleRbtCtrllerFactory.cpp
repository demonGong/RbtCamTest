#include "StdAfx.h"
#include "SimpleRbtCtrllerFactory.h"


CSimpleRbtCtrllerFactory::CSimpleRbtCtrllerFactory(void)
{
}


CSimpleRbtCtrllerFactory::~CSimpleRbtCtrllerFactory(void)
{
}

CAbstractRobotController *CSimpleRbtCtrllerFactory::createRbtCtrller(int type)
{
	CAbstractRobotController *m_RbtController = NULL;
	switch(type)
	{
	case VISION_CB2:
		m_RbtController = new CRobotControllerCB2();
		break;
	case VISION_CB3:
		m_RbtController = new CRobotControllerCB3();
		break;
	case VISION_CB3X2:
		m_RbtController = new CRobotControllerCB3X2();
	default:
		break;
	}
	return m_RbtController;

}