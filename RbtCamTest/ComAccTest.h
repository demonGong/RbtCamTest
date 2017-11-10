#pragma once


// ComAccTest dialog

class ComAccTest : public CDialogEx
{
	DECLARE_DYNAMIC(ComAccTest)

public:
	ComAccTest(CWnd* pParent = NULL);   // standard constructor
	virtual ~ComAccTest();

// Dialog Data
	enum { IDD = IDD_DIALOG_COM_ACC_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRegTarget();
	afx_msg void OnBnClickedMovetoTarget();

public:
	CString markerName;
	double targetPt[3];
	double markerPos[3];
	double markerRotateMat[9];
	double markerPose[16];
	double vecPos[4][3];
	double tcpPin[3];
};
