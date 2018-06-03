using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;

public class LoadScene : MonoBehaviour
{
    public GAMEMODE m_GameMode;
    public int m_StageNumber;
    public void LoadingScene()
    {
        SceneManager.LoadScene("LodingScene");
    }
}
