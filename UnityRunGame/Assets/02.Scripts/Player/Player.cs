using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum PLAYERSTATE { RUN, JUMP, SLIDING, KNOCKDOWN, FALLING,DIE,END };

public class Player : MonoBehaviour
{
    private Road m_NextRoad;
    public Portal m_PortalComponent;
    public int m_OnCurRoadNubmer;
    [Range(0, 36)]
    public float m_fMaxMovedSpeed;
    public float m_fCurSpeed = 0.0f;
    public AnimationCurve m_AnimCurMoveSpeed;
    public float m_fMovedSpeedTimer;

    public float m_fJumpSpeed;
    public float m_fBoostSpeed;

    [Range(0, 4)]
    public float m_fAnimSpeed;
    public float m_fMaxAnimSpeed;
    public float m_fAnimSpeedTimer;
    [Range(0.0f, 30.0f)]
    public float m_fJumpPower;

    public GameObject m_PaticleBoost;

    public CheckBottom m_CheckLeft;
    public CheckBottom m_CheckRight;

    public Vector3 m_vJumpPos;
    public Vector3 m_vJumpStartPos;
    public AnimationCurve m_JumpPositionCurve;
    public float m_fBottomObjDistance;
    public bool isJumping = false;
    public bool isSliding = false;
    public bool isDamageDown = false;
    public bool isRunning = false;
    public RoadManager m_RoadManager;
    public Animator m_Animator;
    public PLAYERSTATE m_PlayerState = PLAYERSTATE.RUN;
    public CapsuleCollider m_PlayerCollider;
    private Vector3 m_ColliderSlidingPostion;
    private Vector3 m_ColliderRunPostion;
    public GameManager m_GameManager;

    public CameraMove m_MainCameraMove;

    public bool isBoosterMode = false;

    public float m_fOneSideValue;
    public float m_fJumpTimer = 0.0f;
    public bool isJumpDown = false;

    private bool isPrevDamageDown = false;
    private float m_fRunningAnimSpeed = 0.0f;

    private Transform m_ThisTransform;
    public Transform GetTransform
    {
        get
        {
            return m_ThisTransform;
        }
    }

    [Range(-1.0f, 1.0f)]
    private float m_fWalkTreeValue = 0;
    [Range(-1.0f, 1.0f)]
    private float m_fRunTreeValue = 0;
    // Use this for initialization
    private void Awake()
    {
        m_ThisTransform = transform;
        m_PlayerCollider = GetComponent<CapsuleCollider>();

    }
    void Start()
    {
        m_Animator.speed = m_fAnimSpeed;
        m_ColliderRunPostion = m_PlayerCollider.center;
        m_ColliderSlidingPostion = m_PlayerCollider.center;

        m_ColliderSlidingPostion.y -= 0.7f;
        SetState(PLAYERSTATE.RUN);
    }



    // Update is called once per frame
    void Update()
    {
        m_ThisTransform = transform;
        m_Animator.speed = m_fAnimSpeed * m_GameManager.GetMulDeltaTime();
        if (m_GameManager.m_eGameState != GAMESTATE.PLAYING)
        {
            if (m_GameManager.m_eGameState == GAMESTATE.WIN ||
                m_GameManager.m_eGameState == GAMESTATE.DIE)
            {
                SetBoostMode(false);
                HitCollider();
            }
            return;
        }
        m_fBoostSpeed = m_fCurSpeed * 2.0f;
        m_Animator.SetFloat("PlayerSpeed", m_fCurSpeed);
        m_Animator.SetBool("isBoostMode", isBoosterMode);
        if (isBoosterMode == true)
            m_fAnimSpeed = 1.3f;
        else
        {
            if (m_fAnimSpeed > 1.0f)
                m_fAnimSpeed -= m_GameManager.GetDeltaTime();
            if (m_fAnimSpeed <= 1.0f)
                m_fAnimSpeed = 1.0f;
        }

        RunningAnim();
        if (FallingCheckBottom() ==false)
            return;

        CheckInput();
        Jump();
        ChangeAnimation();
        HitCollider();
        IncreseBoosterGage();
        GetOnNextRoad();
        HitFrontColliderThreshold();
        DiePlayer();
    }

