#pragma once
#include "afxwin.h"


// CRobotRegView dialog
#define REG_CAP_POINT_NUM  50				  //ÿ��marker �Ĳɼ�����
#define WK_PI 3.14159265358979323846		  //Pi

class CRobotRegView : public CDialogEx
{
	DECLARE_DYNAMIC(CRobotRegView)

public:
	CRobotRegView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRobotRegView();

// Dialog Data
	enum { IDD = IDD_DLG_ROBOT_MANUAL_REG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtMovetoPos1();
	afx_msg void OnBnClickedBtMoveToPos2();
	afx_msg void OnBnClickedBtMoveToPos3();
	afx_msg void OnBnClickedBtMoveToPos4();

	afx_msg void OnBnClickedBtPos1Reregister();
	afx_msg void OnBnClickedBtPos2Reregister();
	afx_msg void OnBnClickedBtPos3Reregister();
	afx_msg void OnBnClickedBtPos4Reregister();

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedBtRmgRegisterAll();
	afx_msg void OnBnClickedBtRmgAutoToZero();
	afx_msg void OnBnClickedBtRmgStopRobot();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

private:
	void CaptureRobotMarker();
	void UpdateRegStatus();
	void BtMoveToPos(int m_nRegRobotIndex);
	void ReRegister(int m_nRegRobotIndex);
	void SetBkColor(CStatic &m_Pic, COLORREF m_Color);
	void RegisterRobot();
	void CheckRegAccuracy();
	void ComputeRobotRegistrationAccurary();

private:
	enum TIMER_RBT_REG_VIEW {
		REG_RBT_MARKER_TIMER,
		UPDATE_BUTTON_TIMER,
		CHECK_ACCURACY_TIME
	};

public:
	CStatic m_stRbtPos1RegStatus;
	CStatic m_stRbtPos2RegStatus;
	CStatic m_stRbtPos3RegStatus;
	CStatic m_stRbtPos4RegStatus;

public:
	BOOL                  m_bRbtPosRegFinished[4];                      // ��е��ĳ��λ���Ƿ�ע�����
	//BOOL                  m_bRbtManualRegFinised;                       // ��е���ֶ�ע���Ƿ�ɹ�
	double                m_regRobotMarkerPtArr[REG_CAP_POINT_NUM][3];  // ��е��Marker���������ϵ�����꼯��
	float                 m_regRobotMarkerPos[4][3];                    // ��е��Marker��������꼯�Ͼ�ֵ
	int                   m_nRegRobotIndex;                             // ��е��ע��λ��������
	int                   m_nRegPointCounter;                           // ����ɼ�Marker�����
	float                 m_regRobotTipPos[4][3];                       // ��е������ϵ�£�marker����
	float                 m_checkAccRobotPos[3];                        // ��֤���Ȼ�е������ϵMark����
	float                 m_checkAccCameraPos[3];                       // ��֤�����������ϵMark����
	float                 m_accuracyTestPoint[3];                       // ���Ծ��ȵ�����
	int                   m_lastRegPointIndex;                          // ���һ������λ������

	COLORREF              m_clrRegUnfinished;                           // û��ע����ɫ
	COLORREF              m_clrRegFinished;                             // ע�������ɫ

	float                 m_rbtInitialPos_1[6];							//��е����λ
	CString				  markerName;									//��е�۰���marker����
	
	afx_msg void OnBnClickedBtPrRbtUnlock();
	afx_msg void OnBnClickedBtPrShutDown();
};
