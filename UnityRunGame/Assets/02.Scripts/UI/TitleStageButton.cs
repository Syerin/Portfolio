using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TitleStageButton : MonoBehaviour
{
    public int m_iStageNumber;
    void Start()
    {
        if (m_iStageNumber <= MainValue.GetInstance().m_UserClearStageTopNumber)
        {
            gameObject.SetActive(false);
        }
    }
}
