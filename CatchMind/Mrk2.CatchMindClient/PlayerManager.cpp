#include "PlayerManager.h"
#include "ResourceManager.h"
#include "Player.h"

PlayerManager::PlayerManager()
{
}


PlayerManager::~PlayerManager()
{
	for (auto iter = m_vecPlayer.begin(); iter != m_vecPlayer.end(); iter++)
	{
		SAFE_DETE_ARRAY(*iter);
		iter = m_vecPlayer.erase(iter);
	}
	m_vecPlayer.clear();
}

void PlayerManager::Init()
{
	Player* pPlayer = NULL;
	for (int i = 0; i < MAXPLAYER; i++)
	{
		pPlayer = new Player();
		pPlayer->Init(i);
		m_vecPlayer.push_back(pPlayer);
	}
}

void PlayerManager::Draw(HDC hdc)
{
	for (auto iter = m_vecPlayer.begin(); iter != m_vecPlayer.end(); iter++)
	{
		(*iter)->Draw(hdc);
	}
}
void PlayerManager::DrawChat(HDC hdc)
{
	for (auto iter = m_vecPlayer.begin(); iter != m_vecPlayer.end(); iter++)
	{
		(*iter)->DrawChat(hdc);
	}
}
void PlayerManager::SetMyPlayer(int Number,Player* pMyPlayer)
{
	m_vecPlayer[Number] = pMyPlayer;
}
void PlayerManager::SetReady(int Number,bool Ready)
{
	if (Ready == true)
		m_vecPlayer[Number]->SetPlayerState(PLAYER_STATE_READY);
	else
		m_vecPlayer[Number]->SetPlayerState(PLAYER_STATE_WAIT);

}
bool PlayerManager::GetStartAct()
{
	int ReadyCount = 0;
	int CurPlayerCount = 0;
	for (auto iter = m_vecPlayer.begin(); iter != m_vecPlayer.end(); iter++)
	{
		if ((*iter)->GetType() == PLAYER_TYPE_NOMAL &&
			(*iter)->GetPlayerState() == PLAYER_STATE_READY )
		{
			ReadyCount++;
		}
	}
	for (auto iter = m_vecPlayer.begin(); iter != m_vecPlayer.end(); iter++)
	{
		if ((*iter)->GetisPlayer())
		{
			CurPlayerCount++;
		}
	}
	if (ReadyCount == CurPlayerCount-1)
		return true;
	else
		return false;

}
void PlayerManager::SetPlayerState(int Number, PLAYER_STATE PlayerState)
{
	m_vecPlayer[Number]->SetPlayerState(PlayerState);
}
void PlayerManager::allocatePlayer(int Number, string PlayerName,PLAYER_TYPE type,PLAYER_STATE state)
{
	if (0 <= Number && Number < MAXPLAYER)
	{
		m_vecPlayer[Number]->SetPlayerId(PlayerName);
		m_vecPlayer[Number]->SetPlayerNumber(Number);
		m_vecPlayer[Number]->SetPlayer(true);
		m_vecPlayer[Number]->SetPlayerType(type);
		m_vecPlayer[Number]->SetPlayerState(state);
	}
}
void PlayerManager::RecvChat(int Number, string Chat)
{
	m_vecPlayer[Number]->SetChat(Chat);
}
int PlayerManager::GetScore(int Number)
{
	return m_vecPlayer[Number]->GetPlayerScore();
}
void PlayerManager::Update(float Elapse)
{
	for (auto iter = m_vecPlayer.begin(); iter != m_vecPlayer.end(); iter++)
	{
		(*iter)->Update(Elapse);
	}
}

void PlayerManager::SetScore(int Number)
{
	m_vecPlayer[Number]->SetScore();
}

void PlayerManager::Init(int Number)
{
	m_vecPlayer[Number]->InitPlayer();
}
void PlayerManager::SetisPlayer(int Number, bool isPlayer)
{
	m_vecPlayer[Number]->SetPlayer(isPlayer);
}