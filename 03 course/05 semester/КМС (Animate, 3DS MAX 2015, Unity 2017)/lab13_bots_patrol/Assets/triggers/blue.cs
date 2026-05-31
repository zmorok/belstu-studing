using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class blue : MonoBehaviour {

    public Transform wall;
    public float rotateSpeed = 30f;

    private void OnTriggerStay(Collider other)
    {
        if (other.name != "body") return;

        wall.Rotate(Vector3.up * rotateSpeed * Time.deltaTime);
    }
}
