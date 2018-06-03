using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIItemMoved : MonoBehaviour
{
    public GameManager m_GameManager;
    public bool isMoved = false;
    public Vector3 m_vStartPosition;
    public Vector3 m_vEndPosition;
    public ITEM m_eItemType;
    public float m_fTimer;
    public GameObject m_Taget;
    public bool isTaget = false;

    private Transform m_ThisTransform;
    public Transform GetTransform
    {
        get
        {
            return m_ThisTransform;
        }
    }
    private void Awake()
    {
        m_ThisTransform = transform;
    }
    // Use this for initialization
    void Start ()
    {
        m_vStartPosition = new Vector3(0.5f * Screen.height , 0.5f * Screen.width);
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (m_GameManager == null)
            return;
		if(isMoved==true)
        {
            if(m_fTimer >= 1.0f)
            {

                if(m_eItemType == ITEM.BOOSTUP)
                {
                    m_GameManager.IncreaseBoosterUpGage(12.0f);
                }
                else if(m_eItemType ==ITEM.HEALTHPOINT)
                {
                    m_GameManager.IncreasedHealth(1);
                }
                gameObject.SetActive(false);
                isMoved = false;
                isTaget = false;
                return;
            }

            m_fTimer += m_GameManager.GetDeltaTime(1.2f);
            if(isTaget == false)
                m_ThisTransform.position = Vector3.Lerp(m_vStartPosition, m_vEndPosition, m_fTimer);
            else
            {
                m_vEndPosition = m_Taget.transform.position;
                m_ThisTransform.position = Vector3.Lerp(m_vStartPosition, m_vEndPosition, m_fTimer);
            }
        }
	}
    public void SetTaget(GameObject Taget)
    {
        isTaget = true;
        m_Taget = Taget;
    }
    public void MovingItem(ITEM type,Vector3 EndPostion)
    {
        isMoved = true;
        m_eItemType = type;
        m_vEndPosition = EndPostion;
        m_fTimer = 0.0f;
    }
}
