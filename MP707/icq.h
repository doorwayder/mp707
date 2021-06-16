//#pragma once

#include "ICQPacket_define.h"
#include "SICQ_define.h" 
#include <winsock2.h>
#include <shlwapi.h>
#pragma comment(lib,"wsock32.lib")

#define EPOCH_BIAS  116444736000000000i64 

class random
{
private:
	int nInit;
public:
	random(void);
	~random(void);

	unsigned int randomDWORD();
	unsigned short randomWORD();
};


//! \}
////variables
	int nError;
	HWND hEventWnd;
	TCHAR szUIN[20];
	TCHAR szPassword[20];
	TCHAR szServerIP[20];
	int nServerPort;
	SOCKET sock;
	int nSequence;
	int nStatus;
	static char HwndList[1000];
	FOODGROUPS FoodGroups;
	HWND hMainWnd;
	char *pPacket;
	int nPacketSize;


//functions
int Recv(SOCKET sock);
int Send(SOCKET sock);
bool IsHelloPacket();
bool IsSignOffChannel();
bool IsTLVPresent(unsigned short Type);
bool IsFLAPPacket();
bool ReadRecvMessagePacket(RECVMESSAGESTRUCT *pRms);
bool IsSNACPresent(unsigned short family,unsigned short subtype);
bool ReadFoodGroupsFamiliesPacket(FOODGROUPS *pFgs);
bool ReadFoodGroupsVersionsPacket(FOODGROUPS *pFgs);
bool _connect(SOCKET sock,TCHAR *pszServerIP,int nServerPort);
void SetFLAPHeader(char Channel,short Sequence);
void SequenceIncrement();
void _CopyMemory(void *pDest,void *pSource,int nSize);
void *_Alloc(int nSize);
void _ZeroMemory(void *pDest,int nSize);
void SetFLAPDataSize(unsigned short DataSize);
void ICQBOSServerConnect(TCHAR *pszBOSServerIPAndPort,char *pCookies,int nCookiesSize);
void SetFLAPChannel(unsigned char channel);
void SetFLAPSequence(unsigned short sequence);
void _closeconnect(SOCKET sock);
int CreatePlainLoginPacket(int nSequence,TCHAR *pszUIN,TCHAR *pszPassword);
int GetTLV_blob(unsigned short Type,char *pBuffer,int nBufferSize);
int GetTLV_string(unsigned short Type,TCHAR *pszBuffer,int nBufferLength);
int Add_u32_BE(unsigned int nU32);
int Add_TLV_string(unsigned short Type,TCHAR *pszString);
int Add_TLV_password(unsigned short Type,TCHAR *pszPassword);
int Add_TLV_u16(unsigned short Type,unsigned short sU16);
int Add_TLV_u32(unsigned short Type,unsigned int nU32);
int Add_TLV_blob(unsigned short Type,char *bData,int nDataSize);
int Add_SNACHeader(unsigned short family,unsigned short subtype,unsigned short flags,unsigned int requestid);
int GetSNACDataSize();
int CreateSetICBMParametersPacket(int nSequence);
int GetPacketSize();
int _MinInt(int n1,int n2);
int Add_u16_BE(unsigned short sU16);
int Add_u32_BE(unsigned int nU32);
int Add_blob(char *bData,int nDataSize);
int Add_TLVHeader(unsigned short Type,unsigned short Length);
int Add_u32_BE(unsigned int nU32);
int Add_string(TCHAR *pszString);
int CreateFoodGroupsVersionsPacket(int nSequence,FOODGROUPS *pFgs);
int CreateRequestRatesPacket(int nSequence);
int CreateAcceptRatesPacket(int nSequence);
int CreateRequestRosterFirstTimePacket(int nSequence);
int CreateLoadRosterAfterLoginPacket(int nSequence);
int CreateRequestBuddyParametersPacket(int nSequence);
int CreateSetStatusPacket(int nSequence,int nStatus);
int CreateClientReadyPacket(int nSequence);
int CreatePingPacket(int nSequence);
int _StringToChars(char *pszChars,int nCharsSize,TCHAR *pszString);
int _recv(SOCKET sock,char *pBuffer,int nBufferSize);
int _send(SOCKET sock,char *pBuffer,int nBufferSize);
int SendText(TCHAR *pszUIN,TCHAR *pszText,int nTextLength);
int _LocalTimeAsUnixTime();
int CreateSendTextUnicodePacket(int nSequence,SENDTEXTSTRUCT *pSts);
int _FileTimeToUnixTime(FILETIME *pFileTeme);
int _StringToUnicode(WCHAR *pszUnicode,int nUnicodeLength,TCHAR *pszString);
int _UnicodeToNet(WCHAR *pszNet,int nNetLength,WCHAR *pszUnicode);
int _CharsToUnicode(WCHAR *pszUnicode,int nUnicodeLength,char *pszChars);
int Add_u8(unsigned char cU8);
int Add_string08(TCHAR *pszString);
int ReadNickInfoFromOffset(char *pOffset,NICKINFOSTRUCT *pNis);
int _NetToUnicode(WCHAR *pszUnicode,int nUnicodeLength,WCHAR *pszNet);
int _UnicodeToString(TCHAR *pszString,int nStringLength,WCHAR *pszUnicode);
int Get_string08(char *pOffset,TCHAR *pszString,int nStringLength);
void ICQSendText(SENDTEXTSTRUCT *pSts);
int _UnicodeToChars(char *pszChars,int nCharsSize,WCHAR *pszUnicode);
char *GetTLVPointer(unsigned short Type);
unsigned int Get_u32_BE_FromOffset(char *pOffset);
unsigned short GetTLVTypeFromOffset(char *pOffset);
unsigned short GetTLVLehgthFromOffset(char *pOffset);
unsigned short GetFragmentLehgthFromOffset(char *pOffset);
unsigned short GetTLV_u16(unsigned short Type);
unsigned short GetFLAPDataSize();
unsigned short Get_u16_BE_FromOffset(char *pOffset);
unsigned char Get_u8_FromOffset(char *pOffset);
unsigned char GetFLAPChannel();
char *GetPacketPointer();
char *GetSNACDataPointer();
int _CharsToString(TCHAR *pszString,int nStringLength,char *pszChars);
SOCKET _socket();
void _Free(void *pMemory);
bool _LoadWS();
void _UnloadWS();





