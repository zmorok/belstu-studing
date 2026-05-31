using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PrefubGenerate : MonoBehaviour {

    MeshRenderer rend;

    public float minX, maxX,
                 minZ, maxZ,
                 nX, nY, nZ;


    public GameObject prefub1;

	// Use this for initialization
	void Start () {
        rend = GetComponent<MeshRenderer>();

        minX = rend.bounds.min.x;
        maxX = rend.bounds.max.x;
        minZ = rend.bounds.min.z;
        maxZ = rend.bounds.max.z;
        nY = gameObject.transform.position.y + 5;
    }
	
	// Update is called once per frame
	void Update () {
        nX = Random.Range(minX, maxX);
        nZ = Random.Range(minZ, maxZ);

        if (Input.GetKeyDown(KeyCode.Space))
		{
			Instantiate(prefub1, new Vector3(nX, nY, nZ), Quaternion.identity);
            prefub1.AddComponent<Rigidbody>();
        }

        
        if (Input.GetKeyDown(KeyCode.Q))
        {
            GameObject cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
            cube.transform.position = new Vector3(nX, nY, nZ);
            cube.AddComponent<Rigidbody>();
        }

        if (Input.GetKey(KeyCode.W))
            transform.Rotate(new Vector3(0, 0, 45) * 0.05f);
        if (Input.GetKey(KeyCode.E))
            transform.Rotate(new Vector3(0, 0, -45) * 0.05f);
    }
}
