using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScriptScale : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		transform.localScale += new Vector3 (0.05f, 0.1f, 0);
    }
}