    private bool FallingCheckBottom()
    {
        if (m_PlayerState == PLAYERSTATE.FALLING)
        {
            m_ThisTransform.Translate(-Vector3.up * 10.0f * Time.deltaTime, Space.World);
            HitFallingBox();
            HitRoad();
            return false;
        }
        return true;
    }
    private void DiePlayer()
    {
        if (GetPlayingAnimNormalTime("Down") >= 0.8f)
        {
            if (m_GameManager.m_iLife == 0)
            {
                SetState(PLAYERSTATE.DIE);
            }
        }
    }

    private void RunningAnim()
    {
        if (m_PlayerState != PLAYERSTATE.KNOCKDOWN || m_PlayerState != PLAYERSTATE.FALLING)
        {
            if (isPrevDamageDown == false)
            {
                m_fMovedSpeedTimer += m_GameManager.GetDeltaTime(0.15f);
                if (m_fMovedSpeedTimer >= 1.0f)
                    m_fMovedSpeedTimer = 1.0f;
                m_fCurSpeed = Mathf.Lerp(0.0f, m_fMaxMovedSpeed, m_AnimCurMoveSpeed.Evaluate(m_fMovedSpeedTimer));

            }
        }

        if (isRunning == true)
        {
            if (isPrevDamageDown == true)
            {
                m_fAnimSpeedTimer = 0.0f;
                isPrevDamageDown = false;
            }
            m_fAnimSpeedTimer += m_GameManager.GetDeltaTime(0.15f);
            if (m_fAnimSpeedTimer >= 1.0f)
                m_fAnimSpeedTimer = 1.0f;

            m_fRunningAnimSpeed = Mathf.Lerp(0.0f, m_fMaxAnimSpeed, m_AnimCurMoveSpeed.Evaluate(m_fAnimSpeedTimer));
            m_Animator.SetFloat("RunAnimSpeed", m_fRunningAnimSpeed);

            float fHorizontalMovedTimer = m_RoadManager.HorizontalMovedTimer();
            DIRECTION eDirection = m_RoadManager.GetDirection();

            SetDirectionMove(eDirection);
        }
    }

