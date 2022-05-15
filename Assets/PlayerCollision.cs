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
            // dynamically creating and destroying pellets probably isn't a good idea for a real game but for this demo its fine.
            Destroy(collider.gameObject);
            gameObject.GetComponent<Movement>().GenerateTailSegment();
        }
        else if (collider.gameObject.tag == "Wall")
        {
            gameObject.GetComponent<Movement>().OnHitWall();
            GameObject.Find("GameOverText").gameObject.SetActive(true);
        }
    }
}
