#include "MathValue.h"
#include "defines.h"
#include <cmath>

MathValue::MathValue()
{
}


MathValue::~MathValue()
{
}

float MathValue::GetAngle(float fPosX,float TagetX,float fPosY,float TagetY)
{
	float disX = TagetX - fPosX;
	float disY = TagetY - fPosY;

	float angle = RadianToDegree(atan2(disX, disY));
	if (angle < 0)
	{
		angle = 360 + angle;
	}
	return angle;
}
float MathValue::RadianToDegree(float rad)
{
	return 180 * rad/ PI;
}
float MathValue::DegreeToRadian(float angle)
{
	return  angle * PI /  180;
}

cocos2d::Vec2 MathValue::GetDirection(float fPosX, float TagetX, float fPosY, float TagetY)
{
	cocos2d::Vec2 vec2Dir;
	float disX = TagetX - fPosX;
	float disY = TagetY - fPosY;
	float distance = sqrt(pow(disX, 2) + pow(disY, 2));
	vec2Dir.x = disX / distance;
	vec2Dir.y = disY / distance;

	return vec2Dir;
}

float MathValue::GetDistance(cocos2d::Vec2 MyPos, cocos2d::Vec2 TagetPos)
{
	float disX = TagetPos.x - MyPos.x;
	float disY = TagetPos.y- MyPos.y;
	float distance = sqrt(pow(disX, 2) + pow(disY, 2));

	return distance;
}