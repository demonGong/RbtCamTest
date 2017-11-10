
// RbtCamTestDoc.cpp : implementation of the CRbtCamTestDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RbtCamTest.h"
#endif

#include "RbtCamTestDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRbtCamTestDoc

IMPLEMENT_DYNCREATE(CRbtCamTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CRbtCamTestDoc, CDocument)
	ON_COMMAND(ID_Register_Rbt, &CRbtCamTestDoc::OnRegisterRbt)
	ON_COMMAND(ID_COM_ACC_TEST, &CRbtCamTestDoc::OnComAccTest)
END_MESSAGE_MAP()


// CRbtCamTestDoc construction/destruction

CRbtCamTestDoc::CRbtCamTestDoc()
{
	// TODO: add one-time construction code here
	settingFileName = GetINIFilePath();
	ReadAllParaFromINIFile(settingFileName);

	m_Cam = new CMT_C();
	RbtCtrllerFactory = new CSimpleRbtCtrllerFactory();
	RbtCtrller = RbtCtrllerFactory->createRbtCtrller(robotCtrlModel);
	
	for (int i=0; i<3; i++)
		testMarkerPos[i] = 0.0;
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
		{
			m_regMtxCam2Rbt[i][j] = 0.0;
			m_regMtxRbt2Cam[i][j] = 0.0;
		}
		
	m_bRbtManualRegFinised = FALSE;
}

CRbtCamTestDoc::~CRbtCamTestDoc()
{
	if (m_Cam)
	{
		delete m_Cam;
		m_Cam = NULL;
	}

	if(RbtCtrllerFactory)
	{
		delete RbtCtrllerFactory;
		RbtCtrllerFactory = NULL;
	}

	if(RbtCtrller)
	{
		delete RbtCtrller;
		RbtCtrller = NULL;
	}
}

CString CRbtCamTestDoc::GetAppExeName()
{
	CString sExeName;
	char exeFullPath[MAX_PATH];
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	sExeName.Format("%s",exeFullPath);
	return sExeName;
}

CString CRbtCamTestDoc::GetAppExePath()
{
	CString AFileName = GetAppExeName();
	CString sPath;
	int iPos,iCount;
	// 如果是文件夹则不能取消后面的字符
	iCount=AFileName.GetLength();
	iPos=AFileName.ReverseFind('\\');
	if(iPos>=0)
	{
		sPath=AFileName.Left(iPos+1);
	}
	else
	{
		sPath="";
	}
	if(sPath.Right(1)!="\\"){
		sPath+="\\";
	}
	return sPath;
}

CString CRbtCamTestDoc::GetINIFilePath()
{
	CString iniPath;
	iniPath = GetAppExePath();
	iniPath += _T("RbtCamTest.ini");

	return iniPath;
}

void CRbtCamTestDoc::ReadAllParaFromINIFile(const char *filePath)
{
	wkIniConfigurator reader(settingFileName);

	/////读取摄像头常用参数//////
	calibDirName = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("calibDirName"),_T("CalibrationFiles"));
	markerDirName = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("markerDirName"),_T("Markers"));

	robotMarkerName = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("robotMarkerName"),_T("1"));
	testMarkerName  = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("testMarkerName"),_T("TTBlock"));

	headMarkerL = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("headMarkerL"),_T("Left"));
	headMarkerR = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("headMarkerR"),_T("Right"));
	headMarkerM = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("headMarkerM"),_T("Front"));
	checkMarker = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("checkMarker"),_T("bionet"));

	markRegVarThreshold = reader.ReadFloat(SECTION_PARAMETER_CAMETER,_T("markRegVarThreshold"),0.2);

	CString extStr = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("markerLoc1"),_T(""));
	reader.ExtractFloatFromString(extStr,markerLoc1);
	extStr = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("markerLoc2"),_T(""));
	reader.ExtractFloatFromString(extStr,markerLoc2);
	extStr = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("markerLoc3"),_T(""));
	reader.ExtractFloatFromString(extStr,markerLoc3);
	extStr = reader.ReadString(SECTION_PARAMETER_CAMETER,_T("markerLoc4"),_T(""));
	reader.ExtractFloatFromString(extStr,markerLoc4);

	/////读取机器人常用参数//////
	robotCtrlModel = reader.ReadInteger(SECTION_PARAMETER_ROBOT,_T("robotCtrlModel"),2);
	ipAddress = reader.ReadString(SECTION_PARAMETER_ROBOT,_T("ipAddress"),_T("192.168.1.106"));
	nPort = reader.ReadInteger(SECTION_PARAMETER_ROBOT,_T("nPort"),30003);

	extStr = reader.ReadString(SECTION_PARAMETER_ROBOT,_T("initialPos_1"),_T(""));
	reader.ExtractFloatFromString(extStr,rbtInitialJointZero);

	extStr = reader.ReadString(SECTION_PARAMETER_ROBOT,_T("robotMarkerPos"),_T(""));
	reader.ExtractDoubleFromString(extStr,robotMarkerPos);

	extStr = reader.ReadString(SECTION_PARAMETER_ROBOT,_T("tcpPin"),_T(""));
	reader.ExtractFloatFromString(extStr,tcpPin);
	
}

