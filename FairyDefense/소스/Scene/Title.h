#pragma once
#include "defines.h"
#include "cocos2d.h"
USING_NS_CC;
class CharacterManager;
class Character;
class UnitLab;
class CallLabel;
class Title : public cocos2d::Layer
{
private:
	Layer* m_pLayerArea;

	Vec2 m_vec2MousePos;
	Sprite* m_pSpriteMenuBarTop;
	Sprite* m_pSpriteMenuBarBottom;
	Vector<MenuItem*> m_vecMenuItem;
	Menu* m_pMenu;
	Sprite* m_pSprTitleStar;
	Sprite* m_pSprTitleGold;
	CallLabel* m_pCallLabelStar;
	CallLabel* m_pCallLabelGold;

	std::vector<Character*> m_vecCharacter;

	Sprite* m_pSpriteMapBack;

	Menu* m_pMenuAbilityButton;
	Sprite* m_pSprAbilityPointBar;
	CallLabel* m_pCallLabelAbilityPointPrice;
	CallLabel* m_pCallLabelAbilityPoint;
	CallLabel* m_pCallLabelSelectCharacter;

	Menu* m_pMenuArea1;

	Menu* m_pMenuEnter;

	Menu* m_pMenuApply;

	Menu* m_pAreaStage;

	Character* m_pCharacter;
	UnitLab* m_pUnitLab;
	Sprite* m_pSpriteAllMap;
	Sprite* m_pSpriteUnitBackGround;
	Menu* m_pMenuReturnButton;

	Sprite* m_pSprStageImage[MAXSTAGENUM];
	Sprite* m_pSprStageImamgeDown[MAXSTAGENUM];
	Sprite* m_pSprStageImamgeDisable[MAXSTAGENUM];

	Sprite* m_pSprStageStar[STAGESTAR* MAXAREASTAGE];
	Sprite* m_pSprStageStarBackGround[STAGESTAR*MAXAREASTAGE];

	bool m_bGoToStage;
	Sprite* m_pSpriteAreaMap;

	MenuItemSprite* m_pItemSpriteAreaStage[MAXAREASTAGE];
	Vector<MenuItem*> m_vecSprItemAreaStage;
	int m_iCurAreaNumber;
	int m_iLoadStageNumber;
	CallLabel* m_pCallLabelArea;
	CallLabel* m_pCallLabelAreaStar;
	CallLabel* m_pCallLabelAreaLimit[MAXAREASTAGE];
	Sprite* m_pSpriteUnitBack;
	bool m_bUnitLayerVisible;

	Sprite* m_pSpriteAbilityButton;
private:

	void SetAllMapDisplay(bool visible);
	void SetMenuDisplay(bool visible);
	void SetUnitDisplay(bool visible);

	void SetEnterUpDownDisplay(bool visible);
	void SetArea1MapDisplay(bool visible);

	void CallBackArea1();
	void DrawStageInfo(int AreaNumber,bool visible);
	void StageLimit(int StageNumber, int Limit, int AreaAllStar);

	void CallBackReturn();

	void CallBackUpAbilityPoint();
	void CallBackEnter();
	
	void CallBackUnitLayer();
	void CallBackUnitInfoApply();
	
	void CallBackMapLayer();

	void CallBackLoadStage1();
	void CallBackLoadStage2();
	void CallBackLoadStage3();
	void CallBackLoadStage4();
public:
	Title();
	~Title();

	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void Update(float fElapse);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	virtual void _OnMouseDown(cocos2d::Event* event);
	virtual void _OnMouseUp(cocos2d::Event* event);


	virtual void _OnKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	
#endif
	CREATE_FUNC(Title);
};


