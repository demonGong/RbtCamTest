#pragma once

#include <WinSock2.h>

#define MAX_BUFF_LENGTH 2088

//socket״̬
enum SocketMode
{
	SOCKET_DISCONNECT = -1,
	SOCKET_CONNECTED,
	SOCKET_SEND_BUF_ERROR
};

class EXT_CLASS myClientSocket
{
public:
	myClientSocket(void);
	virtual ~myClientSocket(void);
public:
	UINT initialClient(); 										//�׽��ֳ�ʼ�����ӷ�����
	UINT ClientSend(char const *pBuf, int nSize);				//�׽��ַ�������
	UINT ClientRecv(char *pBuffer, int len);					//�׽�����ȡ����
	UINT ClientClose();											//�׽��ֹر�

	void setServerIpAddress(const char *address);				//���÷�����IP
	CString getServerIpAddress();								//��ȡ������IP
	void setServerHostPort(UINT hostPort);						//���÷������˿ں�
	UINT getServerHostPort();									//��ȡ�������˿ں�

private:
	SOCKET		m_ClientSocket;									//�ͻ����׽���
	CString		m_ServerIpAddress;								 //������IP��ַ  Ĭ��192.168.1.106
	UINT		m_nHostPort;									//�������˿� Ĭ��30003

public:
	BOOL        m_bClientSocketConnecting;						//�׽������ӱ�־

};

