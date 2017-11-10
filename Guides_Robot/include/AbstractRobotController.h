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
	
	////////////////////////////读取机器人配置初始化机器人等///////////////////
	CString GetAppPath();									                           //获得程序路径		
	CString GetIniFileName();                                                          //获得机器人配置文件路径
	void getParaFromINIFlie(const char *iniFilePath);                                  //获得配置文件的参数
	virtual UINT InitialRobot(RobotParam *m_Parm = NULL);                              //初始化机械臂
	virtual UINT DisConnectRobot();													   //机械臂与上位机断开连接
	
	/////////////////////////// 开关电解锁关机操作 /////////////////////////////////
	virtual UINT RobotPowerOn();													   // 机械臂上电
	virtual UINT RobotBrakeRelease();												   // 机械臂解锁
	virtual UINT RobotPowerOff();													   // 机械臂关电
	virtual UINT RobotPowerDown();												       // 机械臂关机

 	//////////////////////////// 机械臂运动控制 //////////////////////////////////
	
	// 输入target位置，反解，求出并运动到各关节位置 输入double[6]数组指针 毫米 弧度可输入速度、加速度、运动时间(不设置可输入为0)
	virtual UINT MoveRobotInverse(double * target,
								  double acc = JOINT_MOVE_ACC, 
								  double speed = JOINT_MOVE_SPEED, 
								  double time = JOINT_MOVE_TIME);	      
	// 输入各个关节的位置，运动到该位置 输入double[6]数组指针 可输入速度、加速度、运动时间(不设置可输入为0)
	virtual UINT MoveRobotJoint(double * target, 
								double acc = JOINT_MOVE_ACC, 
								double speed = JOINT_MOVE_SPEED, 
								double time = JOINT_MOVE_TIME);		       
	//设置jointNum关节的运动方向，加速度，速度等，并以该速度匀速运动
	virtual UINT RotateJointSpeed(int jointNum, 
								  int direction,
								  double speed = 0.2, 
								  double acc = 1.5, 
								  double time = 100.0);
	
	virtual UINT MoveRobotLineTarget(double * target);									 //机器人TCP直线插值运动到target坐标
 	virtual UINT MoveRobotStop();													     // 机械臂停止运动
 	virtual UINT MovoRobotZeroPos();													 // 机器人回零位
 	virtual UINT SetRobotMoveSpeed(double speed);										 //设置关节运动速度	
// 	virtual UINT RotateAroundX(double Speed_Vector[3]);                                  // 开始绕着X轴旋转？待测试
// 	virtual UINT StopRotateAroundX();                                                    // 停止绕X轴旋转？待测试
	virtual UINT TransAlongPath(double Path_Vector[3]);                                  // Tool沿着路径方向移动
	virtual UINT MoveRobotMutilRoute(double* target, int routeNum);                      // 六关节分别运动到目标关节位置


	//////////////////////////// 机器人状态 ////////////////////////////////////
	virtual UINT SetRobotFreeDriveMode() = 0;								              // 设置机器人示教模式
// 	virtual UINT StartFreeDriveMode() = 0;
// 	virtual UINT StopFreeDriveMode() = 0;
	virtual UINT SetRobotRunningMode() = 0;												  // 机器人运行模式
	virtual UINT SetProtectedStopUnLock() = 0;                                            // 机械臂由安全保护状态设置为一般安全状态
	virtual UINT CheckRbtState();												          // 获取机器人实际状态，详见函数实现中的定义
	virtual BOOL CheckJointMode(UINT state);											  // 获取机械臂各关节状态
	virtual UINT SetPayLoad(double massKilograms, double *centerOfGravity = NULL);        //设置TCP配重

 	//////////////////////////  运动控制参数  /////////////////////////////////
	virtual UINT GetActualJointPos(double * jointpos);								      // 获得当前关节值
	virtual UINT GetActualTcpPos(double * tcppos);									      // 获取当前工具坐标
	virtual UINT GetActualJointMartrix(double* martrix);                                  // 获取末端关节的姿态矩阵
	virtual UINT SetTcpPos(double tcpPos[3]);                                             // 设置tcp位置坐标
	virtual UINT SetTcp(double tcp[6]);													  // 设置tcp的位置和姿态
	virtual UINT GetActualTcpForce(double * tcpforce);								      // 获取当前工具坐标系下各个力
	virtual UINT GetTipData(float * tipdata);											  // 获取当前工具坐标
	virtual UINT GetActualPinVector(double* vect);                                        // 获取手术针朝向
	virtual UINT GetRootData(float * rootdata);										      // 获取针尾坐标
	virtual UINT GetTipRootData(float* tip,float* root);                                  // 获取针尖针尾坐标
	virtual UINT GetKinematicsTipData(double *tipdata, double *m_fTcpCenter_RegRBT2MTC);  // 获取正解工具坐标
	virtual UINT GetKinematicsRootData(double *rootdata, double *m_fTcpCenter_Root);	  // 获取正解针尾坐标
	virtual UINT GetPosInverse(double Joint[6],double* Pos = NULL);                       // 获取关节运动学逆解

	virtual void PackAnalysis() = 0;				                                      //解析机器人打包数据

	//////////////////////  运动学正解  ////////////////////////////////////////
	//运动学正解，tcpcenter是自定义TCP相对于末端关节的坐标，targetpos是计算出来的相对于基坐标系的坐标值
	virtual UINT Kinematics(double *targetpos,double *tcpcenter);						   
	virtual UINT KinematicsMatrix(double* vect);                                           //求取针的方向向量   
	void cuMatrixMultiply(double *A, double *B, double *C, int N, int L, int M);           //矩阵乘法C = A * B

public:
	////////////////////////////// 线程回调函数 ////////////////////////////
	static UINT TimeCheckThread(LPVOID pParam);										      // Socket时间戳检测线程函数
	static UINT PackAnalysisThread(LPVOID pParam);										  // 机械臂数据包分析线程
	static UINT ForceThread(LPVOID pParam);                                               // 检测末端受力是否大于阈值线程
	static UINT BrakeReleaseTimeThread(LPVOID pParam);									  // 检测机械臂是否超过示教时间线程，超过则自动到runmode
	

public:

	CCriticalSection       m_csURCtrl;													  //临界段，锁住数据的读写

	RobotParam *m_RobotParam;															  //机器人参数，包括末端工具针的形状，以及数据包的数据等
	myClientSocket *m_Socket;															  //socket通讯对象	

	double pinlength;																	  //针长
	int m_nPackRecvNum;																	  //当前收到的总的包的个数
	double m_curretPackTime;															  //当前数据打包时间		
	double m_prePackTime;                                                                 //前一数据包打包时间

	BOOL m_bRobotTimeCheckThreadRunning;                                                  //Socket时间戳检测线程状态开关
	BOOL m_bPackParaAnalysisThreadRunning;												  //机械臂控制箱包数据分析线程开关
	BOOL m_bBrakeReleaseTimeThreadRunning;												  //检测机械臂是否超过示教时间线程开关
	
};