bool ilogin ()
{
	TCHAR szBuffer[256];
	char Cookies[256];
	int nCookiesSize;
	pPacket=(char *)_Alloc(ICQPACKET_MAXPACKETSIZE); 

	struct hostent *remoteHost;
	remoteHost = gethostbyname("login.icq.com");
	if (remoteHost==NULL) return false;
	char ip[16];
	strcpy(ip, inet_ntoa(*((in_addr*)remoteHost->h_addr_list[0])));

	sock=_socket();
	if(_connect(sock,ip,5190))
	{
		Recv(sock);

		if(IsHelloPacket())
		{
			CreatePlainLoginPacket(1,"635445356","S1lent~L");
			//CreatePlainLoginPacket(1,"413520834","S1lent~L");
			Send(sock);
			SequenceIncrement();

			Recv(sock);

			if(IsSignOffChannel())
			{
				if(IsTLVPresent(ICQ_TLV_AUTHCOOKIE))
				{
					nCookiesSize=GetTLV_blob(ICQ_TLV_AUTHCOOKIE,Cookies,sizeof(Cookies));
					GetTLV_string(ICQ_TLV_BOSSERVER,szBuffer,sizeof(szBuffer)/sizeof(TCHAR));

					ICQBOSServerConnect(szBuffer,Cookies,sizeof(Cookies));
				}
				else if(IsTLVPresent(ICQ_TLV_AUTHERRORCODE))
				{

					switch(GetTLV_u16(ICQ_TLV_AUTHERRORCODE))
					{
					case ICQ_AUTHERROR_INCORRECTNICKORPASS:
					case ICQ_AUTHERROR_MISMATCHNICKORPASS:
						nError=SICQ_ERROR_INCORRECTUINORPASSWORD;
						break;
					case ICQ_AUTHERROR_RATELIMITEXCEEDEDRES:
						nError=SICQ_ERROR_RATELIMITEXCEEDED;
						break;
					default:
						nError=SICQ_ERROR_UNKNOWN;
					}
				}
			}
			else
			{
				nError=SICQ_ERROR_UNKNOWN;
			}
		}
	}
	else
	{
		nError=SICQ_ERROR_CANNOTCONNECTTOSERVER;
	}

	if(nError==SICQ_ERROR_SUCCESS)
	{
		//AfxMessageBox("Connected");
		nStatus=SICQ_STATUS_ONLINE;
		return true;
	}
	else
	{
		//AfxMessageBox("Error Connecting!");
		nStatus=SICQ_STATUS_OFFLINE;
		return false;
	}
}	

/////////
//Recv...
int Recv(SOCKET sock)
{
	nPacketSize=0;

	if(_recv(sock,pPacket,sizeof(FLAP_HEADER)))
	{
		if(IsFLAPPacket())
		{
			nPacketSize=GetFLAPDataSize();

			if(_recv(sock,pPacket+sizeof(FLAP_HEADER),nPacketSize))
			{
				nPacketSize+=sizeof(FLAP_HEADER);
			}
			else
			{
				nPacketSize=0;
			}
		}
		else
		{

		}


	}


	return nPacketSize;
}

//////////
//Send...
int Send(SOCKET sock)
{
	int nResult=0;

	if(IsFLAPPacket())
	{
		nResult=_send(sock,pPacket,nPacketSize);

		if(nResult=-1)
		{
			//if (WSAGetLastError()!=10035) AfxMessageBox("Socket Error");

		}
	}
	
	return nResult;
}


//HELLO MESSAGE
bool IsHelloPacket()
{
	char channel=GetFLAPChannel();
	int DataSize=GetFLAPDataSize();

	if((channel==ICQ_FLAP_CHANNEL_SIGNON)&&(DataSize==4))
	{
		return true;
	}
	else
	{
		return false;
	}
}


////PACKETS

int CreatePlainLoginPacket(int nSequence,TCHAR *pszUIN,TCHAR *pszPassword)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SIGNON,nSequence);

	Add_u32_BE(0x00000001); // Version
	Add_TLV_string(ICQ_TLV_UIN,pszUIN);
	Add_TLV_password(ICQ_TLV_PASSWORD,pszPassword);
	Add_TLV_string(ICQ_TLV_CLIENTIDSTRING,TEXT("ICQ Inc. - Product of ICQ (TM).2002a.5.37.1.3728.85"));
	Add_TLV_u16(ICQ_TLV_CLIENTID,0x010A);
	Add_TLV_u16(ICQ_TLV_CLIENTMAJORVER,0x0005);
	Add_TLV_u16(ICQ_TLV_CLIENTMINORVER,0x0025);
	Add_TLV_u16(ICQ_TLV_CLIENTLESSERVER,0x0001);
	Add_TLV_u16(ICQ_TLV_CLIENTBUILDNUM,0x0E90);
	Add_TLV_u32(ICQ_TLV_DISTRIBNUMBER,0x00005500);
	Add_TLV_string(ICQ_TLV_CLIENTLANG,TEXT("en"));
	Add_TLV_string(ICQ_TLV_CLIENTCOUNTRY,TEXT("us"));


	return nPacketSize;
}
//! CreateGoodByePacket
//! \param nSequence [in] a Sequence
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int CreateGoodByePacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SIGNOFF,nSequence);


	return nPacketSize;
}



