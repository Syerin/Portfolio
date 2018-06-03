using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckBottom : MonoBehaviour
{
    public bool m_ExistBottom =false;
    void Update()
    {
        m_ExistBottom  = ExistBottomRoad();
    }
    public bool ExistBottomRoad()
    {
        Ray BottomRay = new Ray(transform.position, -Vector3.up);
        RaycastHit hit;
        Debug.DrawRay(transform.position, -Vector3.up * 10.0f);
        if(Physics.Raycast(BottomRay,out hit,Mathf.Infinity))
        {
            if (hit.collider.tag == "Enemy")
                return false;
            if (hit.collider.tag == "FallingBox")
                return false;
            if (hit.collider.tag == "Threshold")
                return false;

            return true;
        }

        return false;
    }
}
