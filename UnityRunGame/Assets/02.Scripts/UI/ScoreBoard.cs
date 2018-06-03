using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class ScoreBoard : MonoBehaviour
{

    public UIAnimation m_UIAnimation;
    public GameManager m_GameManager;
    public Score m_Score;
    public Score m_BonusScore;

    public GameObject m_BoardClear;
    public GameObject m_BoardEnd;

    public GameObject m_BoardMainTitleButton;
    public GameObject m_BoardNextStageButton;
    public GameObject m_BoardRegameButton;

    public GameObject m_BoardBestImage;
    public GameObject m_BoardScore;

    private float m_fTime;

    private float m_fWaitTimer;
    private bool m_ScoreUpdate = false;
    void Start()
    {
        gameObject.SetActive(false);
        m_ScoreUpdate = false;
    }

    private void StartDecreasedBonusScore()
    {
        if (m_UIAnimation.GetAppearAnimEnd() == true)
        {
            if (m_BonusScore.isDecreased() == true)
                return;
            m_fWaitTimer += m_fTime;
            if (m_fWaitTimer >= 1.0f)
            {
                m_fWaitTimer = 0.0f;
                m_BonusScore.StartDecreaseValue(true);
            }
        }

    }

    private void Update()
    {
        m_fTime = Time.deltaTime;
        StartDecreasedBonusScore();
        if (m_BonusScore.isDecreased()==true)
        {
            if (m_BonusScore.DecreaseValue(m_fTime) == true)
            {
                m_GameManager.IncreaseBoosterGage(-m_fTime * m_BonusScore.GetSpeed());
                m_Score.IncreasedValue(m_fTime * m_BonusScore.GetSpeed());
            }
            else
            {
                m_Score.SetLabelValue(m_BonusScore.GetOriginValue() + m_Score.GetOriginValue());
                if (m_ScoreUpdate == false)
                {
                    m_BonusScore.GetOriginValue();
                    if (MainValue.GetInstance().SetStageScore(m_Score.GetcurValue()) == true)
                    {
                        m_BoardBestImage.GetComponent<UIAnimation>().AppearAnimation(true);
                    }
                    m_ScoreUpdate = true;
                }
            }
        }
    }
    public void SetEndState(GAMESTATE state)
    {
        gameObject.SetActive(true);
        m_BoardMainTitleButton.SetActive(false);

        if (state == GAMESTATE.WIN)
        {
            m_BoardClear.SetActive(true);
            m_BoardMainTitleButton.SetActive(true);
            m_BoardNextStageButton.SetActive(true);
            MainValue.GetInstance().OpenNextStage();

            float curScore = m_GameManager.GetScore();
            m_Score.SetOriginValue(curScore);
            m_Score.SetValue(curScore);


            float BonusScore = m_GameManager.GetBonusScore();
            m_BonusScore.SetOriginValue(BonusScore);
            m_BonusScore.SetValue(BonusScore);
        }
        else
        {
            m_BoardEnd.SetActive(true);
            m_BoardRegameButton.SetActive(true);
            m_BoardMainTitleButton.SetActive(true);
            float curScore = m_GameManager.GetScore();
            m_Score.SetOriginValue(curScore);
            m_Score.SetValue(curScore);
        }
    }
}
