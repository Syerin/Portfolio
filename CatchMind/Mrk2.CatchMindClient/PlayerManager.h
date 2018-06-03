#pragma once
#include "defines.h"
#include "Player.h"
class Player;
class PlayerManager
{
private:
	vector<Player*> m_vecPlayer;
public:
	void Init();
	void Init(int Number);
	void Draw(HDC hdc);
	void DrawChat(HDC hdc);
	void SetMyPlayer(int Number,Player* pMyPlayer);

	void SetReady(int Number, bool Ready);
	void SetPlayerState(int Number, PLAYER_STATE PlayerState);
	void Update(float Elapse);
	void RecvChat(int Number, string Chat);
	void allocatePlayer(int Number, string PlayerName,PLAYER_TYPE type, PLAYER_STATE state);

	void SetisPlayer(int Number, bool isPlayer);
	inline bool GetisPlayer(int Number)
	{
		return m_vecPlayer[Number]->GetisPlayer();
	}
	void SetScore(int Number);
	int GetScore(int Number);
	bool GetStartAct();
	inline string GetPlayerName(int Number)
	{
		return m_vecPlayer[Number]->GetPlayerName();
	}
	inline RECT GetPlayerRect(int Number)
	{
		return m_vecPlayer[Number]->GetRect();
	}
	PlayerManager();
	~PlayerManager();
};

