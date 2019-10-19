using System.Collections;
using System.Collections.Generic;
using Helpers;
using UnityEngine;

public class COMTextTest : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Debug.unityLogger.Log("STARTING");
        StartCoroutine(
            COMunicator.COMunicate
            (
                "PING",
                (string s) => Debug.unityLogger.Log(s),
                () => Debug.unityLogger.LogError("ERROR", "Timed out!")
            )
        );
    }

    // Update is called once per frame
    void Update()
    {
        StartCoroutine(
            COMunicator.COMunicate
            (
                "SPEEDS",
                (string s) => GetComponent<TextMesh>().text = s
            )
        );
    }
}