//! CreatePingPacket
//! \param nSequence [in] a Sequence
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int CreatePingPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_KEEPALIVE,nSequence);


	return nPacketSize;
}





//! CreateSendCookiesPacket
//! \param nSequence [in] a Sequence
//! \param pCookies [in] a pointer to a buffer that contains cookies data
//! \param nCookiesSize [in] a size of cookies data
//! \return a size of ICQ Packet
//! \sa FLAP_HEADER, TLV_HEADER
int CreateCookiesPacket(int nSequence,char *pCookies,int nCookiesSize)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SIGNON,nSequence);

	Add_u32_BE(0x00000001); // Version
	Add_TLV_blob(ICQ_TLV_AUTHCOOKIE,pCookies,nCookiesSize);

	return nPacketSize;
}


void SequenceIncrement()
{
	if(nSequence<0x8000)
	{
		nSequence++;
	}
	else
	{
		nSequence=0;
	}
}


bool IsSignOffChannel()
{
	if(GetFLAPChannel()==ICQ_FLAP_CHANNEL_SIGNOFF)
	{
	return true;
	}
	else
	{
		return false;
	}
}

char *GetTLVPointer(unsigned short Type)
{
	int nDataSize=nPacketSize;
	char *pOffset=pPacket;

	if(nDataSize>=sizeof(TLV_HEADER))
	{
		pOffset+=sizeof(FLAP_HEADER);

		while(nDataSize>0)
		{
			if(GetTLVTypeFromOffset(pOffset)==Type)
			{
				return pOffset;
			}
			else
			{
				nDataSize-=sizeof(TLV_HEADER)+GetTLVLehgthFromOffset(pOffset);
				pOffset+=sizeof(TLV_HEADER)+GetTLVLehgthFromOffset(pOffset);
			}
		}
	}
	return 0;

}
unsigned short GetTLVTypeFromOffset(char *pOffset)
{
	return ntohs(((TLV_HEADER *)pOffset)->type);
}
unsigned short GetTLVLehgthFromOffset(char *pOffset)
{
	return ntohs(((TLV_HEADER *)pOffset)->length);
}
unsigned short GetFragmentLehgthFromOffset(char *pOffset)
{
	return ntohs(((FRAGMENT *)pOffset)->Length);
}


bool IsTLVPresent(unsigned short Type)
{
	return (GetTLVPointer(Type)!=0);
}


/////////////
/////////
int GetTLV_blob(unsigned short Type,char *pBuffer,int nBufferSize)
{
	char *pOffset=GetTLVPointer(Type);

	if(pOffset)
	{
		int nSize=GetTLVLehgthFromOffset(pOffset);

		if(nSize<=nBufferSize)
		{
			_CopyMemory(pBuffer,pOffset+sizeof(TLV_HEADER),nSize);
			return nSize;
		}
	}
	return 0;
}


////////////
///////////
int GetTLV_string(unsigned short Type,TCHAR *pszBuffer,int nBufferLength)
{
	char *pOffset=GetTLVPointer(Type);

	if(pOffset)
	{
		int nSize=GetTLVLehgthFromOffset(pOffset);

		if(nSize<=nBufferLength-1)
		{
			char *pBuffer=(char *)_Alloc(nSize+1);
			_CopyMemory(pBuffer,pOffset+sizeof(TLV_HEADER),nSize);
			*(pBuffer+nSize)=0;
			_CharsToString(pszBuffer,nBufferLength,pBuffer);
			_Free(pBuffer);

			return nSize;
		}
	}
	return 0;
}
//! Get TLV short int
//! \param Type TLV type.
//! \return TLV short int
//! \sa TLV_HEADER
unsigned short GetTLV_u16(unsigned short Type)
{

	char *pOffset=GetTLVPointer(Type);

	if(pOffset)
	{
		if(GetTLVLehgthFromOffset(pOffset)==2)
		{
			return htons(*((unsigned short *)(pOffset+sizeof(TLV_HEADER))));
		}
	}
	return 0;
}

char *GetSNACPointer()
{
	if(GetPacketSize()>=sizeof(FLAP_HEADER)+sizeof(SNAC_HEADER))
	{
		return GetPacketPointer()+sizeof(FLAP_HEADER);
	}
	else
	{
		return 0;
	}
}

unsigned short GetSNACFamily()
{
	char *pOffset=GetSNACPointer();

	if(pOffset)
	{
		return ntohs(((SNAC_HEADER *)pOffset)->family);
	}
	else
	{
		return 0;
	}
}
unsigned short GetSNACSubtype()
{
	char *pOffset=GetSNACPointer();

	if(pOffset)
	{
		return ntohs(((SNAC_HEADER *)pOffset)->subtype);
	}
	else
	{
		return 0;
	}
}
unsigned short GetSNACFlags()
{
	char *pOffset=GetSNACPointer();

	if(pOffset)
	{
		return ntohs(((SNAC_HEADER *)pOffset)->flags);
	}
	else
	{
		return 0;
	}
}
unsigned int GetSNACRequestid()
{
	char *pOffset=GetSNACPointer();

	if(pOffset)
	{
		return ntohl(((SNAC_HEADER *)pOffset)->requestid);
	}
	else
	{
		return 0;
	}
}


