#pragma once
#include "cocos2d.h"
#include "defines.h"
USING_NS_CC;
class Offense;
class OffenseManager
{
private:
	Layer* m_pLayerMainScene;
	std::vector<Offense*> m_vecOffense;
	OffenseManager();
	static OffenseManager* m_pThis;
public:
	void Init(Layer* pLayerMainScene);
	void Update(float fElapse);
	Offense* GenerateOffense(int type);

	Offense* GetOffense(int type);

	void visibleOffense(bool visible);

	static OffenseManager* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new OffenseManager;
		}
		return m_pThis;
	}
	~OffenseManager();
};

