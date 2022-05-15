using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameOverCameraSpin : MonoBehaviour
{
    GameObject m_gameOverCamera;

    struct SlideCameraLerpState
    {
        public Vector3 startPos;
        public Quaternion startRot;
        public float t;
    }

    SlideCameraLerpState m_cameraLerpState;

    // Start is called before the first frame update
    void Start()
    {
        m_gameOverCamera = null;
    }

    // Update is called once per frame
    void Update()
    {
        const float ROTATE_SPEED = 5.0f;
        transform.parent.transform.Rotate(0, Time.deltaTime * ROTATE_SPEED, 0);
        if (m_gameOverCamera != null && m_cameraLerpState.t < 1)
        {
            m_cameraLerpState.t += Time.deltaTime;
            float smoothT = Mathf.SmoothStep(0, 1, m_cameraLerpState.t);
            m_gameOverCamera.transform.localPosition = Vector3.Lerp(m_cameraLerpState.startPos, Vector3.zero, smoothT);
            m_gameOverCamera.transform.localRotation = Quaternion.Lerp(m_cameraLerpState.startRot, Quaternion.identity, smoothT);
        }
    }

    public void StartGameOverCameraSpin()
    {
        m_gameOverCamera = GameObject.Find("Main Camera");
        var cameraWorldPos = m_gameOverCamera.transform.position;
        var cameraWorldRotation = m_gameOverCamera.transform.rotation;
        m_gameOverCamera.transform.SetParent(transform, false);
        m_gameOverCamera.transform.localPosition = m_gameOverCamera.transform.InverseTransformPoint(cameraWorldPos);
        m_gameOverCamera.transform.localRotation = Quaternion.Inverse(transform.rotation) * cameraWorldRotation;

        m_cameraLerpState.startPos = m_gameOverCamera.transform.localPosition;
        m_cameraLerpState.startRot = m_gameOverCamera.transform.localRotation;
        m_cameraLerpState.t = 0;
    }
}
