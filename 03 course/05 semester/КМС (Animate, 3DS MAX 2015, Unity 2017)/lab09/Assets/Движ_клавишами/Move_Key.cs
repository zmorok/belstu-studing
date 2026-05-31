using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Move_Key : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKey(KeyCode.W))
		{
			transform.Translate(0, 0.1f, 0);
        }
		else if (Input.GetKey(KeyCode.S))
		{
			transform.Translate(0, -0.1f, 0);
        }
		else if (Input.GetKey(KeyCode.D))
		{
            transform.Translate(0.1f, 0, 0);
        }
		else if (Input.GetKey(KeyCode.A))
		{
            transform.Translate(-0.1f, 0, 0);
        }
		else if (Input.GetKey(KeyCode.Q))
		{
            transform.Translate(0, 0, 0.1f);
        }
		else if (Input.GetKey(KeyCode.E))
		{
            transform.Translate(0, 0, -0.1f);
        }


    }
}