    private void SetDirectionMove(DIRECTION Dir)
    {
        if (DIRECTION.FRONT == Dir)
        {
            m_fWalkTreeValue = 0;
            m_fRunTreeValue = 0;

            m_Animator.SetFloat("WalkDirection", m_fWalkTreeValue);
            m_Animator.SetFloat("RunDirection", m_fRunTreeValue);
        }

        if (m_RoadManager.GetHorizontalMoveEnd() == true)
            return;

        if(DIRECTION.LEFT ==Dir)
        {
            m_fWalkTreeValue = 1;
            m_fRunTreeValue = 1;
            m_Animator.SetFloat("WalkDirection", m_fWalkTreeValue);
            m_Animator.SetFloat("RunDirection", m_fRunTreeValue);
        }
        else if(DIRECTION.RIGHT ==Dir)
        {
            m_fWalkTreeValue = -1;
            m_fRunTreeValue = -1;
            m_Animator.SetFloat("WalkDirection", m_fWalkTreeValue);
            m_Animator.SetFloat("RunDirection", m_fRunTreeValue);
        }
    }
    private void OnTriggerEnter(Collider other)
    {
        if (other.transform.tag == "Enemy")
        {
            RoadObject EnmeyRoadObjComponent = other.GetComponent<RoadObject>();
            if (EnmeyRoadObjComponent == null)
                return;
            bool PrevHitPlaeyr = EnmeyRoadObjComponent.GetPrevHitPlayer();
            if (PrevHitPlaeyr == true)
                return;

            EnmeyRoadObjComponent.SetHittingPlayer(true);

            if (isBoosterMode == true)
                EnmeyRoadObjComponent.SetHitAnimSpeed(GetBoostSpeed());
            else
                EnmeyRoadObjComponent.SetHitAnimSpeed(GetCurSpeed());
            SetBoostMode(false);
            SetState(PLAYERSTATE.KNOCKDOWN);
        }
        if(other.transform.tag == "FallingBox")
        {
            TrrigerEnterFallingBox();
        }

        if(other.transform.tag == "Item")
        {
            Item ObjItemComponent = other.GetComponent<Item>();
            ObjItemComponent.EatItem();
        }

        if(other.transform.tag == "FinishLine")
        {
            m_MainCameraMove.isArroundingMove(true);
            m_GameManager.SetGameState(GAMESTATE.WIN);
            m_Animator.SetBool("isWin", true);
            m_GameManager.VisibleScoreboard();
        }
    }
    public void TrrigerEnterFallingBox()
    {

        m_RoadManager.InitLerpMoved();
        m_RoadManager.GetTransform.position = new Vector3(m_RoadManager.GetLerpMoveStartPoint().x, 0.0f);

        Vector3 LastRoadNextPos = m_NextRoad.GetStartPointToWorld();
        m_ThisTransform.rotation = Quaternion.Euler(Vector3.zero);
        LastRoadNextPos.y += 10.0f;
        LastRoadNextPos.z += 1.0f;
        m_ThisTransform.position = LastRoadNextPos;

        Vector3 curPos = m_ThisTransform.position;

        m_PortalComponent.Init();
        m_PortalComponent.AnimStart(true);
        m_PortalComponent.GetTransform.position = curPos;

        Vector3 CamPosStopPos = curPos;
        CamPosStopPos.y += 5.0f;
        CamPosStopPos.z -= 7.0f;
        m_MainCameraMove.SetNoTracingPos(CamPosStopPos);
    }
    private void GetOnNextRoad()
    {
        if (m_PlayerState == PLAYERSTATE.FALLING)
            return;
        Vector3 RayPosition = m_ThisTransform.position;
        RayPosition.y += 20.0f;
        Ray DownRay = new Ray(RayPosition, -Vector3.up);
        RaycastHit hit;
        Debug.DrawRay(RayPosition, -Vector3.up * 10.0f, Color.red);

        if (Physics.Raycast(DownRay, out hit, Mathf.Infinity,1<<LayerMask.NameToLayer("Road")))
        {
            if (hit.collider.tag == "Enemy" && hit.collider.tag =="FallingBox")
            {
                return;
            }
            else
            {
                Road hitRoad = hit.collider.GetComponent<Road>();
                if (hitRoad == null)
                    return;
                m_OnCurRoadNubmer = hitRoad.m_curRoadNumber;
                m_NextRoad = m_RoadManager.GetcurRoadToNumber(m_OnCurRoadNubmer, 1);
            }
        }
    }
    private void CheckInput()
    {
        if (PLAYERSTATE.KNOCKDOWN == m_PlayerState)
            return;
        if (Input.GetKeyDown(KeyCode.DownArrow))
        {
            if (m_PlayerState != PLAYERSTATE.SLIDING)
            {
                SetState(PLAYERSTATE.SLIDING);
            }
        }
        if (Input.GetKeyDown(KeyCode.Space))
        {
            if (m_PlayerState != PLAYERSTATE.JUMP)
            {
                SetState(PLAYERSTATE.JUMP);
            }
        }
        if (Input.GetKeyUp(KeyCode.Z))
        {
                SetBoostMode(true);
        }
        if (m_PlayerState == PLAYERSTATE.RUN)
        {
            if (Input.GetKeyDown(KeyCode.LeftArrow))
            {
                if (m_CheckLeft.m_ExistBottom == false)
                    return;

                m_RoadManager.SetMovedLerpRoad(m_fOneSideValue);
                HitCollider();
            }
            if (Input.GetKeyDown(KeyCode.RightArrow))
            {
                if (m_CheckRight.m_ExistBottom == false)
                    return;

                m_RoadManager.SetMovedLerpRoad(-m_fOneSideValue);

                HitCollider();
            }
        }
    }

    private void HitFrontColliderThreshold()
    {
        Ray frontRay = new Ray(transform.position, Vector3.forward);
        RaycastHit hit;
        Debug.DrawRay(transform.position, Vector3.forward * 4.5f, Color.blue);
        if(Physics.Raycast(frontRay,out hit))
        {
            if (hit.collider.tag == "Enemy")
                return;

            if(hit.collider.tag == "Threshold"&&
                hit.distance <= 4.5f)
            {
                if (m_PortalComponent.gameObject.activeSelf == false)
                {
                    Vector3 vPotalPos = hit.point;
                    vPotalPos.y += 1.7f;
                    m_PortalComponent.Init();
                    m_PortalComponent.AnimStart(true);
                    m_PortalComponent.SetMovedBack(true);
                    m_PortalComponent.transform.position = vPotalPos;
                    m_PortalComponent.transform.Rotate(Vector3.right, 90.0f);
                }
            }
        }
    }

