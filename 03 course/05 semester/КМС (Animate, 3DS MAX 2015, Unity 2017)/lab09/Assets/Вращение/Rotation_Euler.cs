using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotation_Euler : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}

    public bool reverseOrder = false;

    // Update is called once per frame
    void Update () {
        Vector3 angles = transform.eulerAngles;

        if (!reverseOrder)
        {
            angles.x += 0.5f;
            transform.eulerAngles = angles;
            angles = transform.eulerAngles;
            angles.z += 1.5f;
            transform.eulerAngles = angles;

            reverseOrder = !reverseOrder;
        }
        else
        {
            angles.z += 1.5f;
            transform.eulerAngles = angles;
            angles = transform.eulerAngles;
            angles.x += 0.5f;
            transform.eulerAngles = angles;

            reverseOrder = !reverseOrder;
        }
    }
}
