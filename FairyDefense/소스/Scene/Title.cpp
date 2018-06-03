#include "Title.h"
#include "StageInfo.h"
#include "Stage1.h"
#include "CharacterManager.h"
#include "CallLabelManager.h"
#include "CharacterInfo.h"
#include "Character.h"
#include "CallLabelManager.h"
#include "CallLabel.h"
#include "PlayerInfo.h"
#include "UnitLab.h"
#include "GameManager.h"
Title::Title()
{
}


Title::~Title()
{
}

Scene* Title::createScene()
{
	// 'scene' is an autorelease object
	Scene* scene = Scene::create();

	// 'layer' is an autorelease object
	Layer* layer = Title::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool Title::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	CharacterInfo::GetInstance()->Init(this);


	CallLabelManager::GetInstance()->Init(this);

	PlayerInfo::GetInstace()->Init();

	CharacterManager::GetInstance()->Init(this);

	m_pCallLabelStar = NULL;
	m_pCallLabelGold = NULL;
	m_pCharacter = NULL;
	m_pUnitLab = NULL;

	for (int i = 0; i < CHARACTER_TYPE_END; i++)
	{
		Character* pCharacter = CharacterManager::GetInstance()->GetCharacter((CHARACTER_TYPE)i);
		pCharacter->SetAlive(true);
		m_vecCharacter.push_back(pCharacter);
	}
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	EventListenerMouse* pMouseListener = EventListenerMouse::create();
	pMouseListener->onMouseDown = CC_CALLBACK_1(Title::_OnMouseUp, this);
	pMouseListener->onMouseUp = CC_CALLBACK_1(Title::_OnMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pMouseListener, this);

	this->schedule(schedule_selector(Title::Update));

	EventListenerKeyboard* pKeyBoardListener = EventListenerKeyboard::create();
	pKeyBoardListener->onKeyPressed = CC_CALLBACK_2(Title::_OnKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pKeyBoardListener, this);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif


	m_pCallLabelStar = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelStar->SetEnable(true);
	m_pCallLabelStar->SetColor(Color3B::WHITE);

	m_pCallLabelGold = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelGold->SetEnable(true);
	m_pCallLabelGold->SetColor(Color3B::WHITE);



	m_pSprTitleStar = GameManager::GetInstance()->GetSpriteWithFrameFileName("TitleStar.png");
	m_pSprTitleStar->setScale(0.5f);
	m_pSprTitleStar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(m_pSprTitleStar, 200);


	m_pSprTitleGold = GameManager::GetInstance()->GetSpriteWithFrameFileName("TitleGold.png");
	m_pSprTitleGold->setScale(0.5f);
	m_pSprTitleGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	this->addChild(m_pSprTitleGold, 200);



	Sprite* pSpriteEnter = GameManager::GetInstance()->GetSpriteWithFrameFileName("EnterButton.png");
	Sprite* pSpriteEnterDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("EnterButton.png");
	pSpriteEnterDown->setColor(Color3B(99, 99, 99));

	MenuItemSprite* pMenuEnter = MenuItemSprite::create(pSpriteEnter, pSpriteEnterDown, CC_CALLBACK_0(Title::CallBackEnter, this));
	pMenuEnter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);

	m_pMenuEnter = Menu::create(pMenuEnter, NULL);

	m_pMenuEnter->setVisible(false);
	m_pMenuEnter->setPosition(visibleSize.width, visibleSize.height / 2);
	this->addChild(m_pMenuEnter, 200);

	Sprite* pSpriteAbilityApply = GameManager::GetInstance()->GetSpriteWithFrameFileName("Apply.png");
	Sprite* pSpriteAbilityApplyDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("Apply.png");
	pSpriteAbilityApplyDown->setColor(Color3B(99, 99, 99));

	MenuItemSprite* pSpriteItemApply = MenuItemSprite::create(pSpriteAbilityApply, pSpriteAbilityApplyDown, pSpriteAbilityApply, CC_CALLBACK_0(Title::CallBackUnitInfoApply, this));
	pSpriteItemApply->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	m_pMenuApply = Menu::create(pSpriteItemApply, NULL);
	m_pMenuApply->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	m_pMenuApply->setVisible(false);
	m_pMenuApply->setPosition(visibleSize.width, visibleSize.height / 2);
	this->addChild(m_pMenuApply, 200);


	m_pSpriteMenuBarTop = GameManager::GetInstance()->GetSpriteWithFrameFileName("MenuBar.png");
	m_pSpriteMenuBarTop->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	m_pSpriteMenuBarTop->setPosition(0, visibleSize.height);
	this->addChild(m_pSpriteMenuBarTop);


	m_pSpriteMenuBarBottom = GameManager::GetInstance()->GetSpriteWithFrameFileName("MenuBar.png");
	m_pSpriteMenuBarBottom->setPosition(origin.x, origin.y);
	m_pSpriteMenuBarBottom->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	this->addChild(m_pSpriteMenuBarBottom);

	Sprite* pSpriteMapMenu = GameManager::GetInstance()->GetSpriteWithFrameFileName("MapButton.png");
	Sprite* pSpriteMapMenuDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("MapButton.png");
	pSpriteMapMenuDown->setColor(Color3B(99, 99, 99));
	Sprite* pSpriteUintMenu = GameManager::GetInstance()->GetSpriteWithFrameFileName("UnitButton.png");
	Sprite* pSpriteUintMenuDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("UnitButton.png");
	pSpriteUintMenuDown->setColor(Color3B(99, 99, 99));

	MenuItemSprite* pMapMenu = MenuItemSprite::create(pSpriteMapMenu, pSpriteMapMenuDown, pSpriteMapMenu, CC_CALLBACK_0(Title::CallBackMapLayer, this));
	MenuItemSprite* pUintMenu = MenuItemSprite::create(pSpriteUintMenu, pSpriteUintMenuDown, pSpriteUintMenu, CC_CALLBACK_0(Title::CallBackUnitLayer, this));

	m_vecMenuItem.pushBack(pMapMenu);
	m_vecMenuItem.pushBack(pUintMenu);

	m_pMenu = Menu::createWithArray(m_vecMenuItem);
	float MenuWidth = pSpriteMapMenu->getBoundingBox().getMaxX() - pSpriteMapMenu->getBoundingBox().getMinX();

	float MenuBarHeight = m_pSpriteMenuBarBottom->getBoundingBox().getMaxY() - m_pSpriteMenuBarBottom->getBoundingBox().getMinY();

	m_pMenu->setPosition(MenuWidth + 10, (MenuBarHeight / 2));
	m_pMenu->alignItemsHorizontallyWithPadding(-2);
	m_pSpriteMenuBarBottom->addChild(m_pMenu);

	m_pSprAbilityPointBar = GameManager::GetInstance()->GetSpriteWithFrameFileName("AblityPointbar.png");
	m_pSprAbilityPointBar->setPosition(visibleSize.width*0.5f, m_pSpriteMenuBarTop->getBoundingBox().getMinY() - m_pSprAbilityPointBar->getContentSize().height*0.5);
	this->addChild(m_pSprAbilityPointBar, 110);

	m_pSpriteAbilityButton = GameManager::GetInstance()->GetSpriteWithFrameFileName("StatPlusButton.png");
	Sprite* pSpriteAbilityButtonDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("StatPlusButton.png");
	BlendFunc BlendFunc1 = { GL_SRC_COLOR,GL_ONE };
	pSpriteAbilityButtonDown->setBlendFunc(BlendFunc1);

	MenuItemSprite* pMenuItemAbilityButton = MenuItemSprite::create(m_pSpriteAbilityButton, pSpriteAbilityButtonDown, m_pSpriteAbilityButton, CC_CALLBACK_0(Title::CallBackUpAbilityPoint, this));
	pMenuItemAbilityButton->setPosition(0, 0);
	m_pMenuAbilityButton = Menu::create(pMenuItemAbilityButton, NULL);
	m_pMenuAbilityButton->setPosition(m_pSprAbilityPointBar->getContentSize().width*0.5f, m_pSprAbilityPointBar->getContentSize().height*0.5f);
	m_pSprAbilityPointBar->addChild(m_pMenuAbilityButton);
	m_pSprAbilityPointBar->setVisible(false);

	Sprite* pSpriteArea1 = GameManager::GetInstance()->GetSpriteWithFrameFileName("Area1.png");
	Sprite* pSpriteArea1Down = GameManager::GetInstance()->GetSpriteWithFrameFileName("Area1.png");
	pSpriteArea1Down->setColor(Color3B(99, 99, 99));

	MenuItemSprite* pSpriteItemArea1 = MenuItemSprite::create(pSpriteArea1, pSpriteArea1Down, pSpriteArea1, CC_CALLBACK_0(Title::CallBackArea1, this));

	pSpriteArea1->setPosition(0, 0);
	pSpriteItemArea1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	m_pMenuArea1 = Menu::create(pSpriteItemArea1, NULL);
	m_pMenuArea1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

	m_pMenuArea1->setPosition(0, m_pSpriteMenuBarTop->getBoundingBox().getMinY());
	this->addChild(m_pMenuArea1, 150);

	m_pCallLabelAbilityPointPrice = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelAbilityPointPrice->SetEnable(true);

	m_pCallLabelAbilityPoint = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelAbilityPoint->SetEnable(true);

	m_pSpriteUnitBack = GameManager::GetInstance()->GetSpriteWithFrameFileName("Unit.png");
	m_pSpriteUnitBack->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(m_pSpriteUnitBack, 10);

	m_pSpriteMapBack = GameManager::GetInstance()->GetSpriteWithFrameFileName("TitleMap.png");
	m_pSpriteMapBack->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(m_pSpriteMapBack, 0);

	m_pLayerArea = Layer::create();
	this->addChild(m_pLayerArea, 300);


	m_pSpriteAreaMap = GameManager::GetInstance()->GetSpriteWithFrameFileName("AreaStageInfo.png");
	m_pSpriteAreaMap->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	m_pSpriteAreaMap->setOpacity(200);
	m_pLayerArea->addChild(m_pSpriteAreaMap, 160);

	m_pLayerArea->setVisible(false);
	m_pCallLabelSelectCharacter = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelSelectCharacter->SetString("캐릭터를 선택해주세요.");
	m_pCallLabelSelectCharacter->SetEnable(true);
	m_pCallLabelSelectCharacter->GetLabel()->setColor(Color3B::WHITE);
	m_pCallLabelSelectCharacter->GetLabel()->setSystemFontSize(60);

	m_pCallLabelSelectCharacter->SetPos(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	m_pCallLabelSelectCharacter->GetLabel()->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	m_pCallLabelSelectCharacter->GetLabel()->setVisible(false);
	m_pCallLabelSelectCharacter->GetLabel()->setLocalZOrder(300);



	for (int i = 0; i < 3 * MAXAREASTAGE; i++)
	{
		m_pSprStageStar[i] = GameManager::GetInstance()->GetSpriteWithFrameFileName("StageInfoClearStar.png");
		m_pSprStageStarBackGround[i] = GameManager::GetInstance()->GetSpriteWithFrameFileName("StageInfoClearStarBackGround.png");

		m_pLayerArea->addChild(m_pSprStageStar[i], 302);
		m_pLayerArea->addChild(m_pSprStageStarBackGround[i], 301);

	}


	char szFileName[128];
	for (int i = 0; i < MAXSTAGENUM; i++)
	{
		sprintf(szFileName, "StageImage%.2d.png", i + 1);
		m_pSprStageImage[i] = GameManager::GetInstance()->GetSpriteWithFrameFileName(szFileName);
		m_pSprStageImamgeDown[i] = GameManager::GetInstance()->GetSpriteWithFrameFileName(szFileName);
		m_pSprStageImamgeDown[i]->setColor(Color3B(99, 99, 99));
		m_pSprStageImamgeDisable[i] = GameManager::GetInstance()->GetSpriteWithFrameFileName(szFileName);
		m_pSprStageImamgeDisable[i]->setColor(Color3B(66, 66, 66));
	}
	Sprite* pSpriteReturn = GameManager::GetInstance()->GetSpriteWithFrameFileName("ReturnButton.png");
	Sprite* pSpriteReturnDown = GameManager::GetInstance()->GetSpriteWithFrameFileName("ReturnButton.png");
	pSpriteReturnDown->setColor(Color3B(99, 99, 99));

	MenuItemSprite* pMenuItemReturn = MenuItemSprite::create(pSpriteReturn, pSpriteReturnDown, pSpriteReturn, CC_CALLBACK_0(Title::CallBackReturn, this));
	m_pMenuReturnButton = Menu::create(pMenuItemReturn, NULL);
	m_pMenuReturnButton->setPosition(visibleSize.width / 2 + visibleSize.width / 4, visibleSize.height / 2 - visibleSize.height / 4 + 30);
	this->addChild(m_pMenuReturnButton,300);
	m_pMenuReturnButton->setVisible(false);
	m_bGoToStage = false;


	Vec2 vec2SpriteItem[4];
	vec2SpriteItem[0] = Vec2(-m_pSpriteAreaMap->getContentSize().width / 4, (m_pSpriteAreaMap->getContentSize().height / 4) - 30);
	vec2SpriteItem[1] = Vec2(0, (m_pSpriteAreaMap->getContentSize().height / 4) - 30);
	vec2SpriteItem[2] = Vec2(-m_pSpriteAreaMap->getContentSize().width / 4, -(m_pSpriteAreaMap->getContentSize().height / 4) + 30);
	vec2SpriteItem[3] = Vec2(0, -(m_pSpriteAreaMap->getContentSize().height / 4) + 30);
	Vec2 vec2Center = Vec2(visibleSize.width / 2, visibleSize.height / 2);


	for (int i = 0; i < MAXAREASTAGE; i++)
	{
		switch (i)
		{
		case 0:
			m_pItemSpriteAreaStage[i] = MenuItemSprite::create(m_pSprStageImage[i], m_pSprStageImamgeDown[i], m_pSprStageImamgeDisable[i], CC_CALLBACK_0(Title::CallBackLoadStage1, this));
			m_pItemSpriteAreaStage[i]->setPosition(vec2SpriteItem[0]);
			break;
		case 1:
			m_pItemSpriteAreaStage[i] = MenuItemSprite::create(m_pSprStageImage[i], m_pSprStageImamgeDown[i], m_pSprStageImamgeDisable[i], CC_CALLBACK_0(Title::CallBackLoadStage2, this));
			m_pItemSpriteAreaStage[i]->setPosition(vec2SpriteItem[1]);
			break;
		case 2:
			m_pItemSpriteAreaStage[i] = MenuItemSprite::create(m_pSprStageImage[i], m_pSprStageImamgeDown[i], m_pSprStageImamgeDisable[i], CC_CALLBACK_0(Title::CallBackLoadStage3, this));
			m_pItemSpriteAreaStage[i]->setPosition(vec2SpriteItem[2]);
			break;
		case 3:
			m_pItemSpriteAreaStage[i] = MenuItemSprite::create(m_pSprStageImage[i], m_pSprStageImamgeDown[i], m_pSprStageImamgeDisable[i], CC_CALLBACK_0(Title::CallBackLoadStage4, this));
			m_pItemSpriteAreaStage[i]->setPosition(vec2SpriteItem[3]);
			break;

		}
		m_vecSprItemAreaStage.pushBack(m_pItemSpriteAreaStage[i]);
	}
	int GetStagePosX;
	int GetStagePosY;
	Vec2 vec2PosStar;
	for (int j = 0; j < MAXAREASTAGE; j++)
	{
		GetStagePosX = visibleSize.width / 2 + vec2SpriteItem[j].x + m_pSprStageImage[0]->getContentSize().width / 2 - m_pSprStageStar[0]->getContentSize().width / 2 - 10;
		GetStagePosY = visibleSize.height / 2 + vec2SpriteItem[j].y - m_pSprStageImage[0]->getContentSize().height / 2 + m_pSprStageStar[0]->getContentSize().height / 2;
		for (int i = 0; i < 3; i++)
		{
			vec2PosStar = Vec2(GetStagePosX - (i*(m_pSprStageStar[0]->getContentSize().width - 5)), GetStagePosY);
			m_pSprStageStar[i + (j * 3)]->setPosition(vec2PosStar);
			m_pSprStageStarBackGround[i + (j * 3)]->setPosition(vec2PosStar);
		}
		m_pCallLabelAreaLimit[j] = CallLabelManager::GetInstance()->GetCallLabel();
		m_pCallLabelAreaLimit[j]->SetEnable(true);
		m_pCallLabelAreaLimit[j]->GetLabel()->setColor(Color3B::WHITE);
		m_pCallLabelAreaLimit[j]->GetLabel()->setSystemFontSize(m_pSprStageStar[0]->getContentSize().height);
		m_pCallLabelAreaLimit[j]->GetLabel()->setPosition(GetStagePosX, GetStagePosY);
		this->removeChild(m_pCallLabelAreaLimit[j]->GetLabel());
		m_pLayerArea->addChild(m_pCallLabelAreaLimit[j]->GetLabel(), 303);

	}


	m_pAreaStage = Menu::createWithArray(m_vecSprItemAreaStage);
	m_pAreaStage->setPosition(m_pSpriteAreaMap->getBoundingBox().getMidX(), m_pSpriteAreaMap->getBoundingBox().getMidY());
	m_pLayerArea->addChild(m_pAreaStage, 300);

	m_pCallLabelArea = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelArea->SetEnable(true);
	m_pCallLabelArea->GetLabel()->setPosition(visibleSize.width / 2 + visibleSize.width / 4, visibleSize.height / 2 + visibleSize.height / 6);
	m_pCallLabelArea->GetLabel()->setSystemFontSize(55);
	m_pCallLabelArea->GetLabel()->setParent(m_pLayerArea);
	m_pCallLabelArea->GetLabel()->setLocalZOrder(303);
	m_pCallLabelArea->GetLabel()->setColor(Color3B::WHITE);
	this->removeChild(m_pCallLabelArea->GetLabel());
	m_pLayerArea->addChild(m_pCallLabelArea->GetLabel(), 303);


	m_pCallLabelAreaStar = CallLabelManager::GetInstance()->GetCallLabel();
	m_pCallLabelAreaStar->SetEnable(true);
	m_pCallLabelAreaStar->GetLabel()->setPosition(visibleSize.width / 2 + visibleSize.width / 4, visibleSize.height / 2);
	m_pCallLabelAreaStar->GetLabel()->setSystemFontSize(50);
	m_pCallLabelAreaStar->GetLabel()->setParent(m_pLayerArea);
	m_pCallLabelAreaStar->GetLabel()->setLocalZOrder(303);
	m_pCallLabelAreaStar->GetLabel()->setColor(Color3B::WHITE);
	this->removeChild(m_pCallLabelAreaStar->GetLabel());
	m_pLayerArea->addChild(m_pCallLabelAreaStar->GetLabel(), 303);

	SetMenuDisplay(true);
	SetUnitDisplay(false);
	SetAllMapDisplay(true);
	SetEnterUpDownDisplay(false);

	return true;
}

