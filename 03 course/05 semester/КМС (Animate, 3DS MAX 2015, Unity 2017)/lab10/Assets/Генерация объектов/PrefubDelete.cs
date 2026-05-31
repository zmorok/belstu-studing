using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PrefubDelete : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        if (transform.position.y < -5)
            Destroy(gameObject);
    }
}
