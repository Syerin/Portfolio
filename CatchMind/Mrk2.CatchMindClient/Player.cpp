#include "Player.h"
#include "ResourceManager.h"
#include "Bitmap.h"

Player::Player()
{
	isPlayer = false;
	isDraw = false;
	m_bDrawChat = false;
	m_bAnswer = false;
	m_iPlayerNumber = 9;
	m_iCharacterImage = 1;
	m_iLevel = 0;
	m_ePlayerType = PLAYER_TYPE_NOMAL;
	m_eCurPlayerState = PLAYER_STATE_WAIT;
	m_fChatTimer = 0.0f;
	m_fUITimer = 0.0f;
	m_iAnswerCount = 0;
	m_strAnserCount = "0개";
	m_fAnswer = 0.0f;

	m_strAnswer = "정답";
}


Player::~Player()
{
}


void Player::Init(int Number)
{
	m_iPlayerNumber = Number;
	
	m_pBitmap[PLAYER_TYPE_NOMAL] = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_NAMEBLOCK);
	m_pBitmap[PLAYER_TYPE_HOST] = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_NAMEBLOCK_HOST);


	m_pBitmapFrame[FRAME_TYPE_PINK] = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_NAMEBLOCK_FRAME_DRAW);
	m_pBitmapFrame[FRAME_TYPE_ORANGE] = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_NAMEBLOCK_FRAME_READY);
	
	if (m_iPlayerNumber < (MAXPLAYER / 2))
	{
		m_rcName.left = PLAYER_LEFT_NAME_POS_X;
		m_rcName.top = (PLAYER_NAMEBLOCK_OFFSET_Y*m_iPlayerNumber) + PLAYER_LEFT_NAME_POS_Y;
		m_rcName.right = m_rcName.left + PLAYER_NAME_WIDTH;
		m_rcName.bottom = m_rcName.top + PLAYER_NAME_HEIGHT;

		m_rcScore.left = m_rcName.left;
		m_rcScore.top = (PLAYER_NAMEBLOCK_OFFSET_Y*m_iPlayerNumber) + PLAYER_LEFT_NAME_POS_Y + PLAYER_SCORE_HEIGHT;
		m_rcScore.right = m_rcScore.left + PLAYER_SCORE_WIDTH;
		m_rcScore.bottom = m_rcScore.top + PLAYER_SCORE_HEIGHT;

		m_rcRect.left = PLAYER_LEFT_NAMEBLOCK;
		m_rcRect.top = (PLAYER_NAMEBLOCK_OFFSET_Y * m_iPlayerNumber) + PLAYER_NAMEBLOCK_ORIGIN_Y;
		m_rcRect.right = m_rcRect.left + m_pBitmap[PLAYER_TYPE_NOMAL]->GetSizeBitmap().cx;
		m_rcRect.bottom = m_rcRect.top+ m_pBitmap[PLAYER_TYPE_NOMAL]->GetSizeBitmap().cy;

		for (int i = 0; i < CHAT_LINE_COUNT; i++)
		{
			m_rcChat[i].left = m_rcRect.left + ResourceManager::GetInstance()->GetBitmap(RES_TYPE_NAMEBLOCK)->GetSizeBitmap().cx+ OFFSET_CHAT;
			m_rcChat[i].top = m_rcRect.top + (i*CHAT_HEIGHT);
			m_rcChat[i].right = m_rcChat[i].left + CHAT_WIDTH - OFFSET_CHAT;
			m_rcChat[i].bottom = m_rcChat[i].top + (i*CHAT_HEIGHT);
		}
	}
	else if (m_iPlayerNumber >= (MAXPLAYER / 2))
	{
		m_rcName.left = PLAYER_RIGHT_NAME_POS_X;
		m_rcName.top = (PLAYER_NAMEBLOCK_OFFSET_Y*(m_iPlayerNumber - (MAXPLAYER / 2)) + PLAYER_RIGHT_NAME_POS_Y);
		m_rcName.right = m_rcName.left + PLAYER_NAME_WIDTH;
		m_rcName.bottom = m_rcName.top + 30;


		m_rcScore.left = m_rcName.left;
		m_rcScore.top = (PLAYER_NAMEBLOCK_OFFSET_Y*(m_iPlayerNumber - (MAXPLAYER / 2)) + PLAYER_RIGHT_NAME_POS_Y) + PLAYER_SCORE_HEIGHT;
		m_rcScore.right = m_rcScore.left + PLAYER_SCORE_WIDTH;
		m_rcScore.bottom = m_rcScore.top + PLAYER_SCORE_HEIGHT;


		m_rcRect.left = PLAYER_RIGHT_NAMEBLOCK;
		m_rcRect.top = (PLAYER_NAMEBLOCK_OFFSET_Y * (m_iPlayerNumber - (MAXPLAYER / 2))) + PLAYER_NAMEBLOCK_ORIGIN_Y;
		m_rcRect.right = m_rcRect.left + m_pBitmap[PLAYER_TYPE_NOMAL]->GetSizeBitmap().cx;
		m_rcRect.bottom = m_rcRect.top + m_pBitmap[PLAYER_TYPE_NOMAL]->GetSizeBitmap().cy;
		for (int i = 0; i < CHAT_LINE_COUNT; i++)
		{
			m_rcChat[i].left = m_rcRect.left -OFFSET_CHAT*(CHAT_LINEMAX+5);
			m_rcChat[i].top = m_rcRect.top+ (i*CHAT_HEIGHT);
			m_rcChat[i].right = m_rcChat[i].left + CHAT_WIDTH;
			m_rcChat[i].bottom = m_rcChat[i].top + ((i+1)*CHAT_HEIGHT);
		}
	}
	for (int i = 0; i < CHARACTER_TYPE_END; i++)
	{
		m_pBitmapCharacter[i] = ResourceManager::GetInstance()->GetBitmapCharacter(i);
	}
	m_pBitmapChat = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_CHAT);

	m_pBitmap_Answer = ResourceManager::GetInstance()->GetBitmap(RES_TYPE_STAR);

}

