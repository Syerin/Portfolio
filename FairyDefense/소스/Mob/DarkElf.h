#pragma once
#include "Mob.h"
class DarkElf : public Mob
{
private:

public:
	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);

	DarkElf();
	virtual ~DarkElf();
};

