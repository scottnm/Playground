using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerCollision : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }

    void OnTriggerEnter(Collider collider)
    {
        Debug.Log("Collision! " + collider.gameObject.name);
        if (collider.gameObject.tag == "Pellet")
        {
            (GameObject.Find("PelletSpawner").gameObject.GetComponent<PelletSpawner>()).OnPelletHit(collider.gameObject);
            gameObject.GetComponent<Movement>().GenerateTailSegment();
        }
        else if (collider.gameObject.tag == "Wall")
        {
            TriggerGameOver();
        }
        else if (collider.gameObject.tag == "Tail")
        {
            TriggerGameOver();
        }
    }

    void TriggerGameOver()
    {
        gameObject.GetComponent<Movement>().StopMovement();
        GameObject.Find("UI").GetComponent<GameOverUIController>().TriggerGameOver();
        GameObject.Find("GameOverCameraSpot").GetComponent<GameOverCameraSpin>().StartGameOverCameraSpin();
    }
}
