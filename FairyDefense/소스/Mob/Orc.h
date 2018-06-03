#pragma once
#include "Mob.h"
class Orc : public Mob
{
private:
public:
	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);
	Orc();
	virtual ~Orc();
};

