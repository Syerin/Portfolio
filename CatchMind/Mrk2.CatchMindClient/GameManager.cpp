#include "GameManager.h"
#include "GameScreen.h"
#include "Bitmap.h"
#include "ResourceManager.h"
#include "PlayerManager.h"
#include "../Mrk2.CatchMindServer/Packet.h"
#include "Player.h"
#include "Ranking.h"
GameManager* GameManager::m_pThis = NULL;
GameManager::GameManager()
{
	m_eCurState = CLIENT_STATE_IDCHECK;
	m_strNameLimit = "한글 1~4글자, 영어 1~8글자이하로 생성해주세요.\n (특수문자, 공백 포함)";
	m_strNameCheck = "";
	m_rcTimer.left = TIMER_POS_X;
	m_rcTimer.top = TIMER_POS_Y;
	m_rcTimer.right = m_rcTimer.left + TIMER_FONTSIZE*6;
	m_rcTimer.bottom = m_rcTimer.top + TIMER_FONTSIZE;
	m_stPacket.Recvbyte = 0;
	m_stPacket.Sendbyte = 0;
	m_rcNameCheck.left = 400;
	m_rcNameCheck.top = 400;
	m_rcNameCheck.right = m_rcNameCheck.left + 220;
	m_rcNameCheck.bottom = m_rcNameCheck.top + 50;

	m_rcNameLimit.left = 400;
	m_rcNameLimit.top = 320;
	m_rcNameLimit.right = m_rcNameLimit.left + 220;
	m_rcNameLimit.bottom = m_rcNameLimit.top + 50;	
	m_bDrawChat = false;
	m_bTimer = false;
	m_fTimer = 0.0f;
	m_strTimer = "2 : 00 : 00";
	m_fElapsTime = 0;

	m_rcQuestion.left = DRAWOFFSET_X;
	m_rcQuestion.top = DRAWOFFSET_Y;
	m_rcQuestion.right = m_rcQuestion.left + 100;
	m_rcQuestion.bottom = m_rcQuestion.top + 100;

	m_strQuestion = "디폴트";
	m_Questionsize.cx = 0;
	m_Questionsize.cy = 0;
	m_bAnswer = false;
	m_fGameOver = 0.0f;
	m_bGameOver = false;
	m_bLbutton = false;
	m_iMin = 1;
	m_iSecon = 59;
	m_iMilSecon = 100;
	m_fMilisecond = 0.0f;
	m_fTimerSecond = 0.0f;
	m_stPacket.Len = 0;
	m_rcRound.left = ROUND_POS_X - 10;
	m_rcRound.top = ROUND_POS_Y - 50;

	m_rcRound.right = ROUND_POS_X;
	m_rcRound.bottom = ROUND_POS_Y- 40;
	m_icurRound = 0;
	m_iRoundEnd = 0;

}
GameManager::~GameManager()
{
	SAFE_DELETE(m_pMyPlayer);
	SAFE_DELETE(m_pGameScreen);
	SAFE_DELETE(m_pRanking);
	SAFE_DELETE(m_pPlayerManager);
	for (int i = 0; i < FONT_TYPE_END; i++)
	{
		SelectObject(m_hMemDC, m_hOldFont[i]);
		DeleteObject(m_hFont[i]);
	}

	SelectObject(m_hMemDC, m_hOld);
	DeleteObject(m_hBitmap);
	delete(m_hMemDC);
}