float CRbtCamTestDoc::SolveTransMatrix_Rbt2Cam(float img[4][3],float rbt[4][3],float mtx[4][4],float errArr[4])
{
	CNavArithmetic  NA;
	float VS1[4][3], VS2[4][3];

	float err;

	for (int i=0;i<4; i++)
		for (int j=0; j<3; j++)
		{
			VS1[i][j] = rbt[i][j];
			VS2[i][j] = img[i][j];
		}
		NA.MDD_VS1toVS2(VS1,VS2,mtx);

		err = ComputeCumulativeError(rbt,img,mtx,errArr)/4.0;

		return err;
}

float CRbtCamTestDoc::SolveTransMatrix_Cam2Rbt(float img[4][3],float rbt[4][3],float mtx[][4],float errArr[4])
{
	CNavArithmetic  NA;
	float VS1[4][3], VS2[4][3];

	float err;
	
	for (int i=0;i<4; i++)
		for (int j=0; j<3; j++)
		{
			VS1[i][j] = rbt[i][j];
			VS2[i][j] = img[i][j];
		}

		NA.MDD_VS1toVS2(VS2,VS1,mtx);

		err = ComputeCumulativeError(img,rbt,mtx,errArr)/4.0;
	
		return err;
}

float CRbtCamTestDoc::ComputeCumulativeError(float source[4][3],float target[4][3],float mtx[4][4],float errArr[4])
{
	int len = 30;
	float testSource[4][3],testTarget[4][3];
	float vectSourceX[3],vectSourceY[3],vectSourceZ[3],vectTargetX[3],vectTargetY[3],vectTargetZ[3];

	/// 构造第1个测试点
	vectSourceX[0] = source[1][0]-source[0][0];
	vectSourceX[1] = source[1][1]-source[0][1];
	vectSourceX[2] = source[1][2]-source[0][2];

	vectSourceY[0] = source[2][0]-source[0][0];
	vectSourceY[1] = source[2][1]-source[0][1];
	vectSourceY[2] = source[2][2]-source[0][2];
	
	Vector3f vSourceX;
	vSourceX.x = vectSourceX[0];
	vSourceX.y = vectSourceX[1];
	vSourceX.z = vectSourceX[2];
	vSourceX = vSourceX.Normalize(vSourceX);

	Vector3f vSourceY;
	vSourceY.x = vectSourceY[0];
	vSourceY.y = vectSourceY[1];
	vSourceY.z = vectSourceY[2];
	vSourceY = vSourceY.Normalize(vSourceY);

	Vector3f vSourceZ;
	vSourceZ = vSourceZ.Cross(vSourceX,vSourceY);
	
	vectSourceZ[0] = vSourceZ.x;
	vectSourceZ[1] = vSourceZ.y;
	vectSourceZ[2] = vSourceZ.z;

	for (int j=0;j<3;j++)
	{
		testSource[0][j] = source[0][j] + len*vectSourceZ[j];
	}

	/// 构造第2个测试点
	vectSourceX[0] = source[2][0]-source[1][0];
	vectSourceX[1] = source[2][1]-source[1][1];
	vectSourceX[2] = source[2][2]-source[1][2];

	vectSourceY[0] = source[3][0]-source[1][0];
	vectSourceY[1] = source[3][1]-source[1][1];
	vectSourceY[2] = source[3][2]-source[1][2];
	
	vSourceX.x = vectSourceX[0];
	vSourceX.y = vectSourceX[1];
	vSourceX.z = vectSourceX[2];
	vSourceX = vSourceX.Normalize(vSourceX);

	vSourceY.x = vectSourceY[0];
	vSourceY.y = vectSourceY[1];
	vSourceY.z = vectSourceY[2];
	vSourceY = vSourceY.Normalize(vSourceY);

	vSourceZ = vSourceZ.Cross(vSourceX,vSourceY);

	vectSourceZ[0] = vSourceZ.x;
	vectSourceZ[1] = vSourceZ.y;
	vectSourceZ[2] = vSourceZ.z;

	for (int j=0;j<3;j++)
	{
		testSource[1][j] = source[1][j] + len*vectSourceZ[j];
	}


	/// 构造第3个测试点
	vectSourceX[0] = source[3][0]-source[2][0];
	vectSourceX[1] = source[3][1]-source[2][1];
	vectSourceX[2] = source[3][2]-source[2][2];
	
	vectSourceY[0] = source[0][0]-source[2][0];
	vectSourceY[1] = source[0][1]-source[2][1];
	vectSourceY[2] = source[0][2]-source[2][2];
	
	vSourceX.x = vectSourceX[0];
	vSourceX.y = vectSourceX[1];
	vSourceX.z = vectSourceX[2];
	vSourceX = vSourceX.Normalize(vSourceX);

	vSourceY.x = vectSourceY[0];
	vSourceY.y = vectSourceY[1];
	vSourceY.z = vectSourceY[2];
	vSourceY = vSourceY.Normalize(vSourceY);

	vSourceZ = vSourceZ.Cross(vSourceX,vSourceY);

	vectSourceZ[0] = vSourceZ.x;
	vectSourceZ[1] = vSourceZ.y;
	vectSourceZ[2] = vSourceZ.z;

	for (int j=0;j<3;j++)
	{
		testSource[2][j] = source[2][j] + len*vectSourceZ[j];
	}


	/// 构造第4个测试点
	vectSourceX[0] = source[0][0]-source[3][0];
	vectSourceX[1] = source[0][1]-source[3][1];
	vectSourceX[2] = source[0][2]-source[3][2];
	
	vectSourceY[0] = source[1][0]-source[3][0];
	vectSourceY[1] = source[1][1]-source[3][1];
	vectSourceY[2] = source[1][2]-source[3][2];

	vSourceX.x = vectSourceX[0];
	vSourceX.y = vectSourceX[1];
	vSourceX.z = vectSourceX[2];
	vSourceX = vSourceX.Normalize(vSourceX);

	vSourceY.x = vectSourceY[0];
	vSourceY.y = vectSourceY[1];
	vSourceY.z = vectSourceY[2];
	vSourceY = vSourceY.Normalize(vSourceY);

	vSourceZ = vSourceZ.Cross(vSourceX,vSourceY);

	vectSourceZ[0] = vSourceZ.x;
	vectSourceZ[1] = vSourceZ.y;
	vectSourceZ[2] = vSourceZ.z;

	for (int j=0;j<3;j++)
	{
		testSource[3][j] = source[3][j] + len*vectSourceZ[j];
	}


	/// T 构造第1个测试点	
	vectTargetX[0] = target[1][0]-target[0][0];
	vectTargetX[1] = target[1][1]-target[0][1];
	vectTargetX[2] = target[1][2]-target[0][2];
	
	vectTargetY[0] = target[2][0]-target[0][0];
	vectTargetY[1] = target[2][1]-target[0][1];
	vectTargetY[2] = target[2][2]-target[0][2];
	
	Vector3f vTargetX;
	vTargetX.x = vectTargetX[0];
	vTargetX.y = vectTargetX[1];
	vTargetX.z = vectTargetX[2];
	vTargetX = vTargetX.Normalize(vTargetX);

	Vector3f vTargetY;
	vTargetY.x = vectTargetY[0];
	vTargetY.y = vectTargetY[1];
	vTargetY.z = vectTargetY[2];
	vTargetY = vTargetY.Normalize(vTargetY);

	Vector3f vTargetZ;
	vTargetZ = vTargetZ.Cross(vTargetX,vTargetY);

	vectTargetZ[0] = vTargetZ.x;
	vectTargetZ[1] = vTargetZ.y;
	vectTargetZ[2] = vTargetZ.z;

	for (int j=0;j<3;j++)
	{
		testTarget[0][j] = target[0][j] + len*vectTargetZ[j];
	}

	/// T 构造第2个测试点	
	vectTargetX[0] = target[2][0]-target[1][0];
	vectTargetX[1] = target[2][1]-target[1][1];
	vectTargetX[2] = target[2][2]-target[1][2];
	
	vectTargetY[0] = target[3][0]-target[1][0];
	vectTargetY[1] = target[3][1]-target[1][1];
	vectTargetY[2] = target[3][2]-target[1][2];
	
	vTargetX.x = vectTargetX[0];
	vTargetX.y = vectTargetX[1];
	vTargetX.z = vectTargetX[2];
	vTargetX = vTargetX.Normalize(vTargetX);

	vTargetY.x = vectTargetY[0];
	vTargetY.y = vectTargetY[1];
	vTargetY.z = vectTargetY[2];
	vTargetY = vTargetY.Normalize(vTargetY);

	vTargetZ = vTargetZ.Cross(vTargetX,vTargetY);

	vectTargetZ[0] = vTargetZ.x;
	vectTargetZ[1] = vTargetZ.y;
	vectTargetZ[2] = vTargetZ.z;

	for (int j=0;j<3;j++)
	{
		testTarget[1][j] = target[1][j] + len*vectTargetZ[j];
	}

	/// T 构造第3个测试点	
	vectTargetX[0] = target[3][0]-target[2][0];
	vectTargetX[1] = target[3][1]-target[2][1];
	vectTargetX[2] = target[3][2]-target[2][2];
	
	vectTargetY[0] = target[0][0]-target[2][0];
	vectTargetY[1] = target[0][1]-target[2][1];
	vectTargetY[2] = target[0][2]-target[2][2];
	
	vTargetX.x = vectTargetX[0];
	vTargetX.y = vectTargetX[1];
	vTargetX.z = vectTargetX[2];
	vTargetX = vTargetX.Normalize(vTargetX);

	vTargetY.x = vectTargetY[0];
	vTargetY.y = vectTargetY[1];
	vTargetY.z = vectTargetY[2];
	vTargetY = vTargetY.Normalize(vTargetY);

	vTargetZ = vTargetZ.Cross(vTargetX,vTargetY);

	vectTargetZ[0] = vTargetZ.x;
	vectTargetZ[1] = vTargetZ.y;
	vectTargetZ[2] = vTargetZ.z;

	for (int j=0;j<3;j++)
	{
		testTarget[2][j] = target[2][j] + len*vectTargetZ[j];
	}
	/// T 构造第4个测试点
	vectTargetX[0] = target[0][0]-target[3][0];
	vectTargetX[1] = target[0][1]-target[3][1];
	vectTargetX[2] = target[0][2]-target[3][2];
	
	vectTargetY[0] = target[1][0]-target[3][0];
	vectTargetY[1] = target[1][1]-target[3][1];
	vectTargetY[2] = target[1][2]-target[3][2];
	
	vTargetX.x = vectTargetX[0];
	vTargetX.y = vectTargetX[1];
	vTargetX.z = vectTargetX[2];
	vTargetX = vTargetX.Normalize(vTargetX);

	vTargetY.x = vectTargetY[0];
	vTargetY.y = vectTargetY[1];
	vTargetY.z = vectTargetY[2];
	vTargetY = vTargetY.Normalize(vTargetY);

	vTargetZ = vTargetZ.Cross(vTargetX,vTargetY);

	vectTargetZ[0] = vTargetZ.x;
	vectTargetZ[1] = vTargetZ.y;
	vectTargetZ[2] = vTargetZ.z;

	for (int j=0;j<3;j++)
	{
		testTarget[3][j] = target[3][j] + len*vectTargetZ[j];
	}

	////////////////// 计算累计误差///////////////////////////
	float err = 0.0;
	float xTm,yTm,zTm;
	for (int i=0;i<4;i++)
	{
		xTm = mtx[0][0]*testSource[i][0] + mtx[0][1]*testSource[i][1] + mtx[0][2]*testSource[i][2] + mtx[0][3];
		yTm = mtx[1][0]*testSource[i][0] + mtx[1][1]*testSource[i][1] + mtx[1][2]*testSource[i][2] + mtx[1][3];
		zTm = mtx[2][0]*testSource[i][0] + mtx[2][1]*testSource[i][1] + mtx[2][2]*testSource[i][2] + mtx[2][3];

		errArr[i] = sqrt(pow((testTarget[i][0]-xTm),2) + pow((testTarget[i][1]-yTm),2) + pow( (testTarget[i][2]-zTm) ,2) );
		err += errArr[i];
	}

	return err;
}

