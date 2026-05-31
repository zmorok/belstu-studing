using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class player : MonoBehaviour {

    public float moveSpeed = 3f;     // скорость движения WASD
    public float mouseSpeed = 150f;  // скорость вращения мышью

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        float h = Input.GetAxis("Horizontal"); // A, D
        float v = Input.GetAxis("Vertical");   // W, S

        Vector3 moveDir = new Vector3(h, 0, v);
        transform.Translate(moveDir * moveSpeed * Time.deltaTime);

        float mx = Input.GetAxis("Mouse X"); // движение мыши по X
        transform.Rotate(0, mx * mouseSpeed * Time.deltaTime, 0);
    }
}
