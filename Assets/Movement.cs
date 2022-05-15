using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class Movement : MonoBehaviour
{
    public GameObject TailSegment;

    private List<GameObject> m_segments;
    private bool m_movementStopped;

    // Start is called before the first frame update
    void Start()
    {
        m_segments = new List<GameObject>();
        m_segments.Add(gameObject); // add the head to the segment list
        m_movementStopped = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (m_movementStopped)
        {
            // noop our player movement update if the game is over
            return;
        }

        // Move the head first
        const float PITCH_MAX = 0.1f;
        const float ROLL_MAX = 0.1f;
        const float FORWARD_SPEED = 2.0f;

        var pitch = PITCH_MAX * Input.GetAxis("Vertical") * -1; // -1 to invert the pitch for more natural feel
        var roll = ROLL_MAX * Input.GetAxis("Horizontal") * -1; // -1 to invert roll for more natural feel
        transform.Rotate(pitch, 0, roll, Space.Self);
        transform.position += transform.forward * Time.deltaTime * FORWARD_SPEED;

        // Update all tail segements
        for (int i = m_segments.Count - 1; i >= 1; i--)
        {
            var tailSegment = m_segments[i].transform;
            var leaderSegment = m_segments[i - 1].transform;

            var followLeaderVec = leaderSegment.position - tailSegment.position;
            tailSegment.position = Vector3.Lerp(tailSegment.position, leaderSegment.position, Time.deltaTime * FORWARD_SPEED);
            tailSegment.rotation = Quaternion.Lerp(tailSegment.rotation, leaderSegment.rotation, Time.deltaTime * FORWARD_SPEED);
        }

        // N.B. for ease of debug purposes let's just make it easy to generate tail segments
        if (Input.GetButtonDown("TestGenerateTailSegment"))
        {
            GenerateTailSegment();
        }
    }

    public void GenerateTailSegment()
    {
        Debug.Assert(m_segments.Count > 0); // there should always at least be a head segment
        var lastSegment = m_segments[m_segments.Count - 1];
        var behindLastSegment = lastSegment.transform.position + (lastSegment.transform.forward * -2 * lastSegment.GetComponent<SphereCollider>().radius);
        var newTailSegment = Instantiate(TailSegment, behindLastSegment, lastSegment.transform.rotation);

        // only the 3rd segment onward are collidable
        newTailSegment.GetComponent<SphereCollider>().enabled = m_segments.Count > 2;
        m_segments.Add(newTailSegment);
        GameObject.Find("PointCounter").GetComponent<PointCounterUIController>().SetPointCounter(m_segments.Count - 1);
    }

    public void StopMovement()
    {
        m_movementStopped = true;
    }
}
