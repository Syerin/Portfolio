using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public enum ITEM { HEALTHPOINT,BOOSTUP};
public class UIItem : ObjPool
{
    public GameManager m_GameManager;
    public GameObject[] m_PrefabItem;

    void Awake()
    {
        Init(50, m_PrefabItem[0]);
        Init(200, m_PrefabItem[1]);


    }

    public void GetItem(ITEM ItemNumber, Vector3 EndPos,GameObject Taget = null)
    { 
        GameObject PrefabItem =  GetObject(m_PrefabItem[(int)ItemNumber].name);
        UIItemMoved UIItemComponent =  PrefabItem.GetComponent<UIItemMoved>();
        UIItemComponent.m_GameManager = m_GameManager;
        UIItemComponent.MovingItem(ItemNumber,EndPos);

        if(Taget != null)
        {
            UIItemComponent.SetTaget(Taget);
        }

    }
}
