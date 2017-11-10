#include "StdAfx.h"
#include "myClientSocket.h"


myClientSocket::myClientSocket(void)
{
	m_ClientSocket = -1;
	m_ServerIpAddress = "";
	m_nHostPort = 0;

	m_bClientSocketConnecting = FALSE;
}


myClientSocket::~myClientSocket(void)
{

}

//�׽��ֳ�ʼ�����ӷ�����
UINT myClientSocket::initialClient()
{
	if (TRUE == m_bClientSocketConnecting)
		return SOCKET_CONNECTED;

	WORD wVersionRequested;  
	WSADATA wsaData;  
	int err;  

	wVersionRequested = MAKEWORD( 1, 1 );  

	err = WSAStartup( wVersionRequested, &wsaData );  
	if ( err != 0 )
		return WSAGetLastError();
	
	if ( LOBYTE( wsaData.wVersion ) != 1 ||  
		HIBYTE( wsaData.wVersion ) != 1 ) {  
			WSACleanup( );  
			return WSAGetLastError();   
	}  
	m_ClientSocket = socket(AF_INET,SOCK_STREAM,0);  

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr(m_ServerIpAddress);//�������˵ĵ�ַ  
	addrSrv.sin_family=AF_INET;  
	addrSrv.sin_port=htons(m_nHostPort);  

	if ( SOCKET_CONNECTED == connect(m_ClientSocket,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR)))
	{
		m_bClientSocketConnecting = TRUE;
		return SOCKET_CONNECTED;
	}
	else
	{
		return WSAGetLastError();
	}

}

//�׽��ַ�������
UINT myClientSocket::ClientSend(const char *pBuf, int nSize)
{
	if(m_bClientSocketConnecting)
	{
		if(nSize != send(m_ClientSocket, pBuf, nSize, 0))   //��������
			return SOCKET_SEND_BUF_ERROR;
	}
	else
		return SOCKET_DISCONNECT;

	return SOCKET_CONNECTED;
}

//�׽�����ȡ����
UINT myClientSocket::ClientRecv(char *pBuffer, int len)
{
	int copyBytes;
	copyBytes = recv(m_ClientSocket, pBuffer, len, 0);

	if (0 == copyBytes)
		return SOCKET_DISCONNECT;

	else if (copyBytes < 0)
		return WSAGetLastError();

	else
		return SOCKET_CONNECTED;

}

//�׽��ֹر�
UINT myClientSocket::ClientClose()
{
	m_bClientSocketConnecting = FALSE;
	return closesocket(m_ClientSocket);
}

//���÷�����IP
void myClientSocket::setServerIpAddress(const char *address)
{
	m_ServerIpAddress = address;
}

//��ȡ������IP
CString myClientSocket::getServerIpAddress()
{
	return m_ServerIpAddress;
}

//���÷������˿ں�
void myClientSocket::setServerHostPort(UINT hostPort)
{
	m_nHostPort = hostPort;
}

//��ȡ�������˿ں�
UINT myClientSocket::getServerHostPort()
{
	return m_nHostPort;
}