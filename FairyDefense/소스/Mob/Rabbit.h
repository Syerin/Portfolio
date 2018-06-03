#pragma once
#include "Mob.h"
class Rabbit : public Mob
{
public:
	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);

	Rabbit();
	virtual ~Rabbit();
};

