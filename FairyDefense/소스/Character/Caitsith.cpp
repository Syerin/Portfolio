#include "Caitsith.h"
#include "Mob.h"
#include "Offense.h"
#include "OffenseManager.h"
#include "GameManager.h"
Caitsith::Caitsith()
{
}


Caitsith::~Caitsith()
{
	CC_SAFE_RELEASE(m_pAnimCharging);
}
void Caitsith::Init(Layer* pLayerMainScene)
{
	m_pLayerMainScene = pLayerMainScene;

	Character::SetUpMenu();

	m_bAlive = false;
	m_fAnimDuration = 0.3f;
	m_fDelayTimer = 0.0f;
	m_fRange = 300.0f;
	m_eCharacterType = CHARACTER_TYPE_CAITSITH;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Character/CAIT.SITH/CaitsithAnim.plist");

	char szFileName[256] = { 0 };
	Animate* animate;
	Animation* animation;
	m_pSprFrameCharacterBottom = SpriteFrameCache::getInstance()->getSpriteFrameByName("Caitsith_00.png");
	m_pSprCharacter = GameManager::GetInstance()->GetSpriteWithFrameFileName("Caitsith_00.png");
	m_pLayerMainScene->addChild(m_pSprCharacter, 110);

	m_pSprCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_pSprCharacter->setVisible(false);
	m_pSprCharacter->setPosition(400, 400);
	m_vec2Pos.x = 400;
	m_vec2Pos.y = 400;

	m_pSprRange = GameManager::GetInstance()->GetSpriteWithFrameFileName("Range.png");
	m_pSprRange->setPosition(m_vec2Pos);
	m_pSprRange->setOpacity(40);
	m_pSprRange->setVisible(false);
	m_pSprRange->setScale((m_fRange / m_pSprRange->getContentSize().width) * 2);
	m_pLayerMainScene->addChild(m_pSprRange, 250);

	m_pSpriteOrigin = GameManager::GetInstance()->GetSpriteWithFrameFileName("Origin.png");
	m_pSpriteOrigin->setPosition(m_vec2Pos);
	m_pLayerMainScene->addChild(m_pSpriteOrigin, 777);
	m_isAttacking = false;
	m_eDirection = DIRECTION_B;
	m_pMob = NULL;
	m_pMenu->alignItemsHorizontallyWithPadding((m_pSprCharacter->getBoundingBox().getMaxX() - m_pSprCharacter->getBoundingBox().getMinX()));
	m_pUnitLab = new UnitLab();
	m_pUnitLab->Init(this, m_pLayerMainScene);
	m_pUnitLab->SetActiveStat();
	m_pUnitLab->SetVisible(false);
	m_pSpriteOrigin->setVisible(false);

	Vector<SpriteFrame*> vecSprFrame;
	SpriteFrame* frame;
	SpriteFrame* CharacterOrigin;
	char szFrameName[256];
	for (int i = RES_CAITSITH_01; i <= RES_CAITSITH_04; i++)
	{
		sprintf(szFrameName, "Caitsith_%.2d.png", i);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFrameName);
		vecSprFrame.pushBack(frame);
	}
	Animation* pAnim = Animation::createWithSpriteFrames(vecSprFrame, 0.3f);
	m_pAnimCharging = Animate::create(pAnim);
	m_pAnimCharging->retain();

	vecSprFrame.clear();

	for (int i =RES_CAITSITH_04; i < RES_CAITSITH_END; i++)
	{
		sprintf(szFrameName, "Caitsith_%.2d.png", i);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFrameName);
		vecSprFrame.pushBack(frame);
	}
	sprintf(szFrameName, "Caitsith_00.png");
	frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFrameName);
	vecSprFrame.pushBack(frame);

	 pAnim = Animation::createWithSpriteFrames(vecSprFrame, 0.33f);
	m_pAnimate[DIRECTION_B] = Animate::create(pAnim);
	m_pAnimate[DIRECTION_B]->retain();
	m_fMagicAttackTimer = 0.0f;

	m_fMagicAttackTime = 1.0f +((11)*0.3f);

}
void Caitsith::Update(float fElapse)
{
	m_fElapse = fElapse*m_iUpGradeCount;
	if (m_isAttacking)
	{
		m_fDelayTimer = 0.0f;
		m_fMagicAttackTimer += m_fElapse;

		if (m_fMagicAttackTime < m_fMagicAttackTimer)
		{
			m_fMagicAttackTimer = 0.0f;

			m_pOffenseLightning->SetPosition(m_vec2Area);
			m_pOffenseLightning->HitMob();
			m_isAttacking = false;
			m_pMob = NULL;
			m_pSprCharacter->setSpriteFrame(m_pSprFrameCharacterBottom);
		}
	}
	else
	{
		m_fDelayTimer += m_fElapse;
		if (!m_pSprCharacter->getNumberOfRunningActions() && m_fDelayTimer >=0.3f)
		{
			m_pSprCharacter->runAction(RepeatForever::create(m_pAnimCharging));
			m_bCharging = true;
		}
	}

}
void Caitsith::Attack(DIRECTION direction, Mob* pMob)
{
	//방향에 관계없는 캐릭터임,
	//몹 위치를받고 그 정해진 시간이후 콰가아아앙!
	if (m_pMob != NULL)
		return;
	if (m_bCharging ==  true && m_isAttacking == false && pMob != NULL && m_fDelayTimer >= m_fDelayAttack)
	{
		m_eDirection = DIRECTION_B;

		m_pSprCharacter->stopAllActions();
		m_fMagicAttackTimer = 0.0f;
		m_pMob = pMob;
		Vec2 vec2Mob = Vec2(pMob->GetRect().getMidX(), pMob->GetRect().getMinY());
		m_vec2Area = vec2Mob;
		m_isAttacking = true;
		m_pOffenseLightning = OffenseManager::GetInstance()->GetOffense(OFFENSE_TYPE_LIGHTNING);
		m_pOffenseLightning->SetAlive(true);
		m_pOffenseLightning->SetShootCharacter(this);
		m_pSprCharacter->runAction(m_pAnimate[DIRECTION_B]);
	}
	else
	{
		m_isAttacking = false;
	}
}