void CRbtCamTestDoc::SolveMapMatrixSUV(float source[4][3],float target [4][3],float T[4][4])
{
	float meanSource[3],meanTarget[3];

	ComputeMatchPointMean(source,target,meanSource,meanTarget);

	/// 计算需要SVD分解的H矩阵
	float H[3][3];
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
			H[i][j]= 0.0;
		}

		for (int i=0;i<4;i++)
		{
			float tmpM[3][3];
			for (int x=0;x<3;x++)
				for (int y=0;y<3;y++)
				{
					tmpM[x][y] = (source[i][x]-meanSource[x])*(target[i][y]-meanTarget[y]);
				}

				for (int p=0;p<3;p++)
					for (int q=0;q<3;q++)
					{
						H[p][q] += tmpM[p][q];
					}
		}

		double HData[9] = {0};

		for (int i=0; i<3; i++)
		{
			for (int j=0; j<3; j++)
			{
				HData[i*3+j] = H[i][j];
			}
		}


		CMatrix Mat_H(3,3,HData);
		CMatrix Mat_U(3,3);
		CMatrix Mat_VT(3,3);

		Mat_H.SplitUV(Mat_U,Mat_VT);

		float S[3][3];
		for (int i=0;i<3;i++)
			for (int j=0;j<3;j++)
			{
				S[i][j] = 0.0;
			}

		float det = Mat_H.DetGauss();
		if(det>0)
		{
			S[0][0] = 1.0;
			S[1][1] = 1.0;
			S[2][2] = 1.0;
		}
		else if(det <0)
		{
			S[0][0] = -1.0;
			S[1][1] = -1.0;
			S[2][2] = 1.0;
		}
		else if(det == 0)
		{
			float val = Mat_U.DetGauss()*Mat_VT.DetGauss();
			if(val == 1)
			{
				S[0][0] = 1.0;
				S[1][1] = 1.0;
				S[2][2] = 1.0;
			}
			else if(val == -1)
			{
				S[0][0] = -1.0;
				S[1][1] = -1.0;
				S[2][2] = 1.0;
			}
			else
				return;
		}

		float t[3];

		float UT[3][3],V[3][3];
		Mat_U = Mat_U.Transpose();
		Mat_VT = Mat_VT.Transpose();


		double tmpR[9],tmpUT[9],tmpV[9],tmpS[9];
		for (int i=0;i<9;i++)
		{
			tmpUT[i] = (Mat_U.GetData())[i];
			tmpV[i] = (Mat_VT.GetData())[i];

		}

		for (int i=0; i<3; i++)
			for (int j=0; j<3; j++)
				tmpS[i*3+j] = S[i][j];


		CMatrix Mat_tmpV(3,3,tmpV);
		CMatrix Mat_tmpS(3,3,tmpS);
		CMatrix Mat_tmpR(3,3);
		CMatrix Mat_tmpUT(3,3,tmpUT);
		CMatrix Mat_meanSource(3,1);

		for (int i=0; i<3; i++)
		{
			Mat_meanSource.GetData()[i] = meanSource[i];
		}

		CMatrix Mat_t(3,1);
		Mat_tmpR = Mat_tmpV * Mat_tmpS;
		Mat_tmpR = Mat_tmpR * Mat_tmpUT;
		Mat_t = Mat_tmpR * Mat_meanSource;

		for (int i=0; i<3; i++)
			t[i] = Mat_t.GetData()[i];
		for (int i=0; i<9; i++)
			tmpR[i] = Mat_tmpR.GetData()[i];

		for(int i=0;i<3;i++)
			t[i] = meanTarget[i]-t[i];

		for (int i=0;i<3;i++)
			for (int j=0;j<3;j++)
			{
				T[i][j] = tmpR[i*3+j];
			}
			T[0][3] = t[0];
			T[1][3] = t[1];
			T[2][3] = t[2];
			T[3][0] = 0.0;
			T[3][1] = 0.0;
			T[3][2] = 0.0;
			T[3][3] = 1.0;

}

