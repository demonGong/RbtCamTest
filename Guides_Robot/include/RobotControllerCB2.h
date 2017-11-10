#pragma once
#include "AbstractRobotController.h"

class EXT_CLASS CRobotControllerCB2: public CAbstractRobotController
{
public:
	CRobotControllerCB2(void);
	~CRobotControllerCB2(void);

public:

	UINT RobotInitMove();												// ��е�۳�ʼ������ʼ��ʼ���߳�
	UINT CheckRbtState();											    // ��ȡ������ʵ��״̬����������ڲ�����
	virtual UINT SetRobotFreeDriveMode();								// ���û�����ʾ��ģʽ
	virtual UINT SetRobotRunningMode();									// ����������ģʽ runmode
	virtual UINT SetProtectedStopUnLock();								// ���û����˰�ȫ״̬
	virtual void PackAnalysis();					                    // �������ݰ�
																		
	static UINT RobotInitMoveThread(LPVOID pParam);	                    // ��е�۳�ʼ���̣߳�ÿ���ؽ��˶�һ��Сλ��

public:

	BOOL m_bRobotInitMoveThreadRunning;									//��е�۳�ʼ���߳�״̬
};

