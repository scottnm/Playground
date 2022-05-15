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
        SpawnPellet();
    }

    // Update is called once per frame
    void Update()
    {
        var dbgSpawnPellet = Input.GetButtonDown("TestGeneratePellet");
        if (dbgSpawnPellet)
        {
            SpawnPellet();
        }
    }

    void SpawnPellet()
    {
        // dynamically creating and destroying pellets probably isn't a good idea for a real game but for this demo its fine.
        Instantiate(PelletPrefab, GenerateNewPelletPosition(), Quaternion.identity);
    }

    Vector3 GenerateNewPelletPosition()
    {
        float maxX = 0.0f;
        float minX = 0.0f;
        float maxY = 0.0f;
        float minY = 0.0f;
        float maxZ = 0.0f;
        float minZ = 0.0f;

        foreach (var wallGameObject in GameObject.FindGameObjectsWithTag("Wall"))
        {
            maxX = Mathf.Max(maxX, wallGameObject.transform.position.x);
            minX = Mathf.Min(minX, wallGameObject.transform.position.x);
            maxY = Mathf.Max(maxY, wallGameObject.transform.position.y);
            minY = Mathf.Min(minY, wallGameObject.transform.position.y);
            maxZ = Mathf.Max(maxZ, wallGameObject.transform.position.z);
            minZ = Mathf.Min(minZ, wallGameObject.transform.position.z);
        }

        return new Vector3(
            Random.Range(minX, maxX),
            Random.Range(minY, maxY),
            Random.Range(minZ, maxZ)
        ) * 0.75f;
    }

    public void OnPelletHit(GameObject pellet)
    {
        pellet.transform.position = GenerateNewPelletPosition();
    }
}
