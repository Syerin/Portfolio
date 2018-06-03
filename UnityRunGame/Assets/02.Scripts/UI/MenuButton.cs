using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MenuButton : MonoBehaviour
{
	// Use this for initialization
	void Start ()
    {
        gameObject.SetActive(false);
    }
    public void OnClickTitle()
    {
        MainValue.GetInstance().OnClickTitleScene();
    }

    public void OnClickReGame()
    {
        if(MainValue.GetInstance().m_GameMode == GAMEMODE.STAGEMODE)
        {
            int curStageNumber = MainValue.GetInstance().m_StageNumber;
            MainValue.GetInstance().OnClickStateMode(curStageNumber);
        }
        else
        MainValue.GetInstance().OnClickInfinityMode();
    }

    public void OnNextStage()
    {
        MainValue.GetInstance().OnClickNextStage();
    }
}
