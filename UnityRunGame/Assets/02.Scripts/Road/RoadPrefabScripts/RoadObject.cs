using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum ROADOBJECTTYPE { ROAD,GATE,WALL,STAIR,TREE,BENCH,OBSTACLE,CONTAINER,END};

public class RoadObject : MonoBehaviour
{
    public bool isAnchorAdjust = false;
    public bool isExistChild = false;
    public bool isVisible = false;
    public bool isExistNextAnim = false;
    public bool m_EndVisibleAnim = false;
    public ROADOBJECTTYPE m_RoadObejctType;
    public Transform m_MidBottomwPoint;
    private float m_PlayerInSightDistance;
    private Quaternion m_OriginRot;
    private Quaternion m_AnimRot;
    public Vector3 m_OriginPos;
    public Vector3 m_AnimPos;
    private Vector3 m_OriginScale;
    private Vector3 m_AnimScale;
    private bool m_EndFirstAnim = false;
    private float m_fTimer;
    public float m_fCurDistance;
    private float m_fAnimSpeed = 2.0f;
    private bool m_bHitPlayer = false;
    private float m_fHitAnimTimer = 0.0f;
    private Vector3 m_vObstacleAnimEndPos;
    private Quaternion m_QuaObstacleAnimEndRot;
    private GameManager m_GameManager;
    private bool m_bHitAnimEnd = false;
    private float m_fHitAnimSpeed = 0.0f;
    void Start()
    {
        m_GameManager = GameObject.FindGameObjectWithTag("GameManager").GetComponent<GameManager>();

        m_OriginPos = transform.localPosition;
        m_AnimPos = transform.localPosition;
        m_OriginScale = transform.localScale;
        m_AnimScale = transform.localScale;
        m_OriginRot = transform.localRotation;
        m_AnimRot = transform.localRotation;
        m_QuaObstacleAnimEndRot = transform.localRotation;
        m_vObstacleAnimEndPos = transform.localPosition;
        m_fTimer = 0.0f;
        SetAnim();
    }

    public void SetAnim()
    {
        if (m_RoadObejctType == ROADOBJECTTYPE.ROAD)
        {
            float RandomX = Random.Range(-20.0f, 20.0f);
            float RandomY = Random.Range(-20.0f, 20.0f);

            if (RandomX < 0)
                RandomX -= 10.0f;
            else if (RandomX > 0)
                RandomX += 10.0f;

            m_AnimPos.x += RandomX;

            if (RandomY < 0)
                RandomY -= 5.0f;
            else if (RandomY > 0)
                RandomY += 5.0f;

            m_AnimPos.y += RandomY;
            m_AnimRot = Quaternion.Euler(new Vector3(Random.Range(0.0f, 360.0f), Random.Range(0.0f, 360.0f), Random.Range(0.0f, 360.0f)));
            m_AnimScale *= 2.0f;
        }
        if (m_RoadObejctType == ROADOBJECTTYPE.WALL)
        {
            m_AnimPos.y += 12.0f;
        }
        else if (m_RoadObejctType == ROADOBJECTTYPE.STAIR)
        {
            m_AnimRot = Quaternion.Euler(new Vector3(Random.Range(0.0f, 360.0f), Random.Range(0.0f, 360.0f), Random.Range(0.0f, 360.0f)));

            m_AnimPos.y += 7.0f;
            float RandomX = Random.Range(-10.0f, 10.0f);
            if (RandomX < 0)
                RandomX -= 5.0f;
            else if (RandomX > 0)
                RandomX += 5.0f;

            m_AnimPos.x += RandomX;

            isExistNextAnim = true;
        }
        else if (m_RoadObejctType == ROADOBJECTTYPE.TREE || m_RoadObejctType == ROADOBJECTTYPE.BENCH)
        {
            m_AnimScale *= 0.3f;
            m_AnimPos.y -= 2.0f;
            isExistNextAnim = true;
        }
        else if (m_RoadObejctType == ROADOBJECTTYPE.GATE)
        {
            m_AnimRot = Quaternion.Euler(new Vector3(0.0f, 0.0f, 90.0f));
            m_QuaObstacleAnimEndRot = Quaternion.Euler(new Vector3(0.0f, -90.0f, 0.0f));
        }
        else if (m_RoadObejctType == ROADOBJECTTYPE.CONTAINER)
        {
            m_AnimPos.y += 15.0f;

            m_vObstacleAnimEndPos.y -= 5.0f;
        }
        else if (m_RoadObejctType == ROADOBJECTTYPE.OBSTACLE)
        {
            m_AnimPos.y += 11.0f;

            m_QuaObstacleAnimEndRot = Quaternion.Euler(new Vector3(90.0f, 0.0f, 0.0f));
        }
    }
    public void SetInSightDistance(float InSightDistance)
    {
        m_PlayerInSightDistance = InSightDistance;
    }
    void Update()
    {
        VisibleAnimation();
        ObsTacleAnim();
    }

