#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include "Packet.h"
#include "Room.h"
#include <process.h>
#include <time.h>
#include "defines.h"
#define SERVERPORT 9002

DWORD WINAPI WorkerThread(LPVOID pCompletionPortIO);
unsigned int __stdcall TimerThread(LPVOID arg);


typedef struct
{
	SOCKET clientSock;
	SOCKADDR_IN clinetaddr;
} PER_HADNLE_DATA, *LPPER_HANDLE_DATA;

typedef struct
{
	OVERLAPPED overlapped;
	char szbuf[BUFSIZE];
	WSABUF wsabuf;
} PER_IO_DATA, *LPPER_IO_DATA;

typedef struct
{
	char buf[BUFSIZE];
	int RecvBytes;
	int SendByees;

	int len;
}USER_BUF,*LPUSER_BUF;

void err_quit(char* msg);
void err_display(char* msg);

struct USERINFO
{
	int idLength;
	int iCharacterImage;
	int iLevel;
};


CRITICAL_SECTION cs;

Room* m_pRoom;
DWORD m_dwLastTime = 0;
DWORD m_dwCurTime = 0;
float m_fElapseTime = 0.0f;
bool g_OnTimer = false;
int  g_iMaxTimer = MAXTIME;
float g_fTimer = 0.0f;
void InitTimer();
map<SOCKET, LPUSER_BUF> g_mapUser;

map<string, int> m_mapUserID;
int main()
{

	WSAData wsa;


	InitializeCriticalSection(&cs);

	EnterCriticalSection(&cs);
	m_pRoom = new Room();
	m_pRoom->Init();
	LeaveCriticalSection(&cs);


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		err_quit("WSAStartup() error!!");
	}
	HANDLE hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	HANDLE hThread;
	for (int i = 0; i < (int)sysInfo.dwNumberOfProcessors*2 ; i++)
	{
		hThread = CreateThread(NULL, 0, WorkerThread, hCompletionPort, 0, NULL);
		if (hThread == NULL) return 1;
		CloseHandle(hThread);
	}

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == SOCKET_ERROR) err_quit("socket() Error!!");

	SOCKADDR_IN serveraddr;
	::ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	int retval;
	retval = ::bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind() Error!!");
	_beginthreadex(NULL, 0, TimerThread, NULL, 0, NULL);

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen() Error!!!");

	FILE* pFile = fopen("UserName.txt", "r");
	char szbuf[BUFSIZE];
	if (pFile == NULL)
		fclose(pFile);
	else
	{	
		printf("------- User ID Load------- \n");
		while (!feof(pFile))
		{
			fscanf(pFile, "%s\n", szbuf);
			int len = strlen(szbuf);
			m_mapUserID.insert(make_pair(szbuf, len));
		}
		fclose(pFile);
	}
	for (auto iter = m_mapUserID.begin(); iter != m_mapUserID.end(); iter++)
	{
		printf("%s\n", (*iter).first.c_str());
	}
	printf("------- User ID Complete------- ");


	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);

	LPPER_IO_DATA perIoData;
	LPPER_HANDLE_DATA perHandleData;
	DWORD dwFlags, dwRecvbytes;

	while (true)
	{
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == SOCKET_ERROR)
		{
			err_display("accept() Error!!");
			break;
		}
		//클라이언트 셋팅
		perHandleData = new PER_HADNLE_DATA;
		perHandleData->clientSock = client_sock;
		memcpy(&perHandleData->clinetaddr, &clientaddr, addrlen);
		if (perHandleData == NULL) break;
		//클라이언트 연결
		CreateIoCompletionPort((HANDLE)client_sock, hCompletionPort, (DWORD)perHandleData, 0);
		bool opt_val = TRUE;
		setsockopt(perHandleData->clientSock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));

		//버퍼 설정
		perIoData = new PER_IO_DATA;
		::ZeroMemory(&perIoData->overlapped, sizeof(OVERLAPPED));
		perIoData->wsabuf.buf = perIoData->szbuf;
		perIoData->wsabuf.len = BUFSIZE;
		dwFlags = 0;
		USER_BUF* pUserbuf = new USER_BUF;
		pUserbuf->len = 0;
		pUserbuf->RecvBytes = 0;
		pUserbuf->SendByees = 0;
		EnterCriticalSection(&cs);
		g_mapUser.insert(make_pair(client_sock, pUserbuf));
		LeaveCriticalSection(&cs);



		if (perIoData == NULL) break;

		retval = WSARecv(perHandleData->clientSock, &perIoData->wsabuf, 1, &dwRecvbytes, &dwFlags, &perIoData->overlapped, NULL);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				err_display("WSARecv()");
			}
			continue;
		}
	}
	DeleteCriticalSection(&cs);
	WSACleanup();
	return 0;
}


