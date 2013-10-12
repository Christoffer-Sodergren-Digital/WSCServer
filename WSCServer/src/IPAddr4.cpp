// IPAddr4.cpp

#include "IPAddr4.h"
#include <cstdio>
#include <algorithm>
WinsockInitHolder::WinsockInitHolder()
{
	WSADATA WSAData;
	m_bInitialised=WSAStartup(MAKEWORD(2,1), &WSAData)==0;
};

WinsockInitHolder::~WinsockInitHolder()
{
	WSACleanup();
};


IPAddr4::IPAddr4()
{
	sin_family=AF_INET;
	sin_port=0;
	sin_addr.S_un.S_addr=0;

	for(int i=0;i<8;i++)
	{
		sin_zero[i]=0;
	};
};

IPAddr4::IPAddr4(const char* p_sHostAndPort)
{
	sin_family=AF_INET;
	sin_port=0;
	sin_addr.S_un.S_addr=0;

	for(int i=0;i<8;i++)
	{
		sin_zero[i]=0;
	};
	Set(p_sHostAndPort);
};

void IPAddr4::Dump()
{
	printf("%d.%d.%d.%d:%d\n",sin_addr.S_un.S_un_b.s_b1
		,sin_addr.S_un.S_un_b.s_b2
		,sin_addr.S_un.S_un_b.s_b3
		,sin_addr.S_un.S_un_b.s_b4
		,GetPort());
};

std::ostream & operator<<(std::ostream &out, IPAddr4 const &t){
    out<<(int)t.sin_addr.S_un.S_un_b.s_b1;
    out<<".";
    out<<(int)t.sin_addr.S_un.S_un_b.s_b2;
    out<<".";
    out<<(int)t.sin_addr.S_un.S_un_b.s_b3;
    out<<".";
    out<<(int)t.sin_addr.S_un.S_un_b.s_b4;
    return out;
}

void IPAddr4::SetPort(int p_iPort)
{
	sin_port=htons((u_short)p_iPort);
};

int IPAddr4::GetPort() const
{
	return ntohs(sin_port);
};

// www.pixeltamer.net
// www.hgo.se:80
void IPAddr4::Set(const char* p_sHostAndPort)
{
	sin_addr.S_un.S_un_b.s_b1=127;
	sin_addr.S_un.S_un_b.s_b2=0;
	sin_addr.S_un.S_un_b.s_b3=0;
	sin_addr.S_un.S_un_b.s_b4=1;
	sin_port=0;

	int iL=(int)strlen(p_sHostAndPort);
	iL=min(iL,127);
	char sHost[128];
	strncpy(sHost,p_sHostAndPort,iL);
	sHost[iL]=0;
	char* sColon=strchr(sHost,':');
	if(sColon)
	{
		*sColon=0;
		SetPort(atoi(sColon+1));
	};
	hostent* pHE;
	pHE=gethostbyname(sHost);
	if(!pHE){return;};
	if(pHE->h_length==4)
	{
		sin_addr=*((in_addr*)pHE->h_addr_list[0]);
	};
};

bool SysErrorToText(int p_iErrorCode,char* po_sText,int p_iLen)
{
	if(!FormatMessageA(FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,p_iErrorCode,0,po_sText,p_iLen,NULL))
	{
		strcpy(po_sText,"unknown");
		return false;
	};
	return true;
};

void PrintLastSockError()
{
	char sTmp[512];
	int iErr=WSAGetLastError();
	SysErrorToText(iErr,sTmp,512);
	printf("Error %d: %s\r\n",iErr,sTmp);
};