    public void SetBoostMode(bool Act)
    {
        if (Act == true)
        {
            if (m_GameManager.m_fCurBooster >= m_GameManager.m_fMaxBooster)
            {
                isBoosterMode = true;
                m_GameManager.SetBoostUpGage(0.0f);
                m_PaticleBoost.SetActive(true);
            }
        }
        else
        {
            isBoosterMode = false;
            m_PaticleBoost.SetActive(false);
        }
    }
    private void Jump()
    {
        if (m_PlayerState == PLAYERSTATE.JUMP)
        {
            float curAnim = GetPlayingAnimNormalTime("Jump");

            if (isPlayingEndAnimation("Jump", 0.7f) && m_fBottomObjDistance <= 20.0f)
            {
                SetState(PLAYERSTATE.RUN);
                return;
            }
            CheckJumpBootom(curAnim);
        }
    }
    private void CheckJumpBootom(float AnimNormalTime)
    {
        Vector3 RayPosition = m_ThisTransform.position;
        float CharacterTop = 20.0f;
        RayPosition.y += CharacterTop;
        Ray DownRay = new Ray(RayPosition, -Vector3.up);
        RaycastHit hit;
        Debug.DrawRay(RayPosition, -Vector3.up * CharacterTop, Color.red);

        Vector3 curPosition = m_ThisTransform.position;
        if (Physics.Raycast(DownRay, out hit, Mathf.Infinity, 1 << LayerMask.NameToLayer("Road")))
        {
            if (hit.collider.tag == "Enemy")
            {
                return;
            }
            m_fBottomObjDistance = hit.distance;

            if (m_fBottomObjDistance >= CharacterTop && AnimNormalTime >= 0.7f)
            {
                if(hit.collider.tag == "FallingBox" &&
                    hit.distance <= CharacterTop +8.0f)
                {
                    SetState(PLAYERSTATE.FALLING);
                    return;
                }


                isJumpDown = true;
                if (m_fBottomObjDistance <= CharacterTop + 0.6f)
                {
                    m_Animator.SetFloat("JumpAnimSpeed", m_fAnimSpeed * 1.5f);
                }
                else
                    m_Animator.SetFloat("JumpAnimSpeed", m_fAnimSpeed * 0.2f);

                if (isBoosterMode == false)
                    curPosition.y -= m_GameManager.GetDeltaTime(m_fCurSpeed * 2.0f);
                else
                    curPosition.y -= m_GameManager.GetDeltaTime(m_fBoostSpeed * 2.0f);

            
                Vector3 JumpDownPos = curPosition;
                JumpDownPos.y += CharacterTop;
                Ray JumpdownRay = new Ray(JumpDownPos, -Vector3.up);
                RaycastHit DownHit;
                Debug.DrawRay(JumpDownPos, -Vector3.up * 20.0f, Color.blue);
                if (Physics.Raycast(JumpdownRay, out DownHit, Mathf.Infinity, 1 << LayerMask.NameToLayer("Road")))
                {
                    if (hit.collider.tag == "Enemy")
                    {
                        return;
                    }
                    if (DownHit.distance >= CharacterTop)
                    {
                        m_ThisTransform.position = curPosition;
                    }
                    else if (DownHit.distance < CharacterTop)
                    {
                        curPosition.y += (CharacterTop - DownHit.distance);
                        m_ThisTransform.position = curPosition;
                        return;
                    }
                }
            }
            if (m_fBottomObjDistance < CharacterTop)
            {
                curPosition.y += (CharacterTop - hit.distance);
                m_ThisTransform.position = curPosition;
                return;
            }
            if (isJumpDown == false)
            {
                float AnimNormal = Mathf.Min(1.0f, AnimNormalTime);

                curPosition = Vector3.Lerp(m_vJumpStartPos, m_vJumpPos, m_JumpPositionCurve.Evaluate(AnimNormal));
                m_fJumpSpeed = m_fCurSpeed * (m_JumpPositionCurve.Evaluate(AnimNormal) + 0.5f);


                Vector3 JumpDownPos = curPosition;
                JumpDownPos.y += CharacterTop;
                Ray JumpdownRay = new Ray(JumpDownPos, -Vector3.up);
                RaycastHit DownHit;
                Debug.DrawRay(JumpDownPos, -Vector3.up * 40.0f, Color.green);
                if (Physics.Raycast(JumpdownRay, out DownHit, Mathf.Infinity, 1 << LayerMask.NameToLayer("Road")))
                {
                    if (hit.collider.tag == "Enemy")
                    {
                        return;
                    }
                    if (DownHit.distance >= CharacterTop)
                    {
                        m_ThisTransform.position = curPosition;
                    }
                    else if (DownHit.distance < CharacterTop)
                    {
                        curPosition.y += (CharacterTop - DownHit.distance);
                        m_ThisTransform.position = curPosition;
                        return;
                    }
                }

            }

        }
    }

