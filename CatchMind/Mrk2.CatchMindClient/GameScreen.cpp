#include "GameScreen.h"
#include "ResourceManager.h"
#include "Bitmap.h"
#include "Player.h"
#include "../Mrk2.CatchMindServer/Packet.h"
#include "GameManager.h"
#include <math.h>
DWORD dwRGB[COLOR_TYPE_END] = { {RGB(0,0,0)}, { RGB(244,121,128) },{ RGB(255,168,111) },{ RGB(255,249,136) },{ RGB(100,96,255) },{ RGB(138,255,125) },{ RGB(220,187,169) },{ RGB(255,255,255) } };
POINT Dir_pt[DIRECTION_END] = { {-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0} };

GameScreen::GameScreen()
{
	m_eCurGameState = GAME_STATE_WAIT;
	m_curColor = 0;
	for (int i = 0; i < COLOR_TYPE_END; i++)
	{
		m_rcPaint[i].left = PAINT_POS_X + (i*PAINT_OFFSET)+(i*PAINT_WIDTH);
		m_rcPaint[i].top = PAINT_POS_Y ;
		m_rcPaint[i].right = m_rcPaint[i].left + PAINT_WIDTH;
		m_rcPaint[i].bottom = m_rcPaint[i].top + PAINT_HEIGHT;
	}
	m_fEffectNext = 0.0f;
	m_bEffectNext = false;
	m_fEffectStart = 0.0f;
	m_bEffectStart = false;
	m_fEffectAnswer = 0;
	m_bEffectAnswer = false;
	m_bLButtonDown = false;
	m_bSelectColor = false;
	m_fEffectTimeOut = 0.0f;
	m_bEffectTimeOut = false;
	m_curDrawPlayer = "Defalut";
	m_NextDrawPlayer = "Defalut";

	m_iPrevMouseX = 0;
	m_iPrevMouseY = 0;
	m_bCleanButton = false;
	m_bStrate = false;

	m_iOriginX = 0;
	m_iOriginY = 0;
	m_rcCurRound.left = 0;
	m_rcCurRound.top = DRAWSIZE_HEIGHT/2 - 100;
	m_rcCurRound.right =  DRAWSIZE_WIDTH/2;
	m_rcCurRound.bottom = DRAWSIZE_HEIGHT / 2 ;


}


GameScreen::~GameScreen()
{
	for (int i = 0; i < COLOR_TYPE_END; i++)
	{
		SelectObject(m_hMemDC, m_hOldLbPen[i]);
		SelectObject(m_hMemDC, m_hOldRbPen[i]);
		SelectObject(m_hMemDC, m_hOldBrush[i]);

		DeleteObject(m_hLbPen[i]);
		DeleteObject(m_hRbPen[i]);
		DeleteObject(m_hBrush[i]);
	}

	delete(m_hMemDC);
}

