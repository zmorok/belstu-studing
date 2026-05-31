using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ammo_col_script : MonoBehaviour {

    public GameObject explosion;

    private void OnCollisionEnter(Collision other)
    {
        if (other.gameObject.tag == "goal" )
        {
            Instantiate( explosion, gameObject.transform );
            gameObject.GetComponent<Renderer>().enabled = false;
            other.transform.GetComponent<Renderer>().material.color = Color.green;
            
        }
    }
}
