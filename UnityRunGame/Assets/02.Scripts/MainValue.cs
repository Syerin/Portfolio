using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;

[System.Serializable]
public class StageScore
{
   public int m_iBestScore;
}

public class MainValue : MonoBehaviour
{
    private static MainValue instance = null;
    public  static MainValue GetInstance()
    {
        if(instance ==null)
        {
            instance = GameObject.FindObjectOfType(typeof(MainValue)) as MainValue;
        }
        return instance;
    }

    void Awake()
    {
        if(instance ==null)
        {
            instance = this;
        }
        else if(instance != this)
        {
            Destroy(gameObject);
            return;
        }

        DontDestroyOnLoad(transform.gameObject);

    }


    public int m_UserClearStageTopNumber;


    public int m_iBestScoreInfinityMode;
    public GAMEMODE m_GameMode = GAMEMODE.TITLE;
    public int m_StageNumber;

    public StageScore[] m_arrStageScore;

    public bool SetStageScore(int Score)
    {
        if (m_GameMode == GAMEMODE.STAGEMODE)
        {
            if (Score > m_arrStageScore[m_StageNumber - 1].m_iBestScore)
            {
                m_arrStageScore[m_StageNumber - 1].m_iBestScore = Score;
                return true;
            }
        }
        else if(m_GameMode == GAMEMODE.INFINITYMODE)
        {
            if (Score > m_iBestScoreInfinityMode)
            {
                Score = m_iBestScoreInfinityMode;
                return true;
            }
        }
        return false;
    }
    

    public void OpenNextStage()
    {
        if(GAMEMODE.STAGEMODE == m_GameMode)
        m_UserClearStageTopNumber += 1;
    }

    public void OnClickStageNumberScene(int Number)
    {
        m_GameMode = GAMEMODE.STAGEMODE;
        if (Number > m_UserClearStageTopNumber)
            return;
        m_StageNumber = Number;
        SceneManager.LoadScene("LoadingScene");
    }
    public void OnClickTitleScene()
    {   
        m_GameMode = GAMEMODE.TITLE;
        Debug.Log("타이틀 씬 로드!");

        SceneManager.LoadScene("LoadingScene");
    }
    public void OnClickNextStage()
    {
        m_GameMode = GAMEMODE.STAGEMODE;
        m_StageNumber+=1;
        SceneManager.LoadScene("LoadingScene");
    }
    public void OnClickStateMode(int StageNumber)
    {
        if(m_UserClearStageTopNumber < StageNumber)
        {
            return;
        }
        m_GameMode = GAMEMODE.STAGEMODE;
        m_StageNumber = StageNumber;
        Debug.Log("스테이지 씬 로드!");

        SceneManager.LoadScene("LoadingScene");
    }
    public void OnClickInfinityMode()
    {
        Debug.Log("무한 모드 씬 로드!");

        m_GameMode = GAMEMODE.INFINITYMODE;
        SceneManager.LoadScene("LoadingScene");
    }
}