void CRbtCamTestDoc::ComputeMatchPointMean(float source[4][3],float target [4][3],float outX[3],float outY[3])
{
	for (int i=0;i<3;i++)
	{
		outX[i] = 0.0;
		outY[i] = 0.0;
	}

	for (int i=0;i<4;i++)
		for(int j=0;j<3;j++)
		{
			outX[j] += source[i][j];
			outY[j] += target[i][j];
		}

		for (int i=0;i<3;i++)
		{
			outX[i] = outX[i]/4.0;
			outY[i] = outY[i]/4.0;
		}
}

BOOL CRbtCamTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CRbtCamTestDoc serialization

void CRbtCamTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CRbtCamTestDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CRbtCamTestDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CRbtCamTestDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CRbtCamTestDoc diagnostics

#ifdef _DEBUG
void CRbtCamTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRbtCamTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRbtCamTestDoc commands


void CRbtCamTestDoc::OnRegisterRbt()
{
	// TODO: Add your command handler code here
	CRobotRegView rbtRegDlg;

	if (rbtRegDlg.DoModal() == IDOK)
		return;
}


void CRbtCamTestDoc::OnComAccTest()
{
	// TODO: Add your command handler code here
	ComAccTest rbtCamTestDlg;

	if (rbtCamTestDlg.DoModal() == IDOK)
		return;
}
