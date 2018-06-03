#include "GameManager.h"
#include "CallLabel.h"
#include "CallLabelManager.h"
#include "Title.h"
#include "Stage1.h"
#include "StageInfo.h"
#include "PlayerInfo.h"
#include "CharacterInfo.h"
#include "OffenseManager.h"
GameManager* GameManager::m_pThis = NULL;

GameManager::GameManager()
{
	m_pOptionMenuMoveBy1 =NULL;
	m_pSequenceOptionMenu2 = NULL;
	m_pOptionMenuMoveBy1_reverse = NULL;
	m_pSequenceOptionMenu2_reverse = NULL;
	for (int i = 0; i < 3; i++)
	{
		m_pSeqScaleTo[i] = NULL;
	}

}


GameManager::~GameManager()
{
	CC_SAFE_RELEASE(m_pOptionMenuMoveBy1);
	CC_SAFE_RELEASE(m_pSequenceOptionMenu2);
	CC_SAFE_RELEASE(m_pOptionMenuMoveBy1_reverse);
	CC_SAFE_RELEASE(m_pSequenceOptionMenu2_reverse);

	for (int i = 0; i < 3; i++)
	{
		CC_SAFE_RELEASE(m_pSeqScaleTo[i]);
	}
}
GAME_STATE GameManager::GetGameState()
{
	return m_eGameState;
}
void GameManager::Release()
{
	m_eGameState = GAME_STATE_GAMEOVER;
	CC_SAFE_RELEASE(m_pOptionMenuMoveBy1);
	CC_SAFE_RELEASE(m_pSequenceOptionMenu2);
	CC_SAFE_RELEASE(m_pOptionMenuMoveBy1_reverse);
	CC_SAFE_RELEASE(m_pSequenceOptionMenu2_reverse);

	for (int i = 0; i < 3; i++)
	{
		CC_SAFE_RELEASE(m_pSeqScaleTo[i]);
	}
	for (int i = 0; i < 8; i++)
	{
		CC_SAFE_RELEASE(m_pSeqGameOver[i]);
	}
}
void GameManager::UpSpeedChange()
{
	m_bSpeedUp = !m_bSpeedUp;
}
bool GameManager::GetSpeedButtonState()
{
	return m_bSpeedUp;
}
void GameManager::Init(Layer* pLayerMainScene,int stage)
{
	m_pLayerMainScene = pLayerMainScene;
	m_iStage = stage;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_pSpriteBoard = GameManager::GetInstance()->GetSpriteWithFrameFileName("Board.png");
	m_pSpriteBoard->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	m_pSpriteBoard->setPosition(Vec2(visibleSize.width, visibleSize.height - (m_pSpriteBoard->getBoundingBox().getMaxY())));
	m_pLayerMainScene->addChild(m_pSpriteBoard, 100);
	m_pSpriteBoard->setVisible(true);
	m_bSpeedUp = false;

	Sprite* pSpriteNomalSpeed = GameManager::GetInstance()->GetSpriteWithFrameFileName("SpeedNormal.png");
	Sprite* pSpriteNomalSpeedDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("SpeedNormal.png");
	pSpriteNomalSpeedDown->setColor(Color3B(99, 99, 99));
	Sprite* pSpriteDoubleSpeed = GameManager::GetInstance()->GetSpriteWithFrameFileName("SpeedDouble.png");
	Sprite* pSpriteDoubleSpeedDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("SpeedDouble.png");
	pSpriteDoubleSpeedDown->setColor(Color3B(99, 99, 99));

	MenuItemSprite* pSpriteSpeedNormal = MenuItemSprite::create(pSpriteNomalSpeed, pSpriteNomalSpeedDown, NULL);
	MenuItemSprite* pSpriteSpeedDouble = MenuItemSprite::create(pSpriteDoubleSpeed, pSpriteDoubleSpeedDown,NULL);

	MenuItemToggle* pToggleItem = MenuItemToggle::createWithCallback(CC_CALLBACK_0(GameManager::UpSpeedChange, this), pSpriteSpeedNormal, pSpriteSpeedDouble, NULL);
	pToggleItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	Sprite* pSpriteOption = GameManager::GetInstance()->GetSpriteWithFrameFileName("Option.png");
	Sprite* pSpriteOptionDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("Option.png");

	BlendFunc blendFunc1 = { GL_SRC_COLOR,GL_ONE };
	pSpriteOptionDown->setBlendFunc(blendFunc1);



	MenuItemSprite* pSpriteOptionItem = MenuItemSprite::create(pSpriteOption, pSpriteOptionDown, CC_CALLBACK_0(GameManager::OptionMenu, this));
	pSpriteOptionItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);



	m_pMenuSpeedup = Menu::create(pToggleItem, pSpriteOptionItem, NULL);
	float pToggleItemWidth = (pToggleItem->getBoundingBox().getMaxX() - pToggleItem->getBoundingBox().getMinX());
	m_pMenuSpeedup->setPosition(pToggleItemWidth, visibleSize.height);
	m_pMenuSpeedup->alignItemsHorizontallyWithPadding(pToggleItemWidth / 2);
	m_pLayerMainScene->addChild(m_pMenuSpeedup, 110);


	Sprite* pSpriteLoadTitleSceneMenu = GameManager::GetInstance()->GetSpriteWithFrameFileName("LoadTitleSceneMenu.png");
	Sprite* pSpriteLoadTitleSceneMenuDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("LoadTitleSceneMenu.png");
	pSpriteLoadTitleSceneMenuDown->setColor(Color3B(99, 99, 99));

	m_pSpriteLoadTitleSceneItem = MenuItemSprite::create(pSpriteLoadTitleSceneMenu, pSpriteLoadTitleSceneMenuDown, CC_CALLBACK_0(GameManager::LoadTitleScene, this));
	m_pSpriteLoadTitleSceneItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	Sprite* pSpriteResetStage = GameManager::GetInstance()->GetSpriteWithFrameFileName("ResetStageMenu.png");
	Sprite* pSpriteResetStageDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("ResetStageMenu.png");
	pSpriteResetStageDown->setColor(Color3B(99, 99, 99));
	m_pSpriteResetStageItem = MenuItemSprite::create(pSpriteResetStage, pSpriteResetStageDown, CC_CALLBACK_0(GameManager::ResetStage, this));
	m_pSpriteResetStageItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	m_pOptionMenu = Menu::create(m_pSpriteLoadTitleSceneItem, m_pSpriteResetStageItem, NULL);
	m_pOptionMenu->setPosition(-(pSpriteResetStage->getBoundingBox().getMidX() * 2), visibleSize.height - 250);
	m_pOptionMenu->alignItemsVerticallyWithPadding(40);
	m_pLayerMainScene->addChild(m_pOptionMenu, 110);

	m_pCallLabelMoney = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelMoney->SetColor(Color3B::WHITE);
	m_pCallLabelMoney->SetEnable(true); 
	m_pCallLabelMoney->GetLabel()->setVisible(true);
	m_pCallLabelMoney->GetLabel()->setSystemFontSize(60);
	m_pCallLabelMoney->GetLabel()->setPosition(m_pSpriteBoard->getBoundingBox().getMaxX() - m_pCallLabelMoney->GetLabel()->getSystemFontSize(),
		m_pSpriteBoard->getBoundingBox().getMidY());

	m_pCallLabelCore = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelCore->SetColor(Color3B::WHITE);
	m_pCallLabelCore->SetEnable(true);
	m_pCallLabelCore->GetLabel()->setVisible(true);
	m_pCallLabelCore->GetLabel()->setSystemFontSize(60);
	m_pCallLabelCore->GetLabel()->setPosition(m_pSpriteBoard->getBoundingBox().getMidX() + m_pCallLabelCore->GetLabel()->getSystemFontSize() + 20, m_pSpriteBoard->getBoundingBox().getMidY());

	m_pCallLabelWave = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelWave->SetEnable(true);
	m_pCallLabelWave->GetLabel()->setColor(Color3B::WHITE);
	m_pCallLabelWave->GetLabel()->setSystemFontSize(60);
	m_pCallLabelWave->GetLabel()->setVisible(true);
	m_pCallLabelWave->GetLabel()->setPosition(m_pSpriteBoard->getBoundingBox().getMinX() + 195, m_pSpriteBoard->getBoundingBox().getMidY());


	m_pOptionMenuMoveBy1 = MoveBy::create(0.5f, Vec2((pSpriteResetStage->getBoundingBox().getMidX() * 2), 0));
	m_pOptionMenuMoveBy1->retain();
	DelayTime* pDelayTimeMove = DelayTime::create(0.25f);
	MoveBy* pOptionMenuMoveBy1 = MoveBy::create(0.5f, Vec2((pSpriteResetStage->getBoundingBox().getMidX() * 2), 0));

	m_pSequenceOptionMenu2 = Sequence::create(pDelayTimeMove, pOptionMenuMoveBy1, NULL);
	m_pSequenceOptionMenu2->retain();


	m_pOptionMenuMoveBy1_reverse = MoveBy::create(0.5f, Vec2(-(pSpriteResetStage->getBoundingBox().getMidX() * 2), 0));
	m_pOptionMenuMoveBy1_reverse->retain();

	MoveBy* pOptionMenuMoveBy3 = MoveBy::create(0.5f, -Vec2((pSpriteResetStage->getBoundingBox().getMidX() * 2), 0));
	m_pSequenceOptionMenu2_reverse = Sequence::create(pDelayTimeMove, pOptionMenuMoveBy3, NULL);
	m_pSequenceOptionMenu2_reverse->retain();


	Vec2 vec2CenterPos = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	for (int i = 0; i < 3; i++)
	{
		m_pSprStar[i] = GameManager::GetInstance()->GetSpriteWithFrameFileName("Star.png");
		m_pSprStarOutline[i] = GameManager::GetInstance()->GetSpriteWithFrameFileName("StarOutLine.png");

		m_pLayerMainScene->addChild(m_pSprStarOutline[i], 998);
		m_pSprStarOutline[i]->addChild(m_pSprStar[i],999);

		int Offset = m_pSprStar[i]->getContentSize().width;

		if (i > 0)
		{
			Vec2 vec2PrevOutlieStarPos = m_pSprStarOutline[i - 1]->getPosition();
			m_pSprStarOutline[i]->setPosition(vec2PrevOutlieStarPos.x + Offset, vec2PrevOutlieStarPos.y);

			Vec2 vec2CurOutlieStarPos = m_pSprStarOutline[i]->getContentSize() / 2;
			m_pSprStar[i]->setPosition(vec2CurOutlieStarPos);
		}
		else
		{
			m_pSprStarOutline[i]->setPosition(vec2CenterPos.x + Offset, vec2CenterPos.y);

			Vec2 vec2PrevOutlieStarPos = m_pSprStarOutline[i]->getContentSize() / 2;
			m_pSprStar[i]->setPosition(vec2PrevOutlieStarPos);
		}
		m_pSprStar[i]->setOpacity(0);
		m_pSprStarOutline[i]->setVisible(false);
		ScaleTo* pScaleToOrigin = ScaleTo::create(0.0f, 1.5f);
		ScaleTo* pScaleTo = ScaleTo::create(1.0f, 1.0f);
		DelayTime* pDelTime = DelayTime::create(i*1.0f);
		m_pSeqScaleTo[i] = Sequence::create(pScaleToOrigin, pDelTime, pScaleTo, NULL);
		m_pSeqScaleTo[i]->retain();

	}

	m_pCallLabelGameStart = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelGameStart->SetEnable(true);

	Label* pLabel = m_pCallLabelGameStart->GetLabel();
	pLabel->setColor(Color3B::WHITE);
	pLabel->enableOutline(Color4B::YELLOW, 3);
	pLabel->enableGlow(Color4B::YELLOW);
	pLabel->setString("Game Start");
	pLabel->setLocalZOrder(300);
	pLabel->setSystemFontSize(150);
	pLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);


	m_pCallLabelGameWaveUp = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelGameWaveUp->SetEnable(true);


	m_pSprGameClear = GameManager::GetInstance()->GetSpriteWithFrameFileName("GameClear.png");
	m_pSprGameClear->setPosition(vec2CenterPos.x, vec2CenterPos.y + 200);
	m_pSprGameClear->setVisible(false);
	m_pLayerMainScene->addChild(m_pSprGameClear, 999);





	m_pSprStarString = GameManager::GetInstance()->GetSpriteWithFrameFileName("StarString.png");
	m_pSprStarString->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pSprStarString->setPosition(m_pSprGameClear->getBoundingBox().getMinX(), m_pSprStarOutline[0]->getBoundingBox().getMidY());
	m_pSprStarString->setVisible(false);
	m_pLayerMainScene->addChild(m_pSprStarString, 999);


	m_pSprGoldString = GameManager::GetInstance()->GetSpriteWithFrameFileName("GoldString.png");
	m_pSprGoldString->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	m_pSprGoldString->setPosition(m_pSprGameClear->getBoundingBox().getMinX(), m_pSprStarOutline[0]->getBoundingBox().getMidY() - m_pSprStarString->getContentSize().height*1.5);
	m_pSprGoldString->setVisible(false);
	m_pLayerMainScene->addChild(m_pSprGoldString, 999);


	Sprite* pSpriteRetry = GameManager::GetInstance()->GetSpriteWithFrameFileName("RetryButton.png");
	pSpriteRetry->setPosition(0, 0);
	Sprite* pSpriteRetryDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("RetryButton.png");
	pSpriteRetryDown->setColor(Color3B(99, 99, 99));

	Sprite* pSpriteEnd = GameManager::GetInstance()->GetSpriteWithFrameFileName("EndButton.png");
	pSpriteEnd->setPosition(0, 0);
	Sprite* pSpriteEndDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("EndButton.png");
	pSpriteEndDown->setColor(Color3B(99, 99, 99));

	m_pSprItemRetry = MenuItemSprite::create(pSpriteRetry, pSpriteRetryDown, pSpriteRetry, CC_CALLBACK_0(GameManager::ResetStage, this));
	m_pSprItemEnd = MenuItemSprite::create(pSpriteEnd, pSpriteEndDown, pSpriteEnd, CC_CALLBACK_0(GameManager::LoadTitleScene, this));

	m_pMenuGameover = Menu::create(m_pSprItemRetry, m_pSprItemEnd, NULL);
	m_pMenuGameover->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	m_pMenuGameover->alignItemsHorizontally();
	Vec2 vec2MenuGameOver = Vec2(m_pSprStarOutline[2]->getBoundingBox().getMinX(), m_pSprGoldString->getBoundingBox().getMinY() - m_pSprItemRetry->getContentSize().height / 2);
	m_pMenuGameover->setPosition(vec2MenuGameOver);
	m_pMenuGameover->setVisible(false);
	m_pLayerMainScene->addChild(m_pMenuGameover, 999);



	m_pSprGameOver[0] = GameManager::GetInstance()->GetSpriteWithFrameFileName("G.png");
	m_pSprGameOver[1] = GameManager::GetInstance()->GetSpriteWithFrameFileName("A.png");
	m_pSprGameOver[2] = GameManager::GetInstance()->GetSpriteWithFrameFileName("M.png");
	m_pSprGameOver[3] = GameManager::GetInstance()->GetSpriteWithFrameFileName("E.png");
	m_pSprGameOver[4] = GameManager::GetInstance()->GetSpriteWithFrameFileName("O.png");
	m_pSprGameOver[5] = GameManager::GetInstance()->GetSpriteWithFrameFileName("V.png");
	m_pSprGameOver[6] = GameManager::GetInstance()->GetSpriteWithFrameFileName("E.png");
	m_pSprGameOver[7] = GameManager::GetInstance()->GetSpriteWithFrameFileName("R.png");

	m_pCallLabelClearPlus = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelClearPlus->SetEnable(true);
	m_pCallLabelClearPlus->GetLabel()->setString("+");
	m_pCallLabelClearPlus->GetLabel()->setColor(Color3B::WHITE);

	m_pSprBounus = GameManager::GetInstance()->GetSpriteWithFrameFileName("Bonus.png");
	m_pLayerMainScene->addChild(m_pSprBounus, 999);
	m_pSprBounus->setVisible(false);
	int Width = 0;
	for (int i = 0; i < 8; i++)
	{
		Width += m_pSprGameOver[i]->getContentSize().width;
	}
	float offsetX;
	offsetX = visibleSize.width - Width;
	offsetX *= 0.5f;
	Vec2 vec2GameOverOrigin;
	vec2GameOverOrigin.x = offsetX;
	vec2GameOverOrigin.y = visibleSize.height;

	Vec2 MoveGameOver;

	MoveGameOver.y -= (visibleSize.height/5 + m_pSprGameOver[7]->getContentSize().height*2);


	float DelTime = 0.3f;
	float NextDelTime = 0.0f;
	MoveTo* moveToOrigin;
	DelayTime* delayTime;
	MoveBy* moveBy;



	for (int i = 0; i < 8; i++)
	{
		m_pSprGameOver[i]->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		m_pSprGameOver[i]->setVisible(true);
		NextDelTime = (i*DelTime);
		MoveGameOver.x = 0;

		if (i == 0)
		{
			m_pSprGameOver[0]->setPosition(vec2GameOverOrigin);
			moveToOrigin = MoveTo::create(0.0f, vec2GameOverOrigin);
			delayTime = DelayTime::create(NextDelTime);
			moveBy = MoveBy::create(DelTime, MoveGameOver);

		}
		else
		{

			vec2GameOverOrigin.x = m_pSprGameOver[i - 1]->getBoundingBox().getMinX();
			vec2GameOverOrigin.y = m_pSprGameOver[i - 1]->getBoundingBox().getMinY();
			vec2GameOverOrigin.x += m_pSprGameOver[i - 1]->getContentSize().width;

			m_pSprGameOver[i]->setPosition(vec2GameOverOrigin);

			moveToOrigin = MoveTo::create(0.0f, vec2GameOverOrigin);
			delayTime = DelayTime::create(NextDelTime);

			moveBy = MoveBy::create(DelTime, MoveGameOver);

		}
		m_pSeqGameOver[i] = Sequence::create(moveToOrigin, delayTime, moveBy, NULL);
		m_pSeqGameOver[i]->retain();

		m_pLayerMainScene->addChild(m_pSprGameOver[i], 300);
	}


	m_bOptionMenuState = false;

	m_iCoreLife = 20;
	m_iMoney = StageInfo::GetInstance()->GetStage((StageInfo::GetInstance()->GetCurStage()))->GetStageFirstGold();
	m_iCountDieMob = 0;
	m_iWaveMax = StageInfo::GetInstance()->GetStage((StageInfo::GetInstance()->GetCurStage()))->GetWaveMaxCount();
	m_icurWave = 1;


	m_eGameState = GAME_STATE_GAME;
	m_iClearStar = 0;
	m_iStageClearMoney = StageInfo::GetInstance()->GetStage((StageInfo::GetInstance()->GetCurStage()))->GetStageClearGold();

	m_pCallLabelClearMoney = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelClearMoney->SetEnable(true);
	m_pCallLabelClearMoney->GetLabel()->setVisible(false);
	m_pCallLabelClearBonus = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelClearBonus->SetEnable(true);
	m_pCallLabelClearBonus->GetLabel()->setVisible(false);
	char szTemp[256];
	sprintf(szTemp, "%.3d", m_iMoney);
	m_pCallLabelMoney->GetLabel()->setString(szTemp);

	sprintf(szTemp, "%.2d", m_iCoreLife);
	m_pCallLabelCore->GetLabel()->setString(szTemp);


	sprintf(szTemp, "%.2d %.2d", m_icurWave, m_iWaveMax);
	m_pCallLabelWave->GetLabel()->setString(szTemp);

	m_fGameWaveUp = 0.0f;
}

