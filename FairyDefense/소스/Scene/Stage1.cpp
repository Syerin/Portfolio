#include "Stage1.h"
#include "StageInfo.h"
#include "GameManager.h"
#include "CallLabelManager.h"
#include "CharacterInfo.h"
#include "Title.h"
#include "OffenseManager.h"
Stage1::Stage1()
{
}


Stage1::~Stage1()
{
}
Scene* Stage1::createScene()
{
	Scene* scene = Scene::create();
	Layer* layer = Stage1::create();
	scene->addChild(layer);

	return scene;
}
bool Stage1::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	CharacterInfo::GetInstance()->Init(this);


	CallLabelManager::GetInstance()->Init(this);
	GameManager::GetInstance()->Init(this,StageInfo::GetInstance()->GetCurStage());
	CharacterManager::GetInstance()->Init(this);
	MapTileManager::GetInstance()->Init(this,StageInfo::GetInstance()->GetCurStage());
	CardManager::GetInstance()->Init(this);
	MobManager::GetInstance()->Init(this);
	OffenseManager::GetInstance()->Init(this);


#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

	EventListenerMouse* pMouseListener = EventListenerMouse::create();
	pMouseListener->onMouseDown = CC_CALLBACK_1(Stage1::_OnMouseUp, this);
	pMouseListener->onMouseUp = CC_CALLBACK_1(Stage1::_OnMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pMouseListener, this);

	this->schedule(schedule_selector(Stage1::Update));



	EventListenerKeyboard* pKeyBoardListener = EventListenerKeyboard::create();
	pKeyBoardListener->onKeyPressed = CC_CALLBACK_2(Stage1::_OnKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pKeyBoardListener, this);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif

	GameManager::GetInstance()->GameStartVisible(true);


	return true;
}
void Stage1::Update(float fElapse)
{
	bool UpSpeed = GameManager::GetInstance()->GetSpeedButtonState();
	m_fElapseTime = fElapse;

	if (GameManager::GetInstance()->GetGameState() == GAME_STATE_GAME)
	{
		if (UpSpeed)
		{
			m_fElapseTime *= 2.0f;//2¹è¼Ó.
			CharacterManager::GetInstance()->SetAnimDuration(2.0f);
		}
		else
		{
			CharacterManager::GetInstance()->SetAnimDuration(1.0f);
		}


		CharacterManager::GetInstance()->Update(m_fElapseTime);
		CharacterManager::GetInstance()->SearchRangeMob();
		CardManager::GetInstance()->Update(fElapse);
		OffenseManager::GetInstance()->Update(m_fElapseTime);
		MobManager::GetInstance()->Update(m_fElapseTime);
	}
	CallLabelManager::GetInstance()->Update(m_fElapseTime);
	GameManager::GetInstance()->Update(m_fElapseTime);
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

void Stage1::_OnKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_T:

		GameManager::GetInstance()->Release();
		CharacterInfo::GetInstance()->Release();
		Director::getInstance()->replaceScene(Title::createScene());
		break;
	}
}

void Stage1::_OnMouseDown(cocos2d::Event* event)
{
	EventMouse* pEMouse = (EventMouse*)event;
	m_vec2MousePos = pEMouse->getLocation();
	m_vec2MousePos.x = pEMouse->getCursorX();
	m_vec2MousePos.y = pEMouse->getCursorY();
	Vec2 vec2 = Vec2(m_vec2MousePos.x, m_vec2MousePos.y);
	log("%f , %f", pEMouse->getCursorX(), pEMouse->getCursorY());
	
}
void Stage1::_OnMouseUp(cocos2d::Event* event)
{

	EventMouse* pEMouse = (EventMouse*)event;
	m_vec2MousePos = pEMouse->getLocation();
	m_vec2MousePos.x = pEMouse->getCursorX();
	m_vec2MousePos.y = pEMouse->getCursorY();
	Vec2 vec2 = Vec2(m_vec2MousePos.x, m_vec2MousePos.y);
	log("%f , %f", pEMouse->getCursorX(), pEMouse->getCursorY());

	if(CardManager::GetInstance()->GetCardActiveState() == false)
		MapTileManager::GetInstance()->SetSelectMapTile(false);

	if (GameManager::GetInstance()->GetGameState() == GAME_STATE_GAME)
	{

		if (MapTileManager::GetInstance()->SelectMapTile(m_vec2MousePos) && CharacterManager::GetInstance()->GetSelectCharacter() == false) 
		{
			CardManager::GetInstance()->DrawActionCard(m_vec2MousePos);
		}
		else if (MapTileManager::GetInstance()->GetSelectMapTile() == false && MapTileManager::GetInstance()->GetMenuVisible() == false && CharacterManager::GetInstance()->GetSelectCharacter() == false) 
		{
			CharacterManager::GetInstance()->SelectFieldCharacter(m_vec2MousePos); 
		}
		else if (CardManager::GetInstance()->GetCardActiveState() == true)
		{
			CardManager::GetInstance()->SelectCard(m_vec2MousePos);
		}
		else 
		{
			CardManager::GetInstance()->CancelCardMenu();
			CharacterManager::GetInstance()->CancleCharacterMenu(); 
		}
	}
}
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif
