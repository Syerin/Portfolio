using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIAnimation : MonoBehaviour
{ 
    public UIAnimation m_UIAnimNextObj = null;

    private Vector3 m_vOriginScale;
    private Vector3 m_vOriginPos;
    private Quaternion m_QuaOriginRot;


    public bool isEndInvisible =false;
    public Vector3 m_vStartSclae;
    public Vector3 m_vStartPosition;
    public Vector3 m_vStartRot;

    public Vector3 m_vEndScale;
    public Vector3 m_vEndPosition;
    public Vector3 m_vEndRot;

    private Quaternion m_QuaStartRot;
    private Quaternion m_QuaEndRot;
    public AnimationCurve m_AnimCurveScaling;
    public AnimationCurve m_AnimCurvePosition;
    public AnimationCurve m_AnimCurveRotation;

    public bool isScalingAnim = false;
    public bool isRotationAnim = false;
    public bool isPositionAnim = false;

    public float m_fSpeedAppearAnim = 0.0f;
    private float m_fTimer;

    public bool m_bAppearAnimation =false;
    // Use this for initialization
    void Start ()
    {
        m_vOriginPos = transform.localPosition;
        m_QuaOriginRot = transform.localRotation;
        m_vOriginScale = transform.localScale;

        m_QuaStartRot = Quaternion.Euler(m_vStartRot);
        m_QuaEndRot = Quaternion.Euler(m_vEndRot);
    }
	
	// Update is called once per frame
	void Update ()
    {
        AppearAnimation();
    }

    public bool GetAppearAnimEnd()
    {
        if ( m_UIAnimNextObj != null)
        {
            if (m_bAppearAnimation == true || m_UIAnimNextObj.GetAppearAnimEnd()==false)
            {
                return false;
            }
            else
                return true;
        }
        else
        {
            if (m_bAppearAnimation == true)
            {
                return false;
            }
            else
                return true;
        }
    }
    private void AppearAnimation()
    {
        if (m_bAppearAnimation == false)
            return;

        m_fTimer += Time.deltaTime * m_fSpeedAppearAnim;
        if (m_fTimer >= 1.0f)
        {
            AppearEndAnim();
            if (m_UIAnimNextObj != null)
                m_UIAnimNextObj.AppearAnimation(true);
            return;
        }
        AnimScaling();
        AnimRotation();
        AnimPosition();
    }

    private void AppearEndAnim()
    {
        m_bAppearAnimation = false;
        m_fTimer = 0.0f;

        if (isScalingAnim ==true)
        transform.localScale = m_vOriginScale;

        if(isRotationAnim ==true)
        transform.localRotation = m_QuaOriginRot;

        if(isPositionAnim==true)
        transform.localPosition = m_vOriginPos;

        if (isEndInvisible == true)
            gameObject.SetActive(false);
    }
    private void AnimScaling()
    {
        if (isScalingAnim == false)
            return;

        transform.localScale = Vector3.Lerp(m_vStartSclae, m_vEndScale, m_AnimCurveScaling.Evaluate(m_fTimer)); 
    }
    private void AnimPosition()
    {
        if (isPositionAnim == false)
            return;

        transform.localPosition = Vector3.Lerp(m_vStartPosition, m_vEndPosition, m_AnimCurvePosition.Evaluate(m_fTimer));
    }
    private void AnimRotation()
    {
        if (isRotationAnim== false)
            return;

        transform.localRotation= Quaternion.Slerp(m_QuaStartRot, m_QuaEndRot, m_AnimCurveRotation.Evaluate(m_fTimer));
    }

    public void AppearAnimation(bool act)
    {
        gameObject.SetActive(act);
        m_bAppearAnimation = act;
        if (act == false)
        {
            AppearEndAnim();
        }
    }

    public void SetAllInvisible()
    {
        gameObject.SetActive(false);
        if (m_UIAnimNextObj != null)
            m_UIAnimNextObj.SetAllInvisible();
    }
}
