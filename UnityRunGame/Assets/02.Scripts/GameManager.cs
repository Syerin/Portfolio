using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public enum GAMESTATE { PLAYING,PASUE,WIN,DIE,END}
public enum GAMEMODE { STAGEMODE, INFINITYMODE,TITLE, END }
public class GameManager : MonoBehaviour
{
    public UIAnimation m_UIAnimGo;
    public GameObject m_PauseManu;
    public ScoreBoard m_Scoreboard;
    public UIAnimation m_UIAnimScoreBoard;

    public float m_fBoostUpGage = 0.0f;

    public UIItem m_UIItem;
    public GAMESTATE m_eGameState;
    public Slider m_BoosterGage;
    public float m_fMaxBooster;
    public float m_fCurBooster;
    public RectTransform[] m_RectTransformLife;
    private float m_fLifeAnimTimer = 0.0f;
    public Image[] m_ImageLife;
    public int m_iLife;
    public int m_iHitDamage = 0;
    public Player m_PlayerComponent;
    public Text m_Miter;
    public float m_curMiter;
    private bool m_bAdjustLifeScale;

    public float m_fMulDeltaTime = 1.0f;
    public GameObject m_BoostHandle;
    public float GetMulDeltaTime()
    {
        return m_fMulDeltaTime;
    }
    // Use this for initialization
    void Start()
    {
        m_curMiter = 0.0f;
        m_Miter.text = "0 M";

        for (int i = 0; i < m_ImageLife.Length; i++)
        {
            m_ImageLife[i].enabled = true;
        }
        m_bAdjustLifeScale = false;
        m_UIAnimGo.AppearAnimation(true);
    }

    private void AnimationLife()
    {
        if(m_bAdjustLifeScale==false)
        m_fLifeAnimTimer += GetDeltaTime(0.5f);
        else
            m_fLifeAnimTimer -= GetDeltaTime(0.5f);

        if (m_fLifeAnimTimer >= 0.2f)
            m_bAdjustLifeScale = true;
        else if (m_fLifeAnimTimer <= 0.0f)
            m_bAdjustLifeScale = false;

        float ScaleX = 1.0f - m_fLifeAnimTimer;
        float ScaleY= 1.0f- m_fLifeAnimTimer;
        for (int i = 0; i < m_RectTransformLife.Length; i++)
        {
            Vector3 Scale = new Vector3(ScaleX, ScaleY);
            m_RectTransformLife[i].transform.localScale = Scale;
        }
    }

    public void VisibleScoreboard()
    {
        m_Scoreboard.SetEndState(m_eGameState);
        m_UIAnimScoreBoard.AppearAnimation(true);
    }
    // Update is called once per frame
    void Update ()
    {
        if (m_eGameState != GAMESTATE.PLAYING && m_eGameState != GAMESTATE.PASUE)
        {
            return;
        }
        m_iLife = m_ImageLife.Length - m_iHitDamage;

        AnimationLife();
        if (m_PlayerComponent.isDamageDown == false &&
            m_PlayerComponent.m_PlayerState != PLAYERSTATE.DIE &&
            m_PlayerComponent.m_PlayerState != PLAYERSTATE.FALLING &&
            m_PlayerComponent.m_PlayerState != PLAYERSTATE.KNOCKDOWN)
        {
            if(m_PlayerComponent.isBoosterMode == true)
                m_curMiter += m_PlayerComponent.GetBoostSpeed() * GetDeltaTime();
            else
                m_curMiter += m_PlayerComponent.GetCurSpeed() * GetDeltaTime();
        }
        m_Miter.text = string.Format("{0} M", (int)m_curMiter);

        m_BoosterGage.value = m_fCurBooster / m_fMaxBooster;
        if (m_PlayerComponent.isBoosterMode == true)
        {
            m_fCurBooster -= GetDeltaTime(m_fMaxBooster * 0.2f);
            if(m_fCurBooster <=0.0f)
            {
                m_fCurBooster = 0.0f;
                m_PlayerComponent.SetBoostMode(false);
            }
        }
        if (m_fBoostUpGage > 0.0f)
        {
            float fTime = GetDeltaTime(m_fMaxBooster * 0.4f);
            m_fBoostUpGage -= fTime;
            m_fCurBooster += fTime;
        }
        if (m_fBoostUpGage <= 0.0f)
        {
            m_fBoostUpGage = 0.0f;
        }

        if (m_fCurBooster >= m_fMaxBooster)
        {
            m_fCurBooster = m_fMaxBooster;
        }
    }

    public void SetBoostUpGage(float value)
    {
        m_fBoostUpGage = value;
    }