void Title::DrawStageInfo(int AreaNumber,bool visible)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	int iAllStars = PlayerInfo::GetInstace()->GetAllStageStar();

	for (int j = 0; j < MAXAREASTAGE; j++)
	{
		for (int i = 0; i < STAGESTAR; i++)
		{
			m_pSprStageStar[i + (j * STAGESTAR)]->setVisible(false);
			m_pSprStageStarBackGround[i + (j * STAGESTAR)]->setVisible(true);
		}
	}
	int* piAreaStars = new int[MAXAREASTAGE];
	int AllAreaStar = 0;
	for (int i = 0; i < MAXAREASTAGE; i++)
	{
		piAreaStars[i] = PlayerInfo::GetInstace()->GetStageStar((i+1) + ((AreaNumber - 1)* MAXAREASTAGE));
		AllAreaStar += piAreaStars[i];
	}
	int* piAreaStarsLimit = new int[MAXAREASTAGE];


	char sztemp[64];
	for (int i = 0; i < MAXAREASTAGE; i++)
	{
		piAreaStarsLimit[i] = StageInfo::GetInstance()->GetStage((i+1) + ((AreaNumber-1)* MAXAREASTAGE))->GetStageLimitStar();
		sprintf(sztemp, "%d", piAreaStarsLimit[i]);
		m_pCallLabelAreaLimit[i]->GetLabel()->setString(sztemp);
		m_pCallLabelAreaLimit[i]->GetLabel()->setVisible(false);
	}
	for (int j = 0; j < MAXAREASTAGE; j++)
	{
		for (int i =0; i < piAreaStars[j]; i++)
		{
			m_pSprStageStar[i + (j * STAGESTAR)]->setVisible(true);
		}

	}

	m_iCurAreaNumber = AreaNumber;


	int TopClearStage = PlayerInfo::GetInstace()->GetTopClearStage();
	int StageImageNumber = AreaNumber * MAXAREASTAGE - (1 * MAXAREASTAGE);

	 for (int i = 0; i < MAXAREASTAGE; i++)
	 {
		 StageLimit(StageImageNumber + i, piAreaStarsLimit[i], iAllStars);
	 }
	 sprintf(sztemp, "AREA. %d", AreaNumber);
	 m_pCallLabelArea->GetLabel()->setString(sztemp);
	 sprintf(sztemp, "%d / 12", AllAreaStar);
	 m_pCallLabelAreaStar->GetLabel()->setString(sztemp);

	 m_pMenuReturnButton->setPosition(visibleSize.width / 2 + visibleSize.width / 4, visibleSize.height / 2 - visibleSize.height / 4 + 30);
	 m_pMenuReturnButton->setVisible(visible);
	m_pAreaStage->setVisible(visible);
	m_pLayerArea->setVisible(visible);

	delete[] piAreaStars;
}
void Title::StageLimit(int StageNumber, int Limit, int AreaAllStar)
{
	//스테이지 잠금/해제 변화.
	if (AreaAllStar < Limit)
	{
		m_pCallLabelAreaLimit[StageNumber]->GetLabel()->setVisible(true);

		m_pItemSpriteAreaStage[StageNumber]->setEnabled(false);
		for (int i = StageNumber*STAGESTAR; i < (StageNumber * STAGESTAR) + STAGESTAR; i++)
		{
			m_pSprStageStarBackGround[i]->setVisible(false);

			if (i == (StageNumber * STAGESTAR)+1) 
				m_pSprStageStar[i]->setVisible(true);
			else
				m_pSprStageStar[i]->setVisible(false);
		}

	}
}

