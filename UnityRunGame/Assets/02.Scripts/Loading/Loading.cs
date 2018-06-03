using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEngine;


public class Loading : MonoBehaviour
{
    private string m_NextSceneName;
    public Slider m_LoidngBar;

    void Start()
    {
        if (MainValue.GetInstance().m_GameMode == GAMEMODE.TITLE)
        {
            m_NextSceneName = "TitleScene";
            Debug.Log("타이틀 씬 로드중!");
        }
        else
        {
            m_NextSceneName = "GameScene";
            Debug.Log("게임 씬 로드중!");

        }
        LoadingPage();
    }
    public void LoadingPage()
    {
        StartCoroutine(LoadingProgress());
    }

    IEnumerator LoadingProgress()
    {
        yield return new WaitForSeconds(1f);
        AsyncOperation async = SceneManager.LoadSceneAsync(m_NextSceneName);

        while (async.isDone == false)
        {
            m_LoidngBar.enabled = true;
            m_LoidngBar.value = async.progress;

            Debug.Log(async.progress);

            if (async.progress == .95f)
            {
                async.allowSceneActivation = true;
            }
            yield return true;
        }
        m_LoidngBar.value = 1.0f;
    }
}
