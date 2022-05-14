using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class Movement : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        const float PITCH_MAX = 0.1f;
        const float ROLL_MAX = 0.1f;
        var pitch = PITCH_MAX * Input.GetAxis("Vertical");
        var roll = ROLL_MAX * Input.GetAxis("Horizontal");
        transform.Rotate(-pitch, 0, -roll); // invert the pitch and roll for more natural rotation
        transform.position += transform.forward * Time.deltaTime;
    }
}
