using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Portal : MonoBehaviour
{
    public RoadManager m_RoadManager;
    public Player m_PlayerComponent;
    private float m_fTimer;
    private Vector3 m_vStartScale;
    private Vector3 m_vEndScale;

    private Quaternion m_OriginRot;
    private bool isAnimStart = false;
    public float m_fUpScaleSpeed;
    public GameManager m_GameManager;
    private Transform m_ThisTransform;
    public Transform GetTransform
    {
        get
        {
            return m_ThisTransform;
        }
    }

    private bool isBackMoved = false;
	// Use this for initialization
	void Start ()
    {
        m_ThisTransform = transform;
        gameObject.SetActive(false);
        m_OriginRot = transform.rotation;
        m_vStartScale = Vector3.zero;
        m_vEndScale = Vector3.one * 1.5f;
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (isBackMoved == true)
        {
            if (m_PlayerComponent.isBoosterMode == true)
                m_ThisTransform.Translate(-Vector3.back * m_GameManager.GetDeltaTime(m_PlayerComponent.GetBoostSpeed()));
            else if (m_PlayerComponent.isBoosterMode == false)
                m_ThisTransform.Translate(-Vector3.back * m_GameManager.GetDeltaTime(m_PlayerComponent.GetCurSpeed()));

            if(m_RoadManager.isLerpMovedEnd ==false)
            {
                m_ThisTransform.Translate(Vector3.right * m_RoadManager.GetHorizontalMoveDistance());
            }
            if(m_ThisTransform.position.z+2.0f <m_PlayerComponent.GetTransform.position.z )
            {
                Init();
            }
        }

        if (isAnimStart==true)
        {
            m_fTimer += m_GameManager.GetDeltaTime(m_fUpScaleSpeed );
            if (m_fTimer >= 1.0f)
            {
                m_fTimer = 0.0f;
                isAnimStart = false;
                m_ThisTransform.localScale = m_vEndScale;
                return;
            }
            m_ThisTransform.localScale = Vector3.Lerp(m_vStartScale, m_vEndScale, m_fTimer);
        }
    }

    public Vector2 GetSize()
    {
        return new Vector2(m_ThisTransform.localScale.x, m_ThisTransform.localScale.y);
    }
    public void Init()
    {
        m_ThisTransform.rotation = m_OriginRot;
        isAnimStart = false;
        m_fTimer = 0.0f;
        m_ThisTransform.localScale = m_vStartScale;
        gameObject.SetActive(false);
        isBackMoved = false;
    }

    public void AnimStart(bool Act)
    {
        gameObject.SetActive(true);
        isAnimStart = Act;
    }

    public void SetMovedBack(bool act)
    {
        isBackMoved = act;
    }
}