void GameScreen::Init(HDC hdc,HWND hWnd)
{
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, DRAWSIZE_WIDTH, DRAWSIZE_HEIGHT);
	SelectObject(m_hMemDC, m_hBitmap);
	m_pBitmap = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_BACKGROUND);
	m_pBitmapReady = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_GAME_READY);
	m_pBitmapStart = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_GAME_START);
	m_pBitmapSelectColor = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_SELECTCOLOR);
	m_rcGameScreen.left = 0;
	m_rcGameScreen.top = 0;
	m_rcGameScreen.right = DRAWSIZE_WIDTH;
	m_rcGameScreen.bottom = DRAWSIZE_HEIGHT;
	m_pBitmapTrepass = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_TREPASS);
	m_hWnd = hWnd;
	FillRect(m_hMemDC, &m_rcGameScreen, (HBRUSH)GetStockObject(WHITE_BRUSH));
	m_pBitmapNextTurn= ResourceManager::GetInstance()->GetBitmap(RES_TYPE_NEXT_TURN);
	m_pBitmapCleanButton = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_CLEAN);
	m_rcCurDrawPlayer.left = DRAWSIZE_WIDTH/2-20;
	m_rcCurDrawPlayer.top = DRAWSIZE_HEIGHT/2-40;
	m_rcCurDrawPlayer.right = m_rcCurDrawPlayer.left + 80;
	m_rcCurDrawPlayer.bottom = m_rcCurDrawPlayer.top + 20;
	m_rcNextDrawPlayer.left = DRAWSIZE_WIDTH / 2 -20;
	m_rcNextDrawPlayer.top = DRAWSIZE_HEIGHT / 2 ;
	m_rcNextDrawPlayer.right = m_rcNextDrawPlayer.left +80;
	m_rcNextDrawPlayer.bottom = m_rcNextDrawPlayer.top + 20;

	m_rcCleanButton.left = BUTTON_CLEAN_POS_X;
	m_rcCleanButton.top = BUTTON_CLEAN_POS_Y;
	m_rcCleanButton.right = m_rcCleanButton.left + m_pBitmapCleanButton->GetSizeBitmap().cx;
	m_rcCleanButton.bottom = m_rcCleanButton.top + m_pBitmapCleanButton->GetSizeBitmap().cy;

	for (int i = 0; i < COLOR_TYPE_END; i++)
	{
		m_hLbPen[i] = CreatePen(PS_SOLID, BRUSH_SIZE_LB, dwRGB[i]);
		m_hRbPen[i] = CreatePen(PS_SOLID, BRUSH_SIZE_RB, dwRGB[i]);
		m_hBrush[i] = CreateSolidBrush(dwRGB[i]);
		m_hOldLbPen[i] = (HPEN)SelectObject(m_hMemDC, m_hLbPen[i]);
		m_hOldRbPen[i] = (HPEN)SelectObject(m_hMemDC, m_hRbPen[i]);
		m_hOldBrush[i] = (HBRUSH)SelectObject(m_hMemDC, m_hBrush[i]);
	}
	m_hOldBrush[0] = (HBRUSH)SelectObject(m_hMemDC, m_hBrush[0]);
	m_hOldRbPen[2] = (HPEN)SelectObject(m_hMemDC, m_hRbPen[2]);
	m_pBitmap_AllReady = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_ALLREADY);

}
void GameScreen::Update(float fElapse)
{
	if (m_bEffectStart && m_eCurGameState ==GAME_STATE_START)
	{
		m_fEffectStart += fElapse;
		if (m_fEffectStart >= 2.0f)
		{
			m_bEffectStart = 0.0f;
			SetGameState(GAME_STATE_NEXT);
			m_bEffectStart = false;
		}
	}
	if (m_bEffectNext&& m_eCurGameState == GAME_STATE_NEXT)
	{
		m_fEffectNext += fElapse;
	}
	if (m_bEffectAnswer && m_eCurGameState == GAME_STATE_ANSWER)
	{
		m_fEffectAnswer += fElapse;
	}
	
}
bool GameScreen::GetAnswerEffectEnd()
{
	if (m_fEffectAnswer >= 2.0f)
	{
		m_bEffectAnswer = false;
		SetGameState(GAME_STATE_NEXT);
		m_fEffectAnswer = 0.0f;
		return true;
	}
	return false;
}
bool GameScreen::GetNextEffectEnd()
{
	if (m_fEffectNext >= 3.0f)
	{
		m_bEffectNext = false;
		SetGameState(GAME_STATE_GAME);
		m_fEffectNext = 0.0f;
		return true;
	}
	return false;
}
void GameScreen::Draw(HDC hdc)
{
	SetBkMode(m_hMemDC, TRANSPARENT);
	 DrawGameStateGame();
	 DrawGameNextTurn();
	 DrawGameAnswer();
	 
	 for (int i = 0; i < COLOR_TYPE_END; i++)
	 {
		 Rectangle(hdc, m_rcPaint[i].left, m_rcPaint[i].top, m_rcPaint[i].right, m_rcPaint[i].bottom);
		 FillRect(hdc, &m_rcPaint[i], (HBRUSH)m_hBrush[i]);
	 }
	 m_pBitmapSelectColor->DrawTransparent(hdc,m_rcPaint[m_curColor].left, PAINT_POS_Y);
	BitBlt(hdc, DRAWOFFSET_X, DRAWOFFSET_Y, DRAWSIZE_WIDTH, DRAWSIZE_HEIGHT, m_hMemDC, 0, 0, SRCCOPY);
}
void GameScreen::DrawTrepass()
{
	m_pBitmapTrepass->DrawMidPoint(m_hMemDC, DRAWSIZE_WIDTH / 2, DRAWSIZE_HEIGHT / 2);
}
void GameScreen::DrawCleanButton(HDC hdc)
{
	m_pBitmapCleanButton->DrawTransparent(hdc, BUTTON_CLEAN_POS_X, BUTTON_CLEAN_POS_Y);

}
bool GameScreen::ClenaButton(POINT ptMouse)
{
	if (PtInRect(&m_rcCleanButton, ptMouse))
	{
		if (m_bCleanButton == false && (GetKeyState(MK_LBUTTON) & 0x8000 || (GetKeyState(MK_RBUTTON) & 0x8000)))
		{
			m_bCleanButton = true;
		}
		if ((GetKeyState(MK_LBUTTON) & 0x8000 || GetKeyState(MK_RBUTTON) & 0x8000) && m_bCleanButton == true)
		{
			return false;
		}

		if (m_bLButtonDown)
		{
			FillRect(m_hMemDC, &m_rcGameScreen, (HBRUSH)m_hBrush[m_curColor]);
			m_bCleanButton = false;
			return true;
		}
	}
	else
	{
		m_bCleanButton = false;
		return false;
	}
}
void GameScreen::DrawAllReady()
{
	m_pBitmap_AllReady->DrawMidPoint(m_hMemDC, DRAWSIZE_WIDTH / 2,DRAWSIZE_HEIGHT / 2);
}
void GameScreen::DrawingMouse(POINT ptMouse)
{
	if (GetKeyState(MK_LBUTTON) & 0x8000)
	{
		m_hOldLbPen[m_curColor] = (HPEN)SelectObject(m_hMemDC, m_hLbPen[m_curColor]);
	}
	else if (GetKeyState(MK_RBUTTON) & 0x8000)
	{
		m_hOldRbPen[m_curColor] = (HPEN)SelectObject(m_hMemDC, m_hRbPen[m_curColor]);
	}
	if (m_bLButtonDown == false && (GetKeyState(MK_LBUTTON) & 0x8000 || (GetKeyState(MK_RBUTTON) & 0x8000)))
	{
		if (ptMouse.x >= DRAWOFFSET_X && ptMouse.y >= DRAWOFFSET_Y && ptMouse.y <= DRAWSIZE_HEIGHT + DRAWOFFSET_Y&& ptMouse.x <= DRAWSIZE_WIDTH + DRAWOFFSET_X)
		{
			if (GetKeyState(VK_LSHIFT) & 0x8000)
			{
				LineTo(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y);
			}
			else if (GetKeyState(VK_CONTROL) & 0x8000)
			{
				m_bStrate = true;
				m_iOriginX = ptMouse.x - DRAWOFFSET_X;
				m_iOriginY = ptMouse.y - DRAWOFFSET_Y;

			}
			MoveToEx(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y, NULL);

			m_bLButtonDown = true;
			m_bCleanButton = true;
		}
		else if (PtInRect(&m_rcCleanButton, ptMouse))
		{
			if (m_bCleanButton == false && (GetKeyState(MK_LBUTTON) & 0x8000 || GetKeyState((MK_RBUTTON) & 0x8000)))
			{
				m_bCleanButton = true;
				m_bLButtonDown = true;
			}
		}
	}

	 if ((GetKeyState(MK_LBUTTON) & 0x8000 || GetKeyState(MK_RBUTTON) & 0x8000) && m_bLButtonDown == true)
	{
		 if (ptMouse.x >= DRAWOFFSET_X && ptMouse.y >= DRAWOFFSET_Y && ptMouse.y <= DRAWSIZE_HEIGHT + DRAWOFFSET_Y&& ptMouse.x <= DRAWSIZE_WIDTH + DRAWOFFSET_X)
		 {
			 if (m_bStrate == true)
			 {
				 LineTo(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y);


				 MoveToEx(m_hMemDC, m_iOriginX, m_iOriginY, NULL);
				 LineTo(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y);
			 }

				 LineTo(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y);
				 LineTo(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y);
			 
		 }

		return;
	}

	if (m_bLButtonDown)
	{
		if (PtInRect(&m_rcCleanButton, ptMouse) && m_bCleanButton ==true)
		{
			FillRect(m_hMemDC, &m_rcGameScreen, (HBRUSH)m_hBrush[m_curColor]);
		}

		if (m_bStrate == true)
		{
			MoveToEx(m_hMemDC, m_iOriginX, m_iOriginY, NULL);
			LineTo(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y);
		}
		m_bCleanButton = false;
		m_bStrate = false;
		m_bLButtonDown = false;
	}
}