void GameManager::Init(HDC hdc,HWND hWnd, SOCKET sock,HINSTANCE hInstance)
{
	m_Sock = sock;
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, GAMESIZE_WIDTH, GAMESIZE_HEIGHT);
	SelectObject(m_hMemDC, m_hBitmap);
	ResourceManager::GetInstance()->Init(hdc);
	m_hWnd = hWnd;
	m_pPlayerManager = new PlayerManager();
	m_pPlayerManager->Init();
	m_pGameScreen = new GameScreen();
	m_pGameScreen->Init(m_hMemDC,hWnd);
	m_pBitmap = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_BACKGROUND);

	m_pBitmapLogin[0] = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_LOGIN);
	m_pBitmapLogin[1] = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_LOGIN_ID);

	m_pBitmapReady[0] = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_BUTTON_READY_WAIT);
	m_pBitmapReady[1] = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_BUTTON_READY);
	
	m_pBitmapStart = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_BUTTON_START);

	SetRect(&m_rcButton, BUTTON_READY_POS_X, BUTTON_READY_POS_Y, BUTTON_READY_POS_X+ m_pBitmapReady[0]->GetSizeBitmap().cx, BUTTON_READY_POS_Y+ m_pBitmapReady[0]->GetSizeBitmap().cy);

	m_pBitmap_Explain = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_EXPLAIN);

	m_pMyPlayer = new Player();

	m_pRanking = new Ranking();
	m_pRanking->Init(hdc);
	m_bTimerOut = false;

	m_hEdit = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_BLACKFRAME | SS_LEFT, 
		450, 350, 220, 40, hWnd, (HMENU)ID_EDIT, hInstance, NULL);
	m_hChat = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_BLACKFRAME | SS_LEFT,
		EDIT_ORIGIN_X,EDIT_ORIGIN_Y, EDIT_CHAT_WIDTH, EDIT_CHAT_HEIGHT, hWnd, (HMENU)ID_CHAT, hInstance, NULL);
	ShowWindow(m_hChat, SW_HIDE);
	m_hFont[FONT_TYPE_LOGIN] = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("돋움체"));
	m_hFont[FONT_TYPE_TIMER]= CreateFont(TIMER_FONTSIZE, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("돋움체"));
	m_hFont[FONT_TYPE_CHAT] = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("돋움체"));
	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
	m_pBitmap_Question = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_QUESTION);

}
void GameManager::SetRecvPacketInfo(char* buf, int len)
{

	memcpy(&m_stPacket.buf[m_stPacket.Len], buf, len);
	m_stPacket.Recvbyte = len + m_stPacket.Len;
	m_stPacket.Sendbyte = 0;
	m_stPacket.Len = 0;
}
void GameManager::Update()
{
	m_dwCurTime = GetTickCount();
	m_fElapsTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = GetTickCount();
	m_pPlayerManager->Update(m_fElapsTime);
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	if (m_pGameScreen->GetGameState() == GAME_STATE_GAME &&m_pMyPlayer->GetPlayerState() == PLAYER_STATE_DRAW)
	{
		m_pGameScreen->SendDrawing(m_Sock, ptMouse);
		m_pGameScreen->SelectColor(ptMouse);
	}

	if (m_pGameScreen->GetGameState() == GAME_STATE_WAIT)
	{
		PlayerButton(ptMouse);
		if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_READY || m_pMyPlayer->GetType() == PLAYER_TYPE_HOST)
		{
			m_pGameScreen->DrawingMouse(ptMouse);
			m_pGameScreen->SelectColor(ptMouse);
		}
	}

	m_pGameScreen->Update(m_fElapsTime);
	if (m_pGameScreen->GetAnswerEffectEnd() == true && m_pMyPlayer->GetPlayerState() == PLAYER_STATE_ANSWER && m_bAnswer)
	{
		PACKET_ROOM_NEXTTURN Packet_Next;
		Packet_Next.header.wIndex = PACKET_INDEX_ROOM_NEXTTURN;
		Packet_Next.header.wLen = sizeof(Packet_Next);
		Packet_Next.curDrawPlayer = 0;
		Packet_Next.NextDrawPlayer = 0;
		GetSend(m_Sock, (char*)&Packet_Next, sizeof(Packet_Next));
		m_bAnswer = false;
	}

	if(m_pGameScreen->GetNextEffectEnd() == true && m_pMyPlayer->GetPlayerState() == PLAYER_STATE_DRAW)
	{
		PACKET_REQUEST_QUESTION packet_request;
		packet_request.header.wIndex = PACKET_INDEX_REQUEST_QUESTION;
		char d[] = "아니야.";
		strcpy(packet_request.Question, d);
		strlen(d);
		packet_request.header.wLen = sizeof(PACKET_HEADER) + 4 + strlen(d);
		SetTimeTimer();
		GetSend(m_Sock, (char*)&packet_request, packet_request.header.wLen);
	}


	if (m_pGameScreen->GetGameState() == GAME_STATE_GAME)
	{
		TimerTime(m_fElapsTime);
		m_bTimerOut = false;
	}
	if (m_bGameOver)
	{
		m_fGameOver += m_fElapsTime;
		if (m_fGameOver > 3.0f)
		{
			m_bGameOver = false;
			m_fGameOver = 0.0f;
			for (int i = 0; i < MAXPLAYER; i++)
			{
				m_pPlayerManager->Init(i);
			}
			m_pMyPlayer->InitPlayer();
			m_pRanking->InitChart();
			m_pGameScreen->SetGameState(GAME_STATE_WAIT);
			SetTimeTimer();
			m_strTimer = "2 : 00 : 00";
			if (m_pMyPlayer->GetType() == PLAYER_TYPE_HOST)
			{
				PACKET_ROOM_GAME_OVER Packet_Gameover;
				Packet_Gameover.header.wIndex = PACKET_INDEX_ROOM_GAMEOVER;
				Packet_Gameover.header.wLen = sizeof(Packet_Gameover);
				GetSend(m_Sock, (char*)&Packet_Gameover, sizeof(Packet_Gameover));
			}
			//게임끝남을보냄.
		}
	}
	m_strRound = to_string(m_icurRound) + "/" + to_string(m_iRoundEnd);

	Draw();
}
void GameManager::Draw()
{
	HDC hdc = GetDC(m_hWnd);
	SetBkMode(m_hMemDC, TRANSPARENT);
	m_pBitmap->DrawBitblt(m_hMemDC, 0, 0);
	m_pPlayerManager->Draw(m_hMemDC);
	//로그인에 성공한경우
	if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_TRESPASS)
	{
		m_pGameScreen->DrawTrepass();
	}
	if (m_eCurState == CLIENT_STATE_IDCHECK)
	{
		SetTextColor(m_hMemDC, RGB(0, 0, 0));
		SelectObject(m_hMemDC, m_hFont[FONT_TYPE_CHAT]);
		m_pBitmapLogin[0]->DrawTransparent(m_hMemDC, (GAMESIZE_WIDTH / 2) - (m_pBitmapLogin[0]->GetSizeBitmap().cx / 2), (GAMESIZE_HEIGHT / 2) - (m_pBitmapLogin[0]->GetSizeBitmap().cy / 2));
		m_pBitmapLogin[1]->DrawTransparent(m_hMemDC, (GAMESIZE_WIDTH / 2) - (m_pBitmapLogin[0]->GetSizeBitmap().cx / 2) + 30, (GAMESIZE_HEIGHT / 2) - (m_pBitmapLogin[1]->GetSizeBitmap().cy / 2));
		ShowWindow(m_hEdit, SW_SHOW);
		ShowWindow(m_hChat, SW_HIDE);
		DrawText(m_hMemDC, m_strNameLimit.c_str(), -1, &m_rcNameLimit, DT_CENTER | DT_NOCLIP);
		DrawText(m_hMemDC, m_strNameCheck.c_str(), -1, &m_rcNameCheck, DT_CENTER | DT_NOCLIP);

	}
	else if (m_eCurState == CLIENT_STATE_ROOM_IN)
	{
		if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_DRAW)
		{
			ShowWindow(m_hEdit, SW_HIDE);
			ShowWindow(m_hChat, SW_HIDE);
		}
		else
		{
			ShowWindow(m_hEdit, SW_HIDE);
			ShowWindow(m_hChat, SW_SHOW);
		}

		m_pGameScreen->Draw(m_hMemDC);
		if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_WAIT &&m_pMyPlayer->GetType() == PLAYER_TYPE_NOMAL)
		{
			if (m_pMyPlayer->GetType() != PLAYER_TYPE_HOST)
				m_pGameScreen->DrawGameStateWait();
		}
		m_pGameScreen->DrawCleanButton(m_hMemDC);


		FillRect(m_hMemDC, &m_rcTimer, (HBRUSH)GetStockObject(BLACK_BRUSH));
		SetTextColor(m_hMemDC, RGB(255, 242, 0));
		SelectObject(m_hMemDC, m_hFont[FONT_TYPE_TIMER]);
		TextOut(m_hMemDC, TIMER_POS_X, TIMER_POS_Y, m_strTimer.c_str(), m_strTimer.size());

		m_pBitmap_Explain->DrawMidPoint(m_hMemDC, GAMESIZE_WIDTH / 2, 0+m_pBitmap_Explain->GetSizeBitmap().cy/2);

		
		DrawMyPlayer();
		if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_DRAW)
		{
			ShowWindow(m_hChat, SW_HIDE);
		}
		else
		{
			ShowWindow(m_hChat, SW_SHOW);
		}

		SetTextColor(m_hMemDC, RGB(0, 0, 0));
		
		 if (m_pGameScreen->GetGameState() == GAME_STATE_RESULTS)
		{
			SelectObject(m_hMemDC, m_hFont[FONT_TYPE_LOGIN]);
			m_pRanking->Draw(m_hMemDC);
		}else if(m_pGameScreen->GetGameState() != GAME_STATE_WAIT)
		{
			SelectObject(m_hMemDC, m_hFont[FONT_TYPE_LOGIN]);
			DrawText(m_hMemDC, m_strRound.c_str(), -1, &m_rcRound, DT_RIGHT | DT_NOCLIP);
		}


		if (m_pMyPlayer->GetType() == PLAYER_TYPE_HOST && m_pMyPlayer->GetPlayerState() == PLAYER_STATE_START_ACT)
		{
			m_pGameScreen->DrawAllReady();
		}
		SelectObject(m_hMemDC, m_hFont[FONT_TYPE_CHAT]);
		m_pPlayerManager->DrawChat(m_hMemDC);
		if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_DRAW && m_pGameScreen->GetGameState() == GAME_STATE_GAME)
		{
			m_pBitmap_Question->DrawTransparent(m_hMemDC, QUESTION_POS_X, QUESTION_POS_Y, m_Questionsize.cx*QUESTION_OFFSET_X / 4, 0);
			DrawText(m_hMemDC, m_strQuestion.c_str(), -1, &m_rcQuestion, DT_CENTER | DT_NOCLIP);
		}
	}
	
	
	BitBlt(hdc, 0, 0, GAMESIZE_WIDTH, GAMESIZE_HEIGHT, m_hMemDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);
}
void GameManager::DrawMyPlayer()
{
	if (m_eCurState == CLIENT_STATE_ROOM_IN)
	{
		if (m_pMyPlayer->GetType() == PLAYER_TYPE_HOST)
		{
			if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_START_ACT)
			{
				m_pBitmapStart->DrawTransparent(m_hMemDC, BUTTON_READY_POS_X, BUTTON_READY_POS_Y);
			}
		}
		else if (m_pMyPlayer->GetType() == PLAYER_TYPE_NOMAL)
		{
			if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_WAIT)
			{
				m_pBitmapReady[0]->DrawTransparent(m_hMemDC, BUTTON_READY_POS_X, BUTTON_READY_POS_Y);
			}
			else if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_READY)
			{
				m_pBitmapReady[1]->DrawTransparent(m_hMemDC, BUTTON_READY_POS_X, BUTTON_READY_POS_Y);
			}
		}
	}
}
void GameManager::SendLoginIdCheck(HWND hWnd, SOCKET sock)
{

		if (m_eCurState == CLIENT_STATE_IDCHECK)
		{
			GetWindowText(m_hEdit, (char*)m_strName.c_str(), -1);
			PACKET_ID packet;
			packet.header.wIndex = PACKET_INDEX_LOGIN_ID_CHECK;
			packet.header.wLen = sizeof(PACKET_HEADER) + strlen(m_strName.c_str()) + 1;
			strcpy(packet.Name, m_strName.c_str());
			GetSend(sock, (char*)&packet, packet.header.wLen);
			m_eCurState = CLIENT_STATE_IDCHECKING;
		}
	}

