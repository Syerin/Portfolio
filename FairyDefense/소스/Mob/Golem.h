#pragma once
#include "Mob.h"

class Golem : public Mob
{
public:
	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);


	Golem();
	virtual ~Golem();
};

