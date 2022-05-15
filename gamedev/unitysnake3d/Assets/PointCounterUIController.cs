using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class PointCounterUIController : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        SetPointCounter(0);
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void SetPointCounter(int p)
    {
        gameObject.GetComponent<TextMeshProUGUI>().SetText("Points: {0}", p);
    }
}
