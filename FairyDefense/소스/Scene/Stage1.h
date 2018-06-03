#pragma once
#include "defines.h"
#include "MapTileManager.h"
#include "cocos2d.h"
#include "CharacterManager.h"
#include "CardManager.h"
#include "MobManager.h"
USING_NS_CC;

class Stage1 : public cocos2d::Layer
{
private:
	Vec2 m_vec2MousePos;

	float m_fElapseTime;

public:
	Stage1();
	~Stage1();

	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void Update(float fElapse);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	virtual void _OnMouseDown(cocos2d::Event* event);
	virtual void _OnMouseUp(cocos2d::Event* event);


	virtual void _OnKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#endif
	CREATE_FUNC(Stage1);
};