    private void IncreseBoosterGage()
    {
        if (m_PlayerState != PLAYERSTATE.KNOCKDOWN || 
            m_PlayerState != PLAYERSTATE.DIE || 
            m_PlayerState != PLAYERSTATE.FALLING)
        {
            float fUpBoostGageValue =0.0f;
            float fCurSpeed = 0.0f;
            if (isBoosterMode == false)
            {
                fCurSpeed = GetCurSpeed();
            }
            else if (isBoosterMode == true)
            {
                fCurSpeed = GetBoostSpeed();
            }

            fUpBoostGageValue = m_GameManager.GetDeltaTime(fCurSpeed);
            m_GameManager.IncreaseBoosterGage(fUpBoostGageValue);
        }
    }

    private void HitRoad()
    {
        Vector3 RayPosition = m_ThisTransform.position;
        RayPosition.y += 1.7f;
        Ray DownRay = new Ray(RayPosition, -Vector3.up);
        RaycastHit hit;
        Debug.DrawRay(RayPosition, -Vector3.up * 10.0f, Color.red);

        if (Physics.Raycast(DownRay, out hit, Mathf.Infinity,1<<LayerMask.NameToLayer("Road")))
        {
            if (hit.collider.tag == "Enemy")
            {
                return;
            }
            if (hit.collider.tag == "FallingBox")
                return;
            else if (hit.distance <= 1.7f)
            {
                Vector3 curPostiion = m_ThisTransform.position;
                curPostiion.y += hit.distance + 3.0f;
                m_ThisTransform.position = curPostiion;
                m_Animator.SetBool("isFalling", false);
                SetState(PLAYERSTATE.KNOCKDOWN);
            }
            else
                return;
        }
    }
    private void HitFallingBox()
    {
        Vector3 RayPosition = transform.position;
        Ray DownRay = new Ray(RayPosition, -Vector3.up);
        RaycastHit hit;

        float FallingBoxDistance = 10.0f;
        Debug.DrawRay(RayPosition, -Vector3.up * 15.0f, Color.green);

        if (Physics.Raycast(DownRay, out hit, FallingBoxDistance, 1 <<LayerMask.NameToLayer("Road")))
        {
            if (hit.collider.tag == "FallingBox")
            {
                m_MainCameraMove.StopCamera(true);
                Vector3 curPostiion = transform.position;
                curPostiion.y -= hit.distance;
                m_PortalComponent.transform.position = curPostiion;
                if (m_PortalComponent.gameObject.activeSelf == false)
                {
                    m_PortalComponent.AnimStart(true);
                }
            }
        }
    }
    private void HitCollider()
    {
        Vector3 RayPosition = transform.position;
        float CharacterTop = 20.0f;
        RayPosition.y += CharacterTop;
        Ray DownRay = new Ray(RayPosition, -Vector3.up);
        RaycastHit hit;
        Debug.DrawRay(RayPosition, -Vector3.up * CharacterTop, Color.red);

        if (Physics.Raycast(DownRay, out hit, Mathf.Infinity, 1 << LayerMask.NameToLayer("Road")))
        {
            if (hit.collider.tag == "Enemy")
            {
                return;
            }
            if (hit.collider.tag == "FallingBox")
            {
                if (m_PlayerState == PLAYERSTATE.JUMP)
                {
                    if(hit.distance>= 15.0f)
                    {
                        return;
                    }
                }
                SetState(PLAYERSTATE.FALLING);
                return;
            }
            Vector3 curPosition = transform.position;
            if (m_PlayerState == PLAYERSTATE.JUMP && m_GameManager.m_eGameState== GAMESTATE.PLAYING)
            {
                m_fBottomObjDistance = hit.distance;
                return;
            }
            curPosition.y += (CharacterTop - hit.distance);
            transform.position = curPosition;
        }
    }
    public void SetState(PLAYERSTATE SetState)
    {
        if (SetState == PLAYERSTATE.JUMP)
        {
            Jumping();
        }
        else if (SetState == PLAYERSTATE.SLIDING)
        {
            Sliding();
        }
        else if (SetState == PLAYERSTATE.RUN)
        {
            Running();
        }
        else if (SetState == PLAYERSTATE.KNOCKDOWN)
        {
            KncokDown();
        }
        else if (SetState == PLAYERSTATE.FALLING)
        {
            Falling();
        }   
        else if (SetState == PLAYERSTATE.DIE)
        {
            Die();
        }
    }
    private void Die()
    {
        if (m_PlayerState == PLAYERSTATE.DIE)
            return;

        m_PlayerState = PLAYERSTATE.DIE;
        m_GameManager.SetGameState(GAMESTATE.DIE);
        m_GameManager.VisibleScoreboard();
        m_MainCameraMove.StopCamera(false);
        m_MainCameraMove.DiePlaceCamera();
        m_Animator.SetBool("isDie", true);
    }
    private void Falling()
    {
        if (m_PlayerState == PLAYERSTATE.FALLING)
            return;
        m_PlayerState = PLAYERSTATE.FALLING;
        m_Animator.SetBool("isFalling", true);

        Vector3 vDir = m_MainCameraMove.GetTransform.position - m_ThisTransform.position;
        Quaternion LookAtCamera = Quaternion.LookRotation(vDir);
        m_ThisTransform.rotation = LookAtCamera;
        SetBoostMode(false);
        m_RoadManager.SetHorizontalMoveEnd(true);
        SetDirectionMove(DIRECTION.FRONT);
    }   
    private void ChangeAnimation()
    {
        if (isPlayingEndAnimation("Sliding",0.9f)&& isDamageDown ==false)
        {
            SetState(PLAYERSTATE.RUN);
        }
        if (isPlayingEndAnimation("Down", 0.9f))// && isDamageDown == false)
        {
            SetState(PLAYERSTATE.RUN);
        }
    }
    private bool Sliding()
    {
        if (isJumping == true)
            return false;
        if (isDamageDown == true)
            return false;
        if (isSliding == true)
            return false;

        isRunning = false;
        isSliding = true;


        m_PlayerState = PLAYERSTATE.SLIDING;
        m_MainCameraMove.SetStateCamMoving(PLAYERSTATE.SLIDING);
        m_Animator.SetBool("isRunning", isRunning);
        m_Animator.SetBool("isSliding", isSliding);
        m_Animator.SetFloat("SlidingAnimSpeed", m_fAnimSpeed * 1.5f);
        m_PlayerCollider.center = m_ColliderSlidingPostion;

        return true;
    }
    public float GetAnimSpeed()
    {
        return m_fAnimSpeed;
    }
    private bool Jumping()
    {
        if (isSliding == true)
            return false;
        if (isDamageDown == true)
            return false;
        isJumping = true;
        isJumpDown = false;

        isRunning = false;

        m_PlayerState = PLAYERSTATE.JUMP;
        m_MainCameraMove.SetStateCamMoving(PLAYERSTATE.JUMP);

        m_Animator.SetBool("isJumping", isJumping);
        m_Animator.SetBool("isRunning", isRunning);
        m_Animator.SetFloat("JumpAnimSpeed", m_fAnimSpeed * 1.5f);
        m_vJumpStartPos = m_ThisTransform.position;
        m_vJumpPos = m_ThisTransform.position;
        m_vJumpPos.y += m_fJumpPower;
        m_fJumpTimer = 0.0f;
        return true;
    }


