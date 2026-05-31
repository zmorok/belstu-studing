using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class light : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}

	public Light light1;
	// Update is called once per frame
	private void OnTriggerEnter(Collider other)
	{
		if (other.name == "player")
		{
			light1.enabled = true;
		}
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.name == "player")
        {
            light1.enabled = false;
        }
    }
}
