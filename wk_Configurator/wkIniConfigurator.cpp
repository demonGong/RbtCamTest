#include "StdAfx.h"
#include "wkIniConfigurator.h"


wkIniConfigurator::wkIniConfigurator(const CString& szFileName)
{
	m_szFileName = szFileName;
}


wkIniConfigurator::~wkIniConfigurator(void)
{
}

int wkIniConfigurator::ReadInteger(const CString& szSection, const CString& szKey, int iDefaultValue)
{
	int iResult = GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_szFileName); 
	return iResult;
}
float wkIniConfigurator::ReadFloat(const CString& szSection, const CString& szKey, float fltDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	float fltResult;
	sprintf_s(szDefault,255, "%f",fltDefaultValue);
	GetPrivateProfileString(szSection,  szKey, szDefault, szResult, 255, m_szFileName); 
	fltResult =  (float)atof(szResult);
	return fltResult;
}
bool wkIniConfigurator::ReadBoolean(const CString& szSection, const CString& szKey, bool bolDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	bool bolResult;
	sprintf_s(szDefault,255, "%s", bolDefaultValue? "True" : "False");
	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName); 
	bolResult =  (strcmp(szResult, "True") == 0 || 
		strcmp(szResult, "true") == 0) ? true : false;
	return bolResult;
}
CString wkIniConfigurator::ReadString(const CString& szSection, const CString& szKey, const CString&szDefaultValue)
{
	char szResult[255];
	memset(szResult, 0x00, 255);
	GetPrivateProfileString(szSection,  szKey, 
		szDefaultValue, szResult, 255, m_szFileName); 
	CString result = szResult;
	return result;
}
void wkIniConfigurator::WriteInteger(const CString& szSection, const CString& szKey, int iValue)
{
	char szValue[255];
	sprintf_s(szValue,255,  "%d", iValue);
	WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName); 
}
void wkIniConfigurator::WriteFloat(const CString& szSection, const CString& szKey, float fltValue)
{
	char szValue[255];
	sprintf_s(szValue,255,  "%f", fltValue);
	WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName); 
}
void wkIniConfigurator::WriteBoolean(const CString& szSection, const CString& szKey, bool bolValue)
{
	char szValue[255];
	sprintf_s(szValue, 255, "%s", bolValue ? "True" : "False");
	WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName); 
}
void wkIniConfigurator::WriteString(const CString& szSection, const CString& szKey, const CString& szValue)
{
	WritePrivateProfileString(szSection,  szKey, szValue, m_szFileName);
}

////////////////从字符串中读取特定类型数据//////////////////////////////
BOOL wkIniConfigurator::ExtractFloatFromString(const CString& extrStr, float* outVal)
{
	CString localStr = extrStr;
	int spaceIndex = localStr.Find(" ", 0);
	int spaceCnt = 0;
	while(spaceIndex > 0)
	{
		CString leftStr = localStr.Left(spaceIndex);
		outVal[spaceCnt++] = (float)atof(leftStr);
		if(spaceCnt >= MAX_EXTRACT_PARAM)
		{
			break;
		}
		localStr = localStr.Right(localStr.GetLength() - spaceIndex - 1);
		spaceIndex = localStr.Find(" ", 0);
	}

	// 判断后面是否还有数据
	if(localStr.GetLength()>=1)
		outVal[spaceCnt] = (float)atof(localStr);

	return TRUE;
}
BOOL wkIniConfigurator::ExtractIntFromString(const CString& extrStr, int* outVal)
{
	CString localStr = extrStr;
	int spaceIndex = localStr.Find(" ", 0);
	int spaceCnt = 0;
	while(spaceIndex > 0)
	{
		CString leftStr = localStr.Left(spaceIndex);
		outVal[spaceCnt++] = atol(leftStr);
		if(spaceCnt >= MAX_EXTRACT_PARAM)
		{
			break;
		}
		localStr = localStr.Right(localStr.GetLength() - spaceIndex - 1);
		spaceIndex = localStr.Find(" ", 0);
	}

	// 判断后面是否还有数据
	if(localStr.GetLength()>=1)
		outVal[spaceCnt] = (int)atol(localStr);
	return TRUE;
}
BOOL wkIniConfigurator::ExtractDoubleFromString(const CString& extrStr, double* outVal)
{
	CString localStr = extrStr;
	int spaceIndex = localStr.Find(" ", 0);
	int spaceCnt = 0;
	while(spaceIndex > 0)
	{
		CString leftStr = localStr.Left(spaceIndex);
		outVal[spaceCnt++] = (double)atof(leftStr);
		if(spaceCnt >= MAX_EXTRACT_PARAM)
		{
			break;
		}
		localStr = localStr.Right(localStr.GetLength() - spaceIndex - 1);
		spaceIndex = localStr.Find(" ", 0);
	}

	// 判断后面是否还有数据
	if(localStr.GetLength()>=1)
		outVal[spaceCnt] = (double)atof(localStr);
	return TRUE;
}