void GameManager::SendChat(SOCKET sock)
{
	if (m_eCurState == CLIENT_STATE_ROOM_IN)
	{
		SIZE size;
		char buf[BUFSIZE] = "\0";
		int len = GetWindowTextLength(m_hChat);
		GetWindowText(m_hChat, buf, BUFSIZE);
		m_strChat = buf;
		int sendval = 0;
		if (strlen(m_strChat.c_str()) >0)
		{
			m_bDrawChat = true;
			PACKET_ROOM_CHAT packet_chat;
			packet_chat.header.wIndex = PACKET_INDEX_ROOM_CHAT;
			packet_chat.header.wLen = sizeof(PACKET_HEADER) + 4 + strlen(m_strChat.c_str()) + 1;
			packet_chat.index = m_pMyPlayer->GetPlayerNumber();
			strcpy(packet_chat.Chat, m_strChat.c_str());
			sendval = GetSend(sock, (char*)&packet_chat, packet_chat.header.wLen);
			SendMessage(m_hChat, EM_SETSEL, 0, len);
			// "삭제" 합니다.
			SendMessage(m_hChat, WM_CLEAR, 0, 0);
			SetFocus(m_hChat);
			if (sendval == SOCKET_ERROR)
			{
				MessageBox(m_hWnd, TEXT("안감"), TEXT("아니"), MB_OK);
			}
			if (m_pGameScreen->GetGameState() == GAME_STATE_GAME)
			{
				PACKET_ROOM_ANSWER packet_answer;
				packet_answer.header.wIndex = PAKCET_INDEX_ROOM_ANSWER;
				packet_answer.header.wLen = sizeof(PACKET_HEADER) + 4 + strlen(m_strChat.c_str())+1;
				packet_answer.index = m_pMyPlayer->GetPlayerNumber();
				strcpy(packet_answer.buf, m_strChat.c_str());
				sendval= GetSend(sock, (char*)&packet_answer, packet_answer.header.wLen);

			}
			if (sendval == SOCKET_ERROR)
			{
				MessageBox(m_hWnd, TEXT("안감"), TEXT("아니"), MB_OK);
			}
		}
	}
}