void Player::Update(float Elapse)
{
	if (m_bDrawChat)
	{
		m_fChatTimer += Elapse;
		if (m_fChatTimer >= 2.0f)
		{
			m_fChatTimer = 0.0f;
			m_bDrawChat = false;
		}
	}
	if (m_bAnswer)
	{
		m_fAnswer += Elapse;
		if (m_fAnswer > 2.0f)
		{
			m_fAnswer = 0.0f;
			m_bAnswer = false;
		}
	}
}
void Player::SetPlayerId(string strPlayerName)
{
	m_strPlayerName = strPlayerName;
}
void Player::SetPlayerNumber(int Number)
{
	m_iPlayerNumber = Number;
}

void Player::SetPlayerCharacter(int CharacterType)
{
	m_iCharacterImage = CharacterType;
}
void Player::SetPlayerType(int PlayerType)
{
	m_ePlayerType = (PLAYER_TYPE)PlayerType;
}
void Player::Draw(HDC hdc)
{
	if (isPlayer)
	{
		if (m_iPlayerNumber < (MAXPLAYER / 2))
		{
			m_pBitmap[m_ePlayerType]->DrawBitblt(hdc, PLAYER_LEFT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * m_iPlayerNumber) + PLAYER_NAMEBLOCK_ORIGIN_Y);
		}
		else if (m_iPlayerNumber >= (MAXPLAYER / 2))
		{
			m_pBitmap[m_ePlayerType]->DrawBitblt(hdc, PLAYER_RIGHT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * (m_iPlayerNumber - (MAXPLAYER / 2))) + PLAYER_NAMEBLOCK_ORIGIN_Y);
		}
		if (m_eCurPlayerState == PLAYER_STATE_WAIT)
		{
			if (m_iPlayerNumber < (MAXPLAYER / 2))
			{
				m_pBitmap[m_ePlayerType]->DrawBitblt(hdc, PLAYER_LEFT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * m_iPlayerNumber) + PLAYER_NAMEBLOCK_ORIGIN_Y);
			}
			else if (m_iPlayerNumber >= (MAXPLAYER / 2))
			{
				m_pBitmap[m_ePlayerType]->DrawBitblt(hdc, PLAYER_RIGHT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * (m_iPlayerNumber - (MAXPLAYER / 2))) + PLAYER_NAMEBLOCK_ORIGIN_Y);
			}
		}
		if (m_eCurPlayerState == PLAYER_STATE_DRAW || m_eCurPlayerState == PLAYER_STATE_ANSWER)
		{
			if (m_iPlayerNumber < (MAXPLAYER / 2))
			{
				m_pBitmapFrame[FRAME_TYPE_PINK]->DrawTransparent(hdc, PLAYER_LEFT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * m_iPlayerNumber) + PLAYER_NAMEBLOCK_ORIGIN_Y);
				if (m_eCurPlayerState == PLAYER_STATE_ANSWER && m_bAnswer)
				{
					m_pBitmap_Answer->DrawMidPoint(hdc, PLAYER_LEFT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * m_iPlayerNumber) + PLAYER_NAMEBLOCK_ORIGIN_Y);
				}
			}
			else if (m_iPlayerNumber >= (MAXPLAYER / 2))
			{
				m_pBitmapFrame[FRAME_TYPE_PINK]->DrawTransparent(hdc, PLAYER_RIGHT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * (m_iPlayerNumber - (MAXPLAYER / 2))) + PLAYER_NAMEBLOCK_ORIGIN_Y);
				if (m_eCurPlayerState == PLAYER_STATE_ANSWER && m_bAnswer)
				{
					m_pBitmap_Answer->DrawMidPoint(hdc, PLAYER_RIGHT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * (m_iPlayerNumber - (MAXPLAYER / 2))) + PLAYER_NAMEBLOCK_ORIGIN_Y);
				}

			}
		}
		else if (m_eCurPlayerState == PLAYER_STATE_READY)
		{
			if (m_iPlayerNumber < (MAXPLAYER / 2))
			{
				m_pBitmapFrame[FRAME_TYPE_ORANGE]->DrawTransparent(hdc, PLAYER_LEFT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * m_iPlayerNumber) + PLAYER_NAMEBLOCK_ORIGIN_Y);
			}
			else if (m_iPlayerNumber >= (MAXPLAYER / 2))
			{
				m_pBitmapFrame[FRAME_TYPE_ORANGE]->DrawTransparent(hdc, PLAYER_RIGHT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * (m_iPlayerNumber - (MAXPLAYER / 2))) + PLAYER_NAMEBLOCK_ORIGIN_Y);
			}
		}
		if (m_iPlayerNumber < (MAXPLAYER / 2))
		{
			m_pBitmapCharacter[m_iCharacterImage]->DrawTransparent(hdc, PLAYER_LEFT_NAMEBLOCK + PLAYER_IMAGE_POS_X,
				(PLAYER_NAMEBLOCK_OFFSET_Y * m_iPlayerNumber) + PLAYER_NAMEBLOCK_ORIGIN_Y + PLAYER_IMAGE_POS_Y);
		}
		else if (m_iPlayerNumber >= (MAXPLAYER / 2))
		{
			m_pBitmapCharacter[m_iCharacterImage]->DrawTransparent(hdc, PLAYER_RIGHT_NAMEBLOCK + PLAYER_IMAGE_POS_X,
				(PLAYER_NAMEBLOCK_OFFSET_Y * (m_iPlayerNumber - (MAXPLAYER / 2))) + PLAYER_NAMEBLOCK_ORIGIN_Y + PLAYER_IMAGE_POS_Y);
		}
		DrawText(hdc, m_strPlayerName.c_str(), -1, &m_rcName, DT_CENTER | DT_NOCLIP);
		DrawScore(hdc);
	}
	else
	{
		if (m_iPlayerNumber < (MAXPLAYER / 2))
		{
			m_pBitmap[PLAYER_TYPE_NOMAL]->DrawBitblt(hdc, PLAYER_LEFT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * m_iPlayerNumber) + PLAYER_NAMEBLOCK_ORIGIN_Y);
		}
		else if (m_iPlayerNumber >= (MAXPLAYER / 2))
		{
			m_pBitmap[PLAYER_TYPE_NOMAL]->DrawBitblt(hdc, PLAYER_RIGHT_NAMEBLOCK, (PLAYER_NAMEBLOCK_OFFSET_Y * (m_iPlayerNumber - (MAXPLAYER / 2))) + PLAYER_NAMEBLOCK_ORIGIN_Y);
		}
	}
}