void GameScreen::SelectColor(POINT ptMouse)
{
	if (m_bSelectColor == false && (GetKeyState(MK_LBUTTON) & 0x8000 || (GetKeyState(MK_RBUTTON) & 0x8000)))
	{
		m_bSelectColor = true;
	}
	 if ((GetKeyState(MK_LBUTTON) & 0x8000 || GetKeyState(MK_RBUTTON) & 0x8000) && m_bSelectColor == true)
	{
		m_bSelectColor = true;
		return;
	}


	if (m_bSelectColor)
	{
		for (int i = 0; i < COLOR_TYPE_END; i++)
		{
			if (PtInRect(&m_rcPaint[i], ptMouse))
			{
				m_curColor = i;
			}
		}
		m_bSelectColor = false;
	}
}

void GameScreen::DrawGameStateWait()
{
	if (m_eCurGameState == GAME_STATE_WAIT)
	{
		m_pBitmapReady->DrawMidPoint(m_hMemDC, DRAWSIZE_WIDTH / 2, DRAWSIZE_HEIGHT / 2);
	}
}
void GameScreen::SetRound(int Round)
{
	m_iRound = Round;
	m_strRound = "Round  "+ to_string(Round);
}
void GameScreen::DrawGameStateGame()
{
	if (m_eCurGameState == GAME_STATE_START)
	{
		m_bEffectStart = true;
		m_pBitmapStart->DrawMidPoint(m_hMemDC, DRAWSIZE_WIDTH / 2, DRAWSIZE_HEIGHT / 2);
	}
}
void GameScreen::DrawGameNextTurn()
{
	if (m_eCurGameState == GAME_STATE_NEXT)
	{
		ScreenClear();
		m_bEffectNext = true;
		m_pBitmapNextTurn->DrawMidPoint(m_hMemDC, DRAWSIZE_WIDTH / 2, DRAWSIZE_HEIGHT / 2);
		DrawText(m_hMemDC, m_strRound.c_str(), -1, &m_rcCurRound, DT_CENTER | DT_NOCLIP);
		DrawText(m_hMemDC, m_curDrawPlayer.c_str(), -1, &m_rcCurDrawPlayer, DT_CENTER | DT_NOCLIP);
		DrawText(m_hMemDC, m_NextDrawPlayer.c_str(), -1, &m_rcNextDrawPlayer, DT_CENTER | DT_NOCLIP);
	}
}
void GameScreen::DrawGameAnswer()
{
	if (m_eCurGameState == GAME_STATE_ANSWER)
	{
		m_bEffectAnswer = true; 
	}
}
void GameScreen::SetGameState(GAME_STATE SetGameState)
{
	m_eCurGameState = SetGameState;
	m_fEffectNext = 0.0f;
	m_bEffectNext= false;
	m_fEffectStart = 0.0f;
	m_bEffectStart = false;
	m_fEffectAnswer = 0.0f;
	m_bEffectAnswer = false;
	m_fEffectTimeOut = 0.0f;
	m_bEffectTimeOut = false;

	if(m_eCurGameState!= GAME_STATE_ANSWER)
	ScreenClear();
	
}
void GameScreen::ScreenClear()
{
	FillRect(m_hMemDC, &m_rcGameScreen, (HBRUSH)GetStockObject(WHITE_BRUSH));

}

