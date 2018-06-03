using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjPool : MonoBehaviour
{
    private Dictionary<string, GameObject> m_PrefabList = new Dictionary<string, GameObject>();
    private Dictionary<string, List<GameObject>> m_ObjectList = new Dictionary<string, List<GameObject>>();

    protected void Init(int Reserve, GameObject Prefab)
    {
        if (m_PrefabList.ContainsKey(Prefab.name) == false)
        {
            m_PrefabList.Add(Prefab.name, Prefab);
            m_ObjectList.Add(Prefab.name, new List<GameObject>());
        }

        for(int i = 0; i < Reserve;i++)
        {
           GameObject obj = GameObject.Instantiate<GameObject>(Prefab);
            m_ObjectList[Prefab.name].Add(obj);
            obj.transform.SetParent(transform);
            obj.SetActive(false);
        }
    }

    protected GameObject GetObject(string name)
    {
        if (m_PrefabList.ContainsKey(name))
        {
            foreach (GameObject obj in m_ObjectList[name])
            {
                if(obj.activeSelf ==false)
                {
                    obj.SetActive(true);
                    return obj;
                }
            }
        }
        Init(m_ObjectList[name].Count, m_PrefabList[name]);

        return GetObject(name);
    }
}