void ICQBOSServerConnect(TCHAR *pszBOSServerIPAndPort,char *pCookies,int nCookiesSize)
{
	TCHAR szBOSServer[64],*pszOffset;
	int nBOSServerPort;

	pszOffset=strstr(pszBOSServerIPAndPort,TEXT(":"));
	pszOffset++;
	lstrcpyn(szBOSServer,pszBOSServerIPAndPort,(pszOffset-pszBOSServerIPAndPort));
	nBOSServerPort=atoi(pszOffset);

	CreateGoodByePacket(nSequence);
	Send(sock);
	SequenceIncrement();

	_closeconnect(sock);

	// BOS Server Connection
	sock=_socket();
	if(_connect(sock,szBOSServer,nBOSServerPort))
	{
		while(true)
		{
			Recv(sock);

			if(IsHelloPacket())
			{

				CreateCookiesPacket(nSequence,pCookies,nCookiesSize);
				Send(sock);
				SequenceIncrement();
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_FAMILIES))
			{

				ReadFoodGroupsFamiliesPacket(&FoodGroups);

				CreateFoodGroupsVersionsPacket(nSequence,&FoodGroups);
				Send(sock);
				SequenceIncrement();
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_VERSIONS))
			{
				ReadFoodGroupsVersionsPacket(&FoodGroups);
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_MESSAGEOFTHEDAY))
			{
				CreateRequestRatesPacket(nSequence);
				Send(sock);
				SequenceIncrement();
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_WELLKNOWNURLS))
			{

			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_RATELIMITS))
			{
				CreateAcceptRatesPacket(nSequence);
				Send(sock);
				SequenceIncrement();

				CreateRequestRosterFirstTimePacket(nSequence);
				Send(sock);
				SequenceIncrement();
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_SSI,ICQ_SNAC_SSI_ROSTER))
			{
				CreateLoadRosterAfterLoginPacket(nSequence);
				Send(sock);
				SequenceIncrement();

				CreateSetICBMParametersPacket(nSequence);
				Send(sock);
				SequenceIncrement();

				CreateRequestBuddyParametersPacket(nSequence);
				Send(sock);
				SequenceIncrement();
			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_ONLINEINFO))
			{

			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_EXTENDEDSTATUS))
			{

			}
			else if(IsSNACPresent(ICQ_SNAC_FOODGROUP_BUDDY,ICQ_SNAC_BUDDY_PARAMETERS))
			{
				CreateSetStatusPacket(nSequence,ICQ_STATUS_ONLINE);
				Send(sock);
				SequenceIncrement();

				CreateClientReadyPacket(nSequence);
				Send(sock);
				SequenceIncrement();

				nError=SICQ_ERROR_SUCCESS;

				WSAAsyncSelect(sock,hEventWnd,WM_SICQ_EVENTWND_RECVDATA,FD_READ);
				
				break;
			}
			else
			{
			}
		}
	}
	else
	{
		nError=SICQ_ERROR_CANNOTCONNECTTOSERVER;
	}
}


////////////
/////////////
bool IsFLAPPacket()
{
	return *pPacket==ICQ_FLAP_SIGNATURE;
}
//! \return FLAP channel
//! \sa FLAP_HEADER
unsigned char GetFLAPChannel()
{
	return ((FLAP_HEADER *)pPacket)->channel;
}


unsigned short GetFLAPDataSize()
{
	return (ntohs)(((FLAP_HEADER *)pPacket)->lenght);
}



void SetFLAPHeader(char Channel,short Sequence)
{
	((FLAP_HEADER *)pPacket)->signature=0x2A;
	SetFLAPChannel(Channel);
	SetFLAPSequence(Sequence);
	SetFLAPDataSize(0);

	nPacketSize=sizeof(FLAP_HEADER);
}


int Add_TLV_string(unsigned short Type,TCHAR *pszString)
{
	int nStringLength=lstrlen(pszString);

	Add_TLVHeader(Type,nStringLength);
	Add_string(pszString);

	return nStringLength+sizeof(TLV_HEADER);
}


int Add_TLV_password(unsigned short Type,TCHAR *pszPassword)
{
	unsigned char MagicXor[]={0xF3, 0x26, 0x81, 0xC4, 0x39, 0x86, 0xDB, 0x92, 0x71, 0xA3, 0xB9, 0xE6, 0x53, 0x7A, 0x95, 0x7C};

	int nPasswordLength=lstrlen(pszPassword);
	char *pChars=(char *)_Alloc(nPasswordLength+1);

	_StringToChars(pChars,nPasswordLength+1,pszPassword);

	for(int i=0;i<nPasswordLength;i++)
	{
		pChars[i]^=MagicXor[i];
	}

	Add_TLVHeader(Type,nPasswordLength);
	Add_blob(pChars,nPasswordLength);

	_Free(pChars);

	return nPasswordLength+sizeof(TLV_HEADER);
}



int Add_TLV_u16(unsigned short Type,unsigned short sU16)
{
	Add_TLVHeader(Type,2);
	Add_u16_BE(sU16);

	return 2+sizeof(TLV_HEADER);
}
int Add_TLV_u32(unsigned short Type,unsigned int nU32)
{
	Add_TLVHeader(Type,4);
	Add_u32_BE(nU32);

	return 4+sizeof(TLV_HEADER);
}

int Add_TLV_blob(unsigned short Type,char *bData,int nDataSize)
{
	Add_TLVHeader(Type,nDataSize);
	Add_blob(bData,nDataSize);

	return nDataSize+sizeof(TLV_HEADER);
}


void _CopyMemory(void *pDest,void *pSource,int nSize)
{
	if(nSize)
	{
		__asm
		{
			mov		ecx,nSize
			mov		esi,pSource
			mov		edi,pDest
			cld	
			rep		movsb
		}
	}
}


