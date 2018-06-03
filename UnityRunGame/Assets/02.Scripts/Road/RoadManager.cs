using System.Collections;
using System.Collections.Generic;
using UnityEngine;
[System.Serializable]
public class StageInfo
{
    public GameObject[] m_PrefabRoad;
    public int[] m_ArrStageRoadNumber;
}
public enum DIRECTION { FRONT,LEFT,RIGHT,END};
public class RoadManager : ObjPool
{
    public GameManager m_GameManager;

    public StageInfo[] m_StageInfo;

    public float m_oneSideValue;
    public int m_nObjectPool;
    public Player m_PlayerComponent;
    public float m_PlayerMovedSpeed;
    public GameObject[] m_PrefabRoad;
    public GameObject[] m_curRoad;


    public Vector3 m_vStartPos;
    public Vector3 m_vEndPos;
    public float m_fLerpMovedTimer = 0.0f;
    public bool isLerpMovedEnd=true;
    
    public float m_RoadOutDistance;
    private DIRECTION m_eDirection;
    private Transform m_ThisTransform;
    public Transform GetTransform
    {
        get
        {
            return m_ThisTransform;
        }
    }
    // Use this for initializationf
    void Start()
    {
        m_ThisTransform = transform;
        m_vStartPos = transform.position;
        m_vEndPos = transform.position;
        m_eDirection = DIRECTION.FRONT;
        if (MainValue.GetInstance().m_GameMode == GAMEMODE.STAGEMODE)
        {
            int StageNumber = Mathf.Max(0, MainValue.GetInstance().m_StageNumber - 1);
            StageNumber = Mathf.Min(StageNumber, m_StageInfo.Length - 1);
            int[] ArrStageRoadNumber = m_StageInfo[StageNumber].m_ArrStageRoadNumber;
            GameObject[] m_PrefabStageRoad = m_StageInfo[StageNumber].m_PrefabRoad;
            for (int i = 0; i < ArrStageRoadNumber.Length; i++)
            {
                int PrefabNumber = ArrStageRoadNumber[i];

                GameObject obj = GameObject.Instantiate<GameObject>(m_PrefabStageRoad[PrefabNumber]);
                obj.transform.parent = transform;

                m_curRoad[i] = obj;
                obj.GetComponent<Road>().m_curRoadNumber = i;

                if (i == 0)
                {
                    Vector3 vFirstPosition = m_PlayerComponent.transform.position;
                    vFirstPosition.y -= 1.9f;
                    obj.transform.position = vFirstPosition;
                }
            }
        }
        else if (MainValue.GetInstance().m_GameMode == GAMEMODE.INFINITYMODE)
        {
            for (int i = 0; i < m_PrefabRoad.Length; i++)
            {
                Init(m_nObjectPool, m_PrefabRoad[i]);
            }

            int OnewayNumber = 0;
            for (int i = 0; i < m_PrefabRoad.Length; i++)
            {
                if (m_PrefabRoad[i].GetComponent<Road>().m_eRoadType == ROADTYPE.ONEWAY)
                {
                    OnewayNumber = i;
                    break;
                }
            }
            for (int i = 0; i < 1;i++)
            {
                GameObject obj = GetObject(m_PrefabRoad[OnewayNumber].name);
                obj.transform.parent = transform;

                m_curRoad[i] = obj;
                obj.GetComponent<Road>().m_curRoadNumber = i;
            }
            for (int i = 1; i < m_curRoad.Length * 0.5f; i++)
            {
                int RandomIndex = Random.Range(0, m_PrefabRoad.Length);

                GameObject obj = GetObject(m_PrefabRoad[RandomIndex].name);
                obj.transform.parent = transform;

                m_curRoad[i] = obj;
                obj.GetComponent<Road>().m_curRoadNumber = i;
            }
            for (int i = (int)(m_curRoad.Length * 0.5f); i < m_curRoad.Length; i++)
            {
                GameObject obj = GetObject(m_PrefabRoad[Random.Range(0, m_PrefabRoad.Length)].name);
                obj.transform.parent = transform;
                m_curRoad[i] = obj;
                obj.GetComponent<Road>().m_curRoadNumber = i;
            }
        }
    }
    public void GenerateRoad()
    {
        for (int i = 0; i < m_curRoad.Length; i++)
        {
            if (m_curRoad[i] == null)
                continue;
            if (m_PlayerComponent.GetTransform.position.z < m_curRoad[i].transform.position.z)
                continue;
            if (m_curRoad[i].GetComponent<Road>().m_PrevRoadObject != null)
                continue;

            if (Vector3.Distance(m_curRoad[i].transform.position, m_PlayerComponent.GetTransform.position) >= 50.0f)
            {
                int RandomIndex = Random.Range(0, m_PrefabRoad.Length);
                GameObject obj = GetObject(m_PrefabRoad[RandomIndex].name);
                Road objRoadComponent = obj.GetComponent<Road>();
                if (objRoadComponent == null)
                    continue;
                objRoadComponent.Init();
                objRoadComponent.m_curRoadNumber = i;
                obj.transform.position = new Vector3(0.0f, 300.0f, -200.0f);


                GameObject PrevObj = m_curRoad[OutofCurIndex(i - 1)];
                Road PrevObjRoadComponent = PrevObj.GetComponent<Road>();
                int RandomPrevObjEndObject = Random.Range(0, PrevObjRoadComponent.m_EndObject.Length);
                objRoadComponent.SetPrevObject(PrevObjRoadComponent.m_EndObject[RandomPrevObjEndObject]);
                obj.SetActive(true);
                objRoadComponent.MovedRoad(0.0f);

                m_curRoad[i].GetComponent<Road>().Init();
                m_curRoad[i] = obj;
                m_curRoad[OutofCurIndex(i + 1)].GetComponent<Road>().m_PrevRoadObject = null;

                break;
            }
        }
    }


