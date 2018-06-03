#pragma once
#include "cocos2d.h"


class MathValue
{
public:
	static float GetAngle(float fPosX, float TagetX, float fPosY, float TagetY);
	static cocos2d::Vec2 GetDirection(float fPosX, float TagetX, float fPosY, float TagetY);
	static float DegreeToRadian(float angle);
	static float RadianToDegree(float rad);
	static float GetDistance(cocos2d::Vec2 MyPos, cocos2d::Vec2 TagetPos);
	MathValue();
	~MathValue();
};