void *_Alloc(int nSize)
{
	return (void *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,nSize);
}


int _CharsToString(TCHAR *pszString,int nStringLength,char *pszChars)
{
#ifdef  UNICODE
	return _CharsToUnicode(pszString,nStringLength,pszChars);
#else
	lstrcpyn(pszString,pszChars,nStringLength);
	return lstrlen(pszString);
#endif
}


void _Free(void *pMemory)
{
	GlobalFree(pMemory);
}


int GetPacketSize()
{
	return nPacketSize;
}


char *GetPacketPointer()
{
	return pPacket;
}


int Add_u16_BE(unsigned short sU16)
{
	*(unsigned short *)(pPacket+nPacketSize)=htons(sU16);
	nPacketSize+=2;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP_HEADER));

	return 2;
}
int Add_u32_BE(unsigned int nU32)
{
	*(unsigned int *)(pPacket+nPacketSize)=htonl(nU32);
	nPacketSize+=4;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP_HEADER));

	return 4;
}
int Add_blob(char *bData,int nDataSize)
{
	_CopyMemory(pPacket+nPacketSize,bData,nDataSize);
	nPacketSize+=nDataSize;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP_HEADER));

	return nDataSize;
}

void SetFLAPDataSize(unsigned short DataSize)
{
	((FLAP_HEADER *)pPacket)->lenght=htons(DataSize);
}


int Add_TLVHeader(unsigned short Type,unsigned short Length)
{
	Add_u16_BE(Type);
	Add_u16_BE(Length);

	return sizeof(TLV_HEADER);
}


int Add_string(TCHAR *pszString)
{
	int nStringLength=lstrlen(pszString);
	char *pChars=(char *)_Alloc(nStringLength+1);

	_StringToChars(pChars,nStringLength+1,pszString);
	Add_blob(pChars,nStringLength);

	_Free(pChars);

	return nStringLength;
}



/////////////////
////////////////////
bool IsSNACPresent(unsigned short family,unsigned short subtype)
{
	if((GetSNACFamily()==family)&&(GetSNACSubtype()==subtype))
	{


		return true;
	}
	else
	{
		return false;
	}
}



////////////////////////////
/////////////////////////////
bool ReadFoodGroupsFamiliesPacket(FOODGROUPS *pFgs)
{
	_ZeroMemory(pFgs,sizeof(FOODGROUPS));

	int nSize=GetSNACDataSize();
	char *pOffset=GetSNACDataPointer();

	if(nSize&&IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_FAMILIES))
	{
		while(nSize>0)
		{
			switch(Get_u16_BE_FromOffset(pOffset))
			{
			case ICQ_SNAC_FOODGROUP_OSERVICE:
				pFgs->Oservice.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_LOCATE:
				pFgs->Locate.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_BUDDY:
				pFgs->Buddy.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_ICBM:
				pFgs->ICBM.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_INVITE:
				pFgs->Invite.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_POPUP:
				pFgs->Popup.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_PD:
				pFgs->PD.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_LOOKUP:
				pFgs->Lookup.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_STATS:
				pFgs->Stats.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_TRANS:
				pFgs->Trans.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_SSI:
				pFgs->SSI.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_SPEC:
				pFgs->Spec.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_UNK1:
				pFgs->Unk1.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_UNK2:
				pFgs->Unk2.Support=true;
				break;
			case ICQ_SNAC_FOODGROUP_UNK3:
				pFgs->Unk3.Support=true;
				break;
			}

			pOffset+=2;
			nSize-=2;
		}

		return true;
	}
	else
	{
		return false;
	}
}
/////////////////////
/////////////////////
////////////////
int CreateFoodGroupsVersionsPacket(int nSequence,FOODGROUPS *pFgs)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_REQUESTVERSIONS,0,nSequence);

	if(pFgs->Unk1.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_UNK1);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Oservice.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_OSERVICE);
		Add_u16_BE(0x0004); // Version
	}
	if(pFgs->SSI.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_SSI);
		Add_u16_BE(0x0004); // Version
	}
	if(pFgs->Locate.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_LOCATE);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Buddy.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_BUDDY);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Spec.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_SPEC);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->ICBM.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_ICBM);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Invite.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_INVITE);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->PD.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_PD);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Lookup.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_LOOKUP);
		Add_u16_BE(0x0001); // Version
	}
	if(pFgs->Stats.Support)
	{
		Add_u16_BE(ICQ_SNAC_FOODGROUP_STATS);
		Add_u16_BE(0x0001); // Version
	}

	return nPacketSize;
}


///////////////////////
////////////////////////
int CreateRequestRatesPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_REQUESTRATELIMITS,0,nSequence);

	return nPacketSize;
}





bool ReadFoodGroupsVersionsPacket(FOODGROUPS *pFgs)
{
	int nSize=GetSNACDataSize();
	char *pOffset=GetSNACDataPointer();

	if(nSize&&IsSNACPresent(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_VERSIONS))
	{
		while(nSize>0)
		{
			switch(Get_u16_BE_FromOffset(pOffset))
			{
			case ICQ_SNAC_FOODGROUP_OSERVICE:
				pFgs->Oservice.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_LOCATE:
				pFgs->Locate.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_BUDDY:
				pFgs->Buddy.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_ICBM:
				pFgs->ICBM.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_INVITE:
				pFgs->Invite.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_POPUP:
				pFgs->Popup.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_PD:
				pFgs->PD.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_LOOKUP:
				pFgs->Lookup.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_STATS:
				pFgs->Stats.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_TRANS:
				pFgs->Trans.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_SSI:
				pFgs->SSI.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_SPEC:
				pFgs->Spec.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_UNK1:
				pFgs->Unk1.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_UNK2:
				pFgs->Unk2.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			case ICQ_SNAC_FOODGROUP_UNK3:
				pFgs->Unk3.Version=Get_u16_BE_FromOffset(pOffset+2);
				break;
			}

			pOffset+=4;
			nSize-=4;
		}

		return true;
	}
	else
	{
		return false;
	}
}



int CreateAcceptRatesPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_ACCEPTRATELIMITS,0,nSequence);

	Add_u16_BE(0x0001);
	Add_u16_BE(0x0002);
	Add_u16_BE(0x0003);
	Add_u16_BE(0x0004);
	Add_u16_BE(0x0005);

	return nPacketSize;
}


int CreateRequestRosterFirstTimePacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_SSI,ICQ_SNAC_SSI_REQUESTROSTERFIRSTTIME,0,nSequence);

	return nPacketSize;
}


unsigned short Get_u16_BE_FromOffset(char *pOffset)
{
	return ntohs(*((unsigned short *)pOffset));
}
unsigned char Get_u8_FromOffset(char *pOffset)
{
	return *pOffset;
}


int CreateLoadRosterAfterLoginPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_SSI,ICQ_SNAC_SSI_LOADROSTERAFTERLOGIN,0,nSequence);

	return nPacketSize;
}


int CreateSetICBMParametersPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_ICBM,ICQ_SNAC_ICBM_SETPARAMETERS,0,nSequence);

	Add_u16_BE(0x0002);
	Add_u32_BE(0x00000003); // ICBM__PARAMETER_FLAGS
	Add_u16_BE(0x1F40);
	Add_u16_BE(0x03E7);
	Add_u16_BE(0x03E7);
	Add_u32_BE(0x00000000);

	return nPacketSize;
}


int Add_SNACHeader(unsigned short family,unsigned short subtype,unsigned short flags,unsigned int requestid)
{
	Add_u16_BE(family);
	Add_u16_BE(subtype);
	Add_u16_BE(flags);
	Add_u32_BE(requestid);

	return sizeof(SNAC_HEADER);
}


char *GetSNACDataPointer()
{
	if(GetSNACDataSize())
	{
		return GetSNACPointer()+sizeof(SNAC_HEADER);
	}
	else
	{
		return 0;
	}
}


int GetSNACDataSize()
{
	int nSize=GetPacketSize()-(sizeof(FLAP_HEADER)+sizeof(SNAC_HEADER));
	if(nSize>0)
	{
		return nSize;
	}
	else
	{
		return 0;
	}
}


void _ZeroMemory(void *pDest,int nSize)
{

	if(nSize)
	{
		__asm
		{
			xor		eax,eax
			mov		ecx,nSize
			mov		edi,pDest
			cld	
			rep		stosb
		}
	}
}


int CreateRequestBuddyParametersPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_BUDDY,ICQ_SNAC_BUDDY_REQUESTPARAMETERS,0,nSequence);

	Add_u16_BE(0x0005);
	Add_u16_BE(0x0002);
	Add_u16_BE(0x0003);

	return nPacketSize;
}


int CreateSetStatusPacket(int nSequence,int nStatus)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_SETSTATUS,0,nSequence);

	Add_TLV_u32(ICQ_TLV_USERSTATUS,nStatus);

	return nPacketSize;
}


int CreateClientReadyPacket(int nSequence)
{
	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_OSERVICE,ICQ_SNAC_OSERVICE_CLIENTREADY,0,nSequence);

	Add_u16_BE(0x0022);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0001);
	Add_u16_BE(0x0004);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0013);
	Add_u16_BE(0x0004);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0002);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0003);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0015);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0004);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0006);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x0009);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x000A);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	Add_u16_BE(0x000B);
	Add_u16_BE(0x0001);
	Add_u16_BE(0x0110);
	Add_u16_BE(0x164F);

	return nPacketSize;
}


void SetFLAPChannel(unsigned char channel)
{
	((FLAP_HEADER *)pPacket)->channel=channel;
}


int _StringToChars(char *pszChars,int nCharsSize,TCHAR *pszString)
{
#ifdef  UNICODE
	return _UnicodeToChars(pszChars,nCharsSize,pszString);
#else
	lstrcpyn(pszChars,pszString,nCharsSize);
	return lstrlen(pszChars);
#endif
}


void SetFLAPSequence(unsigned short sequence)
{
	((FLAP_HEADER *)pPacket)->sequence=htons(sequence);
}


bool _connect(SOCKET sock,TCHAR *pszServerIP,int nServerPort)
{
	sockaddr_in sa;
	char szBuffer[256];
	int nResult=0;

	_StringToChars(szBuffer,sizeof(szBuffer),pszServerIP);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(nServerPort);
	sa.sin_addr.s_addr = inet_addr(szBuffer);

	nResult=connect(sock,(const sockaddr *)&sa,sizeof(sa));

	if(nResult!=SOCKET_ERROR)
	{
		return true;
	}
	else
	{
		return false;
	}
}


SOCKET _socket()
{
	int nResult=0;
	nResult=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(nResult!=SOCKET_ERROR)
	{
		return nResult;
	}
	else
	{

		return 0;
	}
}


int _recv(SOCKET sock,char *pBuffer,int nBufferSize)
{
	int nSize=0,nResult=0,err;
	
	for(int i=0,j=nBufferSize;j>0;i+=1000,j-=1000)
	{
		nResult=recv(sock,&pBuffer[i],_MinInt(j,1000),0);
		if(nResult!=SOCKET_ERROR)
		{
			nSize+=nResult;
		}
		else 
		{
			err=WSAGetLastError();
			if (err!=10035){
			CString tmp;tmp.Format("%d",err);
			AfxMessageBox(tmp);
			nSize=0;
			j=0;}
		}
	}

	return nSize;

}

