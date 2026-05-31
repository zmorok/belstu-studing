using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Input_GetAxis : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}

    float rotationX = 0f, rotationY = 0f;
	float? tempX, tempY;

	
    // Update is called once per frame
    void Update () {
		
		float x = Input.GetAxis("Horizontal");
		float z = Input.GetAxis("Vertical");

		Debug.Log("Horizontal: " + x + " Vertical: " + z);

        transform.Translate(x * 0.1f, 0, z * 0.1f);

		float mouseX = Input.GetAxis("Mouse X");
		float mouseY = Input.GetAxis("Mouse Y");

		Debug.Log("Mouse X: " + mouseX + " Mouse Y: " + mouseY);

        // --- накопление вращений ---
        rotationX += mouseY;
        rotationY += mouseX;

        // --- ограничение углов от -90 до 90 градусов ---
        rotationX = Mathf.Clamp(rotationX, -90f, 90f);
        rotationY = Mathf.Clamp(rotationY, -90f, 90f);

        //transform.Rotate(rotationY, -rotationX, 0);
        transform.localRotation = Quaternion.Euler(rotationX, -rotationY, 0f);
    }
}
