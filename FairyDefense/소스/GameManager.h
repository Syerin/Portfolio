#pragma once
#include "defines.h"
#include "cocos2d.h"
USING_NS_CC;
class CallLabel;
class GameManager
{
private:
	int m_iCoreLife;
	int m_iMoney;
	int m_iStage;
	Layer* m_pLayerMainScene;
	Sprite* m_pSpriteBoard;
	CallLabel* m_pCallLabelMoney;
	CallLabel* m_pCallLabelCore;
	CallLabel* m_pCallLabelWave;
	Menu* m_pMenuSpeedup;
	Menu* m_pOptionMenu;
	MoveBy* m_pOptionMenuMoveBy1;
	Sequence* m_pSequenceOptionMenu2;

	MoveBy* m_pOptionMenuMoveBy1_reverse;
	Sequence* m_pSequenceOptionMenu2_reverse;

	bool m_bOptionMenuState;

	MenuItemSprite* m_pSpriteResetStageItem;
	MenuItemSprite* m_pSpriteLoadTitleSceneItem;

	Sprite* m_pSprStar[3];
	Sprite* m_pSprStarOutline[3];

	Sprite* m_pSprGameClear;
	Sprite* m_pSprGameOver[8];

	Sprite* m_pSprGoldString;
	Sprite* m_pSprStarString;

	Sprite* m_pSprBounus;

	MenuItemSprite* m_pSprItemRetry;
	MenuItemSprite* m_pSprItemEnd; 
	Menu* m_pMenuGameover;

	Sequence* m_pSeqGameOver[8];

	Sequence* m_pSeqScaleTo[3];
	int m_iClearStar;
	int m_iWaveMax;
	int m_icurWave;
	int m_iCountDieMob;
	int m_iStageClearMoney;
	GAME_STATE m_eGameState;
	CallLabel* m_pCallLabelClearMoney;

	CallLabel* m_pCallLabelClearPlus;
	CallLabel* m_pCallLabelClearBonus;

	
	CallLabel* m_pCallLabelGameStart;
	CallLabel* m_pCallLabelGameWaveUp;
	bool m_bGameStartVisible;
	bool m_bGameWaveUpVisible;
	float m_fGameStart;
	float m_fGameWaveUp;

	bool m_bSpeedUp;

	void UpSpeedChange();
	void OptionMenu();

	void VisibleGameOver();

	void LoadTitleScene();
	void ResetStage();
	GameManager();
	static GameManager* m_pThis;
public:
	void Init(Layer* pLayerMainScene,int stage);
	void Update(float fElapse);

	void CoreLifeMinus(int value);
	void PlusAndMinusMoney(int PlusAndMinusValue);


	void SetCurWave(int curWave);

	void CountDieMob();


	int GetMoney();
	bool GameOver();

	bool GetSpeedButtonState();

	GAME_STATE GetGameState();

	void GameStartVisible(bool visible);
	void GameWaveUpVisible(bool visible);

	void Release();

	Sprite* GetSpriteWithFrameFileName(char* szFileName);

	inline bool GetGameWaveUpVisible()
	{
		return m_bGameWaveUpVisible;
	}
	inline int GetWaveMax()
	{
		return m_iWaveMax;
	}
	static GameManager* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new GameManager();
		}
		return m_pThis;
	}
	~GameManager();
};

