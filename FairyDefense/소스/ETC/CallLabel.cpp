#include "CallLabel.h"


CallLabel::CallLabel()
{
	m_pLabel = NULL;
}


CallLabel::~CallLabel()
{
	CC_SAFE_RELEASE(m_pLabel);
}

void CallLabel::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;
	m_pLabel = Label::createWithTTF(" ", "fonts/arial.ttf",30);
	m_pLayerMainScene->addChild(m_pLabel, 110);

	m_bEnable = false;
	m_bVerticalAnim = false;
	m_pLabel->setVisible(m_bEnable);
	m_pLabel->setColor(Color3B::RED);
	m_pLabel->retain();
	m_fVisibleTime = 0.0f;
	m_fVisibleMaxTimer = 1.0f;
	m_fUpMoveSpeed = 40.0f;
	m_bScaleAnim = false;
	m_fScale = 1.0f;
}

void CallLabel::Update(float fElapse)
{
	if (m_bVerticalAnim)
	{
		if (m_bScaleAnim == false)
		{
			m_fVisibleTime += fElapse;
			m_vec2Pos.y += m_fUpMoveSpeed*fElapse;
			m_pLabel->setPosition(m_vec2Pos);
		}
		else if(m_bScaleAnim == true)
		{
			m_fScale -= fElapse*15;

			if (m_fScale < 1.0f)
			{
				m_fScale = 1.0f;
				m_bScaleAnim = false;
			}

			m_pLabel->setPosition(m_vec2Pos);
			m_pLabel->setScale(m_fScale);
		}
		if (m_fVisibleMaxTimer < m_fVisibleTime)
		{
			m_fVisibleTime = 0.0f;
			m_bVerticalAnim = false;
			m_bEnable = false;
			m_bScaleAnim = false;
			m_fScale = 1.0f;
		}

		m_pLabel->setVisible(m_bVerticalAnim);
	}
}
void CallLabel::SetColor(Color3B color)
{
	m_pLabel->setColor(color);
}
Label* CallLabel::GetLabel()
{
	return m_pLabel;
}
bool CallLabel::GetEnable()
{
	return m_bEnable;
}
void CallLabel::SetPosX(float PosX)
{
	m_vec2Pos.x = PosX;
}
void CallLabel::SetPos(Vec2 vec2Pos)
{
	m_vec2Pos = vec2Pos;
}
void CallLabel::SetString(char* szChar)
{
	m_pLabel->setString(szChar);
}
void CallLabel::SetVerticalAnim(bool visible)
{
	m_bVerticalAnim = visible;
	m_bEnable = true;
	m_fVisibleTime = 0.0f;
}
void CallLabel::SetScaleVerticalAnim(bool visible)
{
	m_bEnable = visible;
	m_bVerticalAnim = visible;
	m_bScaleAnim = visible;
	m_pLabel->setVisible(m_bEnable);
	m_fScale = 6.0f;
	m_pLabel->setScale(m_fScale);
}
void CallLabel::SetEnable(bool isEnable)
{
	m_bEnable = isEnable;
	m_bVerticalAnim = false;
	m_fVisibleTime = 0.0f;
	m_pLabel->setVisible(m_bEnable);
}

