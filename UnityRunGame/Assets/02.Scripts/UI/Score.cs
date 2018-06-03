using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Score : MonoBehaviour
{
    public float m_fSpeed;
    public Text m_LabelText;

    public float m_fValue;

    public float m_fOriginValue;


    private bool isDecreaseStart = false;
	// Use this for initialization
	void Start ()
    {
        m_LabelText.text = "0";
    }
	
    public void StartDecreaseValue(bool act)
    {
        isDecreaseStart = act;
    }
    public bool isDecreased()
    {
        return isDecreaseStart;
    }

    public void SetOriginValue(float value)
    {
        m_fOriginValue = value;
        m_LabelText.text = string.Format("{0}", (int)value);
    }

    public void SetLabelValue(float Value)
    {
        m_fValue = Value;
        m_LabelText.text = string.Format("{0}", (int)m_fValue);
    }
    public void IncreasedValue(float value)
    {
        m_fValue += value;
        m_LabelText.text = string.Format("{0}", (int)m_fValue);
    }
    public void SetValue(float value)
    {
        m_fValue = value;
    }

    public int GetOriginValue()
    {
        return (int)m_fOriginValue;
    }

    public int GetcurValue()
    {
        return (int)m_fValue;
    }
    public float GetSpeed()
    {
        return m_fSpeed;
    }
    public bool DecreaseValue(float Time)
    {
        if (m_fValue - (Time * m_fSpeed)> -1.0f)
        {
            m_fValue -= Time * m_fSpeed;
            m_LabelText.text = string.Format("{0}", (int)m_fValue);
            return true;
        }
        else
        {
            m_fValue = 0.0f;
            m_LabelText.text = string.Format("{0}", (int)m_fValue);
            return false;
        }
    }
}
