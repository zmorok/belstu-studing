using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class yellow : MonoBehaviour {

    public Light yellow_red_light;

    void OnTriggerEnter(Collider other)
    {
        if (other.name != "body") return;

        yellow_red_light.intensity = 100;
    }

    void OnTriggerExit(Collider other)
    {
        if (other.name != "body") return;

        yellow_red_light.intensity = 0;
    }
}
