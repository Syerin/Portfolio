using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
 
public class ChangedFont : MonoBehaviour
{
    public Text m_UIText;
    private float m_fTimer = 0.0f;

    Color m_StartColor;
    Color m_EndColor;
    private void Start()
    {
        m_StartColor = GetRandomColor();
        m_EndColor = GetRandomColor();
    }
    private void Update()
    {
        m_fTimer += Time.deltaTime;
        m_UIText.color = Color.Lerp(m_StartColor, m_EndColor, m_fTimer);
        if(m_fTimer >= 1.5f)
        {
            m_StartColor = m_EndColor;
            m_EndColor = GetRandomColor();
            m_fTimer = 0.0f;
        }
    }
    private Color GetRandomColor()
    {
        Color makeColor;
        makeColor.r = Random.Range(0.0f,1.0f);
        makeColor.g = Random.Range(0.0f, 1.0f);
        makeColor.b = Random.Range(0.0f, 1.0f);
        makeColor.a = Random.Range(0.5f, 1.0f);
        return makeColor;
    }
}
