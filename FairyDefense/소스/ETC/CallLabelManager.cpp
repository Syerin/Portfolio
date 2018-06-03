#include "CallLabelManager.h"
#include "CallLabel.h"

CallLabelManager* CallLabelManager::m_pThis = NULL;

CallLabelManager::CallLabelManager()
{
}


CallLabelManager::~CallLabelManager()
{
	for (auto iter = m_vecCallLabel.begin(); iter != m_vecCallLabel.end(); iter++)
	{
		CC_SAFE_DELETE((*iter));
	}

}

void CallLabelManager::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;

	CallLabel* pCallLabel = NULL;

	for (auto iter = m_vecCallLabel.begin(); iter != m_vecCallLabel.end(); iter++)
	{
		CC_SAFE_DELETE((*iter));
	}
	m_vecCallLabel.clear();

	for (int i = 0; i < 150; i++)
	{
		pCallLabel = new CallLabel();
		pCallLabel->Init(m_pLayerMainScene);
		m_vecCallLabel.push_back(pCallLabel);
	}
}
void CallLabelManager::Update(float fElapse)
{
	for (auto iter = m_vecCallLabel.begin(); iter != m_vecCallLabel.end(); iter++)
	{
		(*iter)->Update(fElapse);
	}
}
CallLabel* CallLabelManager::GetCallLabel()
{
	CallLabel* pCallLabel = NULL;

	
	for (auto iter = m_vecCallLabel.begin(); iter != m_vecCallLabel.end(); iter++)
	{
		if ((*iter)->GetEnable()==false)
		{
			pCallLabel = (*iter);
			pCallLabel->Init(m_pLayerMainScene);
			return pCallLabel;
		}
	}
	log("[LabelTTF] 추가 생산되었다.");
	pCallLabel = new CallLabel();
	pCallLabel->Init(m_pLayerMainScene);
	m_vecCallLabel.push_back(pCallLabel);
	return pCallLabel;
}