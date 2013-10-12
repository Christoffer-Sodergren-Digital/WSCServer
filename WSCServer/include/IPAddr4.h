// IPAddr4.h

#pragma once
#ifndef IPADDR4_H_INCLUDED
#define IPADDR4_H_INCLUDED

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <windows.h>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")


class IPAddr4 : public SOCKADDR_IN
{
public:
	IPAddr4();
	IPAddr4(const char* p_sHostAndPort);

	void Dump();
	friend std::ostream & operator<< (std::ostream &out, IPAddr4 const &t);

	void SetPort(int p_iPort);
	int GetPort() const;

	void Set(const char* p_sHostAndPort);

	bool operator==(const IPAddr4& p_xA) const
	{
		return (sin_port==p_xA.sin_port)&&
			(sin_addr.S_un.S_addr==p_xA.sin_addr.S_un.S_addr);
	};
};

class WinsockInitHolder
{
	bool m_bInitialised;
public:
	WinsockInitHolder();
	~WinsockInitHolder();

	bool Initialised() const {return m_bInitialised;};
};

bool SysErrorToText(int p_iErrorCode,char* po_sText,int p_iLen);
void PrintLastSockError();

#endif // IPADDR4_H_INCLUDED
