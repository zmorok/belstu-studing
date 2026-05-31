using System.Collections;
using UnityEngine;

public class bot_script : MonoBehaviour
{
    [Header("Наведение")]
    public float rotSpeedBash = 3f;        // скорость поворота башни

    [Header("Орудие бота")]
    public Transform turret;                // turret_b
    public Transform barrel;               // barrel_b
    public GameObject ammo;               // тот же префаб, что и у игрока
    private float spawnDistance = 3f;      // как в barrel_script
    private float ammoLifetime = 2f;       // как в barrel_script
    private float fireCooldown = 3f;       // перезарядка между выстрелами

    private int life = 10;

    private bool canShoot = true;
    private Transform playerTarget;

    private int ammoCount = 5;


    [Header("Движение")]
    public float moveSpeed = 4f;      // скорость корпуса
    public float rotSpeedBody = 1f;   // поворот корпуса
    public float stopDistance = 20f;   // ближе этого — не едем

    [Header("Корпус")]
    public Transform body;            // если корпус = этот объект, можно не задавать


    // игрок входит в триггер бота
    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Player"))
        {
            playerTarget = other.transform;
        }
    }

    // игрок выходит из триггера
    private void OnTriggerExit(Collider other)
    {
        if (other.transform == playerTarget)
        {
            playerTarget = null;
        }
    }

    private void Update()
    {
        if (playerTarget == null) return;

        Transform bodyTr = body != null ? body : transform;

        // вектор до игрока по земле
        Vector3 toPlayer = playerTarget.position - bodyTr.position;
        toPlayer.y = 0f;

        float dist = toPlayer.magnitude;

        if (dist > 0.001f)
        {
            // поворачиваем корпус к игроку
            Quaternion bodyRot = Quaternion.LookRotation(toPlayer);
            bodyTr.rotation = Quaternion.Slerp(bodyTr.rotation, bodyRot, Time.deltaTime * rotSpeedBody);

            // едем только если далеко
            if (dist > stopDistance && ammoCount > 0)
            {
                bodyTr.position += bodyTr.forward * moveSpeed * Time.deltaTime;
            }
            else if (ammoCount == 0)
            {
                bodyTr.position += bodyTr.forward * moveSpeed * Time.deltaTime;
            }
        }


        // направление от башни к игроку (крутим только по горизонтали)
        Vector3 dirToPlayer = playerTarget.position - turret.position;
        dirToPlayer.y = 0f;

        if (dirToPlayer.sqrMagnitude < 0.001f) return;

        Quaternion lookRotation = Quaternion.LookRotation(dirToPlayer);

        // поворачиваем башню на игрока
        turret.rotation = Quaternion.Slerp(
            turret.rotation,
            lookRotation,
            Time.deltaTime * rotSpeedBash
        );

        // если можем стрелять и перед стволом игрок — запускаем BotShoot
        float aimDot = Vector3.Dot(turret.forward, dirToPlayer.normalized); // 1 = идеально смотрит
        RaycastHit hit;
        if (ammoCount > 0 && canShoot && aimDot > 0.98f && Physics.Raycast(barrel.position, barrel.forward, out hit, 100f))
        {
            if (hit.transform.CompareTag("Player"))
                StartCoroutine(BotShoot());
        }
    }

    private IEnumerator BotShoot()
    {
        canShoot = false;

        // позиция появления снаряда — немного впереди barrel_b
        Vector3 spawnPos = barrel.position +
                           barrel.TransformDirection(Vector3.forward * spawnDistance);

        GameObject newAmmo = Instantiate(ammo, spawnPos, barrel.rotation);

        // передаём в ammo_script коллайдер ствола бота
        var ammoScript = newAmmo.GetComponent<ammo_script>();
        if (ammoScript != null)
        {
            var barrelCollider = barrel.GetComponent<Collider>();
            ammoScript.Init(barrelCollider);
            ammoCount--;
        }

        Destroy(newAmmo, ammoLifetime);

        yield return new WaitForSeconds(fireCooldown);

        canShoot = true;
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("ammo"))
        {
            life--;
            if (life <= 0)
                Destroy(gameObject);
        }
    }
}
