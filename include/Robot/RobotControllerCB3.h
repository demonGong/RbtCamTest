#pragma once

#include "AbstractRobotController.h"

class EXT_CLASS CRobotControllerCB3: public CAbstractRobotController
{
public:
	CRobotControllerCB3(void);
	virtual ~CRobotControllerCB3(void);

public:

	virtual UINT InitialRobot(RobotParam *m_Parm = NULL);							 //��ʼ�������ˣ�������λ��������������������������������߳�
	virtual UINT SetRobotFreeDriveMode();								             // ���û�����ʾ��ģʽ teach_mode()
	virtual UINT SetRobotRunningMode();												 // ���û���������ģʽ end_teach_mode()
	virtual UINT CheckRbtState();												     // ��ȡ������ʵ��״̬����������ڲ�����
	virtual UINT MoveRobotStop();                                                    // ��е��ֹͣ�˶���������

	virtual void PackAnalysis();													 //�������ݰ�
	BOOL CheckSafetyMode(UINT state);												 //��ȡ��ǰ��е�۰�ȫ״̬
	UINT SetProtectedStopUnLock();                                                   //����е�۰�ȫ����״̬����Ϊ��ͨ��ȫ״̬

	static UINT UnlockRobotThreadProc(LPVOID pParam);								  //��е�۽����̣߳�Ŀǰset robotmode run��������ɣ���������

private:
	BOOL m_robotDriveMode;
};

