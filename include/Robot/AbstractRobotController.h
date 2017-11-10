#pragma once

#include "RobotParam.h"
#include "myClientSocket.h"
#include <afxmt.h>
#include <math.h>

class EXT_CLASS CAbstractRobotController
{
public:
	CAbstractRobotController(void);
	virtual ~CAbstractRobotController(void);

public:
	
	////////////////////////////��ȡ���������ó�ʼ�������˵�///////////////////
	CString GetAppPath();									                           //��ó���·��		
	CString GetIniFileName();                                                          //��û����������ļ�·��
	void getParaFromINIFlie(const char *iniFilePath);                                  //��������ļ��Ĳ���
	virtual UINT InitialRobot(RobotParam *m_Parm = NULL);                              //��ʼ����е��
	virtual UINT DisConnectRobot();													   //��е������λ���Ͽ�����
	
	/////////////////////////// ���ص�����ػ����� /////////////////////////////////
	virtual UINT RobotPowerOn();													   // ��е���ϵ�
	virtual UINT RobotBrakeRelease();												   // ��е�۽���
	virtual UINT RobotPowerOff();													   // ��е�۹ص�
	virtual UINT RobotPowerDown();												       // ��е�۹ػ�

 	//////////////////////////// ��е���˶����� //////////////////////////////////
	
	// ����targetλ�ã����⣬������˶������ؽ�λ�� ����double[6]����ָ�� ���� ���ȿ������ٶȡ����ٶȡ��˶�ʱ��(�����ÿ�����Ϊ0)
	virtual UINT MoveRobotInverse(double * target,
								  double acc = JOINT_MOVE_ACC, 
								  double speed = JOINT_MOVE_SPEED, 
								  double time = JOINT_MOVE_TIME);	      
	// ��������ؽڵ�λ�ã��˶�����λ�� ����double[6]����ָ�� �������ٶȡ����ٶȡ��˶�ʱ��(�����ÿ�����Ϊ0)
	virtual UINT MoveRobotJoint(double * target, 
								double acc = JOINT_MOVE_ACC, 
								double speed = JOINT_MOVE_SPEED, 
								double time = JOINT_MOVE_TIME);		       
	//����jointNum�ؽڵ��˶����򣬼��ٶȣ��ٶȵȣ����Ը��ٶ������˶�
	virtual UINT RotateJointSpeed(int jointNum, 
								  int direction,
								  double speed = 0.2, 
								  double acc = 1.5, 
								  double time = 100.0);
	
	virtual UINT MoveRobotLineTarget(double * target);									 //������TCPֱ�߲�ֵ�˶���target����
 	virtual UINT MoveRobotStop();													     // ��е��ֹͣ�˶�
 	virtual UINT MovoRobotZeroPos();													 // �����˻���λ
 	virtual UINT SetRobotMoveSpeed(double speed);										 //���ùؽ��˶��ٶ�	
// 	virtual UINT RotateAroundX(double Speed_Vector[3]);                                  // ��ʼ����X����ת��������
// 	virtual UINT StopRotateAroundX();                                                    // ֹͣ��X����ת��������
	virtual UINT TransAlongPath(double Path_Vector[3]);                                  // Tool����·�������ƶ�
	virtual UINT MoveRobotMutilRoute(double* target, int routeNum);                      // ���ؽڷֱ��˶���Ŀ��ؽ�λ��


	//////////////////////////// ������״̬ ////////////////////////////////////
	virtual UINT SetRobotFreeDriveMode() = 0;								              // ���û�����ʾ��ģʽ
// 	virtual UINT StartFreeDriveMode() = 0;
// 	virtual UINT StopFreeDriveMode() = 0;
	virtual UINT SetRobotRunningMode() = 0;												  // ����������ģʽ
	virtual UINT SetProtectedStopUnLock() = 0;                                            // ��е���ɰ�ȫ����״̬����Ϊһ�㰲ȫ״̬
	virtual UINT CheckRbtState();												          // ��ȡ������ʵ��״̬���������ʵ���еĶ���
	virtual BOOL CheckJointMode(UINT state);											  // ��ȡ��е�۸��ؽ�״̬
	virtual UINT SetPayLoad(double massKilograms, double *centerOfGravity = NULL);        //����TCP����

 	//////////////////////////  �˶����Ʋ���  /////////////////////////////////
	virtual UINT GetActualJointPos(double * jointpos);								      // ��õ�ǰ�ؽ�ֵ
	virtual UINT GetActualTcpPos(double * tcppos);									      // ��ȡ��ǰ��������
	virtual UINT GetActualJointMartrix(double* martrix);                                  // ��ȡĩ�˹ؽڵ���̬����
	virtual UINT SetTcpPos(double tcpPos[3]);                                             // ����tcpλ������
	virtual UINT SetTcp(double tcp[6]);													  // ����tcp��λ�ú���̬
	virtual UINT GetActualTcpForce(double * tcpforce);								      // ��ȡ��ǰ��������ϵ�¸�����
	virtual UINT GetTipData(float * tipdata);											  // ��ȡ��ǰ��������
	virtual UINT GetActualPinVector(double* vect);                                        // ��ȡ�����볯��
	virtual UINT GetRootData(float * rootdata);										      // ��ȡ��β����
	virtual UINT GetTipRootData(float* tip,float* root);                                  // ��ȡ�����β����
	virtual UINT GetKinematicsTipData(double *tipdata, double *m_fTcpCenter_RegRBT2MTC);  // ��ȡ���⹤������
	virtual UINT GetKinematicsRootData(double *rootdata, double *m_fTcpCenter_Root);	  // ��ȡ������β����
	virtual UINT GetPosInverse(double Joint[6],double* Pos = NULL);                       // ��ȡ�ؽ��˶�ѧ���

	virtual void PackAnalysis() = 0;				                                      //���������˴������

	//////////////////////  �˶�ѧ����  ////////////////////////////////////////
	//�˶�ѧ���⣬tcpcenter���Զ���TCP�����ĩ�˹ؽڵ����꣬targetpos�Ǽ������������ڻ�����ϵ������ֵ
	virtual UINT Kinematics(double *targetpos,double *tcpcenter);						   
	virtual UINT KinematicsMatrix(double* vect);                                           //��ȡ��ķ�������   
	void cuMatrixMultiply(double *A, double *B, double *C, int N, int L, int M);           //����˷�C = A * B

public:
	////////////////////////////// �̻߳ص����� ////////////////////////////
	static UINT TimeCheckThread(LPVOID pParam);										      // Socketʱ�������̺߳���
	static UINT PackAnalysisThread(LPVOID pParam);										  // ��е�����ݰ������߳�
	static UINT ForceThread(LPVOID pParam);                                               // ���ĩ�������Ƿ������ֵ�߳�
	static UINT BrakeReleaseTimeThread(LPVOID pParam);									  // ����е���Ƿ񳬹�ʾ��ʱ���̣߳��������Զ���runmode
	

public:

	CCriticalSection       m_csURCtrl;													  //�ٽ�Σ���ס���ݵĶ�д

	RobotParam *m_RobotParam;															  //�����˲���������ĩ�˹��������״���Լ����ݰ������ݵ�
	myClientSocket *m_Socket;															  //socketͨѶ����	

	double pinlength;																	  //�볤
	int m_nPackRecvNum;																	  //��ǰ�յ����ܵİ��ĸ���
	double m_curretPackTime;															  //��ǰ���ݴ��ʱ��		
	double m_prePackTime;                                                                 //ǰһ���ݰ����ʱ��

	BOOL m_bRobotTimeCheckThreadRunning;                                                  //Socketʱ�������߳�״̬����
	BOOL m_bPackParaAnalysisThreadRunning;												  //��е�ۿ���������ݷ����߳̿���
	BOOL m_bBrakeReleaseTimeThreadRunning;												  //����е���Ƿ񳬹�ʾ��ʱ���߳̿���
	
};