void GameScreen::SendDrawing(SOCKET sock, POINT ptMouse)
{
	m_Sock = sock;
	if (GetKeyState(MK_LBUTTON) & 0x8000)
	{
		m_hOldLbPen[m_curColor] = (HPEN)SelectObject(m_hMemDC, m_hLbPen[m_curColor]);
	}
	else if (GetKeyState(MK_RBUTTON) & 0x8000)
	{
		m_hOldRbPen[m_curColor] = (HPEN)SelectObject(m_hMemDC, m_hRbPen[m_curColor]);
	}
	if (m_bLButtonDown == false && (GetKeyState(MK_LBUTTON) & 0x8000 || (GetKeyState(MK_RBUTTON) & 0x8000)))
	{
		if (ptMouse.x >= DRAWOFFSET_X && ptMouse.y >= DRAWOFFSET_Y && ptMouse.y <= DRAWSIZE_HEIGHT + DRAWOFFSET_Y&& ptMouse.x <= DRAWSIZE_WIDTH + DRAWOFFSET_X)
		{
		PACKET_ROOM_DRAWDOWN Packet_click;
		Packet_click.Color = m_curColor;
		Packet_click.header.wIndex = PACKET_INDEX_DRAWDOWN;
		Packet_click.header.wLen = sizeof(Packet_click);
		Packet_click.OriginX = ptMouse.x - DRAWOFFSET_X;
		Packet_click.OriginY = ptMouse.y - DRAWOFFSET_Y;
		if (GetKeyState(VK_LSHIFT) & 0x8000)
		{
			LineTo(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y);

			if (GetKeyState(MK_LBUTTON) & 0x8000)
				Packet_click.BrushType = DRAWTYPE_PREV_STRATE_LB;

			else if ((GetKeyState(MK_RBUTTON) & 0x8000))
				Packet_click.BrushType = DRAWTYPE_PREV_STRATE_RB;

			GameManager::GetInstance()->GetSend(sock, (char*)&Packet_click, sizeof(Packet_click));

		}
		else if (GetKeyState(MK_LBUTTON) & 0x8000)
		{
			Packet_click.BrushType = DRAWTYPE_CLICK_LB;
			GameManager::GetInstance()->GetSend(sock, (char*)&Packet_click, sizeof(Packet_click));

		}
		else if ((GetKeyState(MK_RBUTTON) & 0x8000))
		{
			Packet_click.BrushType = DRAWTYPE_CLICK_RB;
			GameManager::GetInstance()->GetSend(sock, (char*)&Packet_click, sizeof(Packet_click));
		}
		m_bLButtonDown = true;
		m_bCleanButton = true;
		MoveToEx(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y, NULL);
		}
		else if (PtInRect(&m_rcCleanButton, ptMouse))
		{
			if (m_bCleanButton == false && (GetKeyState(MK_LBUTTON) & 0x8000 || GetKeyState((MK_RBUTTON) & 0x8000)))
			{
				m_bCleanButton = true;
				m_bLButtonDown = true;
			}
		}
	}
	if ((GetKeyState(MK_LBUTTON) & 0x8000 || GetKeyState(MK_RBUTTON) & 0x8000) && m_bLButtonDown == true)
	{
		int MoveMouseX;
		int MoveMouseY;
		if (ptMouse.x >= DRAWOFFSET_X && ptMouse.y >= DRAWOFFSET_Y && ptMouse.y <= DRAWSIZE_HEIGHT + DRAWOFFSET_Y&& ptMouse.x <= DRAWSIZE_WIDTH + DRAWOFFSET_X)
		{
			MoveMouseX = abs((ptMouse.x - DRAWOFFSET_X) - m_iPrevMouseX);
			MoveMouseY = abs((ptMouse.y - DRAWOFFSET_Y) - m_iPrevMouseY);
			if (MoveMouseX > 1 || MoveMouseY > 1)
			{
				LineTo(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y);
				LineTo(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y);

			}
			if (MoveMouseX >3 || MoveMouseY > 3)
			{
				PACKET_ROOM_DRAWMOVE Packet_move;
				Packet_move.Color = m_curColor;
				Packet_move.OriginX = ptMouse.x - DRAWOFFSET_X;
				Packet_move.OriginY = ptMouse.y - DRAWOFFSET_Y;
				Packet_move.header.wIndex = PACKET_INDEX_DRAWMOVE;
				Packet_move.header.wLen = sizeof(Packet_move);

				if (GetKeyState(MK_LBUTTON) & 0x8000)
				{
					Packet_move.BrushType = DRAWTYPE_MOVE_LB;
					GameManager::GetInstance()->GetSend(sock, (char*)&Packet_move, sizeof(Packet_move));
				}
				else if (GetKeyState(MK_RBUTTON) & 0x8000)
				{
					Packet_move.BrushType = DRAWTYPE_MOVE_RB;
					GameManager::GetInstance()->GetSend(sock, (char*)&Packet_move, sizeof(Packet_move));
				}
				m_iPrevMouseX = ptMouse.x - DRAWOFFSET_X;
				m_iPrevMouseY = ptMouse.y - DRAWOFFSET_Y;
				MoveToEx(m_hMemDC, ptMouse.x - DRAWOFFSET_X, ptMouse.y - DRAWOFFSET_Y, NULL);
			}
		}
		return;
	}
	if (m_bLButtonDown)
	{
		if (PtInRect(&m_rcCleanButton, ptMouse) && m_bCleanButton == true)
		{		
			PACKET_ROOM_DRAWDOWN Packet_paint;
			Packet_paint.Color = m_curColor;
			Packet_paint.header.wIndex = PACKET_INDEX_DRAWDOWN;
			Packet_paint.header.wLen = sizeof(Packet_paint);
			Packet_paint.OriginX = ptMouse.x - DRAWOFFSET_X;
			Packet_paint.OriginY = ptMouse.y - DRAWOFFSET_Y;

			if ((GetKeyState(MK_LBUTTON) & 0x8000))
			{
				Packet_paint.BrushType = DRAWTYPE_PAINT_LB;
			}
			else if (GetKeyState(MK_RBUTTON) & 0x8000)
			{
				Packet_paint.BrushType = DRAWTYPE_PAINT_RB;
			}
			GameManager::GetInstance()->GetSend(sock, (char*)&Packet_paint, sizeof(Packet_paint));
			FillRect(m_hMemDC, &m_rcGameScreen, (HBRUSH)m_hBrush[m_curColor]);
		}
		m_bLButtonDown = false;
		m_bCleanButton = false;
	}
}
void GameScreen::RecvDrawMouseDown(int color, int type, int posX, int posY)
{
	if (m_eCurGameState != GAME_STATE_GAME)
	{
		return;
	}
	m_curColor = color;
	if (type == DRAWTYPE_CLICK_LB)
	{
		m_hOldLbPen[m_curColor] = (HPEN)SelectObject(m_hMemDC, m_hLbPen[m_curColor]);
		MoveToEx(m_hMemDC, posX, posY, NULL);
		LineTo(m_hMemDC, posX, posY);
	}
	else if (type == DRAWTYPE_CLICK_RB)
	{
		m_hOldRbPen[m_curColor] = (HPEN)SelectObject(m_hMemDC, m_hRbPen[m_curColor]);
		MoveToEx(m_hMemDC, posX, posY, NULL);
		LineTo(m_hMemDC, posX, posY);
	}
	else  if (type == DRAWTYPE_PREV_STRATE_LB || type == DRAWTYPE_PREV_STRATE_RB)
	{
		if (type == DRAWTYPE_PREV_STRATE_LB)
			m_hOldLbPen[m_curColor] = (HPEN)SelectObject(m_hMemDC, m_hLbPen[m_curColor]);
		else if(type == DRAWTYPE_PREV_STRATE_RB)
			m_hOldRbPen[m_curColor] = (HPEN)SelectObject(m_hMemDC, m_hRbPen[m_curColor]);

		LineTo(m_hMemDC, posX, posY);
	}
	else
	{
		FillRect(m_hMemDC, &m_rcGameScreen, (HBRUSH)m_hBrush[m_curColor]);
	}
}
void GameScreen::RecvDrawMouseMove(int color, int type, int posX, int posY)
{
	if (m_eCurGameState != GAME_STATE_GAME)
	{
		return;
	}

	if (type == DRAWTYPE_MOVE_LB)
	{
		m_hOldLbPen[color] = (HPEN)SelectObject(m_hMemDC, m_hLbPen[color]);
	}
	else if (type == DRAWTYPE_MOVE_RB)
	{
		m_hOldRbPen[color] = (HPEN)SelectObject(m_hMemDC, m_hRbPen[color]);
	}

	 if (type == DRAWTYPE_MOVE_LB || type == DRAWTYPE_MOVE_RB)
	{
		LineTo(m_hMemDC, posX, posY);
		LineTo(m_hMemDC, posX, posY);
	}
}

void GameScreen::SetNextTurn(string curDrawPlayer, string NextDrawPlayer)
{
	m_curDrawPlayer = curDrawPlayer;
	m_NextDrawPlayer = NextDrawPlayer;
}