void Title::CallBackReturn()
{
	if (m_pLayerArea->isVisible() == true)
	{
		m_pLayerArea->setVisible(false);
		m_pMenuArea1->setEnabled(true);
		m_pMenuReturnButton->setVisible(false);
	}
	else
	{
		SetEnterUpDownDisplay(false);
		m_pMenuReturnButton->setVisible(false);
		SetUnitDisplay(true);
	}
}
void Title::SetArea1MapDisplay(bool visible)
{
	DrawStageInfo(1, true);
}

void Title::CallBackLoadStage1()
{
	m_bGoToStage = true;
	m_iLoadStageNumber = m_iCurAreaNumber + ((m_iCurAreaNumber-1)*MAXSTAGENUM);

	StageInfo::GetInstance()->SetCurStage(m_iLoadStageNumber);
	CharacterInfo::GetInstance()->Release();
	Director::getInstance()->replaceScene(Stage1::createScene());


}
void Title::CallBackLoadStage2()
{
	m_bGoToStage = true;
	m_iLoadStageNumber = m_iCurAreaNumber + ((m_iCurAreaNumber - 1)*MAXSTAGENUM) +1;

	StageInfo::GetInstance()->SetCurStage(m_iLoadStageNumber);
	CharacterInfo::GetInstance()->Release();
	Director::getInstance()->replaceScene(Stage1::createScene());
}