//! The _send function sends data on a connected socket 
//! \param sock [in] a descriptor identifying a connected socket
//! \param pBuffer [in] A pointer to a buffer containing the data to be transmitted
//! \param nBufferSize [in] the size, in bytes, of the buffer pointed to by the pBuffer parameter
//! \return If no error occurs, send returns the total number of bytes sent, which can be less 
//! than the number requested to be sent in the nBufferSize parameter
int _send(SOCKET sock,char *pBuffer,int nBufferSize)
{
	int nSize=0;

	for(int i=0,j=nBufferSize;j>0;i+=1000,j-=1000)
	{
		nSize+=send(sock,&pBuffer[i],_MinInt(j,1000),0);
	}

	return nSize;
}
//! Close connection
//! \param sock [in] a descriptor identifying a connected socket
void _closeconnect(SOCKET sock)
{
	shutdown(sock,2);
	closesocket(sock);
}

int _MinInt(int n1,int n2)
{
	if(n1<n2)
		return n1;
	else
		return n2;
}



bool _LoadWS()
{
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);

	return WSAStartup(wVersionRequested, &wsaData)==0;
}
//! Unload WinSocket
//! 


//class Random
random::random(void)
{
	unsigned int nTemp;

	__asm
	{
		rdtsc
			bswap	eax

			mov		nTemp,eax
	}

	nInit=nTemp;

	randomDWORD();
}
//! Destructor
random::~random(void)
{
}
//! \return a random number between 0 and 4294967295
unsigned int random::randomDWORD()
{
	nInit=((400014+40692)*nInit)%2147483563;
	return nInit;
}
//! \return a random number between 0 and 65534
unsigned short random::randomWORD()
{
	return randomDWORD()>>16;
}
///////////
////////


void _UnloadWS()
{
	WSACleanup();
}


int SendText(TCHAR *pszUIN,TCHAR *pszText,int nTextLength)
{
	random rand;
	SENDTEXTSTRUCT sts;

	if(nStatus!=SICQ_STATUS_OFFLINE)
	{
		int nMessageID=rand.randomDWORD();

		sts.MessageTime=_LocalTimeAsUnixTime();
		sts.cookie.nCookies1=nMessageID;
		sts.cookie.nCookies2=nMessageID;
		sts.pszText=pszText;
		sts.pszUIN=pszUIN;
		sts.nTextLength=nTextLength;

		SendMessage(hEventWnd,WM_SICQ_EVENTWND_SENDMESSAGE,0,(LPARAM)&sts);

		return nMessageID;
	}
	else
	{
		return 0;
	}
}
void ICQSendText(SENDTEXTSTRUCT *pSts)
{
	if(nStatus!=SICQ_STATUS_OFFLINE)
	{
		CreateSendTextUnicodePacket(nSequence,pSts);
		Send(sock);
		SequenceIncrement();
	}
}


int _LocalTimeAsUnixTime()
{
	FILETIME ft;
	SYSTEMTIME st;

	GetLocalTime(&st);

	SystemTimeToFileTime(&st,&ft);

	return _FileTimeToUnixTime(&ft);
}

int CreateSendTextUnicodePacket(int nSequence,SENDTEXTSTRUCT *pSts)
{
	char *pMemory;
	int nMessageSize=pSts->nTextLength*sizeof(WCHAR);

	pMemory=(char *)_Alloc(nMessageSize);
	_StringToUnicode((WCHAR *)pMemory,pSts->nTextLength,pSts->pszText);
	_UnicodeToNet((WCHAR *)pMemory,pSts->nTextLength,(WCHAR *)pMemory);

	SetFLAPHeader(ICQ_FLAP_CHANNEL_SNACDATA,nSequence);
	Add_SNACHeader(ICQ_SNAC_FOODGROUP_ICBM,ICQ_SNAC_ICBM_SENDMESSAGE,0,nSequence);

	Add_u32_BE(pSts->cookie.nCookies1);
	Add_u32_BE(pSts->cookie.nCookies2);
	Add_u16_BE(0x0001);	// Channel
	Add_string08(pSts->pszUIN);
	Add_u16_BE(0x0002);
	Add_u16_BE(0x000E+nMessageSize);
	Add_u8(0x05);
	Add_u8(0x01);
	Add_u16_BE(0x0002);
	Add_u16_BE(0x0106);
	Add_u8(0x01);
	Add_u8(0x01);
	Add_u16_BE(0x0004+nMessageSize);
	Add_u16_BE(0x0002);
	Add_u16_BE(0x0000);
	Add_blob(pMemory,nMessageSize);
	Add_TLV_blob(ICQ_TLV_USERSTATUS,0,0);

	_Free(pMemory);

	return nPacketSize;
}

int _FileTimeToUnixTime(FILETIME *pFileTeme)
{
	return (int)(((*(__int64*) pFileTeme)-EPOCH_BIAS)/10000000i64);
}

int _StringToUnicode(WCHAR *pszUnicode,int nUnicodeLength,TCHAR *pszString)
{
#ifdef  UNICODE
	lstrcpyn(pszUnicode,pszString,nUnicodeLength);
	return lstrlen(pszString);
#else
	return _CharsToUnicode(pszUnicode,nUnicodeLength,pszString);
#endif
}

int _UnicodeToNet(WCHAR *pszNet,int nNetLength,WCHAR *pszUnicode)
{
	for(int i=0;i<nNetLength;i++)
	{
		pszNet[i]=htons(pszUnicode[i]);
	}

	return nNetLength;
}

