using System.Collections;
using System.Collections.Generic;
using UnityEditor.VersionControl;
using UnityEngine;

public class CapsuleScript : MonoBehaviour {

    // Use this for initialization
    void Start() {

    }

    // Update is called once per frame
    void Update() {
        if (Input.GetKey(KeyCode.D))
            transform.Translate(new Vector3(0.05f, 0, 0));
        if (Input.GetKey(KeyCode.A))
            transform.Translate(new Vector3(-0.05f, 0, 0));
        if (Input.GetKey(KeyCode.W))
            transform.Translate(new Vector3(0, 0, 0.05f));
        if (Input.GetKey(KeyCode.S))
            transform.Translate(new Vector3(0, 0, -0.05f));
    }

    public Texture2D Texture;

    void OnCollisionEnter(Collision col) 
    {
        if (col.gameObject.name == "Cube1")
        { col.gameObject.GetComponent<Renderer>().material.mainTexture = Texture; }

        if (col.gameObject.name == "Cube")
        { col.gameObject.GetComponent<Renderer>().material.color = new Color(0, 23, 1); }
    }
}