    private bool Running()
    {
        if (isRunning == true)
            return false;
        m_MainCameraMove.SetStateCamMoving(PLAYERSTATE.RUN);

        m_Animator.SetFloat("RunAnimSpeed", m_fAnimSpeed);
        m_PlayerCollider.center = m_ColliderRunPostion;
        m_PlayerState = PLAYERSTATE.RUN;
        isSliding = false;
        isJumping = false;
        isRunning = true;
        isDamageDown = false;

        m_Animator.SetBool("isSliding", isSliding);
        m_Animator.SetBool("isJumping", isJumping);
        m_Animator.SetBool("isRunning", isRunning);
        m_Animator.SetBool("isDown", isDamageDown);

        return true;
    }
    private bool KncokDown()
    {
        m_GameManager.DecreaseLife(1);
        isRunning = false;
        isJumping = false;
        isSliding = false;
        isDamageDown = true;
        isPrevDamageDown = true;

        m_fCurSpeed = 0.0f;
        m_fBoostSpeed = 0.0f;
        m_fMovedSpeedTimer = 0.0f;
        m_MainCameraMove.StopCamera(false);
        m_PortalComponent.gameObject.SetActive(false);
        m_GameManager.SetCurBoostGage(0.0f);
        m_GameManager.SetBoostUpGage(0.0f);
        m_MainCameraMove.SetStateCamMoving(PLAYERSTATE.KNOCKDOWN);

        m_Animator.SetBool("isSliding", isSliding);
        m_Animator.SetBool("isJumping", isJumping);
        m_Animator.SetBool("isRunning", isRunning);
        m_Animator.SetBool("isDown", isDamageDown);
        m_Animator.SetFloat("PlayerSpeed", m_fCurSpeed);
        m_Animator.SetBool("isBoostMode", isBoosterMode);

        m_PlayerState = PLAYERSTATE.KNOCKDOWN;

        return true;
    }
    private bool isPlayingEndAnimation(string AnimationName, float LimitTime = 1.0f, int LayerNumber = 0)
    {
        AnimatorStateInfo curAnimInfo = m_Animator.GetCurrentAnimatorStateInfo(LayerNumber);//.IsName(AnimationName);
        if (!curAnimInfo.IsName(AnimationName))
            return false;
        float curLength = curAnimInfo.normalizedTime;
        if (LimitTime != 1.0f)
        {
            LimitTime = Mathf.Min(1, LimitTime);
            return LimitTime < curLength;
        }

        if (0.9f < curLength)
        {
            return true;
        }
        else
            return false;
    }
    public float GetBoostSpeed()
    {
        float fMultipleSpeed = 2.0f;
        if (PLAYERSTATE.JUMP == m_PlayerState)
            return m_fJumpSpeed * fMultipleSpeed;
        else
        m_fBoostSpeed = m_fCurSpeed * fMultipleSpeed;

        return m_fBoostSpeed;
    }
    public float GetCurSpeed()
    {
        if(PLAYERSTATE.JUMP == m_PlayerState)
            return m_fJumpSpeed;
        else
        return m_fCurSpeed;
    }


