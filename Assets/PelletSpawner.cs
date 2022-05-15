using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PelletSpawner : MonoBehaviour
{
    public GameObject PelletPrefab;
    private GameObject g_snakeHead;

    // Start is called before the first frame update
    void Start()
    {
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
        // dynamically creating and destroying pellets probably isn't a good idea for a real game but for this demo its fine.
        Instantiate(PelletPrefab, inFrontOfSnakeHead, Quaternion.identity);
    }
}
