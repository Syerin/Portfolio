using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum ROADTYPE { ONEWAY,STAIRDOWN,STAIRUP,GARDEN,FINISHLINE,END};

public class Road : MonoBehaviour
{
    public bool isInSightPlayer = false;
    public int m_curRoadNumber;
    public ROADTYPE m_eRoadType;
    public float RoadInSightDistance;
    public float ObjectInSightDistance;

    public GameObject[] m_RoadObject;

    public GameObject m_StartObject;
    public GameObject[] m_EndObject;

    public Vector3 m_vStartPosition;
    public Vector3[] m_vEndPosition;
    public Vector3 m_PrevRoadEndPosition;
    public Vector3 m_vCurPosition;

    public GameManager m_GameManager;
    public GameObject m_PrevRoadObject ;


    public int m_PattenItem;
    public GameObject[] m_PrefabItem;

    public List<Item> m_PrefabListItemComponent = new List<Item>();
    public List<RoadObject> m_PrefabListRoadObjectComponent = new List<RoadObject>();

    void Awake()
    {
        isInSightPlayer = false;
        m_GameManager = GameObject.FindGameObjectWithTag("GameManager").GetComponent<GameManager>();
    }
    void Start()
    {
        m_vEndPosition = new Vector3[m_EndObject.Length];

        for (int i = 0; i < m_PrefabItem.Length; i++)
            m_PrefabListItemComponent.Add(m_PrefabItem[i].GetComponent<Item>());
        for(int i = 0; i < m_RoadObject.Length; i++ )
        {
            if (m_RoadObject == null)
            {
                Debug.Log(gameObject.name);

                Debug.Log("오브젝트가 비어있습니다. 인스펙터 창을 확인하세여.");
            }
            RoadObject ObjRoadComponent = m_RoadObject[i].GetComponent<RoadObject>();
            if (ObjRoadComponent == null)
                continue;
            ObjRoadComponent.SetInSightDistance(ObjectInSightDistance);
            m_PrefabListRoadObjectComponent.Add(ObjRoadComponent);
        }
    }
    public bool CheckingEndAnimRoadObject(ROADOBJECTTYPE type)
    {
        for(int i = 0; i <m_PrefabListRoadObjectComponent.Count;i++)
        {
            if (m_PrefabListRoadObjectComponent[i].m_RoadObejctType== type&&
                m_PrefabListRoadObjectComponent[i].m_EndVisibleAnim == false)
            {
                return false;
            }
        }
        return true;
    }
    public void Init()
    {
        isInSightPlayer = false;
        m_curRoadNumber = 0;
        m_PrevRoadObject = null;
        for(int i = 0; i < m_PrefabItem.Length;i++)
        {
            m_PrefabItem[i].SetActive(true);
        }
        for(int i = 0; i < m_PrefabListItemComponent.Count;i++)
        {
            m_PrefabListItemComponent[i].Init();
        }
        for (int i = 0; i < m_PrefabListRoadObjectComponent.Count;i++)
        {
            m_PrefabListRoadObjectComponent[i].GenerateInit();
        }
        gameObject.SetActive(false);
    }
    public void MovedRoad(float fMovedSpeed)
    {
        Vector3 vTemp = transform.position;

        vTemp.z -= m_GameManager.GetDeltaTime(fMovedSpeed);
        transform.position = vTemp;
    }

    public void SetPrevObjectPosition(Vector3 LastPosition)
    {
        LastPosition.z += Mathf.Abs(m_StartObject.transform.localPosition.z);
        LastPosition.y += Mathf.Abs(m_StartObject.transform.localPosition.y);
        m_PrevRoadEndPosition = LastPosition;
        transform.position = LastPosition;
    }

    public void SetPrevObject(GameObject PrevObj)
    {
        if(m_PrevRoadObject == null)
        {
            m_PrevRoadObject = PrevObj;
        }
    }
    
    void Update()
    {
        m_vStartPosition = m_StartObject.transform.position;
        for(int i = 0; i < m_EndObject.Length;i++)
        m_vEndPosition[i] = m_EndObject[i].transform.position;

        if (m_PrevRoadObject != null)
            SetPrevObjectPosition(m_PrevRoadObject.transform.position);
        //VisibleObject();

        if (m_eRoadType == ROADTYPE.STAIRDOWN ||
            m_eRoadType == ROADTYPE.STAIRUP)
            return;
    }
    //public void SetLineActive(ROADOBJECTTYPE objType)
    //{
    //    if (CheckingEndAnimRoadObject(objType))
    //    {
    //        for (int i = 0; i < m_PrefabListRoadObjectComponent.Count; i++)
    //        {
    //            if (m_PrefabListRoadObjectComponent[i].m_RoadObejctType == objType)
    //                m_PrefabListRoadObjectComponent[i].gameObject.SetActive(false);
    //        }
    //        for (int i = 0; i < m_PrefabLine.Length; i++)
    //        {
    //            m_PrefabLine[i].SetActive(true);
    //        }
    //    }
    //}

    public bool AttachingPlayer(Vector3 PlayerPos)
    {

        for (int i = 0; i < m_PrefabListRoadObjectComponent.Count; i++)
        {
            m_PrefabListRoadObjectComponent[i].InSightPlayer(PlayerPos);
        }

        return true;
    }

    public Vector3 GetStartPointToWorld()
    {
        return m_StartObject.transform.position;
    }
}