    public float GetPlayingAnimNormalTime(string AnimatorName, string LayerName = "Base Layer",int LayerIndex = 0)
    {
        string Name = string.Format(LayerName + "." + AnimatorName);
        float normalTime = 0.0f;
        float fAnimatorNormalTime = 0.0f;
        int iAnimatorNormalTime = 0;

        if (m_Animator.GetCurrentAnimatorStateInfo(LayerIndex).fullPathHash == Animator.StringToHash(Name))
        {
             fAnimatorNormalTime = m_Animator.GetCurrentAnimatorStateInfo(0).normalizedTime;
             iAnimatorNormalTime = (int)m_Animator.GetCurrentAnimatorStateInfo(0).normalizedTime;

            if (fAnimatorNormalTime >= 1.0f)
                normalTime = fAnimatorNormalTime - iAnimatorNormalTime;
            else
                normalTime = fAnimatorNormalTime;
        }
        else if(m_Animator.GetNextAnimatorStateInfo(LayerIndex).fullPathHash == Animator.StringToHash(Name))
        {
            fAnimatorNormalTime = m_Animator.GetNextAnimatorStateInfo(0).normalizedTime;
            iAnimatorNormalTime = (int)m_Animator.GetNextAnimatorStateInfo(0).normalizedTime;

            if (fAnimatorNormalTime >= 1.0f)
                normalTime = fAnimatorNormalTime - iAnimatorNormalTime;
            else
                normalTime = fAnimatorNormalTime;


        }

        return normalTime;
    }
}