void Title::CallBackLoadStage3()
{
	m_bGoToStage = true;
	m_iLoadStageNumber = m_iCurAreaNumber + ((m_iCurAreaNumber - 1)*MAXSTAGENUM) +2 ;

	StageInfo::GetInstance()->SetCurStage(m_iLoadStageNumber);
	CharacterInfo::GetInstance()->Release();
	Director::getInstance()->replaceScene(Stage1::createScene());
}
void Title::CallBackLoadStage4()
{
	m_bGoToStage = true;
	m_iLoadStageNumber = m_iCurAreaNumber + ((m_iCurAreaNumber - 1)*MAXSTAGENUM) +3;

	StageInfo::GetInstance()->SetCurStage(m_iLoadStageNumber);
	CharacterInfo::GetInstance()->Release();
	Director::getInstance()->replaceScene(Stage1::createScene());
}

void Title::CallBackArea1()
{
	log("CallBackArea1");
	SetArea1MapDisplay(true);
	m_pMenuArea1->setEnabled(false);
}


void Title::SetAllMapDisplay(bool setvisible)
{
	m_pCharacter = NULL;
	for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
	{
		(*iter)->SetAnimIdle();
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_pMenuArea1->setEnabled(setvisible);
	m_pSpriteMapBack->setVisible(setvisible);
	m_pMenuArea1->setVisible(setvisible);
}
void Title::SetMenuDisplay(bool setvisible)
{
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float TopMenubarHeight = m_pSpriteMenuBarTop->getBoundingBox().getMaxY() - m_pSpriteMenuBarTop->getBoundingBox().getMinY();
	m_pSprTitleGold->setPosition(m_pSpriteMenuBarTop->getBoundingBox().getMaxX() - 20, m_pSpriteMenuBarTop->getBoundingBox().getMidY());
	m_pSprTitleStar->setPosition(m_pSpriteMenuBarTop->getBoundingBox().getMidX(), m_pSpriteMenuBarTop->getBoundingBox().getMidY());

	Label* pLabelTTFStar = m_pCallLabelStar->GetLabel();
	char szTemp[512];
	sprintf(szTemp, "%d", PlayerInfo::GetInstace()->GetAllStageStar());
	pLabelTTFStar->setString(szTemp);
	pLabelTTFStar->setSystemFontSize(TopMenubarHeight / 2);
	pLabelTTFStar->setPosition(m_pSprTitleStar->getBoundingBox().getMinX(), m_pSpriteMenuBarTop->getBoundingBox().getMidY());
	pLabelTTFStar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);

	Label* pLabelTTFGold = m_pCallLabelGold->GetLabel();
	sprintf(szTemp, "%.6d",PlayerInfo::GetInstace()->GetGold());
	pLabelTTFGold->setString(szTemp);
	pLabelTTFGold->setSystemFontSize(TopMenubarHeight / 2);
	pLabelTTFGold->setPosition(m_pSprTitleGold->getBoundingBox().getMinX(), m_pSpriteMenuBarTop->getBoundingBox().getMidY());
	pLabelTTFGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);


	m_pSpriteMenuBarBottom->setVisible(setvisible);
	m_pSpriteMenuBarTop->setVisible(setvisible);
	m_pMenu->setVisible(setvisible);

	int AbilityPoint = PlayerInfo::GetInstace()->GetAbilityPoint();

	Label* pLabelTTFApPrice = m_pCallLabelAbilityPointPrice->GetLabel();
	pLabelTTFApPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	pLabelTTFApPrice->setPosition(m_pSprAbilityPointBar->getBoundingBox().getMaxX() - 60, m_pSprAbilityPointBar->getBoundingBox().getMidY());

	sprintf(szTemp, "%.5d", (AbilityPoint + 1) * 100);

	pLabelTTFApPrice->setString(szTemp);
	pLabelTTFApPrice->setSystemFontSize(60);
	pLabelTTFApPrice->setLocalZOrder(120);
	pLabelTTFApPrice->setColor(Color3B::WHITE);
	pLabelTTFApPrice->setVisible(false);

	sprintf(szTemp, "%.3d", AbilityPoint);

	Label* pLabelTTFAp = m_pCallLabelAbilityPoint->GetLabel();
	pLabelTTFAp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	pLabelTTFAp->setString(szTemp);
	pLabelTTFAp->setPosition(m_pSprAbilityPointBar->getPosition().x - m_pSpriteAbilityButton->getContentSize().width / 2, m_pSprAbilityPointBar->getBoundingBox().getMidY());

	pLabelTTFAp->setSystemFontSize(60);	
	pLabelTTFAp->setColor(Color3B::WHITE);
	pLabelTTFAp->setLocalZOrder(120);
	pLabelTTFAp->setVisible(false);
}


