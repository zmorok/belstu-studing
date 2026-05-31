using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScriptPosition : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		transform.position += new Vector3(0.01f, 0, 0.05f);
    }
}
