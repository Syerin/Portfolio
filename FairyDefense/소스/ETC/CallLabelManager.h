#pragma once
#include "cocos2d.h"
USING_NS_CC;
class CallLabel;
class CallLabelManager
{
private:
	Layer* m_pLayerMainScene;
	std::vector<CallLabel*> m_vecCallLabel;
	static CallLabelManager* m_pThis;
	CallLabelManager();
public:
	void Init(Layer* pLayerMainScene);
	void Update(float fElapse);
	CallLabel* GetCallLabel();
	static CallLabelManager* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new CallLabelManager();
		}
		return m_pThis;
	}
	~CallLabelManager();
};