void Title::CallBackUnitInfoApply()
{
	CharacterInfo::GetInstance()->SetCharacterInfo(m_pCharacter);
	m_pCharacter->GetUnitLab()->SetPrevStat();
}
void Title::SetUnitDisplay(bool visible)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	int count = 0;
	float CharacterOriginX = visibleSize.width/4;
	float offset = CharacterOriginX / 2;

	for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
	{
		(*iter)->SetAnimIdle();

		Sprite* pCharacterSprite = (*iter)->GetSpriteCharacter();
		pCharacterSprite->setVisible(visible);

		float CharacterOriginY = (visibleSize.height / 2) + (visibleSize.height / 6);
		CharacterOriginY += (pCharacterSprite->getContentSize().height / 2);

		pCharacterSprite->setPosition(CharacterOriginX + (count*offset), CharacterOriginY);
		(*iter)->SetRect();
		count++;
	}

	m_pSpriteUnitBack->setVisible(visible);
	m_bUnitLayerVisible = visible;
	m_pMenuEnter->setVisible(visible);

}

void Title::SetEnterUpDownDisplay(bool visible)
{
	if (m_pCharacter == NULL)
	{
		Label* pLabelTTFAp = m_pCallLabelAbilityPoint->GetLabel();

		pLabelTTFAp->setVisible(false);
		m_pMenuApply->setVisible(false);
		m_pSprAbilityPointBar->setVisible(false);
		m_pCallLabelAbilityPoint->GetLabel()->setVisible(false);
		m_pCallLabelAbilityPointPrice->GetLabel()->setVisible(false);

		if(m_pUnitLab != NULL)
		m_pUnitLab->SetVisible(false);
		return;
	}

	m_pUnitLab = m_pCharacter->GetUnitLab();
	m_pUnitLab->SetVisible(visible);
	m_pUnitLab->SetRemainingAPLabel(m_pCallLabelAbilityPoint->GetLabel());

	m_pSprAbilityPointBar->setVisible(visible);
	m_pCallLabelAbilityPoint->GetLabel()->setVisible(visible);
	m_pCallLabelAbilityPointPrice->GetLabel()->setVisible(visible);

		auto visibleSize = Director::getInstance()->getVisibleSize();

		m_pMenuReturnButton->setVisible(visible);
		m_pMenuReturnButton->setPosition(visibleSize.width-115, visibleSize.height/4);
	char szTemp[64];
	int RemainingAP = m_pUnitLab->GetRemainingAP();
	if (RemainingAP <= 0)
		RemainingAP = 0;
	sprintf(szTemp, "%.3d", RemainingAP);
	Label* pLabelTTFAp = m_pCallLabelAbilityPoint->GetLabel();
	pLabelTTFAp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	pLabelTTFAp->setString(szTemp);
	pLabelTTFAp->setPosition(m_pSprAbilityPointBar->getPosition().x - m_pSpriteAbilityButton->getContentSize().width / 2, m_pSprAbilityPointBar->getBoundingBox().getMidY());

	pLabelTTFAp->setSystemFontSize(60);
	pLabelTTFAp->setColor(Color3B::WHITE);
	pLabelTTFAp->setLocalZOrder(120);
	pLabelTTFAp->setVisible(visible);
	m_pMenuApply->setVisible(visible);
	m_pSpriteUnitBack->setVisible(visible);
}

