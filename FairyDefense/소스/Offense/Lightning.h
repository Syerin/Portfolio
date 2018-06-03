#pragma once
#include "Offense.h"
class Lightning : public Offense
{
private:
	float m_fvisibleTimer;
public:
	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);
	virtual void Move();
	virtual void HitMob();
	Lightning();
	~Lightning();
};

