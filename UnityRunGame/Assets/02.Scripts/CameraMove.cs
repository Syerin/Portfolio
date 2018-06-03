using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMove : MonoBehaviour
{
    public Player m_PlayerComponent;
    public PLAYERSTATE m_curPlayerState;

    public GameObject[] m_CameraMovetoPlayer;

    public Vector3 m_vStartPosition;
    public Quaternion m_vStartRot;

    public Vector3 m_vEndPosition;
    public Quaternion m_vEndRot;

    public Vector3 m_vRunningEndPos;
    public Vector3 m_vJumpingEndPos;
    public Vector3 m_vSlidingEndPos;
    public Quaternion m_vRunningEndRot;
    public Quaternion m_vJumpingEndRot;
    public Quaternion m_vSlidingEndRot;

    public float m_fMoveTimer;
    public float m_MulfTimer;
    public bool m_bMovedCamera;

    public GameManager m_GameManager;
    public bool isNoTracingPlayer =false;
    public Vector3 m_vNoTracingPos;

    public float m_fangle = 0.0f;
    public bool isLookAtRotation =false;
    private bool m_bWolrdPointMoved =false;

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

    private void Start()
    {

        m_vRunningEndPos = m_CameraMovetoPlayer[(int)PLAYERSTATE.RUN].transform.localPosition;
        m_vJumpingEndPos = m_CameraMovetoPlayer[(int)PLAYERSTATE.JUMP].transform.localPosition;
        m_vSlidingEndPos = m_CameraMovetoPlayer[(int)PLAYERSTATE.SLIDING].transform.localPosition;

        m_vRunningEndRot = m_CameraMovetoPlayer[(int)PLAYERSTATE.RUN].transform.localRotation;
        m_vJumpingEndRot = m_CameraMovetoPlayer[(int)PLAYERSTATE.JUMP].transform.localRotation;
        m_vSlidingEndRot = m_CameraMovetoPlayer[(int)PLAYERSTATE.SLIDING].transform.localRotation;
    }
    // Update is called once per frame
    void Update()
    {
        if (m_PlayerComponent == null)
            return;
        if (isLookAtRotation == true)
        {
            SetArroundToPlayer();
            return;
        }

        if (isNoTracingPlayer == true)
        {
            m_ThisTransform.position = m_vNoTracingPos;
            Vector3 vDirect = m_PlayerComponent.GetTransform.position - m_ThisTransform.position;
            m_ThisTransform.rotation = Quaternion.LookRotation(vDirect);
            return;
        }

        MovedPlayerStateCamera();

    }
    public void MovedPlayerStateCamera()
    {
        if (m_bMovedCamera == true)
        {
            float fAnimSpeedPlayer = m_PlayerComponent.GetAnimSpeed();
            m_fMoveTimer += m_GameManager.GetDeltaTime(fAnimSpeedPlayer * 2.0f);
            if (m_fMoveTimer >= 1.0f)
            {
                SetEndPosState();
                m_bMovedCamera = false;
                m_fMoveTimer = 0.0f;
                if (m_bWolrdPointMoved == true)
                    m_bWolrdPointMoved = false;
                return;
            }
            MovedCamera();
            RotationCamera();
        }
    }
    public void SetEndPosState()
    {
        m_ThisTransform.localPosition = m_CameraMovetoPlayer[(int)m_curPlayerState].transform.localPosition;
        m_ThisTransform.localRotation = m_CameraMovetoPlayer[(int)m_curPlayerState].transform.localRotation;
    }
    public void StopCamera(bool Act)
    {
        if (Act == true && isNoTracingPlayer == false)
            m_vNoTracingPos = m_ThisTransform.position;

        isNoTracingPlayer = Act;
    }
    public void DiePlaceCamera()
    {
        m_ThisTransform.localPosition = m_CameraMovetoPlayer[(int)PLAYERSTATE.DIE].transform.localPosition;
        Vector3 vDirect = m_PlayerComponent.GetTransform.position - m_ThisTransform.localPosition;
        m_ThisTransform.rotation = Quaternion.LookRotation(vDirect);
    }

    public void MovedCamera()
    {
        if (m_bWolrdPointMoved == false)
            m_ThisTransform.localPosition = Vector3.Lerp(m_vStartPosition, m_vEndPosition, m_fMoveTimer);
        else
            m_ThisTransform.position = Vector3.Lerp(m_vStartPosition, m_vEndPosition, m_fMoveTimer);
    }
    public void RotationCamera()
    {
        m_ThisTransform.localRotation = Quaternion.Slerp(m_vStartRot, m_vEndRot, m_fMoveTimer);
    }
    public void isArroundingMove(bool act)
    {
        isLookAtRotation = act;
        m_ThisTransform.Rotate(Vector3.up, 180.0f);
    }
    public void SetArroundToPlayer()
    {
        if (isLookAtRotation == false)
            return;
        m_fangle += m_GameManager.GetDeltaTime(30.0f);
        m_ThisTransform.localPosition = new Vector3(0.0f,3.0f,0.0f);
        m_ThisTransform.localRotation = Quaternion.Euler(new Vector3(0.0f, 0.0f, 0.0f));
        m_ThisTransform.Rotate(Vector3.up, m_fangle);
        m_ThisTransform.Translate(Vector3.back * 3.0f);
        m_ThisTransform.LookAt(m_PlayerComponent.GetTransform);
    }


    public void SetStateCamMoving(PLAYERSTATE playerstate)
    {
        if (m_bWolrdPointMoved == true)
            return;
        m_curPlayerState = playerstate;
        m_vStartPosition = m_ThisTransform.localPosition;
        m_vEndPosition = m_CameraMovetoPlayer[(int)playerstate].transform.localPosition;

        m_vStartRot = m_ThisTransform.localRotation;
        m_vEndRot = m_CameraMovetoPlayer[(int)playerstate].transform.localRotation;
        m_bMovedCamera = true;
        m_fMoveTimer = 0.0f;
    }

    public void SetNoTracingPos(Vector3 Position)
    {
        m_vNoTracingPos = Position;
    }
    public void SetLerpMovedPointToWorldPoint(Vector3 StartPoint , Vector3 EndPoint)
    {
        m_bMovedCamera = true;
        m_vStartPosition = StartPoint;
        m_vEndPosition = EndPoint;
        m_fMoveTimer = 0.0f;
        m_bWolrdPointMoved = true;
    }
 }