void Title::CallBackEnter()
{
	if (m_pCharacter == NULL)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		m_pCallLabelSelectCharacter->SetPos(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		m_pCallLabelSelectCharacter->SetVerticalAnim(true);
		return;
	}
	SetUnitDisplay(false);
	SetEnterUpDownDisplay(true);
}

void Title::CallBackMapLayer()
{
	 SetAllMapDisplay(true);

	 SetUnitDisplay(false);
	 SetEnterUpDownDisplay(false);
	 SetArea1MapDisplay(false);
	 m_pLayerArea->setVisible(false);
	 m_pMenuReturnButton->setVisible(false);
}
void Title::CallBackUnitLayer()
{
	 SetAllMapDisplay(false);
	 SetUnitDisplay(true);
	 SetEnterUpDownDisplay(false);
	 m_pLayerArea->setVisible(false);
	 m_pMenuReturnButton->setVisible(false);

}
void Title::CallBackUpAbilityPoint()
{
	char szTemp[128];

	if (PlayerInfo::GetInstace()->GetGold() < (PlayerInfo::GetInstace()->GetAbilityPoint() + 1) * 100)
	{
		return;
	}

	PlayerInfo::GetInstace()->PlusAbilityPoint((PlayerInfo::GetInstace()->GetAbilityPoint() + 1) * 100);



		int RemainingAP = m_pUnitLab->GetRemainingAP();
		sprintf(szTemp, "%.3d", RemainingAP);
		Label* pLabelTTFAp = m_pCallLabelAbilityPoint->GetLabel();
		pLabelTTFAp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		pLabelTTFAp->setString(szTemp);



	Label* pLabelTTFStar = m_pCallLabelStar->GetLabel();
	sprintf(szTemp, "%d", PlayerInfo::GetInstace()->GetAllStageStar());
	pLabelTTFStar->setString(szTemp);	

	Label* pLabelTTFGold = m_pCallLabelGold->GetLabel();
	sprintf(szTemp, "%.6d",PlayerInfo::GetInstace()->GetGold());
	pLabelTTFGold->setString(szTemp);

	Label* pLabelTTFApPrice = m_pCallLabelAbilityPointPrice->GetLabel();

	sprintf(szTemp, "%.5d", (PlayerInfo::GetInstace()->GetAbilityPoint() + 1) * 100);

	pLabelTTFApPrice->setString(szTemp);


}


