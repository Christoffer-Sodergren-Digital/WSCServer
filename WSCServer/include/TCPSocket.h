// TCPSocket.h

#pragma once
#ifndef TCPSOCKET_H_INCLUDED
#define TCPSOCKET_H_INCLUDED

#ifndef IPADDR4_H_INCLUDED
#include "IPAddr4.h"
#endif

class TCPAcceptSocket;

class TCPSocket
{
	friend class TCPAcceptSocket;
private:
	TCPSocket(const TCPSocket&);
	TCPSocket& operator=(const TCPSocket&);

	SOCKET m_xSock;
	WSAEVENT m_hClose;
public:

	TCPSocket();
	~TCPSocket();

	bool Connect(IPAddr4 p_xTarget);
	void Close();

	__int64 GetSocket() const {return m_xSock;};
	bool IsConnected() const;

	int PendingData() const;
	bool CanSendData() const;
	bool CanRecvData() const;

	bool WaitForRecvData(int p_iTimeoutMS) const;

	bool HasHalfClose() const;

	int Recv(void* p_pData,int p_iSize,bool p_bPeek=false);
	int Send(const void* p_pData,int p_iSize);

	IPAddr4 GetLocalAddr() const;
	IPAddr4 GetRemoteAddr() const;
};

class TCPAcceptSocket
{
private:
	TCPAcceptSocket(const TCPAcceptSocket&);
	TCPAcceptSocket& operator=(const TCPAcceptSocket&);

	SOCKET m_xSock;
	WSAEVENT			m_hEvent;
public:
	TCPAcceptSocket();
	~TCPAcceptSocket();

	bool IsOpen() const;
	bool Open(IPAddr4 p_xAddr,int p_iMaxPending=-1,int p_iMaxRetriesOnUsed=0);
	void Close();

	__int64 GetSocket() const {return m_xSock;};

	IPAddr4 GetLocalAddr() const;

	bool ConnectionPending(int p_iTimeout=0);
	bool Accept(TCPSocket& po_rxConnection);

	void* GetEventHandle() const {return m_hEvent;};
};

#endif // TCPSOCKET_H_INCLUDED
