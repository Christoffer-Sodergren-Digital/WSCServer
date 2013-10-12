// TCPSocket.cpp
#include "TCPSocket.h"

TCPSocket::TCPSocket()
{
	m_xSock=INVALID_SOCKET;
	m_hClose=WSA_INVALID_EVENT;
};

TCPSocket::~TCPSocket()
{
	Close();
};

bool TCPSocket::Connect(IPAddr4 p_xTarget)
{
	m_xSock=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,
		NULL,0,WSA_FLAG_OVERLAPPED);
	if(m_xSock==INVALID_SOCKET)
	{
		return false;
	};
	if(connect(m_xSock,(struct sockaddr*)&p_xTarget,sizeof(p_xTarget))<0)
	{
		return false;
	};
	m_hClose=WSACreateEvent();
	WSAEventSelect(m_xSock,m_hClose,FD_CLOSE);
	return true;
};

void TCPSocket::Close()
{
	if(m_xSock!=INVALID_SOCKET)
	{
		shutdown(m_xSock,SD_BOTH);
		closesocket(m_xSock);
		m_xSock=INVALID_SOCKET;
		if(m_hClose!=WSA_INVALID_EVENT){WSACloseEvent(m_hClose);};
		m_hClose=WSA_INVALID_EVENT;
	};
};

bool TCPSocket::IsConnected() const
{
	return (m_xSock!=INVALID_SOCKET);
};

bool TCPSocket::HasHalfClose() const
{
	if(!IsConnected()){return false;};
	if(WSAWaitForMultipleEvents(1,&m_hClose,TRUE,0,FALSE)==WSA_WAIT_EVENT_0)
	{
		return true;
	};
	return false;
};

int TCPSocket::PendingData() const
{
	if(!IsConnected()){return -1;};
	DWORD uSize;
	int iRet=ioctlsocket(m_xSock,FIONREAD,&uSize);
	if(iRet!=0){return -1;};
	return uSize;
};

bool TCPSocket::CanSendData() const
{
	fd_set xFS;
	xFS.fd_count=1;
	xFS.fd_array[0]=m_xSock;
	TIMEVAL tv;
	tv.tv_sec=0;
	tv.tv_usec=10;
	return (select(0,0,&xFS,0,&tv)==1);
};

bool TCPSocket::CanRecvData() const
{
	fd_set xFS;
	xFS.fd_count=1;
	xFS.fd_array[0]=m_xSock;
	TIMEVAL tv;
	tv.tv_sec=0;
	tv.tv_usec=10;
	return select(0,&xFS,0,0,&tv)==1;
};

bool TCPSocket::WaitForRecvData(int p_iTimeoutMS) const
{
	fd_set xFS;
	xFS.fd_count=1;
	xFS.fd_array[0]=m_xSock;
	TIMEVAL tv;
	if(p_iTimeoutMS<=0)
	{
		tv.tv_sec=0;
		tv.tv_usec=10;
	}
	else
	{
		tv.tv_sec=p_iTimeoutMS/1000;p_iTimeoutMS=p_iTimeoutMS%1000;
		tv.tv_usec=p_iTimeoutMS*1000;
	};
	return select(0,&xFS,0,0,&tv)==1;
};

int TCPSocket::Recv(void* p_pData,int p_iSize,bool p_bPeek)
{
	if(!IsConnected())
	{	
		return -1;
	};

	int iBytesReceived=0;
	int iBytesRemaining=p_iSize;
	int iRet=0;
	do
	{
		iRet=recv(m_xSock,((char*)p_pData)+iBytesReceived,
			iBytesRemaining,p_bPeek?MSG_PEEK:0);
		if(iRet>0)
		{
			iBytesReceived+=iRet;
			iBytesRemaining-=iRet;
		};
	}
	while ((iRet!=SOCKET_ERROR)&&(iBytesRemaining>0)&&(iRet>0));
	return iBytesReceived;
};

int TCPSocket::Send(const void* p_pData,int p_iSize)
{
	return send(m_xSock,(const char*)p_pData,p_iSize,0);
};