#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

void Title::_OnKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_1:
		m_bGoToStage = true;
		StageInfo::GetInstance()->SetCurStage(1);
		CharacterInfo::GetInstance()->Release();
		Director::getInstance()->replaceScene(Stage1::createScene());
		break;
	case EventKeyboard::KeyCode::KEY_2:
		m_bGoToStage = true;
		StageInfo::GetInstance()->SetCurStage(2);
		CharacterInfo::GetInstance()->Release();
		Director::getInstance()->replaceScene(Stage1::createScene());
		break;
	case EventKeyboard::KeyCode::KEY_3:
		m_bGoToStage = true;
		StageInfo::GetInstance()->SetCurStage(3);
		CharacterInfo::GetInstance()->Release();
		Director::getInstance()->replaceScene(Stage1::createScene());
		break;
	case EventKeyboard::KeyCode::KEY_4:
		m_bGoToStage = true;
		StageInfo::GetInstance()->SetCurStage(4);
		CharacterInfo::GetInstance()->Release();
		Director::getInstance()->replaceScene(Stage1::createScene());
		break;

	}
}

void Title::_OnMouseDown(cocos2d::Event* event)
{
	EventMouse* pEMouse = (EventMouse*)event;
	m_vec2MousePos = pEMouse->getLocation();
	m_vec2MousePos.x = pEMouse->getCursorX();
	m_vec2MousePos.y = pEMouse->getCursorY();
	Vec2 vec2 = Vec2(m_vec2MousePos.x, m_vec2MousePos.y);
	log("%f , %f", pEMouse->getCursorX(), pEMouse->getCursorY());
}
void Title::_OnMouseUp(cocos2d::Event* event)
{
	EventMouse* pEMouse = (EventMouse*)event;
	m_vec2MousePos = pEMouse->getLocation();
	m_vec2MousePos.x = pEMouse->getCursorX();
	m_vec2MousePos.y = pEMouse->getCursorY();
	Vec2 vec2 = Vec2(m_vec2MousePos.x, m_vec2MousePos.y);
	log("%f , %f", pEMouse->getCursorX(), pEMouse->getCursorY());

	if (m_bUnitLayerVisible == true)
	{
		for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
		{
			Rect rcRect = (*iter)->GetRect();
			if (rcRect.containsPoint(m_vec2MousePos))
			{
				for (auto iterAnim = m_vecCharacter.begin(); iterAnim != m_vecCharacter.end(); iterAnim++)
				{
					(*iterAnim)->SetAnimIdle();
				}
				Animate* pAnimCharacterDirection = (*iter)->GetAnimDirectionCharacter(DIRECTION_B);
				(*iter)->GetSpriteCharacter()->runAction(RepeatForever::create(pAnimCharacterDirection));
				m_pCharacter = (*iter);
			}
		}
	}
}
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif
void Title::Update(float fElapse)
{
	if (m_pCallLabelSelectCharacter != NULL && m_bGoToStage == false)
	{
		m_pCallLabelSelectCharacter->Update(fElapse);

	}

}