int _CharsToUnicode(WCHAR *pszUnicode,int nUnicodeLength,char *pszChars)
{
	return MultiByteToWideChar(CP_ACP,0,pszChars,-1,pszUnicode,nUnicodeLength);
}

int Add_u8(unsigned char cU8)
{
	*(pPacket+nPacketSize)=cU8;
	nPacketSize++;

	SetFLAPDataSize(nPacketSize-sizeof(FLAP_HEADER));

	return 1;
}

int Add_string08(TCHAR *pszString)
{
	int nLength=lstrlen(pszString);

	Add_u8((char)nLength);
	nLength+=Add_string(pszString);

	return nLength;
}


bool ReadRecvMessagePacket(RECVMESSAGESTRUCT *pRms)
{
	short sTLVType,sTLVLength,sEncode;
	int nFragmentLength=0;
	char *pData=0;
	char Buffer[2048];
	int nSize=GetSNACDataSize();
	char *pOffset=GetSNACDataPointer();

	if(nSize&&IsSNACPresent(ICQ_SNAC_FOODGROUP_ICBM,ICQ_SNAC_ICBM_RECVMESSAGE))
	{
		pRms->cookie.nCookies1=Get_u32_BE_FromOffset(pOffset);
		pOffset+=4;
		pRms->cookie.nCookies2=Get_u32_BE_FromOffset(pOffset);
		pOffset+=4;
		pRms->sChannel=Get_u16_BE_FromOffset(pOffset);
		pOffset+=2;
		pOffset+=ReadNickInfoFromOffset(pOffset,&(pRms->NickInfo));

		nSize-=pOffset-GetSNACDataPointer();

		while(nSize>0)
		{
			sTLVType=GetTLVTypeFromOffset(pOffset);
			sTLVLength=GetTLVLehgthFromOffset(pOffset);
			pOffset+=sizeof(TLV_HEADER);
			nSize-=sizeof(TLV_HEADER);

			if(sTLVType==0x0002)
			{
				pData=pOffset;
				nFragmentLength=GetFragmentLehgthFromOffset(pData);

				pData+=sizeof(FRAGMENT);
				pData+=nFragmentLength;

				nFragmentLength=GetFragmentLehgthFromOffset(pData);

				pData+=sizeof(FRAGMENT);
				sEncode=Get_u16_BE_FromOffset(pData);
				pData+=4;

				switch(sEncode)
				{
				case 2:
					_NetToUnicode((WCHAR *)Buffer,(nFragmentLength-4)/sizeof(WCHAR)+1,(WCHAR *)pData);
					_UnicodeToString(pRms->szText,(nFragmentLength-4)/sizeof(WCHAR)+1,(WCHAR *)Buffer);
					break;
				default:
					_CharsToString(pRms->szText,(nFragmentLength-4)+1,pData);
				}

			}

			pOffset+=sTLVLength;
			nSize-=sTLVLength;
		}

		return true;
	}
	else
	{
		return false;
	}
}


unsigned int Get_u32_BE_FromOffset(char *pOffset)
{
	return ntohl(*((unsigned int *)pOffset));
}


int ReadNickInfoFromOffset(char *pOffset,NICKINFOSTRUCT *pNis)
{
	int nTLVs=0;
	short sTLVType,sTLVLength;
	char *pData=pOffset;

	pData+=Get_string08(pData,pNis->szUIN,sizeof(pNis->szUIN));
	pData++;
	pNis->sWarningLevel=Get_u16_BE_FromOffset(pData);
	pData+=2;
	nTLVs=Get_u16_BE_FromOffset(pData);
	pData+=2;

	for(int i=0;i<nTLVs;i++)
	{
		sTLVType=GetTLVTypeFromOffset(pData);
		sTLVLength=GetTLVLehgthFromOffset(pData);
		pData+=sizeof(TLV_HEADER);

		if(sTLVType==0x0001) // User Class
		{
			
		}
		else if(sTLVType==0x0003) // Online since
		{
			
		}
		else if(sTLVType==0x0005) // Registered
		{
			
		}
		else if(sTLVType==0x0006) // Status
		{
			
		}
		else if(sTLVType==0x000A) // 
		{
			
		}
		else if(sTLVType==0x000C) // CliToCli
		{
			
		}
		else if(sTLVType==0x000F) // Session Timer
		{
			
		}

		pData+=sTLVLength;
	}


	return pData-pOffset;
}

int _NetToUnicode(WCHAR *pszUnicode,int nUnicodeLength,WCHAR *pszNet)
{
	for(int i=0;i<nUnicodeLength-1;i++)
	{
		pszUnicode[i]=ntohs(pszNet[i]);
	}
	pszUnicode[nUnicodeLength-1]=0;

	return nUnicodeLength;
}

int _UnicodeToString(TCHAR *pszString,int nStringLength,WCHAR *pszUnicode)
{
#ifdef  UNICODE
	lstrcpyn(pszString,pszUnicode,nStringLength);
	return lstrlen(pszUnicode);
#else
	return _UnicodeToChars(pszString,nStringLength,pszUnicode);
#endif
}


int _UnicodeToChars(char *pszChars,int nCharsSize,WCHAR *pszUnicode)
{
	return WideCharToMultiByte(CP_ACP,0,pszUnicode,-1,pszChars,nCharsSize,NULL,NULL);
}

int Get_string08(char *pOffset,TCHAR *pszString,int nStringLength)
{
	int nSize=Get_u8_FromOffset(pOffset);

	if(nSize<nStringLength)
	{
		pOffset++;
		return _CharsToString(pszString,nStringLength,pOffset);
	}

	return 0;
}

