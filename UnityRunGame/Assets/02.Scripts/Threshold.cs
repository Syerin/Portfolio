using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Threshold : MonoBehaviour {

    private void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Player")
        {
            Player PlayerComponent = other.GetComponent<Player>();
            PlayerComponent.SetState(PLAYERSTATE.FALLING);
            PlayerComponent.TrrigerEnterFallingBox();
        }
    }
}

