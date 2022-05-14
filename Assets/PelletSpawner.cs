using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PelletSpawner : MonoBehaviour
{
    private List<GameObject> m_pellets;
    public GameObject PelletPrefab;
    private GameObject g_snakeHead;

    // Start is called before the first frame update
    void Start()
    {
        m_pellets = new List<GameObject>();
        g_snakeHead = GameObject.Find("SnakeHead");
        Debug.Assert(g_snakeHead != null);
    }

    // Update is called once per frame
    void Update()
    {
        var generatePellet = Input.GetButtonDown("TestGeneratePellet");
        if (generatePellet)
        {
            SpawnPellet();
        }
    }

    void SpawnPellet()
    {
        var inFrontOfSnakeHead = g_snakeHead.transform.position + (g_snakeHead.transform.forward * 5);
        m_pellets.Add(Instantiate(PelletPrefab, inFrontOfSnakeHead, Quaternion.identity));
    }
}
