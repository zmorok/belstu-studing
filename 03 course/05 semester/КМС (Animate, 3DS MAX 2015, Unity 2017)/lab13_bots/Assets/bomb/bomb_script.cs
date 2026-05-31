using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class bomb_script : MonoBehaviour {

    AudioSource BombSound;
    private bool hasExploded = false;
    public GameObject BombExplosion;

    void Start() { BombSound = GetComponent<AudioSource>(); BombSound.Stop(); }

    private void OnCollisionEnter(Collision collision)
    {
        if (hasExploded) return;
        hasExploded = true;
        
        AudioSource.PlayClipAtPoint(BombSound.clip, transform.position);
        gameObject.GetComponent<Renderer>().enabled = false;
        Instantiate(BombExplosion, gameObject.transform);
    }
}