void GameManager::PlayerButton(POINT ptMouse)
{
	if (PtInRect(&m_rcButton, ptMouse) && m_eCurState == CLIENT_STATE_ROOM_IN)
	{
		if (GetKeyState(MK_LBUTTON) & 0x8000 && m_bLbutton == false)
		{
			m_bLbutton = true;
		}
		 if (GetKeyState(MK_LBUTTON) & 0x8000 && m_bLbutton == true)
		{
			return;
		}

		if (m_bLbutton == true && PtInRect(&m_rcButton, ptMouse))
		{
			m_bLbutton = false;
			if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_WAIT || m_pMyPlayer->GetPlayerState() == PLAYER_STATE_START_ACT || m_pMyPlayer->GetPlayerState() == PLAYER_STATE_READY)
			{
				PACKET_ROOM_PLAYER_READY Packet_ready;
				Packet_ready.header.wIndex = PACKET_INDEX_ROOM_READY;
				Packet_ready.header.wLen = sizeof(Packet_ready);
				Packet_ready.PlayerNumber = m_pMyPlayer->GetPlayerNumber();

				if (m_pMyPlayer->GetType() == PLAYER_TYPE_NOMAL)
				{
					if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_WAIT)
					{
						m_pMyPlayer->SetPlayerState(PLAYER_STATE_READY);
						m_pGameScreen->ScreenClear();

						Packet_ready.m_bReady = true;
					}
					else if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_READY)
					{
						m_pMyPlayer->SetPlayerState(PLAYER_STATE_WAIT);
						m_pGameScreen->ScreenClear();

						Packet_ready.m_bReady = false;
					}
					send(m_Sock, (char*)&Packet_ready, sizeof(Packet_ready), 0);
				}
				else if (m_pMyPlayer->GetType() == PLAYER_TYPE_HOST)
				{
					if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_START_ACT)
					{
						PACKET_ROOM_START Packet_start;
						Packet_start.header.wIndex = PACKET_INDEX_ROOM_START;
						Packet_start.header.wLen = sizeof(Packet_start);
						GetSend(m_Sock, (char*)&Packet_start, sizeof(Packet_start));
					}
				}
			}
		}
	}
}
LRESULT GameManager::SettingWindowOption(WPARAM wParam, LPARAM lParam)
{
	if ((HWND)lParam ==m_hEdit)
	{
		SetTextColor((HDC)wParam, RGB(0, 0, 0));
		SetBkColor((HDC)wParam, RGB(175, 228, 255));

		if (m_eCurState == CLIENT_STATE_IDCHECK)
		{
			SendMessage(m_hEdit, WM_SETFONT, (WPARAM)m_hFont[FONT_TYPE_LOGIN], (LPARAM)FALSE);

			return (LRESULT)GetStockObject(NULL_BRUSH);
		}
	}
	else if ((HWND)lParam == m_hChat)
	{
		SetTextColor((HDC)wParam, RGB(0, 0, 0));
		SetBkColor((HDC)wParam, RGB(175, 228, 255));
		if (m_eCurState == CLIENT_STATE_ROOM_IN)
		{
			SendMessage(m_hChat, WM_SETFONT, (WPARAM)m_hFont[FONT_TYPE_CHAT], (LPARAM)FALSE);
			return (LRESULT)GetStockObject(NULL_BRUSH);
		}
	}
	
}


bool GameManager::GetCompletePacketProcess()
{
	if (m_stPacket.Recvbyte <= m_stPacket.Sendbyte && m_stPacket.Recvbyte != 0)
	{
		m_stPacket.Recvbyte = 0;
		m_stPacket.Sendbyte = 0;
		m_stPacket.Len = 0;
		return true;
	}
	else
		return false;
}

void GameManager::TimerTime(float fElapse)
{
	if (m_bTimer)
	{
		m_fTimer += fElapse;
		m_fTimerSecond += fElapse;
		m_fMilisecond += fElapse*100;

		if (m_iSecon - (int)m_fTimerSecond < 0)
		{
			m_iSecon = 59;
			m_fTimerSecond = 0.0f;
		}
		if (m_iMilSecon - m_fMilisecond < 0)
		{
			m_iMilSecon = 99;
			m_fMilisecond = 0;
		}
		m_strTimer = to_string(m_iMin - (int)(m_fTimer / 60.0f)) + " : "; 
		if (m_iSecon - (int)m_fTimerSecond >= 10)
		{
			m_strTimer += to_string(m_iSecon - (int)m_fTimerSecond) + " : ";
		}
		else
		{
			m_strTimer += "0" + to_string(m_iSecon - (int)m_fTimerSecond) + " : ";
		}
		m_strTimer +=to_string((int)(m_iMilSecon - m_fMilisecond));

		
	}
}

