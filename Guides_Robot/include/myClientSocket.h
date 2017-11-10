#pragma once

#include <WinSock2.h>

#define MAX_BUFF_LENGTH 2088

//socket状态
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
	UINT initialClient(); 										//套接字初始化连接服务器
	UINT ClientSend(char const *pBuf, int nSize);				//套接字发送数据
	UINT ClientRecv(char *pBuffer, int len);					//套接字收取数据
	UINT ClientClose();											//套接字关闭

	void setServerIpAddress(const char *address);				//设置服务器IP
	CString getServerIpAddress();								//获取服务器IP
	void setServerHostPort(UINT hostPort);						//设置服务器端口号
	UINT getServerHostPort();									//获取服务器端口号

private:
	SOCKET		m_ClientSocket;									//客户端套接字
	CString		m_ServerIpAddress;								 //服务器IP地址  默认192.168.1.106
	UINT		m_nHostPort;									//服务器端口 默认30003

public:
	BOOL        m_bClientSocketConnecting;						//套接字连接标志

};