void GameManager::SetCurWave(int curWave)
{
	m_icurWave = curWave;
	char szWave[128];
	int Wave = m_icurWave + 1;
	if (Wave > m_iWaveMax)
		Wave = m_iWaveMax;
	sprintf(szWave, "%.2d %.2d", Wave,m_iWaveMax);
	m_pCallLabelWave->GetLabel()->setString(szWave);

}
void GameManager::GameStartVisible(bool visible)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_pCallLabelGameStart->SetScaleVerticalAnim(visible);
	m_pCallLabelGameStart->SetPos(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	m_pCallLabelGameStart->GetLabel()->setVisible(visible);
}
void GameManager::GameWaveUpVisible(bool visible)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_bGameWaveUpVisible = visible;

	m_pCallLabelGameStart->SetPos(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	Label* pLabel = m_pCallLabelGameWaveUp->GetLabel();
	pLabel->setColor(Color3B::WHITE);
	pLabel->enableOutline(Color4B(218, 165, 116, 255));
	pLabel->setLocalZOrder(300);
	pLabel->setSystemFontSize(150);
	pLabel->setString("Wave Up!!");
	pLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	pLabel->setVisible(visible);
	m_fGameWaveUp = 0.0f;
}


void GameManager::CountDieMob()
{
	m_iCountDieMob++;
}
void GameManager::OptionMenu()
{
	if (m_pSpriteResetStageItem->getNumberOfRunningActions() || m_pSpriteLoadTitleSceneItem->getNumberOfRunningActions()
		|| m_pSpriteLoadTitleSceneItem->getNumberOfRunningActions() || m_pSpriteResetStageItem->getNumberOfRunningActions())
	{
		return;
	}

	m_bOptionMenuState = !m_bOptionMenuState;

		if (m_bOptionMenuState)
		{
			m_pSpriteLoadTitleSceneItem->runAction(m_pOptionMenuMoveBy1);
			m_pSpriteResetStageItem->runAction(m_pSequenceOptionMenu2);
		}
		
		else if(!m_bOptionMenuState)
		{
			m_pSpriteLoadTitleSceneItem->runAction(m_pOptionMenuMoveBy1_reverse);
			m_pSpriteResetStageItem->runAction(m_pSequenceOptionMenu2_reverse);
		}
	
}
void GameManager::LoadTitleScene()
{
	m_eGameState = GAME_STATE_GAMEOVER;
	GameManager::GetInstance()->Release();
	CharacterInfo::GetInstance()->Release();

	Director::getInstance()->replaceScene(Title::createScene());
}
void GameManager::ResetStage()
{
	m_eGameState = GAME_STATE_GAMEOVER;
	GameManager::GetInstance()->Release();
	CharacterInfo::GetInstance()->Release();

	Director::getInstance()->replaceScene(Stage1::createScene());
}
void GameManager::Update(float fElapse)
{

	if (m_eGameState != GAME_STATE_GAMEOVER)
	{
		if (m_iCoreLife <= 0)
		{
			m_iCoreLife = 0;
			VisibleGameOver();
		}
		if (m_iWaveMax < (m_icurWave+1))
		{
			VisibleGameOver();
		}
	}
	else if (m_eGameState == GAME_STATE_GAMEOVER)
	{
		if (!m_pSprStar[m_iClearStar]->getNumberOfRunningActions())
		{
			for (int i = 0; i < m_iClearStar; i++)
			{
				m_pSprStar[i]->setOpacity(255);
				if (i > 0)
				{
					if (!m_pSprStar[i - 1]->getNumberOfRunningActions())
					{
						m_pSprStar[i]->setVisible(true);
					}
					else
					{
						m_pSprStar[i]->setVisible(false);
					}
				}
			}
			m_pMenuGameover->setVisible(true);
		}

	}

	if (m_bGameWaveUpVisible == true)
	{
		m_fGameWaveUp += fElapse;
		if (m_fGameWaveUp > 3.0f)
		{
			m_fGameWaveUp = 0.0f;
			m_pCallLabelGameWaveUp->GetLabel()->setVisible(false);
			m_bGameWaveUpVisible = false;
		}
	}
}
void GameManager::VisibleGameOver()
{
	m_eGameState = GAME_STATE_GAMEOVER;

	Size visibleSize = Director::getInstance()->getVisibleSize();

	if (m_iCoreLife >= 20)
		m_iClearStar = 3;
	else if (20 > m_iCoreLife &&m_iCoreLife >= 10)
		m_iClearStar = 2;
	else if (10 > m_iCoreLife&&m_iCoreLife > 0)
		m_iClearStar = 1;
	else
		m_iClearStar = 0;

	if (m_iClearStar == 0)
	{
		m_pSprGameClear->setVisible(false);
		m_pSprGoldString->setVisible(false);
		m_pSprStarString->setVisible(false);
		m_pSprBounus->setVisible(false);

		for (int i = 0; i < 8; i++)
		{
			m_pSprGameOver[i]->setVisible(true);
			m_pSprGameOver[i]->runAction(m_pSeqGameOver[i]);
		}
		m_pMenuGameover->setPosition(visibleSize.width / 2, visibleSize.height / 4);
		m_pMenuGameover->alignItemsHorizontallyWithPadding(100);

		m_iStageClearMoney = 0;
	}
	else
	{
		m_pSprGameClear->setVisible(true);
		m_pSprGoldString->setVisible(true);
		m_pSprStarString->setVisible(true);
		m_pSprBounus->setVisible(true);

		for (int i = 0; i < STAGESTAR; i++)
		{
			m_pSprStarOutline[i]->setVisible(true);
			if (i < m_iClearStar)
			{
				m_pSprStar[i]->setOpacity(100);
				m_pSprStar[i]->runAction(m_pSeqScaleTo[i]);
			}
		}
		Vec2 vec2MenuGameOver = Vec2(m_pSprStarOutline[2]->getBoundingBox().getMinX(), m_pSprGoldString->getBoundingBox().getMinY() - m_pSprItemRetry->getContentSize().height / 2);
		m_pMenuGameover->setPosition(vec2MenuGameOver);
		char szTemp[512];
		Label* pLabelTTFMoney = m_pCallLabelClearMoney->GetLabel();
		pLabelTTFMoney->setPosition(m_pSprStarOutline[1]->getBoundingBox().getMinX(), m_pSprGoldString->getBoundingBox().getMidY());
		m_pCallLabelClearMoney->SetEnable(true);
		sprintf(szTemp, "%d", m_iStageClearMoney);
		pLabelTTFMoney->setString(szTemp);
		pLabelTTFMoney->setSystemFontSize(120);
		pLabelTTFMoney->setLocalZOrder(200);
		pLabelTTFMoney->setColor(Color3B::WHITE);
		pLabelTTFMoney->setLocalZOrder(999);

		m_pCallLabelClearBonus = CallLabelManager::GetInstance()->GetCallLabel();
		m_pCallLabelClearBonus->SetEnable(true);

		m_pSprBounus->setPosition(pLabelTTFMoney->getPosition().x + 220, m_pSprGoldString->getBoundingBox().getMaxY() + m_pSprBounus->getContentSize().height/2 );
		Label* pLabelTTFBonus = m_pCallLabelClearBonus->GetLabel();

		m_pCallLabelClearPlus->GetLabel()->setPosition(pLabelTTFMoney->getPosition().x + 120, m_pSprGoldString->getBoundingBox().getMidY());
		m_pCallLabelClearPlus->GetLabel()->setSystemFontSize(50);
		m_pCallLabelClearPlus->GetLabel()->setLocalZOrder(999);
		m_pCallLabelClearPlus->SetEnable(true);
		pLabelTTFBonus->setPosition(pLabelTTFMoney->getPosition().x + 200, m_pSprGoldString->getBoundingBox().getMidY());
		sprintf(szTemp, "%d", m_iMoney);
		pLabelTTFBonus->setString(szTemp);
		pLabelTTFBonus->setSystemFontSize(50);
		pLabelTTFBonus->setLocalZOrder(999);
		pLabelTTFBonus->setColor(Color3B::WHITE);

		m_iStageClearMoney += m_iMoney;
	}
	 PlayerInfo::GetInstace()->SetStageClearInfo(m_iStage, m_iClearStar, m_iStageClearMoney);
	 CardManager::GetInstance()->CancelCardMenu();
	 CharacterManager::GetInstance()->CancleCharacterMenu();
	 OffenseManager::GetInstance()->visibleOffense(false);
}
bool GameManager::GameOver()
{
	return true; 
}
void GameManager::CoreLifeMinus(int value)
{
	char szTemp[256];
	if (m_iCoreLife > 0)
	{
		m_iCoreLife += value;
		log("[CoreLife : %d]", m_iCoreLife);
		sprintf(szTemp, "%.2d", m_iCoreLife);
		m_pCallLabelCore->GetLabel()->setString(szTemp);

	}
}


void GameManager::PlusAndMinusMoney(int PlusAndMinusValue)
{
	m_iMoney += PlusAndMinusValue;

	if (m_iMoney >= 999)
	{
		m_iMoney = 999;
	}

	char szTemp[256];
	sprintf(szTemp, "%.3d", m_iMoney);
	m_pCallLabelMoney->GetLabel()->setString(szTemp);

}

int GameManager::GetMoney()
{
	return m_iMoney;
}

Sprite* GameManager::GetSpriteWithFrameFileName(char* szFileName)
{
	return Sprite::createWithSpriteFrameName(szFileName);
}