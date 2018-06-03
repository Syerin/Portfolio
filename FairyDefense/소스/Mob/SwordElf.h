#pragma once
#include "Mob.h"
class SwordElf : public Mob
{
public:
	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);

	SwordElf();
	virtual ~SwordElf();
};

