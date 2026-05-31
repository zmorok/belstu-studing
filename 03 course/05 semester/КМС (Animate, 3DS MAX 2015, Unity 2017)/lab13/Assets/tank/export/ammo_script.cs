using System.Collections;
using UnityEngine;

public class ammo_script : MonoBehaviour
{
    public float speed = 20f;
    public float lifeTime = 2f;
    public GameObject explosion;
     AudioSource sound;

    private Rigidbody rb;
    private CapsuleCollider col;

    public void Init(Collider barrelCollider)
    {
        if (barrelCollider != null && col != null)
        {
            Physics.IgnoreCollision(col, barrelCollider, true);
        }
    }

    private void Awake()
    {
        col = gameObject.AddComponent<CapsuleCollider>();
        col.isTrigger = false;
        col.height = 1f;
        col.radius = 0.1f;
        col.direction = 2;

        rb = gameObject.AddComponent<Rigidbody>();
        rb.mass = 2000f;
        rb.useGravity = false;
        rb.isKinematic = false;
        rb.collisionDetectionMode = CollisionDetectionMode.ContinuousDynamic;
        rb.interpolation = RigidbodyInterpolation.Interpolate;

        sound = GetComponent<AudioSource>();
    }

    private void Start()
    {
        Destroy(gameObject, lifeTime);
    }

    private void FixedUpdate()
    {
        // двигаем пулю вперёд через физику
        Vector3 newPos = rb.position + transform.forward * speed * Time.fixedDeltaTime;
        rb.MovePosition(newPos);
    }

    private void OnCollisionEnter(Collision other)
    {
        if (other.gameObject.tag == "goal")
        {
            Debug.Log("Снаряд попал в: " + other.gameObject.name);
            Instantiate(explosion, gameObject.transform);
            other.transform.GetComponent<Renderer>().material.color = Color.green;
        }
        else
        {
            Debug.Log("Снаряд попал в: " + other.gameObject.name);
        }
        AudioSource.PlayClipAtPoint(sound.clip, transform.position);
        gameObject.GetComponent<Renderer>().enabled = false;
        Destroy(gameObject, 10f);
    }
}