bool GameManager::ProcessPacket(SOCKET sock)
{
	char debug[512];
	if (m_stPacket.Recvbyte < sizeof(PACKET_HEADER))
	{
		sprintf(debug, "[SendByte : %d , Recvbyte : %d, Len : %d", m_stPacket.Sendbyte, m_stPacket.Recvbyte, m_stPacket.Recvbyte - m_stPacket.Sendbyte);

		memcpy(m_stPacket.buf, &m_stPacket.buf[m_stPacket.Sendbyte],m_stPacket.Recvbyte - m_stPacket.Sendbyte);
		m_stPacket.Len = m_stPacket.Recvbyte - m_stPacket.Sendbyte;
		m_stPacket.Recvbyte = 0;
		m_stPacket.Sendbyte = 0;
		MessageBox(m_hWnd, debug, "경고", MB_OK);

		return false;
	}
	if (m_stPacket.Recvbyte - m_stPacket.Sendbyte < sizeof(PACKET_HEADER))
	{
		sprintf(debug, "[SendByte : %d , Recvbyte : %d, Len : %d", m_stPacket.Sendbyte, m_stPacket.Recvbyte, m_stPacket.Recvbyte - m_stPacket.Sendbyte);

		memcpy(m_stPacket.buf, &m_stPacket.buf[m_stPacket.Sendbyte], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
		m_stPacket.Len = m_stPacket.Recvbyte - m_stPacket.Sendbyte;
		m_stPacket.Recvbyte = 0;
		m_stPacket.Sendbyte = 0;
		MessageBox(m_hWnd, debug, "경고", MB_OK);

		return false;
	}

	PACKET_HEADER header;
	memcpy(&header, m_stPacket.buf, sizeof(PACKET_HEADER));

	if (header.wLen < sizeof(PACKET_HEADER))
	{
		sprintf(debug, "[SendByte : %d , Recvbyte : %d, Len : %d, header.wIndex : %d,header.wLen : %d", m_stPacket.Sendbyte, m_stPacket.Recvbyte, m_stPacket.Recvbyte - m_stPacket.Sendbyte,header.wIndex ,header.wLen);
		MessageBox(m_hWnd, debug, "경고", MB_OK);
		return false;

	}
	else if (header.wLen >100)
	{
		sprintf(debug, "[SendByte : %d , Recvbyte : %d, Len : %d, header.wIndex : %d,header.wLen : %d", m_stPacket.Sendbyte, m_stPacket.Recvbyte, m_stPacket.Recvbyte - m_stPacket.Sendbyte, header.wIndex, header.wLen);
		MessageBox(m_hWnd, debug, "경고", MB_OK);
		return false;

	}

	if (m_stPacket.Recvbyte - m_stPacket.Sendbyte < header.wLen)
	{
		sprintf(debug, "[SendByte : %d , Recvbyte : %d, Len : %d, header.wLen : %d", m_stPacket.Sendbyte, m_stPacket.Recvbyte, m_stPacket.Recvbyte - m_stPacket.Sendbyte, header.wLen);

		memcpy(m_stPacket.buf, &m_stPacket.buf[m_stPacket.Sendbyte], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
		m_stPacket.Len = m_stPacket.Recvbyte - m_stPacket.Sendbyte;
		m_stPacket.Recvbyte = 0;
		m_stPacket.Sendbyte = 0;
		MessageBox(m_hWnd, debug, "경고", MB_OK);
		return false;
	}


	if (m_stPacket.Recvbyte < header.wLen)
	{
		sprintf(debug, "[SendByte : %d , Recvbyte : %d, Len : %d, header.wLen : %d", m_stPacket.Sendbyte, m_stPacket.Recvbyte, m_stPacket.Recvbyte - m_stPacket.Sendbyte,header.wLen);

		memcpy(m_stPacket.buf, &m_stPacket.buf[m_stPacket.Sendbyte], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
		m_stPacket.Len = m_stPacket.Recvbyte - m_stPacket.Sendbyte;
		m_stPacket.Recvbyte = 0;
		m_stPacket.Sendbyte = 0;
		MessageBox(m_hWnd, debug, "경고", MB_OK);

		return false;
	}


	switch (header.wIndex)
	{
	case PACKET_INDEX_LOGIN:
		break;
	case PACKET_INDEX_LOGIN_ID_CHECK:
		break;
	case PACKET_INDEX_LOGIN_ID_GENERATE:
	{
		PACKET_ID_GENERATE Packet_gerate_id;
		memcpy(&Packet_gerate_id, m_stPacket.buf, header.wLen);
		if (Packet_gerate_id.avaliableId == true)
		{
			m_pMyPlayer->SetPlayerId(m_strName);
			PACKET_ROOM_IN Packet_Room_in;
			Packet_Room_in.header.wIndex = PACKET_INDEX_ROOM_IN;
			Packet_Room_in.header.wLen = sizeof(header) + 20 + strlen(m_strName.c_str()) + 1;
			strcpy(Packet_Room_in.name, m_strName.c_str());
			GetSend(sock, (char*)&Packet_Room_in, Packet_Room_in.header.wLen);
			m_strNameCheck = "생성에 성공하였습니다. 접속합니다.";
			SetWindowText(m_hEdit, "");
			ShowWindow(m_hEdit, SW_HIDE);
			ShowWindow(m_hChat, SW_SHOW);
		}
		else if (Packet_gerate_id.avaliableId == false)
		{
			m_strNameCheck = "이미 사용중인 아이디이거나\n\n 적합하지않은 아이디입니다.";
		}

		m_eCurState = CLIENT_STATE_IDCHECK;

		m_stPacket.Sendbyte += sizeof(PACKET_ID_GENERATE);
		memcpy(m_stPacket.buf, &m_stPacket.buf[header.wLen], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
	}
	break;
	case PACKET_INDEX_ROOM_IN:
	{
		PACKET_ROOM_IN PacketRoomIn;
		memcpy(&PacketRoomIn, m_stPacket.buf, header.wLen);
		m_pMyPlayer->Init(PacketRoomIn.index);
		m_pMyPlayer->SetPlayerId(PacketRoomIn.name);
		m_pMyPlayer->SetPlayerNumber(PacketRoomIn.index);
		m_pMyPlayer->SetPlayerType(PacketRoomIn.type);
		m_pMyPlayer->SetPlayer(true);
		m_pMyPlayer->SetPlayerState(PacketRoomIn.state);
		m_pPlayerManager->SetMyPlayer(PacketRoomIn.index, m_pMyPlayer);
		if (m_pMyPlayer->GetPlayerState()!= PLAYER_STATE_TRESPASS)
		{
			m_icurRound = PacketRoomIn.curRound;
		}
		m_iRoundEnd = PacketRoomIn.RoundEnd;

		m_stPacket.Sendbyte += header.wLen;
		memcpy(m_stPacket.buf, &m_stPacket.buf[header.wLen], m_stPacket.Sendbyte);

		m_eCurState = CLIENT_STATE_ROOM_IN;
		PACKET_ROOM_PLAYER_INFO Packet_OtherPlayer;
		Packet_OtherPlayer.header.wIndex = PACKET_INDEX_ROOM_PLAYER_INFO;
		Packet_OtherPlayer.header.wLen = sizeof(PACKET_HEADER) + 12;
		GetSend(sock, (char*)&Packet_OtherPlayer, Packet_OtherPlayer.header.wLen);
	}
	break;
	case PACKET_INDEX_ROOM_PLAYER_INFO:
	{
		PACKET_ROOM_PLAYER_INFO Packet_OtherPlayer;
		memcpy(&Packet_OtherPlayer, m_stPacket.buf, header.wLen);
		m_pPlayerManager->allocatePlayer(Packet_OtherPlayer.index, Packet_OtherPlayer.name, Packet_OtherPlayer.type, Packet_OtherPlayer.State);
		if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_START_ACT)
		{
			if (m_pMyPlayer->GetType() == PLAYER_TYPE_HOST)		
			{
				if (m_pPlayerManager->GetStartAct())
				{
					m_pMyPlayer->SetPlayerState(PLAYER_STATE_START_ACT);
				}
				else
				{
					m_pMyPlayer->SetPlayerState(PLAYER_STATE_GAME);
					m_pGameScreen->ScreenClear();
				}
			}
		}

		m_stPacket.Sendbyte += header.wLen;
		memcpy(m_stPacket.buf, &m_stPacket.buf[header.wLen], m_stPacket.Recvbyte - m_stPacket.Sendbyte);

	}
	break;
	case  PACKET_INDEX_ROOM_INITIALIZED:
	{		
	PACKET_ROOM_INITIALIZED packet_room_Init;
	memcpy(&packet_room_Init, m_stPacket.buf, sizeof(packet_room_Init));

	m_pMyPlayer->SetPlayerState(packet_room_Init.PlayerState);

	for (int i = 0; i < MAXPLAYER; i++)
	{
		m_pPlayerManager->SetPlayerState(i, packet_room_Init.PlayerState);
	}
	m_pGameScreen->SetGameState(GAME_STATE_WAIT);

	m_stPacket.Sendbyte += sizeof(PACKET_ROOM_INITIALIZED);
	memcpy(m_stPacket.buf, &m_stPacket.buf[header.wLen], m_stPacket.Recvbyte - m_stPacket.Sendbyte);

	}
	break;
	case PACKET_INDEX_TIMER:
	{
		PACKET_TIMER packet_timer;
		memcpy(&packet_timer, m_stPacket.buf, sizeof(packet_timer));
		m_pGameScreen->SetGameState(GAME_STATE_NEXT);
		m_pGameScreen->SetEffectNext(true);
		m_pGameScreen->SetRound(packet_timer.curRound);
		m_pPlayerManager->SetPlayerState(packet_timer.CurDrawPlayer, PLAYER_STATE_DRAW);
		if (m_pMyPlayer->GetPlayerNumber() == packet_timer.CurDrawPlayer)
		{
			m_pMyPlayer->SetPlayerState(PLAYER_STATE_DRAW);
		}
		else
		{
			m_pMyPlayer->SetPlayerState(PLAYER_STATE_GAME);
		}
		for (int i = 0; i < MAXPLAYER; i++)
		{
			if (i == packet_timer.CurDrawPlayer)
				continue;
			m_pPlayerManager->SetPlayerState(i, PLAYER_STATE_GAME);
		}
		m_pGameScreen->SetNextTurn(m_pPlayerManager->GetPlayerName(packet_timer.CurDrawPlayer), m_pPlayerManager->GetPlayerName(packet_timer.NextDrawPlayer));

		SetTimeTimer();
		m_strTimer = "0 : 00 : 00";

		m_stPacket.Sendbyte += sizeof(PACKET_TIMER);
		memcpy(m_stPacket.buf, &m_stPacket.buf[header.wLen], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
	}
	break;
	case PACKET_INDEX_ROOM_READY:
	{
		PACKET_ROOM_PLAYER_READY Packet_ready;
		memcpy(&Packet_ready, m_stPacket.buf, sizeof(Packet_ready));

		m_pPlayerManager->SetReady(Packet_ready.PlayerNumber, Packet_ready.m_bReady);
		if (m_pMyPlayer->GetType() == PLAYER_TYPE_HOST)
		{
			if (m_pPlayerManager->GetStartAct())
			{
				m_pMyPlayer->SetPlayerState(PLAYER_STATE_START_ACT);
			}
			else
			{
				m_pMyPlayer->SetPlayerState(PLAYER_STATE_GAME);
				m_pGameScreen->ScreenClear();
			}
		}

		m_stPacket.Sendbyte += sizeof(PACKET_ROOM_PLAYER_READY);
		memcpy(m_stPacket.buf, &m_stPacket.buf[sizeof(Packet_ready)], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
		
	}
	break;
	case PACKET_INDEX_ROOM_START:
	{
		PACKET_ROOM_START Packet_start;
		memcpy(&Packet_start, m_stPacket.buf, sizeof(Packet_start));
		m_pGameScreen->SetRound(Packet_start.curRound);
		m_pGameScreen->SetGameState(GAME_STATE_START);
		if (m_pMyPlayer->GetType() == PLAYER_TYPE_HOST)
		{
			m_pMyPlayer->SetPlayerState(PLAYER_STATE_DRAW);
		}
		else
			m_pMyPlayer->SetPlayerState(PLAYER_STATE_START);

		m_pPlayerManager->SetPlayerState(Packet_start.CurDrawPlayer, PLAYER_STATE_DRAW);
		for (int i = 0; i < MAXPLAYER; i++)
		{
			if (i == Packet_start.CurDrawPlayer)
				continue;
			m_pPlayerManager->SetPlayerState(i, PLAYER_STATE_GAME);
		}
		m_pGameScreen->SetNextTurn(m_pPlayerManager->GetPlayerName(Packet_start.CurDrawPlayer), m_pPlayerManager->GetPlayerName(Packet_start.NextDrawPlayer));

		SetTimeTimer();
		m_icurRound = Packet_start.curRound;

		m_stPacket.Sendbyte += sizeof(PACKET_ROOM_START);
		memcpy(m_stPacket.buf, &m_stPacket.buf[sizeof(Packet_start)], m_stPacket.Recvbyte - m_stPacket.Sendbyte);

	}
	break;
	case PACKET_INDEX_ROOM_NEXTTURN:
	{
		PACKET_ROOM_NEXTTURN packet_Next;
		memcpy(&packet_Next, m_stPacket.buf, sizeof(packet_Next));
		m_bAnswer = false;
		m_pGameScreen->SetRound(packet_Next.curRound);
		m_pGameScreen->ScreenClear();
		m_pPlayerManager->SetPlayerState(packet_Next.curDrawPlayer, PLAYER_STATE_DRAW);
		if (m_pMyPlayer->GetPlayerNumber() == packet_Next.curDrawPlayer)
		{
			m_pMyPlayer->SetPlayerState(PLAYER_STATE_DRAW);
		}
		else
		{
			m_pMyPlayer->SetPlayerState(PLAYER_STATE_GAME);
		}
		for (int i = 0; i < MAXPLAYER; i++)
		{
			if (i == packet_Next.curDrawPlayer)
				continue;
			m_pPlayerManager->SetPlayerState(i, PLAYER_STATE_GAME);
		}
		SetTimeTimer();
		
		m_icurRound = packet_Next.curRound;

		m_pGameScreen->SetNextTurn(m_pPlayerManager->GetPlayerName(packet_Next.curDrawPlayer), m_pPlayerManager->GetPlayerName(packet_Next.NextDrawPlayer));

		m_stPacket.Sendbyte += sizeof(PACKET_ROOM_NEXTTURN);
		memcpy(m_stPacket.buf, &m_stPacket.buf[sizeof(packet_Next)], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
	}
	break;
	case PACKET_INDEX_REQUEST_QUESTION:
	{
		PACKET_REQUEST_QUESTION packet_request_question;
		memcpy(&packet_request_question, m_stPacket.buf, header.wLen);

		if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_DRAW)
		{
			strcpy((char*)m_strQuestion.c_str(), packet_request_question.Question);
			GetTextExtentPoint32(m_hMemDC, m_strQuestion.c_str(), m_strQuestion.size(), &m_Questionsize);
			m_rcQuestion.left = QUESTION_POS_X + QUESTION_OFFSET_X + 10;
			m_rcQuestion.top = QUESTION_POS_Y + QUESTION_OFFSET_Y;
			m_rcQuestion.right = m_rcQuestion.left + m_pBitmap_Question->GetSizeBitmap().cx + m_Questionsize.cx;
			m_rcQuestion.bottom = m_rcQuestion.top + m_pBitmap_Question->GetSizeBitmap().cy;
		}


		m_stPacket.Sendbyte += header.wLen;
		memcpy(m_stPacket.buf, &m_stPacket.buf[header.wLen], m_stPacket.Recvbyte - m_stPacket.Sendbyte);

	}
	break;
	case PACKET_INDEX_ROOM_CHAT:
	{
		PACKET_ROOM_CHAT Packet_chat;

		memcpy(&Packet_chat, m_stPacket.buf, header.wLen);

		m_pPlayerManager->RecvChat(Packet_chat.index, Packet_chat.Chat);

		m_stPacket.Sendbyte += header.wLen;
		memcpy(m_stPacket.buf, &m_stPacket.buf[header.wLen], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
	}
	break;
	case PACKET_INDEX_DRAWDOWN:
	{
		PACKET_ROOM_DRAWDOWN Packet_down;
		memcpy(&Packet_down, m_stPacket.buf, sizeof(Packet_down));

		
		m_pGameScreen->RecvDrawMouseDown(Packet_down.Color, Packet_down.BrushType, Packet_down.OriginX, Packet_down.OriginY);

		m_stPacket.Sendbyte += sizeof(Packet_down);
		memcpy(m_stPacket.buf, &m_stPacket.buf[header.wLen], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
	}
	break;
	case PACKET_INDEX_DRAWMOVE:
	{
		PACKET_ROOM_DRAWMOVE Packet_move;
		memcpy(&Packet_move, m_stPacket.buf, sizeof(Packet_move));

		m_pGameScreen->RecvDrawMouseMove(Packet_move.Color, Packet_move.BrushType, Packet_move.OriginX, Packet_move.OriginY);

		m_stPacket.Sendbyte += sizeof(Packet_move);
		memcpy(m_stPacket.buf, &m_stPacket.buf[header.wLen], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
	}
	break;
	case PAKCET_INDEX_ROOM_ANSWER:
	{
		PACKET_ROOM_ANSWER packet_answer;
		memcpy(&packet_answer, m_stPacket.buf, header.wLen);
		if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_DRAW)
		{
			m_pPlayerManager->SetPlayerState(m_pMyPlayer->GetPlayerNumber(), PLAYER_STATE_ANSWER);
			m_bAnswer = true;
			ShowWindow(m_hChat, SW_SHOW);
		}
		m_pPlayerManager->SetPlayerState(packet_answer.index, PLAYER_STATE_ANSWER);
		m_pPlayerManager->SetScore(packet_answer.index);
		m_pGameScreen->SetGameState(GAME_STATE_ANSWER);

		if (m_iMilSecon <= 0)
			m_iMilSecon = 0;
		m_stPacket.Sendbyte += header.wLen;

		memcpy(m_stPacket.buf, &m_stPacket.buf[header.wLen], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
	}
	break;
	case PACKET_INDEX_ROOM_RANK:
	{
		PACKET_RANKING packet_ranking;
		memcpy(&packet_ranking, m_stPacket.buf, header.wLen);

		m_pRanking->SetScore(packet_ranking.Socre, packet_ranking.PlayerName);

		memcpy(m_stPacket.buf, &m_stPacket.buf[header.wLen], m_stPacket.Recvbyte - header.wLen);
		m_stPacket.Sendbyte += header.wLen;
	}
		break;
	case PACKET_INDEX_ROOM_GAMEOVER:
	{
		PACKET_ROOM_GAME_OVER Packet_Gameover;
		memcpy(&Packet_Gameover, m_stPacket.buf, sizeof(Packet_Gameover));
		m_pGameScreen->SetGameState(GAME_STATE_RESULTS);
		m_pMyPlayer->SetPlayerState(PLAYER_STATE_GAME);
		for (int i = 0; i < MAXPLAYER; i++)
		{
			m_pPlayerManager->SetPlayerState(i, PLAYER_STATE_GAME);
		}

		m_pRanking->ArrayScore();

		m_bGameOver = true;

		m_stPacket.Sendbyte += sizeof(PACKET_ROOM_GAME_OVER);
		memcpy(m_stPacket.buf, &m_stPacket.buf[sizeof(Packet_Gameover)], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
	}
	break;

	case PACKET_INDEX_ROOM_SHUTDOWN_DRAW_PLYAER:
	{
		PACKET_ROOM_SHUTDOWN_DRAW_PLYAER Packet_Shutdown_draw;
		memcpy(&Packet_Shutdown_draw, m_stPacket.buf, sizeof(Packet_Shutdown_draw));

		m_pPlayerManager->Init(Packet_Shutdown_draw.index);
		m_pPlayerManager->SetisPlayer(Packet_Shutdown_draw.index, false);

		if (m_pMyPlayer->GetPlayerNumber() == Packet_Shutdown_draw.NextHost)
		{
			m_pMyPlayer->SetPlayerType(PLAYER_TYPE_HOST);
			m_pPlayerManager->SetMyPlayer(m_pMyPlayer->GetPlayerNumber(), m_pMyPlayer);
		}



		if (Packet_Shutdown_draw.GameOver == false)
		{

			SetTimeTimer();
			m_pMyPlayer->SetPlayerState(PLAYER_STATE_GAME);
			m_pGameScreen->SetGameState(GAME_STATE_NEXT);


			PACKET_ROOM_NEXTTURN Packet_Next;
			Packet_Next.header.wIndex = PACKET_INDEX_ROOM_NEXTTURN;
			Packet_Next.header.wLen = sizeof(Packet_Next);
			Packet_Next.curDrawPlayer = 0;
			Packet_Next.NextDrawPlayer = 0;
			GetSend(m_Sock, (char*)&Packet_Next, sizeof(Packet_Next));
		}
		m_stPacket.Sendbyte += sizeof(Packet_Shutdown_draw);
		memcpy(m_stPacket.buf, &m_stPacket.buf[sizeof(Packet_Shutdown_draw)], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
	
	}
		break;
	case PACKET_INDEX_ROOOM_SHUTDOWN_NOMAL_PLAYER:
	{
		PACKET_ROOM_SHUTDOWN_NOMAL_PLAYER Packet_shutdown_nomal;
		memcpy(&Packet_shutdown_nomal, m_stPacket.buf, sizeof(Packet_shutdown_nomal));
		m_pPlayerManager->Init(Packet_shutdown_nomal.index);
		m_pPlayerManager->SetisPlayer(Packet_shutdown_nomal.index, false);
		if (m_pMyPlayer->GetPlayerNumber() == Packet_shutdown_nomal.NextHost)
		{
			m_pMyPlayer->SetPlayerType(PLAYER_TYPE_HOST);
			m_pPlayerManager->SetMyPlayer(m_pMyPlayer->GetPlayerNumber(), m_pMyPlayer);
		}
		if (Packet_shutdown_nomal.actTrespass == true)
		{
			SetTimeTimer();
			m_pMyPlayer->SetPlayerState(PLAYER_STATE_GAME);
			m_pGameScreen->SetGameState(GAME_STATE_NEXT);


			PACKET_ROOM_NEXTTURN Packet_Next;
			Packet_Next.header.wIndex = PACKET_INDEX_ROOM_NEXTTURN;
			Packet_Next.header.wLen = sizeof(Packet_Next);
			Packet_Next.curDrawPlayer = 0;
			Packet_Next.NextDrawPlayer = 0;
			GetSend(m_Sock, (char*)&Packet_Next, sizeof(Packet_Next));
		}

		m_stPacket.Sendbyte += sizeof(Packet_shutdown_nomal);
		memcpy(m_stPacket.buf, &m_stPacket.buf[sizeof(Packet_shutdown_nomal)], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
	}
	break;
	case PACKET_INDEX_ROOM_SHUTDOWN_PLAYER:
	{
		PACKET_ROOM_SHUTDOWN_PLAYER Packet_Shutdown;
		memcpy(&Packet_Shutdown, m_stPacket.buf, sizeof(Packet_Shutdown));

		m_pPlayerManager->Init(Packet_Shutdown.index);
		m_pPlayerManager->SetisPlayer(Packet_Shutdown.index, false);
		if (m_pMyPlayer->GetPlayerNumber() == Packet_Shutdown.NextHost)
		{
			m_pMyPlayer->SetPlayerType(PLAYER_TYPE_HOST);
			m_pPlayerManager->SetMyPlayer(m_pMyPlayer->GetPlayerNumber(), m_pMyPlayer);
		}
		if (m_pMyPlayer->GetPlayerState() == PLAYER_STATE_START_ACT)
		{
			if (m_pMyPlayer->GetType() == PLAYER_TYPE_HOST)
			{
				if (m_pPlayerManager->GetStartAct())
				{
					m_pMyPlayer->SetPlayerState(PLAYER_STATE_START_ACT);
				}
				else
				{
					m_pMyPlayer->SetPlayerState(PLAYER_STATE_GAME);
					m_pGameScreen->ScreenClear();
				}
			}
		}
		m_stPacket.Sendbyte += sizeof(Packet_Shutdown);
		memcpy(m_stPacket.buf, &m_stPacket.buf[sizeof(Packet_Shutdown)], m_stPacket.Recvbyte - m_stPacket.Sendbyte);
		
	}
		break;
	}
	m_stPacket.Len= 0;
	return true;
}

int GameManager::GetSend(SOCKET sock, char* buf, int len)
{
	return send(sock, buf, len, 0);
}
void GameManager::SetTimeTimer()
{
	m_bTimer = true;
	m_fTimer = 0.0f;
	m_fTimerSecond = 0.0f;
	m_iMin = 1;
	m_iSecon = 59;
	m_iMilSecon = 100;
	m_strTimer = "2 : 00 : 00";
}