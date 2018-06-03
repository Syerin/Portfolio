using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonStageMode : MonoBehaviour
{
    private bool isActive = false;
    public GameObject m_ObjScrollView;
    public UIAnimation m_UIAnimObjScrollView;

    public void OnScrollView()
    {
        isActive = !isActive;
        m_ObjScrollView.SetActive(isActive);
        m_UIAnimObjScrollView.AppearAnimation(isActive);
    }
}
