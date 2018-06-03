using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LoadStage : MonoBehaviour
{
    public void LoadStageNumber(int number)
    {
        MainValue.GetInstance().OnClickStageNumberScene(number);
    }
}