    // Update is called once per frame
    void Update()
    {
        if (m_GameManager.m_eGameState != GAMESTATE.PLAYING)
            return;

        if (m_PlayerComponent == null)
            return;
        if (m_PlayerComponent.m_PlayerState == PLAYERSTATE.DIE)
            return;
        if (m_PlayerComponent.m_PlayerState == PLAYERSTATE.FALLING)
            return;
        if (m_PlayerComponent.m_PlayerState == PLAYERSTATE.KNOCKDOWN)
            return;

        if (m_PlayerComponent.isBoosterMode == true)
        {
            m_PlayerMovedSpeed = m_PlayerComponent.GetBoostSpeed();
        }
        else
        m_PlayerMovedSpeed = m_PlayerComponent.GetCurSpeed();

        if (m_PlayerComponent.isDamageDown == true)
            return;
        SetPrevObject();
        MovedCurRoad();
        MovedLerpRoad();

        if (MainValue.GetInstance().m_GameMode == GAMEMODE.INFINITYMODE)
        {
            AttachingToPlayer();
            GenerateRoad();
        }
    }
    public Road GetcurRoadToNumber(int Number,int Offset = 0)
    {
        Road GetRoad = new Road();
        for(int i = 0; i < m_curRoad.Length;i++)
        {
            if (m_curRoad[i] == null)
                continue;
            GetRoad = m_curRoad[i].GetComponent<Road>();
            if (GetRoad == null)
                continue;
            if (GetRoad.m_curRoadNumber == Number + Offset)
                break;
        }
        return GetRoad;
    }
    public float GetHorizontalMoveDistance()
    {
        return transform.position.x - m_vStartPos.x; 
    }
    public void SetMovedLerpRoad(float fDist)
    {
        if (isLerpMovedEnd == false)
            return;

        if (fDist < 0.0f)
            m_eDirection = DIRECTION.LEFT;
        else if (fDist > 0.0f)
            m_eDirection = DIRECTION.RIGHT;
        m_vStartPos = transform.position;
        m_vEndPos = m_vStartPos;
        m_vEndPos.x += fDist;
        m_fLerpMovedTimer = 0.0f;
        isLerpMovedEnd = false;
    }
    public float HorizontalMovedTimer()
    {
        return m_fLerpMovedTimer;
    }
    private void MovedLerpRoad()
    {
        if (isLerpMovedEnd == true)
            return;
        if (m_fLerpMovedTimer >= 1.0f)
        {
            transform.position = m_vEndPos;
            isLerpMovedEnd = true;
            m_eDirection = DIRECTION.FRONT;
            return;
        }

        m_fLerpMovedTimer += m_GameManager.GetDeltaTime(m_PlayerMovedSpeed*0.5f);
        transform.position = Vector3.Lerp(m_vStartPos, m_vEndPos, m_fLerpMovedTimer);
    }
    public Vector3 MovedTranslateRoad(Vector3 vDir ,float fdist ,Space space  = Space.World)
    {
        transform.Translate(vDir * fdist, space);
        return transform.position;
    }

