using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Item : MonoBehaviour
{

    public GameManager m_GameManager;
    protected float m_fTimer;
    protected bool isGetItem = false;
    protected Player m_PlayerComponent;

    public bool isRotationAnim;
    public bool isScalingAnim;
    public bool isTranslateAnim;

    private float m_fRandomAnimSpeed;

    public float m_fRotateSpeed = 30.0f;
    public float m_fMovedSpeed = 1.0f;

    private Vector3 m_vOriginPos;
    private Vector3 m_vOriginScale;
    private Quaternion m_QuaOriginRot;


    private Vector3 m_vScaleStart;
    public Vector3 m_vScaleEnd;
    public Vector3 m_vTranslateStart;
    public Vector3 m_vTranslateEnd;


    private Transform m_ThisTransform;
    public Transform GetTransform
    {
        get
        {
            return m_ThisTransform;
        }
    }
    // Use this for initialization
    void Awake()
    {
        m_ThisTransform = transform;
           m_GameManager = GameObject.FindGameObjectWithTag("GameManager").GetComponent<GameManager>();
        m_PlayerComponent = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
        m_fRandomAnimSpeed = Random.Range(1.0f, 2.0f);
        m_vTranslateStart = m_ThisTransform.localPosition;
        m_vScaleStart = m_ThisTransform.localScale;
        m_vOriginPos = m_ThisTransform.localPosition;
        m_vOriginScale = m_ThisTransform.localScale;
        m_QuaOriginRot = m_ThisTransform.rotation;
        m_vTranslateEnd = new Vector3(m_ThisTransform.localPosition.x, Random.Range(1.0f, 2.5f), m_ThisTransform.localPosition.z);
        m_vScaleEnd = Vector3.one;
    }

    // Update is called once per frame
    void Update()
    {
        m_fTimer += m_GameManager.GetDeltaTime(m_fRandomAnimSpeed);

        Anim();
        if (isGetItem == false)
            return;
        if (m_fTimer >= 1.0f)
        {
            gameObject.SetActive(false);
            isGetItem = false;
            m_fTimer = 0.0f;
        }
    }

    virtual public void Init()
    {
        m_ThisTransform.localPosition = m_vOriginPos;
        m_ThisTransform.localScale = m_vOriginScale;
        m_ThisTransform.rotation = m_QuaOriginRot;

        isGetItem = false;
        m_fTimer = 0.0f;
        m_fRandomAnimSpeed = Random.Range(1.0f, 3.0f);

    }
    public void Anim()
    {
        if (isGetItem == true)
            return;
        if (isRotationAnim ==true)
            m_ThisTransform.Rotate(Vector3.up, m_GameManager.GetDeltaTime(m_fRotateSpeed ));
        if(isScalingAnim  ==true)
        {
            m_ThisTransform.localScale =  Vector3.Lerp(m_vScaleStart, m_vScaleEnd,m_fTimer);
        }
        if (isTranslateAnim == true)
        {
            m_ThisTransform.localPosition = Vector3.Lerp(m_vTranslateStart, m_vTranslateEnd, m_fTimer);
        }

        if(m_fTimer >= 1.0f)
        {
            Vector3 vTemp;
            vTemp = m_vScaleStart;
            m_vScaleStart = m_vScaleEnd;
            m_vScaleEnd = vTemp;

            vTemp = m_vTranslateStart;
            m_vTranslateStart = m_vTranslateEnd;
            m_vTranslateEnd = vTemp;
            m_fTimer = 0.0f;
        }
    }
    virtual public void EatItem()
    {
        Init();
        gameObject.SetActive(false);
        m_fTimer = 0.0f;
    }
}
