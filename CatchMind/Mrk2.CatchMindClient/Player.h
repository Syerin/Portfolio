#pragma once
#include "defines.h"
#include "../Mrk2.CatchMindServer/defines.h"
class Bitmap;
class Player
{
private:
	enum FRAME_TYPE
	{
		FRAME_TYPE_PINK,
		FRAME_TYPE_ORANGE,
		FRAME_TYPE_END,
	};
	int m_iPlayerNumber;
	int m_iCharacterImage;
	int m_iLevel;
	bool isPlayer;
	bool isDraw;

	int m_iAnswerCount;
	string m_strAnserCount;

	PLAYER_TYPE m_ePlayerType;
	PLAYER_STATE m_eCurPlayerState;
	Bitmap* m_pBitmap[PLAYER_TYPE_END];
	Bitmap* m_pBitmapFrame[FRAME_TYPE_END];
	Bitmap* m_pBitmapCharacter[CHARACTER_TYPE_END];
	Bitmap* m_pBitmapChat;

	string m_strChat[CHAT_LINE_COUNT];
	RECT m_rcChat[CHAT_LINE_COUNT];
	bool m_bDrawChat;
	string m_strPlayerName;
	RECT m_rcName;
	RECT m_rcRect;
	RECT m_rcScore;

	string m_strAnswer;

	float m_fUITimer;
	float m_fChatTimer;

	string m_strCopyChat;

	Bitmap* m_pBitmap_Answer;
	bool m_bAnswer;
	float m_fAnswer;
public:
	void Init(int Number);
	void InitPlayer();
	void SetPlayerId(string strPlayerName);
	void SetPlayerNumber(int Number);
	void SetPlayerCharacter(int CharacterType);
	void SetPlayerType(int PlayerType);
	void SetScore();

	void Draw(HDC hdc);
	void DrawChat(HDC hdc);
	void DrawScore(HDC hdc);


	void Update(float Elapse);

	void SetChat(string chat);
	int GetPlayerScore();

	inline string GetPlayerName()
	{
		return m_strPlayerName;
	}
	inline bool GetisPlayer()
	{
		return isPlayer;
	}
	inline void SetPlayer(bool act)
	{
		isPlayer = act;
	}
	inline PLAYER_TYPE GetType()
	{
		return m_ePlayerType;
	}
	inline RECT GetRect()
	{
		return m_rcRect;
	}
	inline int GetPlayerNumber()
	{
		return m_iPlayerNumber;
	}
	inline void SetPlayerState(PLAYER_STATE PlayerState)
	{
		m_eCurPlayerState = PlayerState;
	}
	inline PLAYER_STATE GetPlayerState()
	{
		return m_eCurPlayerState;
	}
	Player();
	~Player();
};

