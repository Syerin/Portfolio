#pragma once
#include "Mob.h"
class Puppet : public Mob
{
public:
	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);

	Puppet();
	virtual ~Puppet();
};

