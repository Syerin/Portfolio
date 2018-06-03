#include "Mob.h"
#include "MapTile.h"
#include "GameManager.h"
#include "CallLabelManager.h"
#include "CallLabel.h"
#include"Character.h"
#include"StageInfo.h"
#include "Stage.h"
#include "MobManager.h"
Mob::Mob()
{
}


Mob::~Mob()
{
}


void Mob::SetNextTile()
{

}

Vec2 Mob::GetVec2Pos()
{
	return m_vec2Pos;
}

bool Mob::GetAlive()
{
	return m_bAlive;
}
MOB_TYPE Mob::GetMobType()
{
	return m_eMobType;
}
void Mob::SetAlive(bool live)
{
	m_bAlive = live;
}
Sprite* Mob::GetSpriteMob()
{
	return m_pSprCharacter;
}
void Mob::SetDirection(DIRECTION MapConerTiledir)
{
	m_eDirection = MapConerTiledir;
	if (m_eDirection == DIRECTION_L)
	{
		m_pSprCharacter->stopAllActions();
		m_pSprCharacter->runAction(RepeatForever::create(m_pAnimate_Left));
	}
	else if (m_eDirection == DIRECTION_R)
	{
		m_pSprCharacter->stopAllActions();
		m_pSprCharacter->runAction(RepeatForever::create(m_pAnimate_right));
	}
}

DIRECTION Mob::GetDirection()
{
	return m_eDirection;
}