    public void IncreasedHealth(int value)
    {
        for (int i = 0; i < 3; i++)
        {
            m_ImageLife[i].enabled = false;
        }

        m_iHitDamage -= value;
        if (m_iHitDamage <= 0)
            m_iHitDamage = 0;
        m_iLife = m_ImageLife.Length - m_iHitDamage;
        for (int i = 0; i < m_iLife; i++)
        {
            m_ImageLife[i].enabled = true;
        }

    }
    public void IncreaseBoosterUpGage(float fValue)
    {
        if (m_PlayerComponent.m_PlayerState == PLAYERSTATE.KNOCKDOWN)
            return;
        if (m_PlayerComponent.m_PlayerState == PLAYERSTATE.DIE)
            return;

        m_fBoostUpGage += fValue;
    }

    public void IncreaseBoosterGage(float fValue)
    {
        if (m_PlayerComponent.isBoosterMode == true)
            return;
        if (m_PlayerComponent.m_PlayerState == PLAYERSTATE.KNOCKDOWN)
            return;
        if (m_PlayerComponent.m_PlayerState == PLAYERSTATE.DIE)
            return;

        m_fCurBooster += fValue;
        if (m_fCurBooster >= m_fMaxBooster)
            m_fCurBooster = m_fMaxBooster;
        m_BoosterGage.value = m_fCurBooster / m_fMaxBooster;
    }
    public void DecreaseLife(int Damage)
    {
        for (int i = 0; i < 3; i++)
        {
            m_ImageLife[i].enabled = false;
        }
        m_iHitDamage += Damage;
        m_iLife = m_ImageLife.Length - m_iHitDamage;
        for (int i = 0; i < m_iLife; i++)
        {
            m_ImageLife[i].enabled = true;
        }

    }
    public void SetCurBoostGage(float boostGage)
    {
        m_fCurBooster = boostGage;
    }
    public float GetDeltaTime(float individualTimer = 1.0f)
    {
        return m_fMulDeltaTime * Time.deltaTime * individualTimer;
    }
    public void SetGameState(GAMESTATE gameState)
    {
        m_eGameState = gameState;
    }

    public void DiePlayer()
    {
    }
    public void UIItemMoved(ITEM type)
    {
        if (type == ITEM.HEALTHPOINT)
            m_UIItem.GetItem(type, GetEmptyLifePos());
        else if (type == ITEM.BOOSTUP)
        {
            m_UIItem.GetItem(type, m_BoostHandle.transform.position, m_BoostHandle);
        }
    }

    private Vector3 GetEmptyLifePos()
    {
        Vector3 EmptyPos = m_ImageLife[m_ImageLife.Length -1].transform.position;
        Vector3 LifeSpriteScale = m_ImageLife[m_ImageLife.Length - 1].rectTransform.localScale;
        LifeSpriteScale.x *= m_ImageLife[m_ImageLife.Length - 1].rectTransform.pivot.x;
        LifeSpriteScale.y *= m_ImageLife[m_ImageLife.Length - 1].rectTransform.pivot.y;

        EmptyPos.x -= LifeSpriteScale.x;
        EmptyPos.y -= LifeSpriteScale.y;

        for (int i = 0; i <3; i++)
        {

            if(m_ImageLife[i].enabled == false)
            {
                EmptyPos = m_ImageLife[i].transform.position;
                EmptyPos.x += 75.0f;
                EmptyPos.y -= 75.0f;

                return EmptyPos;
            }
        }
        return EmptyPos;
    }

    public void ClickPauseButton()
    {
        m_PauseManu.SetActive(true);
        UIAnimation AnimPauseMesnu= m_PauseManu.GetComponent<UIAnimation>();
        AnimPauseMesnu.AppearAnimation(true);
        m_eGameState = GAMESTATE.PASUE;
        m_fMulDeltaTime = 0.0f;
    }
    public void ClickResumeButton()
    {
        m_PauseManu.SetActive(false);
        m_eGameState = GAMESTATE.PLAYING;
        m_fMulDeltaTime = 1.0f;
    }


    public int GetScore()
    {
        return (int)m_curMiter;
    }
    public int GetBonusScore()
    {
        return (int)m_fCurBooster;
    }

    public bool DecreaseScore(float value)
    {
        if (m_curMiter - value <= 0)
            return false;
        m_curMiter -= value;
        return true;
    }
    public bool DeceaseBonusScore(float value)
    {
        if (m_fCurBooster - value <= 0)
            return false;

            m_fCurBooster -= value;
        return true;
    }
}
