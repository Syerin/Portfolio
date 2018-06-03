#pragma once
#include "Mob.h"
class WizardElf : public Mob
{
public:
	virtual void Init(Layer* pLayerMainScene);
	virtual void Update(float fElapse);
	WizardElf();
	~WizardElf();
};