void Mob::Move(float fElapse)
{
	if (m_eDirection == DIRECTION_L)
	{
		m_vec2Pos.x -= (m_fMoveSpeed*fElapse);
	}
	else if (m_eDirection == DIRECTION_T)
	{
		m_vec2Pos.y += (m_fMoveSpeed*fElapse);
	}
	else if (m_eDirection == DIRECTION_R)
	{
		m_vec2Pos.x += (m_fMoveSpeed*fElapse);
	}
	else if (m_eDirection == DIRECTION_B)
	{
		m_vec2Pos.y -= (m_fMoveSpeed*fElapse);
	}
	m_pSprCharacter->setPosition(m_vec2Pos);
	m_rcRect.setRect(m_pSprCharacter->getBoundingBox().getMinX(), m_pSprCharacter->getBoundingBox().getMinY(),
		(m_pSprCharacter->getBoundingBox().getMaxX()- m_pSprCharacter->getBoundingBox().getMinX()), (m_pSprCharacter->getBoundingBox().getMaxY()- m_pSprCharacter->getBoundingBox().getMinY()));
	m_pSprHealthBar->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	m_pSprHealthBarBackground->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	
	Vec2 vec2HealthBarPosition;
	vec2HealthBarPosition.x += (((m_rcRect.getMaxX() - m_rcRect.getMinX()) - (m_pSprHealthBarBackground->getBoundingBox().getMaxX() - m_pSprHealthBarBackground->getBoundingBox().getMinX())) / 2);
	vec2HealthBarPosition.y = m_rcRect.getMaxY() - m_rcRect.getMinY() + (m_pSprHealthBarBackground->getBoundingBox().getMaxY() - m_pSprHealthBarBackground->getBoundingBox().getMinY()) + 10;

	m_pSprHealthBar->setPosition(vec2HealthBarPosition);
	m_pSprHealthBarBackground->setPosition(vec2HealthBarPosition);
}
void Mob::SetPosition(MapTile* pMapTile)
{
	m_vec2Pos = pMapTile->GetCenterPosition();
	Stage* pStage = StageInfo::GetInstance()->GetStage(StageInfo::GetInstance()->GetCurStage());
	
	if (pMapTile->GetMapTileWidth() == 0)
	{
		m_vec2Pos.x -= (m_pSprCharacter->getContentSize().width - pMapTile->GetSprite()->getContentSize().width/2);
		m_vec2Pos.y += pMapTile->GetSprite()->getContentSize().height / 2;

		m_pSprCharacter->setPosition(m_vec2Pos);
		m_eDirection = DIRECTION_R;
		m_pSprCharacter->runAction(RepeatForever::create(m_pAnimate_right));

	}
	else if (pMapTile->GetMapTileWidth() == pStage->GetWidth()-1)
	{
		m_vec2Pos.x += (m_pSprCharacter->getContentSize().width + pMapTile->GetSprite()->getContentSize().width / 2);
		m_vec2Pos.y += pMapTile->GetSprite()->getContentSize().height / 2;

		m_pSprCharacter->setPosition(m_vec2Pos);
		m_eDirection = DIRECTION_L;
		m_pSprCharacter->runAction(RepeatForever::create(m_pAnimate_Left));
	}
	else if (pMapTile->GetMapTileHeight() == 0)
	{
		m_vec2Pos.y += m_pSprCharacter->getContentSize().height + pMapTile->GetSprite()->getContentSize().height / 2;
		m_pSprCharacter->setPosition(m_vec2Pos.x , m_vec2Pos.y+ m_pSprCharacter->getContentSize().height);

		m_eDirection = DIRECTION_B;

	}
	else if (pMapTile->GetMapTileHeight() == pStage->GetHeight()-1)
	{
		m_vec2Pos.y -= m_pSprCharacter->getContentSize().height - -pMapTile->GetSprite()->getContentSize().height/ 2;
		m_pSprCharacter->setPosition(m_vec2Pos);

		m_eDirection = DIRECTION_T;
	}
	m_rcRect = m_pSprCharacter->getBoundingBox();
}
Rect Mob::GetRect()
{
	return m_rcRect;
}
void Mob::HittiingMob(Character* pCharacter) 
{
	if (m_bAlive == false)
		return;
	if (m_iHealthPoint <= 0)
		return;
	int CharacterDamage = pCharacter->GetAttackDamage()+pCharacter->GetiUpGradeDamageCount();


	m_iHealthPoint -= CharacterDamage;//방어력 ~~ 저항력, 스턴 비율,슬로우 등등~~
	float m_fHealthScaleX = ((float)m_iHealthPoint / (float)m_iHealthPointMax);

	m_pSprHealthBar->setScaleX(m_fHealthScaleX);
	char Damage[128];
	sprintf(Damage, "%d", CharacterDamage);


	CallLabel* pCallLabel = NULL;
	pCallLabel  = CallLabelManager::GetInstance()->GetCallLabel();
	pCallLabel->SetPos(m_vec2Pos);
	pCallLabel->SetScaleVerticalAnim(true);
	pCallLabel->GetLabel()->setString(String::createWithFormat("%d", CharacterDamage)->getCString());
	pCallLabel->GetLabel()->enableShadow(Color4B::BLACK, Size(2, 2), 100);
	pCallLabel->GetLabel()->setOpacity(200);

	if (m_iHealthPoint <= 0)
	{
		MobManager::GetInstance()->DieMobCount();
		m_bAlive = false;
		GameManager::GetInstance()->PlusAndMinusMoney(m_iMobZenMoeny);
		CallLabel* pCallLabel = NULL;
		pCallLabel = CallLabelManager::GetInstance()->GetCallLabel();
		pCallLabel->SetVerticalAnim(true);
		pCallLabel->GetLabel()->enableShadow(Color4B::BLACK, Size(2, 2), 100);
		pCallLabel->GetLabel()->setOpacity(200);
		char MobZenMoeny[128];
		sprintf(MobZenMoeny, "+%d", m_iMobZenMoeny);
		pCallLabel->GetLabel()->setString(MobZenMoeny);
		pCallLabel->GetLabel()->setColor(Color3B::YELLOW);
		pCallLabel->GetLabel()->setSystemFontSize(30);
		pCallLabel->SetPos(Vec2(m_pSprCharacter->getBoundingBox().getMidX(), m_pSprCharacter->getBoundingBox().getMaxY()+15));
		GameManager::GetInstance()->CountDieMob();
	}
}
int Mob::GetHealthPoint()
{
	return m_iHealthPoint;
}
bool Mob::DieCoreInMob(MapTile* pMapTile)
{
	if (pMapTile != NULL)
	{
		if (pMapTile->GetRect().intersectsRect(m_rcRect))
		{
			if (m_eDirection == DIRECTION_L)
			{
				if (pMapTile->GetCenterPosition().x < m_vec2Pos.x)
				{
					m_bAlive = false;
					m_pSprCharacter->setVisible(false);
					GameManager::GetInstance()->CoreLifeMinus(-1);
					return true;
				}
			}
			else if (m_eDirection == DIRECTION_T)
			{
				if (pMapTile->GetCenterPosition().y < m_vec2Pos.y)
				{
					m_bAlive = false;
					m_pSprCharacter->setVisible(false);
					GameManager::GetInstance()->CoreLifeMinus(-1);
					return true;
				}
			}
			else if (m_eDirection == DIRECTION_R)
			{
				if (pMapTile->GetCenterPosition().x < m_vec2Pos.x)
				{
					m_bAlive = false;
					m_pSprCharacter->setVisible(false);
					GameManager::GetInstance()->CoreLifeMinus(-1);
					return true;
				}
			}
			else if (m_eDirection == DIRECTION_B)
			{
				if (pMapTile->GetCenterPosition().y > m_vec2Pos.y)
				{
					m_bAlive = false;
					m_pSprCharacter->setVisible(false);
					GameManager::GetInstance()->CoreLifeMinus(-1);
					return true;
				}
			}
		}
	}
	return false;
}

int Mob::GetMobZenMoney()
{
	return m_iMobZenMoeny;
}

void Mob::SetStat(float MoveSpeed, int HP, int ZenMoney)
{
	m_fMoveSpeed = MoveSpeed;
	m_iHealthPoint = HP;
	m_iHealthPointMax = HP;

	m_iMobZenMoeny = ZenMoney;
}

float Mob::GetMoveSpeed()
{
	return m_fMoveSpeed;
}
int Mob::GetHPMax()
{
	return m_iHealthPointMax;
}
int Mob::GetZenMoney()
{
	return m_iMobZenMoeny;
}


void Mob::SetMobInfoTypeNum(int TypeNumber)
{
	m_iMobInfoTypeNum = TypeNumber;
}
int Mob::GetMobInfoTypeNum()
{
	return m_iMobInfoTypeNum;
}
void Mob::SetPosition(int x, int y)
{
	m_vec2Pos.x = x;
	m_vec2Pos.y = y;

	m_pSprCharacter->setPosition(m_vec2Pos);
}
