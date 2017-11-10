/************************************************************************/
/* fuction: .ini配置文件读写器                                         */
/* Data:  2015-03-21                                                   */
/* Autor: yongzhao xie                                                  */
/* e-mail:xyzhhaa@163.com                                               */
/* version: 1.0                                                         */
/************************************************************************/

#pragma once

#pragma pack(push,8)
#define MAX_EXTRACT_PARAM                   1024          // 最大提取参数数目
class AFX_EXT_CLASS wkIniConfigurator
{
public:
	wkIniConfigurator(const CString& szFileName);
	~wkIniConfigurator(void);

public:
	/// 从.ini 配置文件中读取配置参数
	int ReadInteger(const CString& szSection, const CString&szKey, int iDefaultValue);
	float ReadFloat(const CString& szSection, const CString& szKey, float fltDefaultValue);
	bool ReadBoolean(const CString& szSection, const CString& szKey, bool bolDefaultValue);
	CString ReadString(const CString& szSection, const CString& szKey, const CString& szDefaultValue);

	/// 向.ini 配置文件中写配置参数
	void WriteInteger(const CString&szSection, const CString&szKey, int iValue);
	void WriteFloat(const CString&szSection, const CString& szKey, float fltValue);
	void WriteBoolean(const CString&szSection, const CString&szKey, bool bolValue);
	void WriteString(const CString& szSection, const CString& szKey, const CString& szValue);

public:
	////////////////从字符串中读取特定类型数据//////////////////////////////
	static BOOL ExtractFloatFromString(const CString& extrStr, float* outVal);
	static BOOL ExtractIntFromString(const CString& extrStr, int* outVal);
	static BOOL ExtractDoubleFromString(const CString& extrStr, double* outVal);
private:
	CString m_szFileName;
};
#pragma pack(pop)