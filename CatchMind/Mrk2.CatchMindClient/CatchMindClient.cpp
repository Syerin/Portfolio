#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include "defines.h"
#include <stdio.h>
#include "GameManager.h"
#include "../Mrk2.CatchMindServer/Packet.h"
#define SERVERPORT 9002
#define SERVERIP "127.0.0.1"
#define WM_SOCKET (WM_USER + 1)


HINSTANCE g_hInst;
SOCKET g_sock;
LPCTSTR lpszClass = TEXT("CatchMind");
WNDPROC OldSubProc;
WNDPROC OldSubProcChat;
int g_iIndex = 0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);


void err_quit(char* msg);
void err_display(char* msg);
void err_display(int errcode);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;

	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	WndClass.hInstance = hInstance;

	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN|WS_CLIPSIBLINGS , CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, g_hInst, NULL);
	ShowWindow(hWnd, nCmdShow);

	int retval = 0;
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	g_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sock == INVALID_SOCKET) err_quit("socket");

	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(g_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	retval = WSAAsyncSelect(g_sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);

	GameManager::GetInstance()->SetSocket(g_sock);
	while (true)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) 
		{
			if (Message.message == WM_QUIT)
				break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			GameManager::GetInstance()->Update();
		}
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK EditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int   nLen;
	char  szStr[1025];

	switch (iMessage)
	{
	case WM_CHAR:
		if (wParam == VK_RETURN)  return 0;
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RETURN) 
		{
			GameManager::GetInstance()->SendLoginIdCheck(hWnd, g_sock);
			return 0;
		}
		break;
	}

	return CallWindowProc(OldSubProc, hWnd, iMessage, wParam, lParam);
}

LRESULT CALLBACK ChatSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int   nLen;
	char  szStr[1025];

	switch (iMessage)
	{
	case WM_CHAR:
		if (wParam == VK_RETURN)  return 0;
		break;

	case WM_KEYDOWN:
		if (wParam == VK_RETURN)
		{
			GameManager::GetInstance()->SendChat(g_sock);
			return 0;
		}
		break;
	}

	return CallWindowProc(OldSubProcChat, hWnd, iMessage, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	POINT ptMouse;
	RECT rcClient, rcWindow;
	int CXFrame;
	int CYFrame;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWindow);
	CXFrame = (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
	CYFrame = (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);


	switch (iMessage)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		GameManager::GetInstance()->Init(hdc,hWnd,g_sock,g_hInst);
		OldSubProc = (WNDPROC)SetWindowLong(GameManager::GetInstance()->GetWindowEdit(), GWL_WNDPROC, (LONG)EditSubProc);
		OldSubProcChat = (WNDPROC)SetWindowLong(GameManager::GetInstance()->GetWindowChat(), GWL_WNDPROC, (LONG)ChatSubProc);
		{
			int nX = 0;
			int nY = 0;
			nX =  (GetSystemMetrics(SM_CXMAXIMIZED) - GAMESIZE_WIDTH) / 2;
			nY = (GetSystemMetrics(SM_CYMAXIMIZED) - GAMESIZE_HEIGHT) / 2;
			MoveWindow(hWnd, nX, nY, GAMESIZE_WIDTH, GAMESIZE_HEIGHT, FALSE);
		}
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_SOCKET:
		ProcessSocketMessage(hWnd, iMessage, wParam, lParam);
		return 0;
	case WM_CTLCOLOREDIT:
		return 	GameManager::GetInstance()->SettingWindowOption(wParam, lParam);
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMaxTrackSize.x = GAMESIZE_WIDTH;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = GAMESIZE_HEIGHT + CYFrame;
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = GAMESIZE_WIDTH;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = GAMESIZE_HEIGHT + CYFrame;
		return FALSE;
	case WM_DESTROY:
		PostQuitMessage(0);
		SetWindowLong(GameManager::GetInstance()->GetWindowEdit(), GWL_WNDPROC, (LONG)EditSubProc);
		SetWindowLong(GameManager::GetInstance()->GetWindowChat(), GWL_WNDPROC, (LONG)ChatSubProc);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen = 0;
	int retval = 0;
	int ilen = 0;
	char buf[512];
	if (WSAGETSELECTERROR(lParam))
	{
		int err_code = WSAGETSELECTERROR(lParam);
		err_display(err_code);
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
	{
		char szBuf[BUFSIZE];
		retval = recv(wParam, szBuf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				err_display("recv()");
			}
		}
		if (retval == -1)
		{
			break;
		}
			GameManager::GetInstance()->SetRecvPacketInfo(szBuf, retval);
		while (true)
		{
			if (GameManager::GetInstance()->ProcessPacket(wParam)==false)
			{
				SendMessage(hWnd, WM_SOCKET, wParam, lParam);
				break;
			}
			if (GameManager::GetInstance()->GetCompletePacketProcess())
				break;
		}
	}
	break;
	case FD_CLOSE:
		break;
	}
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


void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[¿À·ù] %s", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}