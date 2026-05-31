using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class bot_script : MonoBehaviour
{
    [Header("Движение")]
    public bool patrolEnabled = true;
    public float moveSpeed = 6f;
    public float bodyTurnSpeed = 180f;
    public float pointReachDistance = 2f;
    [Tooltip("Если задать точки, бот будет ездить по ним туда-сюда (ping-pong).")]
    public Transform[] patrolPoints;
    [Tooltip("Ограничивать движение границами Terrain.")]
    public bool limitToTerrain = true;
    [Tooltip("Отступ от края Terrain (чтобы бот не съезжал за границы).")]
    public float terrainBorderPadding = 2f;
    [Tooltip("Если точки не заданы — строим маршрут от текущей позиции до границы Terrain по направлению вперёд.")]
    public bool autoRouteToTerrainBorder = true;
    [Tooltip("Длина авто-маршрута, если Terrain не найден или autoRouteToTerrainBorder выключен.")]
    public float autoRouteDistance = 40f;

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

    private Rigidbody rb;

    private Terrain cachedTerrain;
    private bool hasTerrainBounds;
    private float minX, maxX, minZ, maxZ;

    private Vector3[] patrolPositions;
    private int patrolIndex;
    private int patrolDirection = 1;

    private void Awake()
    {
        rb = GetComponent<Rigidbody>();
    }

    private void Start()
    {
        CacheTerrainBounds();
        BuildPatrolPositions();

        if (hasTerrainBounds)
        {
            Vector3 clamped = ClampToTerrainBounds(transform.position);
            if ((clamped - transform.position).sqrMagnitude > 0.0001f)
            {
                transform.position = clamped;
                if (rb != null)
                    rb.position = clamped;
            }
        }
    }

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
        AimAndShoot();
    }

    private void FixedUpdate()
    {
        PatrolMove();
    }

    private void AimAndShoot()
    {
        if (playerTarget == null)
            return;

        // направление от башни к игроку (крутим только по горизонтали)
        Vector3 dirToPlayer = playerTarget.position - turret.position;
        dirToPlayer.y = 0f;

        if (dirToPlayer.sqrMagnitude < 0.001f)
            return;

        Quaternion lookRotation = Quaternion.LookRotation(dirToPlayer);

        // поворачиваем башню на игрока
        turret.rotation = Quaternion.Slerp(
            turret.rotation,
            lookRotation,
            Time.deltaTime * rotSpeedBash
        );

        // если можем стрелять и перед стволом игрок — запускаем BotShoot
        RaycastHit hit;
        if (canShoot && Physics.Raycast(barrel.position, barrel.forward, out hit, 100f))
        {
            if (hit.transform.CompareTag("Player"))
            {
                StartCoroutine(BotShoot());
            }
        }
    }

    private void PatrolMove()
    {
        if (!patrolEnabled)
            return;
        if (rb == null)
            return;
        if (patrolPositions == null || patrolPositions.Length < 2)
            return;

        Vector3 target = patrolPositions[patrolIndex];
        Vector3 toTarget = target - rb.position;
        toTarget.y = 0f;

        float sqrDist = toTarget.sqrMagnitude;
        float reachSqr = pointReachDistance * pointReachDistance;
        if (sqrDist <= reachSqr)
        {
            AdvancePatrolPoint();
            target = patrolPositions[patrolIndex];
            toTarget = target - rb.position;
            toTarget.y = 0f;
            sqrDist = toTarget.sqrMagnitude;
            if (sqrDist < 0.0001f)
                return;
        }

        Vector3 moveDir = toTarget.normalized;

        Quaternion desiredRotation = Quaternion.LookRotation(moveDir, Vector3.up);
        rb.MoveRotation(Quaternion.RotateTowards(rb.rotation, desiredRotation, bodyTurnSpeed * Time.fixedDeltaTime));

        Vector3 nextPos = rb.position + moveDir * moveSpeed * Time.fixedDeltaTime;
        nextPos = ClampToTerrainBounds(nextPos);
        rb.MovePosition(nextPos);
    }

    private void AdvancePatrolPoint()
    {
        if (patrolPositions == null || patrolPositions.Length < 2)
            return;

        int next = patrolIndex + patrolDirection;
        if (next >= patrolPositions.Length || next < 0)
        {
            patrolDirection = -patrolDirection;
            next = patrolIndex + patrolDirection;
        }

        patrolIndex = Mathf.Clamp(next, 0, patrolPositions.Length - 1);
    }

    private void BuildPatrolPositions()
    {
        if (patrolPoints != null && patrolPoints.Length > 0)
        {
            List<Vector3> positions = new List<Vector3>(patrolPoints.Length);
            foreach (Transform point in patrolPoints)
            {
                if (point == null)
                    continue;
                positions.Add(ClampToTerrainBounds(point.position));
            }

            if (positions.Count >= 2)
            {
                patrolPositions = positions.ToArray();
                patrolIndex = 0;
                patrolDirection = 1;
                return;
            }
        }

        Vector3 start = ClampToTerrainBounds(transform.position);
        Vector3 dir = transform.forward;
        dir.y = 0f;
        if (dir.sqrMagnitude < 0.001f)
            dir = Vector3.forward;
        dir.Normalize();

        Vector3 end = start + dir * autoRouteDistance;
        if (hasTerrainBounds && autoRouteToTerrainBorder)
        {
            float t = DistanceToTerrainBoundsAlongRay(start, dir);
            if (t > 0.5f)
                end = start + dir * t;
        }
        end = ClampToTerrainBounds(end);

        if ((end - start).sqrMagnitude < 1f)
        {
            dir = -dir;
            end = start + dir * autoRouteDistance;
            if (hasTerrainBounds && autoRouteToTerrainBorder)
            {
                float t = DistanceToTerrainBoundsAlongRay(start, dir);
                if (t > 0.5f)
                    end = start + dir * t;
            }
            end = ClampToTerrainBounds(end);
        }

        patrolPositions = new[] { start, end };
        patrolIndex = 0;
        patrolDirection = 1;
    }

    private void CacheTerrainBounds()
    {
        hasTerrainBounds = false;
        if (!limitToTerrain)
            return;

        cachedTerrain = Terrain.activeTerrain;
        if (cachedTerrain == null)
            cachedTerrain = FindObjectOfType<Terrain>();
        if (cachedTerrain == null || cachedTerrain.terrainData == null)
            return;

        Vector3 pos = cachedTerrain.transform.position;
        Vector3 size = cachedTerrain.terrainData.size;

        minX = pos.x + terrainBorderPadding;
        maxX = pos.x + size.x - terrainBorderPadding;
        minZ = pos.z + terrainBorderPadding;
        maxZ = pos.z + size.z - terrainBorderPadding;

        hasTerrainBounds = (maxX > minX && maxZ > minZ);
    }

    private Vector3 ClampToTerrainBounds(Vector3 worldPos)
    {
        if (!hasTerrainBounds)
            return worldPos;

        worldPos.x = Mathf.Clamp(worldPos.x, minX, maxX);
        worldPos.z = Mathf.Clamp(worldPos.z, minZ, maxZ);
        return worldPos;
    }

    private float DistanceToTerrainBoundsAlongRay(Vector3 start, Vector3 dir)
    {
        if (!hasTerrainBounds)
            return 0f;

        float tMax = float.PositiveInfinity;

        if (Mathf.Abs(dir.x) > 0.0001f)
        {
            float tx = (dir.x > 0f) ? (maxX - start.x) / dir.x : (minX - start.x) / dir.x;
            tMax = Mathf.Min(tMax, tx);
        }

        if (Mathf.Abs(dir.z) > 0.0001f)
        {
            float tz = (dir.z > 0f) ? (maxZ - start.z) / dir.z : (minZ - start.z) / dir.z;
            tMax = Mathf.Min(tMax, tz);
        }

        if (float.IsInfinity(tMax) || tMax <= 0f)
            return 0f;

        return tMax;
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

    private void OnDrawGizmosSelected()
    {
        if (patrolPoints == null || patrolPoints.Length < 2)
            return;

        Gizmos.color = Color.cyan;
        Transform prevPoint = patrolPoints[0];
        Vector3 prevPos = prevPoint != null ? prevPoint.position : transform.position;

        for (int i = 1; i < patrolPoints.Length; i++)
        {
            Transform point = patrolPoints[i];
            if (point == null)
                continue;

            Gizmos.DrawLine(prevPos, point.position);
            prevPos = point.position;
        }
    }
}
