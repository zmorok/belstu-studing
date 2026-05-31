using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class barrel_script : MonoBehaviour
{
    public GameObject ammo;          // префаб снаряда
    public float spawnDistance = 1f; // на сколько вперёд от дула спавнить
    public float ammoLifetime = 2f;  // время жизни снаряда

    void Update()
    {
        // одиночный выстрел по нажатию пробела
        if (Input.GetKeyDown(KeyCode.Space))
        {
            Fire();
        }
    }

    private void Fire()
    {
        // позиция появления снаряда — немного впереди дула
        Vector3 spawnPos = transform.position +
                           transform.TransformDirection(Vector3.forward * spawnDistance);

        GameObject newAmmo = Instantiate(ammo, spawnPos, transform.rotation);

        // передаём ссылку на коллайдер дула, чтобы игнорировать его
        var ammoScript = newAmmo.GetComponent<ammo_script>();
        if (ammoScript != null)
        {
            var barrelCollider = GetComponent<Collider>();
            ammoScript.Init(barrelCollider);
        }

        Destroy(newAmmo, ammoLifetime);
    }
}