IPAddr4 TCPSocket::GetLocalAddr() const
{
	if(!IsConnected()){return IPAddr4();};
	IPAddr4 xAdr;
	int iSize=sizeof(xAdr);
	getsockname(m_xSock,(sockaddr*)&xAdr,&iSize);
	return xAdr;
};

IPAddr4 TCPSocket::GetRemoteAddr() const
{
	if(!IsConnected()){return IPAddr4();};
	IPAddr4 xAdr;
	int iSize=sizeof(xAdr);
	getpeername(m_xSock,(sockaddr*)&xAdr,&iSize);
	return xAdr;
};


TCPAcceptSocket::TCPAcceptSocket()
{
	m_xSock=INVALID_SOCKET;
	m_hEvent=WSA_INVALID_EVENT;
};

TCPAcceptSocket::~TCPAcceptSocket()
{
	Close();
};

bool TCPAcceptSocket::IsOpen() const
{
	return (m_xSock!=INVALID_SOCKET);
};

bool TCPAcceptSocket::Open(IPAddr4 p_xAddr,int p_iMaxPending,int p_iMaxRetriesOnUsed)
{
	do
	{
		p_iMaxRetriesOnUsed--;
		m_xSock=socket(AF_INET,SOCK_STREAM,0);
		if(m_xSock!=INVALID_SOCKET)
		{
			sockaddr_in	xAdr;
			memcpy((char FAR *)&(xAdr.sin_addr),&p_xAddr.sin_addr,4);
			xAdr.sin_family=AF_INET;
			xAdr.sin_port=p_xAddr.sin_port;
			if(bind(m_xSock,(struct sockaddr*)&p_xAddr,sizeof(p_xAddr))==0)
			{
				if(p_iMaxPending==-1){p_iMaxPending=SOMAXCONN;};
				if(listen(m_xSock,p_iMaxPending)==0)
				{
					m_hEvent=WSACreateEvent();
					if(WSAEventSelect(m_xSock,m_hEvent,FD_ACCEPT)==0)
					{
						return true;
					};
				};
			};
		};
		Close();
		Sleep(100);
	}
	while(p_iMaxRetriesOnUsed>=0);
	return false;
};

void TCPAcceptSocket::Close()
{
	if(m_xSock!=INVALID_SOCKET)
	{
		shutdown(m_xSock,SD_BOTH);
		closesocket(m_xSock);
		m_xSock=INVALID_SOCKET;
	};
	if(m_hEvent)
	{
		WSACloseEvent(m_hEvent);
		m_hEvent=WSA_INVALID_EVENT;
	};
};

bool TCPAcceptSocket::ConnectionPending(int p_iTimeout)
{
	if(!IsOpen()){return false;};
	if(WSAWaitForMultipleEvents(1,&m_hEvent,TRUE,p_iTimeout,FALSE)==WSA_WAIT_EVENT_0)
	{
		return true;
	}
	else
	{
		return false;
	};
};

bool TCPAcceptSocket::Accept(TCPSocket& po_rxConnection)
{
	if(!IsOpen()){return false;};
	po_rxConnection.Close();
	WSAResetEvent(m_hEvent);

	sockaddr xA;
	int iSize=sizeof(xA);
	po_rxConnection.m_xSock=WSAAccept(m_xSock,&xA,&iSize,NULL,0);
	if((po_rxConnection.m_xSock==INVALID_SOCKET)||(po_rxConnection.m_xSock==WSAEWOULDBLOCK))
	{
		return false;
	};
	po_rxConnection.m_hClose=WSACreateEvent();
	WSAEventSelect(po_rxConnection.m_xSock,po_rxConnection.m_hClose,FD_CLOSE);
	return true;
};

IPAddr4 TCPAcceptSocket::GetLocalAddr() const
{
	if(!IsOpen()){return IPAddr4();};
	IPAddr4 xAdr;
	int iSize=sizeof(xAdr);
	getsockname(m_xSock,(sockaddr*)&xAdr,&iSize);
	return xAdr;
};