    public void InSightPlayer(Vector3 PlayerPos)
    {
        m_fCurDistance = Vector3.Distance(PlayerPos, transform.position);
        if (isVisible == true)
            return;
        if (m_PlayerInSightDistance * 1.2f >= m_fCurDistance && m_fCurDistance >= m_PlayerInSightDistance)
        {
            isVisible = true;
        }
    }
    public void GenerateInit()
    {
        m_fTimer = 0.0f;
        m_fHitAnimTimer = 0.0f;

        m_AnimPos = transform.localPosition;
        m_AnimScale = transform.localScale;
        m_AnimRot = transform.localRotation;
        isVisible = false;
        isExistNextAnim = false;
        m_EndVisibleAnim = false;
        m_bHitPlayer = false;
        m_bHitAnimEnd = false;
        SetAnim();
    }
    public void Init()
    {
        transform.localRotation = m_OriginRot;
        transform.localPosition = m_OriginPos;
        transform.localScale = m_OriginScale;
        m_fTimer = 0.0f;
    }
    public bool AnimGATE()
    {
        if (m_RoadObejctType != ROADOBJECTTYPE.GATE)
            return false;
        transform.localRotation = Quaternion.Slerp(m_AnimRot, m_OriginRot, m_fTimer);

        return false;
    }

    public bool AnimRoad()
    {
        if (m_RoadObejctType != ROADOBJECTTYPE.ROAD)
            return false;
        transform.localRotation = Quaternion.Slerp(m_AnimRot, m_OriginRot, m_fTimer);
        transform.localScale = Vector3.Lerp(m_AnimScale, m_OriginScale, m_fTimer);
        transform.localPosition = Vector3.Lerp(m_AnimPos, m_OriginPos, m_fTimer);

        return false;
    }
    public bool AnimStair()
    {
        if (m_RoadObejctType != ROADOBJECTTYPE.STAIR)
            return false;
        transform.localRotation = Quaternion.Slerp(m_AnimRot, m_OriginRot, m_fTimer);
        transform.localPosition = Vector3.Lerp(m_AnimPos, m_OriginPos, m_fTimer);

        return false;
    }

    public bool AnimTree()
    {
        if (m_RoadObejctType != ROADOBJECTTYPE.TREE)
            return false;
        if (m_EndFirstAnim == true)
        {
            transform.localScale = Vector3.Lerp(m_AnimScale, m_OriginScale, m_fTimer);
        }
        else if (m_EndFirstAnim == false)
        {
            transform.localPosition = Vector3.Lerp(m_AnimPos, m_OriginPos, m_fTimer);
        }

        return false;
    }
    public bool AnimWall()
    {
        if (m_RoadObejctType != ROADOBJECTTYPE.WALL)
            return false;
        transform.localPosition = Vector3.Lerp(m_AnimPos, m_OriginPos, m_fTimer);

        return false;
    }
    public bool AnimBench()
    {
        if (m_RoadObejctType != ROADOBJECTTYPE.BENCH)
            return false;

        if (m_EndFirstAnim == true)
        {
            transform.localScale = Vector3.Lerp(m_AnimScale, m_OriginScale, m_fTimer);
        }
        else
        {
            transform.localPosition = Vector3.Lerp(m_AnimPos, m_OriginPos, m_fTimer);
        }

        return false;
    }

    public void VisibleAnimation()
    {
        if (m_EndVisibleAnim == true)
            return;
        if (isVisible == true)
        {
            m_fTimer += m_GameManager.GetDeltaTime(m_fAnimSpeed);

            AnimRoad();
            AnimBench();
            AnimWall();
            AnimTree();
            AnimStair();
            AnimGATE();
            if (m_fTimer >= 1.0f)
            {
                if (isExistNextAnim == true && m_EndFirstAnim == false)
                {
                    m_EndFirstAnim = true;
                    m_fTimer = 0.0f;
                    return;
                }
                Init();
                m_EndVisibleAnim = true;
            }
        }
    }
    private void ObsTacleAnim()
    {
        if (m_bHitPlayer == false)
            return;
        if (m_bHitAnimEnd == true)
            return;
        m_fHitAnimTimer += m_GameManager.GetDeltaTime(0.5f * m_fHitAnimSpeed);

        if (m_fHitAnimTimer >= 1.0f)
        {
            m_fHitAnimTimer = 0.0f;
            m_bHitAnimEnd = true;
            return;
        }

        HitAnimGate();
        HitAnimContainer();
        HitAnimObstacle();
    }
    public void SetHitAnimSpeed(float fSpeedvalue)
    {
        m_fHitAnimSpeed= fSpeedvalue;
    }

    private void HitAnimGate()
    {
        if (ROADOBJECTTYPE.GATE != m_RoadObejctType)
            return;

        transform.localRotation = Quaternion.Slerp(m_OriginRot, m_QuaObstacleAnimEndRot, m_fHitAnimTimer);
    }
    private void HitAnimContainer()
    {
        if (ROADOBJECTTYPE.CONTAINER != m_RoadObejctType)
            return;
        transform.localPosition = Vector3.Lerp(m_OriginPos, m_vObstacleAnimEndPos, m_fHitAnimTimer);
    }
    private void HitAnimObstacle()
    {
        if (ROADOBJECTTYPE.OBSTACLE != m_RoadObejctType)
            return;
        transform.localRotation = Quaternion.Slerp(m_OriginRot, m_QuaObstacleAnimEndRot, m_fHitAnimTimer);
    }

    public bool GetPrevHitPlayer()
    {
        return m_bHitPlayer;
    }
    public void SetHittingPlayer(bool Hit)
    {
        m_bHitPlayer = Hit;
        m_bHitAnimEnd = false;
    }
}