DWORD WINAPI WorkerThread(LPVOID pCompletionPortIO)
{
	int retval;
	HANDLE hCompletionPort = (HANDLE)pCompletionPortIO;

	LPPER_HANDLE_DATA perHandleData;
	LPPER_IO_DATA     perIoData;
	DWORD  dwFlags;
	DWORD dwBytesTransferred;
	EnterCriticalSection(&cs);
	srand((unsigned int)time(NULL));
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	LeaveCriticalSection(&cs);
	while (true)
	{
		retval = GetQueuedCompletionStatus(hCompletionPort, &dwBytesTransferred, (LPDWORD)&perHandleData, (LPOVERLAPPED*)&perIoData, INFINITE);
		EnterCriticalSection(&cs);
		GetLocalTime(&sysTime);
		LeaveCriticalSection(&cs);

		if ((retval == false && dwBytesTransferred == 0) || (retval == true && dwBytesTransferred == 0)) 
		{
			EnterCriticalSection(&cs);

			m_pRoom->ShutDownPlayer(perHandleData->clientSock, perIoData->wsabuf.buf);
			g_mapUser.erase(perHandleData->clientSock);
			LeaveCriticalSection(&cs);

			shutdown(perHandleData->clientSock, SD_SEND);
			printf("%d가 종료하였습니다.\n", perHandleData->clientSock);
			delete perHandleData;
			delete perIoData;

			continue;
		}

		perIoData->wsabuf.len = dwBytesTransferred; 
		if (perIoData->wsabuf.len == SOCKET_ERROR)
		{
			printf("데이터가 잘못왓습니다.\n");
		}
		EnterCriticalSection(&cs);

		if (dwBytesTransferred < sizeof(PACKET_HEADER))
		{
			printf("[dwBytesTransferred : %d] Packet_header보다 적게 왓습니다.", dwBytesTransferred);
			memcpy(g_mapUser[perHandleData->clientSock]->buf, perIoData->wsabuf.buf, dwBytesTransferred);
			g_mapUser[perHandleData->clientSock]->len = dwBytesTransferred;
			LeaveCriticalSection(&cs);
			continue;
		}

		
		
		memcpy(&g_mapUser[perHandleData->clientSock]->buf[g_mapUser[perHandleData->clientSock]->len], perIoData->wsabuf.buf, dwBytesTransferred);

		if (dwBytesTransferred >= BUFSIZE)
		{
			printf("[dwBytesTransferred : %d] 버퍼크기를 초과하였습니다. ", dwBytesTransferred);
			LeaveCriticalSection(&cs);
			continue;
		}
		printf("[ %d : %d : %d ] \n------------보내왔습니다. ----------------\n[SOCKET] %d, [Recv] %d \n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond,perHandleData->clientSock, dwBytesTransferred);
		g_mapUser[perHandleData->clientSock]->RecvBytes = dwBytesTransferred + g_mapUser[perHandleData->clientSock]->len;
		g_mapUser[perHandleData->clientSock]->SendByees = 0;
		g_mapUser[perHandleData->clientSock]->len = 0;


	
		LeaveCriticalSection(&cs);


		while (true)
		{
			PACKET_HEADER header;
			EnterCriticalSection(&cs);
			memcpy(&header, g_mapUser[perHandleData->clientSock]->buf, sizeof(PACKET_HEADER));
			if (header.wLen > g_mapUser[perHandleData->clientSock]->RecvBytes)
			{
				printf("header.wLen 보다 dwBytesTransferred 적습니다.\n [header.wLen :%d] , [dwBytesTransferred : %d]", header.wLen, dwBytesTransferred);
				memcpy(&g_mapUser[perHandleData->clientSock]->buf, (char*)&perIoData->wsabuf.buf, dwBytesTransferred);
				g_mapUser[perHandleData->clientSock]->len = g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees;
				printf("유저버퍼 저장 [UserBuf_len : %d]", g_mapUser[perHandleData->clientSock]->len);
				LeaveCriticalSection(&cs);
				continue;
			}

			printf("------------header 진입---------\n,[header.wIndex] %d , [Recv] %d, [send] %d \n",header.wIndex, g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);
			if (header.wLen > BUFSIZE)
			{ 
				printf("------------잘못된 진입--------\n[header.wLen : %d] \n",header.wLen);
				g_mapUser[perHandleData->clientSock]->RecvBytes = 0;
				g_mapUser[perHandleData->clientSock]->SendByees = 0;
				g_mapUser[perHandleData->clientSock]->len = 0;
				LeaveCriticalSection(&cs);

				break;
			}
			LeaveCriticalSection(&cs);

			switch (header.wIndex)
			{
			case PACKET_INDEX_LOGIN_ID_CHECK:
			{
				EnterCriticalSection(&cs);
				PACKET_ID packet;
				string id;			
				memcpy(&packet, g_mapUser[perHandleData->clientSock]->buf, header.wLen);

				id = packet.Name;
				bool isSamePlayer = false;
				bool ableName = true;

				if (ableName == true)
				{
					for (auto iter = m_mapUserID.begin(); iter != m_mapUserID.end(); iter++)
					{
						if ((*iter).first == id)
						{
							isSamePlayer = true;
							break;
						}
					}

					if (isSamePlayer == false)
					{
						FILE* pFile = fopen("UserName.txt", "a");
						fprintf(pFile, "\n%s", id.c_str());
						fclose(pFile);
						m_mapUserID.insert(make_pair(id, perHandleData->clientSock));
					}

				}
				printf("PACKET_INDEX_LOGIN_ID_CHECK 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes,g_mapUser[perHandleData->clientSock]->SendByees);
				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;

				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[header.wLen], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				if (strlen(packet.Name) < 2 || strlen(packet.Name) > 8)
				{
					ableName = false;
					isSamePlayer = true;
				}
				
				PACKET_ID_GENERATE Packet_Gerate_id;
				Packet_Gerate_id.header.wIndex = PACKET_INDEX_LOGIN_ID_GENERATE;
				Packet_Gerate_id.header.wLen = sizeof(Packet_Gerate_id);

				Packet_Gerate_id.avaliableId = !isSamePlayer;
				send(perHandleData->clientSock, (char*)&Packet_Gerate_id, sizeof(Packet_Gerate_id), 0);

				LeaveCriticalSection(&cs);
			}
			break;
			case PACKET_INDEX_ROOM_IN:
			{
				EnterCriticalSection(&cs);

				printf("PACKET_INDEX_ROOM_IN 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);
				printf("\n[header.wLen : %d] 만큼 들어왓습니다.", header.wLen);
				m_pRoom->EnterPlayer(perHandleData->clientSock, g_mapUser[perHandleData->clientSock]->buf);
				m_pRoom->JoinPlayer(perHandleData->clientSock);
				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[header.wLen], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				LeaveCriticalSection(&cs);
			}
			break;
			case PACKET_INDEX_ROOM_PLAYER_INFO:
			{
				EnterCriticalSection(&cs);
				m_pRoom->NoticeOtherPlayer(perHandleData->clientSock); //누군가 들어왔음을 알림.
				printf("PACKET_INDEX_ROOM_PLAYER_INFO 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);
				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[header.wLen], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				LeaveCriticalSection(&cs);
			}
			break;
			case  PACKET_INDEX_ROOM_READY:
			{
				EnterCriticalSection(&cs);

				m_pRoom->ReadyPlayer(perHandleData->clientSock, g_mapUser[perHandleData->clientSock]->buf);
				printf("PACKET_INDEX_ROOM_READY 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);
				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[header.wLen], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				LeaveCriticalSection(&cs);
			}
			break;
			case PACKET_INDEX_ROOM_START:
			{				
				InitTimer();
				EnterCriticalSection(&cs);

				m_pRoom->StartGame(perHandleData->clientSock, g_mapUser[perHandleData->clientSock]->buf);
				printf("PACKET_INDEX_ROOM_START 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);

				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[header.wLen], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				LeaveCriticalSection(&cs);
			}
			break;
			case PACKET_INDEX_ROOM_CHAT:
			{
				EnterCriticalSection(&cs);

				m_pRoom->ChatPlayer(perHandleData->clientSock, perIoData->wsabuf.buf, header.wLen);
				printf("PACKET_INDEX_ROOM_CHAT 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);
				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[header.wLen], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				LeaveCriticalSection(&cs);
			}
			break;
			case PACKET_INDEX_REQUEST_QUESTION:
			{
				InitTimer();
				g_OnTimer = true;
				EnterCriticalSection(&cs);

				m_pRoom->SendQuestion(perHandleData->clientSock, perIoData->wsabuf.buf);
				printf("PACKET_INDEX_REQUEST_QUESTION 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);

				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[header.wLen], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				LeaveCriticalSection(&cs);
			}
			break;
			case PACKET_INDEX_DRAWDOWN:
			{
				EnterCriticalSection(&cs);
				m_pRoom->DrawMouseDown(perHandleData->clientSock, g_mapUser[perHandleData->clientSock]->buf);
				printf("PACKET_INDEX_DRAWDOWN 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);
				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[sizeof(PACKET_ROOM_DRAWDOWN)], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				LeaveCriticalSection(&cs);
			}
			break;
			case PACKET_INDEX_DRAWMOVE:
			{
				EnterCriticalSection(&cs);
				m_pRoom->DrawMouseMove(perHandleData->clientSock, g_mapUser[perHandleData->clientSock]->buf);
				printf("PACKET_INDEX_DRAWMOVE 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);

				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[sizeof(PACKET_ROOM_DRAWMOVE)], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);

				LeaveCriticalSection(&cs);
			}
			break;
			case PACKET_INDEX_ROOM_OUT:
			{
				EnterCriticalSection(&cs); 
				int SendByees = g_mapUser[perHandleData->clientSock]->SendByees;
				m_pRoom->ExitPlayer(perHandleData->clientSock, &g_mapUser[perHandleData->clientSock]->buf[SendByees]);

				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[sizeof(PACKET_ROOM_DRAWMOVE)], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				LeaveCriticalSection(&cs);

			}
			break;

			case PAKCET_INDEX_ROOM_ANSWER:
			{

				EnterCriticalSection(&cs);
				m_pRoom->AnswerPlayer(perHandleData->clientSock, perIoData->wsabuf.buf, header.wLen);
				printf("PAKCET_INDEX_ROOM_ANSWER 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);
				
				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[header.wLen], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				LeaveCriticalSection(&cs);
			}
			break;
			case PACKET_INDEX_ROOM_NEXTTURN:
			{
				InitTimer();
				EnterCriticalSection(&cs);
				m_pRoom->NextTurn(perHandleData->clientSock);
				printf("PACKET_INDEX_ROOM_NEXTTURN 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);
				
				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[sizeof(PACKET_ROOM_NEXTTURN)], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				LeaveCriticalSection(&cs);
			}
			break;
			case PACKET_INDEX_ROOM_GAMEOVER:
			{
				InitTimer();
				EnterCriticalSection(&cs);
				m_pRoom->InitRoom();
				printf("PACKET_INDEX_ROOM_GAMEOVER 호출 [Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);
	
				g_mapUser[perHandleData->clientSock]->SendByees += header.wLen;
				memcpy(g_mapUser[perHandleData->clientSock]->buf, &g_mapUser[perHandleData->clientSock]->buf[sizeof(PACKET_ROOM_GAME_OVER)], g_mapUser[perHandleData->clientSock]->RecvBytes - g_mapUser[perHandleData->clientSock]->SendByees);
				LeaveCriticalSection(&cs);

			}
				break;
			default:
				g_mapUser[perHandleData->clientSock]->RecvBytes = 0;
				g_mapUser[perHandleData->clientSock]->SendByees = 0;
				g_mapUser[perHandleData->clientSock]->len = 0;
				printf("잘못된 패킷 헤더, Recv가 패킷의 전송 버퍼용량을 초과하였습니다.");

				break;
			}
			printf("------------header 처리중---------\n[Recv] %d, [send] %d \n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);

			if (g_mapUser[perHandleData->clientSock]->RecvBytes == g_mapUser[perHandleData->clientSock]->SendByees)
			{
				printf("------------header 완료---------\n[Recv] %d, [send] %d \n\n\n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);
				g_mapUser[perHandleData->clientSock]->RecvBytes = 0;
				g_mapUser[perHandleData->clientSock]->SendByees = 0;
				break;
			}
			else if (g_mapUser[perHandleData->clientSock]->RecvBytes <= g_mapUser[perHandleData->clientSock]->SendByees)
			{
				system("cls");
				printf("값이 조금 더 많이 갔다. [Recv] %d, [send] %d \n\n\n", g_mapUser[perHandleData->clientSock]->RecvBytes, g_mapUser[perHandleData->clientSock]->SendByees);
				g_mapUser[perHandleData->clientSock]->RecvBytes = 0;
				g_mapUser[perHandleData->clientSock]->SendByees = 0;
				break;

			}
		}
		g_mapUser[perHandleData->clientSock]->len = 0;

		::ZeroMemory(&perIoData->overlapped, sizeof(OVERLAPPED));
		perIoData->wsabuf.len = BUFSIZE;
		perIoData->wsabuf.buf = perIoData->szbuf;
		dwFlags = 0;
		WSARecv(perHandleData->clientSock, &perIoData->wsabuf, 1, NULL, &dwFlags, &perIoData->overlapped, NULL);
	}
	return 0;
}

unsigned int __stdcall TimerThread(LPVOID arg)
{

	SYSTEMTIME sysTimer;
	WORD m_dwprevSecond = 0;
	WORD m_dwcurSecond = 0;
	while (true)
	{
		GetLocalTime(&sysTimer);

		if (GetKeyState(VK_NUMPAD0) & 0x8000)
		{
			printf("지난 시간 %d.0초 \n[curSecond :%d], [prevSecond : %d]\n\n", (int)g_fTimer, m_dwcurSecond, m_dwprevSecond);
		}

		if (g_OnTimer == false)
		{
			m_dwprevSecond = sysTimer.wSecond;
			m_dwcurSecond = sysTimer.wSecond;
			g_fTimer = 0.0f;
			continue;
		}


		if (g_OnTimer)
		{
			m_dwcurSecond = sysTimer.wSecond;

			if (m_dwcurSecond != m_dwprevSecond)
			{
				g_fTimer += 1.0f;
				m_dwprevSecond = sysTimer.wSecond;
			}
		}

		if (g_iMaxTimer - (int)g_fTimer <= 0)
		{
			EnterCriticalSection(&cs);
			if (m_pRoom->GameOver())
			{
				InitTimer();
				LeaveCriticalSection(&cs);
				continue;
			}
			PACKET_TIMER packet_time;
			packet_time.header.wIndex = PACKET_INDEX_TIMER;
			packet_time.header.wLen = sizeof(packet_time);
			packet_time.ElapseTime = m_fElapseTime;
			packet_time.curRound = m_pRoom->GetNextRound();
			packet_time.CurDrawPlayer = m_pRoom->GetCurDrawPlayer();
			packet_time.NextDrawPlayer = m_pRoom->GetNextDrawing();

			printf("[curDraw : %d] , [NextDrawPlayer : %d]", packet_time.CurDrawPlayer, packet_time.NextDrawPlayer);
			for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
			{
				send((*iter).first, (char*)&packet_time, sizeof(packet_time), 0);
				printf("[Send : %d] TimeOver를 보냈습니다.", (*iter).first);
			}
			m_pRoom->TransTresPassToGame();
			LeaveCriticalSection(&cs);

			InitTimer();
		}

		continue;
	}
	return 0;
}
void InitTimer()
{
	g_OnTimer = false;
	g_iMaxTimer = MAXTIME;
	g_fTimer = 0.0f;
}

void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
// 소켓 함수 오류 출력
void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[오류] %d", errcode);
	LocalFree(lpMsgBuf);
}