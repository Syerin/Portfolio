#include "CharacterManager.h"
#include "ELF.h"
#include "MathValue.h"
#include "MapTile.h"
#include "MobManager.h"
#include "CharacterInfo.h"
#include "Mob.h"
#include "Caitsith.h"
#include "Cusith.h"

CharacterManager* CharacterManager::m_pThis =NULL;


CharacterManager::CharacterManager()
{
}


CharacterManager::~CharacterManager()
{
	for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
	{
		CC_SAFE_DELETE((*iter));
		*iter = NULL;
	}
}


void CharacterManager::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;

	Character* pCharacter = NULL;
		for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
		{
			CC_SAFE_DELETE(*iter);
		}
		m_vecCharacter.clear();

	for (int j = 0; j < CHARACTER_TYPE_END; j++)
	{
		for (int i = 0; i < 40; i++)
		{
			pCharacter = CharacterInfo::GetInstance()->GetCharacterInfo((CHARACTER_TYPE)j);
			pCharacter->Init(m_pLayerMainScene);
			m_vecCharacter.push_back(pCharacter);
			
		}
	}
}

Character* CharacterManager::GetCharacter(CHARACTER_TYPE type)
{
	Character* pCharacter = NULL;

	
	for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
	{
		if ((*iter)->GetCharacterType() == type && (*iter)->GetAlive()==false)
		{
			(*iter)->Init(m_pLayerMainScene);
			pCharacter = (*iter);
			return pCharacter;
		}
	}
	if (pCharacter == NULL)
	{
		switch (type)
		{
		case CHARACTER_TYPE_ELF:
			pCharacter = new ELF();
			break;
		case CHARACTER_TYPE_CAITSITH:
			pCharacter = new Caitsith();
			break;
		case CHARACTER_TYPE_CUSITH:
			pCharacter = new Cusith();
			break;

		case CHARACTER_TYPE_END:
			break;
		default:
			break;
		}
		pCharacter->Init(m_pLayerMainScene);
		m_vecCharacter.push_back(pCharacter);
	}
	
	return pCharacter;
}
bool CharacterManager::GetSelectCharacter()
{
	for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
	{
		if ((*iter)->GetSelectCharacter())
			return true;
	}
	return false;
}
void CharacterManager::CancleCharacterMenu()
{
	for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
	{
		if ((*iter)->GetAlive() == true)
		{
			(*iter)->SetMenuVisible(false);
		}
	}
}
void CharacterManager::SearchRangeMob()
{

	float distance;
	Mob* pMob = NULL;
	for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
	{
		if ((*iter)->GetAlive())
		{
			Vec2 vec2CharacterAttack = Vec2((*iter)->GetOnTile()->GetSprite()->getBoundingBox().getMidX(), (*iter)->GetOnTile()->GetSprite()->getBoundingBox().getMidY());

			pMob = MobManager::GetInstance()->GetNearMob(vec2CharacterAttack); //캐릭터로부터 가까운 몹가져오기,

			if (pMob != NULL)
			{
				float MobMidX = pMob->GetSpriteMob()->getBoundingBox().getMidX();
				float MobBottomY = pMob->GetSpriteMob()->getBoundingBox().getMinY();
				Vec2 vec2Mob = Vec2(MobMidX, MobBottomY);

				distance = MathValue::GetDistance(vec2Mob, vec2CharacterAttack);//몹까지의 거리

				Attack((*iter),pMob,distance);
			}
		}
	}
	
}

void CharacterManager::Update(float fElapse)
{
	for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
	{
		if ((*iter)->GetAlive())
		{
			(*iter)->Update(fElapse);
		}
	}
}
void CharacterManager::SetAnimDuration(float speed)
{
	for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
	{
		if ((*iter)->GetAlive() == true)
		{
			(*iter)->SetAnimateDuration(speed);
		}
	}
}
void CharacterManager::SelectFieldCharacter(Vec2 vec2Mouse)
{
	for (auto iter = m_vecCharacter.begin(); iter != m_vecCharacter.end(); iter++)
	{
		if ((*iter)->GetAlive() == true)
		{
			if ((*iter)->SelectCharacterMouse(vec2Mouse))
			{
				break;
			}
		}
	}
}


bool CharacterManager::SurmonCharacter(CHARACTER_TYPE type, MapTile* pMapTile)
{
	return (GetCharacter(type)->Surmon(pMapTile));
}
void CharacterManager::Attack(Character* pCharacter, Mob* pMob, float distance)
{
	DIRECTION direction = DIRECTION_LT;
	float angle = 0.0f;
	float baseAngleLine = 0.0f;
	Vec2  vec2Mob = Vec2(pMob->GetVec2Pos().x, pMob->GetVec2Pos().y -pMob->GetSpriteMob()->getContentSize().height/2);
	if (distance < pCharacter->GetRange())
	{
		angle = MathValue::GetAngle(pCharacter->GetVec2Postion().x, vec2Mob.x, pCharacter->GetVec2Postion().y, vec2Mob.y);
		for (int i = 0; i < DIRECTION_END; i++)
		{
			baseAngleLine = (i * 45.0f);
			 if (angle > baseAngleLine - 22.5f && angle < baseAngleLine + 22.5f)
			 {
				if (i < DIRECTION_T)
					 direction = (DIRECTION)(i + DIRECTION_T);
				 else
				 {
					 direction = (DIRECTION)(i - DIRECTION_T);
				 }
			 }
		}
		pCharacter->Attack(direction,pMob);

	}
}