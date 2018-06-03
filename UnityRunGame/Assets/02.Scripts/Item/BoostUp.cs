using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class BoostUp : Item
{
    private float m_fBoosterGageUp;

    // Update is called once per frame
    void Start()
    {
        if (isGetItem == false)
        {
            isTranslateAnim = true;
            isRotationAnim = true;
        }
    }
    override public void EatItem()
    {
        isGetItem = true;
        m_GameManager.UIItemMoved(ITEM.BOOSTUP);
        base.EatItem();
    }
}
