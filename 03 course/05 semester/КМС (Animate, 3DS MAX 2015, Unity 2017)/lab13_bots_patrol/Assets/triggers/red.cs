using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class red : MonoBehaviour {

	public Light red_blue_light;

	void OnTriggerEnter(Collider other)
	{
		if (other.name != "body") return;

        red_blue_light.intensity = 100;
	}

	void OnTriggerExit(Collider other)
	{
        if (other.name != "body") return;

        red_blue_light.intensity = 0;
    }

}
