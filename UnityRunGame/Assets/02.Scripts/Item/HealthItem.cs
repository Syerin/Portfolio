using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthItem : Item
{

    public int m_iIncreasedHealth = 1;

    void Start()
    {
        if (isGetItem == false)
        {
            isTranslateAnim = true;
        }
    }

    public override void EatItem()
    {
        m_GameManager.UIItemMoved(ITEM.HEALTHPOINT);
        base.EatItem();
    }
}
