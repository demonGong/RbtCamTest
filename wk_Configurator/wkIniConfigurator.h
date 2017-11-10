/************************************************************************/
/* fuction: .ini�����ļ���д��                                         */
/* Data:  2015-03-21                                                   */
/* Autor: yongzhao xie                                                  */
/* e-mail:xyzhhaa@163.com                                               */
/* version: 1.0                                                         */
/************************************************************************/

#pragma once

#pragma pack(push,8)
#define MAX_EXTRACT_PARAM                   1024          // �����ȡ������Ŀ
class AFX_EXT_CLASS wkIniConfigurator
{
public:
	wkIniConfigurator(const CString& szFileName);
	~wkIniConfigurator(void);

public:
	/// ��.ini �����ļ��ж�ȡ���ò���
	int ReadInteger(const CString& szSection, const CString&szKey, int iDefaultValue);
	float ReadFloat(const CString& szSection, const CString& szKey, float fltDefaultValue);
	bool ReadBoolean(const CString& szSection, const CString& szKey, bool bolDefaultValue);
	CString ReadString(const CString& szSection, const CString& szKey, const CString& szDefaultValue);

	/// ��.ini �����ļ���д���ò���
	void WriteInteger(const CString&szSection, const CString&szKey, int iValue);
	void WriteFloat(const CString&szSection, const CString& szKey, float fltValue);
	void WriteBoolean(const CString&szSection, const CString&szKey, bool bolValue);
	void WriteString(const CString& szSection, const CString& szKey, const CString& szValue);

public:
	////////////////���ַ����ж�ȡ�ض���������//////////////////////////////
	static BOOL ExtractFloatFromString(const CString& extrStr, float* outVal);
	static BOOL ExtractIntFromString(const CString& extrStr, int* outVal);
	static BOOL ExtractDoubleFromString(const CString& extrStr, double* outVal);
private:
	CString m_szFileName;
};
#pragma pack(pop)