    public Vector3 GetLerpMoveStartPoint()
    {
        return m_vStartPos;
    }

    public void InitLerpMoved()
    {
        m_fLerpMovedTimer = 0.0f;
        isLerpMovedEnd = true;
    }
    private void SetPrevObject()
    {
        GameObject Firstobj = GetFirstObject();
        for (int i = 0; i < m_curRoad.Length; i++)
        {
            if (m_curRoad[i] == null)
                continue;
            Road RoadComponent = m_curRoad[i].GetComponent<Road>();
            if (Firstobj.GetComponent<Road>().m_curRoadNumber == m_curRoad[i].GetComponent<Road>().m_curRoadNumber)
                continue;
            if (RoadComponent.m_PrevRoadObject != null)
                continue;
            Road PrevRoad = m_curRoad[OutofCurIndex(i - 1)].GetComponent<Road>();

            int RandomIndex = Random.Range(0, PrevRoad.m_EndObject.Length);
            RoadComponent.SetPrevObject(PrevRoad.m_EndObject[RandomIndex]);

        }
    }
    private GameObject GetFirstObject()
    {
        GameObject obj = null;
        for (int i = 0; i < m_curRoad.Length; i++)
        {
            if (m_curRoad[i] == null)
                continue;
            Road RoadComponent = m_curRoad[i].GetComponent<Road>();
            if (RoadComponent == null)
                continue;
            if (RoadComponent.m_PrevRoadObject == null)
            {
                obj = m_curRoad[i];
                break;
            }
        }
        return obj;
    }
    private void AttachingToPlayer()
    {
        for (int i = 0; i < m_curRoad.Length; i++)
        {
            if (m_curRoad[i] == null)
                continue;
            Road RoadComponent = m_curRoad[i].GetComponent<Road>();
            RoadComponent.AttachingPlayer(m_PlayerComponent.GetTransform.position);
        }
    }

    private void MovedCurRoad()
    {
        for (int i = 0; i < m_curRoad.Length; i++)
        {
            if (m_curRoad[i] == null)
                continue;
            Road RoadComponent = m_curRoad[i].GetComponent<Road>();
            RoadComponent.MovedRoad(m_PlayerMovedSpeed);
        }
        
    }
    private int OutofCurIndex(int Index)
    {
        if (Index >= m_curRoad.Length)
            Index = 0;
        else if (Index < 0)
            Index = m_curRoad.Length - 1;
        return Index;
    }
    public bool GetHorizontalMoveEnd()
    {
        return isLerpMovedEnd;
    }
    public void SetHorizontalMoveEnd(bool isEnd)
    {
        isLerpMovedEnd = isEnd;
    }

    public DIRECTION GetDirection()
    {
        return m_eDirection;
    }
}
