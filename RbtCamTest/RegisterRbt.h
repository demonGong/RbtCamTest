#pragma once


// CRegisterRbt dialog

class CRegisterRbt : public CPropertyPage
{
	DECLARE_DYNAMIC(CRegisterRbt)

public:
	CRegisterRbt();
	virtual ~CRegisterRbt();

// Dialog Data
	enum { IDD = IDD_DIALOG_REG_RBT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtMoveToPos1();
	afx_msg void OnBnClickedBtMoveToPos2();
	afx_msg void OnBnClickedBtMoveToPos3();
	afx_msg void OnBnClickedBtMoveToPos4();
	afx_msg void OnBnClickedBtPos1Reregister();
	afx_msg void OnBnClickedBtPos2Reregister();
	afx_msg void OnBnClickedBtPos3Reregister();
	afx_msg void OnBnClickedBtPos4Reregister();
	afx_msg void OnBnClickedBtRmgRegisterAll();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	

private:
	void SetBkColor(CStatic &m_Pic, COLORREF m_Color);
	void UpdateRegStatus();
	bool BtMoveToPos(int m_nRegRobotIndex);
	void CaptureRobotMarker();
	void RegisterRobot();
	void ReRegister(int m_nRegRobotIndex);
	void CheckRegAccuracy();
	void ComputeRobotRegistrationAccurary();

private:
	enum TIMER_RBT_REG_VIEW {
		REG_RBT_MARKER_TIMER,
		UPDATE_BUTTON_TIMER,
		CHECK_ACCURACY_TIME
	};

	CStatic m_stRbtPos1RegStatus;
	CStatic m_stRbtPos2RegStatus;
	CStatic m_stRbtPos3RegStatus;
	CStatic m_stRbtPos4RegStatus;

public:
	BOOL                  m_bRbtPosRegFinished[4];                      // ��е��ĳ��λ���Ƿ�ע�����
	
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
	
	
	afx_msg void OnBnClickedBtLoadResister();
};
