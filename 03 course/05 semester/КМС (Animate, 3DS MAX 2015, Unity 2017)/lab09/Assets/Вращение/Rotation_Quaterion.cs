using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotation_Quaterion : MonoBehaviour {

    float dx, dy , dz;
    Vector3 baseVec;

    // Use this for initialization
    void Start () {
		Quaternion from = transform.rotation;

        dx = 0.4f;
        dy = 0.2f;
        dz = 0.3f;

        baseVec = new Vector3(dx, dy, dz);
    }
	
	// Update is called once per frame
	void Update () {
		transform.rotation *= Quaternion.AngleAxis(5, baseVec);

    }
}
