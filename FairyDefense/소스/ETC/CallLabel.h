#pragma once
#include "cocos2d.h"
USING_NS_CC;
class CallLabel
{
private:
	Layer* m_pLayerMainScene;
	Label* m_pLabel;
	Vec2 m_vec2Pos;
	bool m_bEnable;
	bool m_bVerticalAnim;
	bool m_bScaleAnim;
	float m_fUpMoveSpeed;
	float m_fScale;

	float m_fVisibleMaxTimer;
	float m_fVisibleTime;
public:
	void Init(Layer* pLayerMainScene);
	void Update(float fElapse);
	Label* GetLabel();
	void SetColor(Color3B color);
	void SetPos(Vec2 CallPos);
	void SetPosX(float PosX);
	void SetString(char* szChar);
	void SetEnable(bool isEnable);
	bool GetEnable();
	void SetVerticalAnim(bool visible);
	void SetScaleVerticalAnim(bool visible);
	
	CallLabel();
	~CallLabel();
};