void Player::SetChat(string chat)
{
	m_bDrawChat = true;
	m_fChatTimer = 0.0f;
	m_strCopyChat = chat;
	char buf[BUFSIZE];
	strcpy(buf, chat.c_str());

	int bufchat = 0;
	for (int i = 1; i < strlen(chat.c_str())/ CHAT_LINEMAX+1; i++)
	{
		bufchat = buf[i * CHAT_LINEMAX];
		for (int j = 0; j < 127; j++)
		{
			if (bufchat == j)
			{
				bufchat = 32;
				char chSpace[] = " ";
				m_strCopyChat.insert(i*CHAT_LINEMAX, chSpace);
			}
		}

	}
}
void Player::DrawChat(HDC hdc)
{
	if (m_bDrawChat)
	{
		SIZE size;

		int Text_Y = 0;
		int Text_X = 0;

		int length = 0;

		length = strlen(m_strCopyChat.c_str()) + 1;

		if (length > CHAT_LINEMAX * 2)
		{
			Text_Y = CHAT_SIZE_HEIGHT * 3;
		}
		else if (length > CHAT_LINEMAX)
		{
			Text_Y = CHAT_SIZE_HEIGHT * 2;
		}
		else
		{
			Text_Y = CHAT_SIZE_HEIGHT;
		}

		int Width = 0;
		if (strlen(m_strCopyChat.c_str()) > CHAT_LINEMAX)
			Width = CHAT_LINEMAX;
		else
			Width = strlen(m_strCopyChat.c_str());
		RECT rcTemp;

		rcTemp.left = m_rcRect.left - Width*OFFSET_CHAT;
		rcTemp.top = m_rcChat[0].top+5;
		rcTemp.right = rcTemp.left + Width*OFFSET_CHAT;
		rcTemp.bottom = rcTemp.top + 5;

		if (m_iPlayerNumber < (MAXPLAYER / 2))
		{

				m_pBitmapChat->DrawTransparent(hdc, m_rcRect.right, m_rcChat[0].top-5, Width*8, Text_Y);
				DrawText(hdc, m_strCopyChat.c_str(), -1, &m_rcChat[0], DT_WORDBREAK | DT_LEFT | DT_NOCLIP);	//DT_WORDBREAK;<--영어 출력시 스페이스바를 안누르면 오류남.

		}
		else if (m_iPlayerNumber >= (MAXPLAYER / 2))
		{
			m_pBitmapChat->DrawRTpoint(hdc, m_rcRect.left, m_rcRect.top, Width * 10, Text_Y);
			DrawText(hdc, m_strCopyChat.c_str(), -1, &rcTemp, DT_WORDBREAK | DT_LEFT | DT_NOCLIP);	//DT_WORDBREAK;<--영어 출력시 스페이스바를 안누르면 오류남.
		}
	}
}

void Player::DrawScore(HDC hdc)
{
	if (m_iPlayerNumber < (MAXPLAYER / 2))
	{
		TextOut(hdc, PLAYER_LEFT_NAME_POS_X-10, (PLAYER_NAMEBLOCK_OFFSET_Y*m_iPlayerNumber) + PLAYER_LEFT_NAME_POS_Y+20, m_strAnswer.c_str(), m_strAnswer.size());
	}
	else if (m_iPlayerNumber >= (MAXPLAYER / 2))
	{
		TextOut(hdc, PLAYER_RIGHT_NAME_POS_X-10, (PLAYER_NAMEBLOCK_OFFSET_Y*(m_iPlayerNumber - (MAXPLAYER / 2)) + PLAYER_RIGHT_NAME_POS_Y) + 20, m_strAnswer.c_str(), m_strAnswer.size());
	}
	DrawText(hdc, m_strAnserCount.c_str(), -1, &m_rcScore, DT_CENTER|DT_NOCLIP);
}
void Player::SetScore()
{
	m_iAnswerCount++;
	m_bAnswer = true;
	m_strAnserCount = to_string(m_iAnswerCount) + "개";
}
int Player::GetPlayerScore()
{
		return m_iAnswerCount;
}

void Player::InitPlayer()
{
	isDraw = false;
	m_bDrawChat = false;
	m_bAnswer = false;
	m_eCurPlayerState = PLAYER_STATE_WAIT;
	m_fChatTimer = 0.0f;
	m_fUITimer = 0.0f;
	m_iAnswerCount = 0;
	m_strAnserCount = "0개";
	m_fAnswer = 0.0f;
}