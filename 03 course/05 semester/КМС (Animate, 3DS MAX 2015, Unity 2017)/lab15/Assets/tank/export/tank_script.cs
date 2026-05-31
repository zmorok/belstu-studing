using System.Collections;
using System.Collections.Generic;
using System.Security.Policy;
using UnityEngine;

public class tank_script : MonoBehaviour {

    Transform turret;									// башня
    Transform barrel;									// дуло
		
	public float TankMoveSpeed = 1f;							// скорость движения танка
    float nitroMultiplier = 3f;                         // во сколько раз ускоряемся при нитро
    float TurretRotateSpeed = 0.6f;						// скорость поворота башни
	
    float barrelAngleZ;                                 // собственный угол ствола
    float BarrelMinAngle = 0f, BarrelMaxAngle = 20f;	// углы поворота ствола

    public GameObject Bomb;                             // префаб бомбы
    public float BombForwardOffset = 15f;               // смещение для бомб
    public float BombAreaSize = 10f;                    // зона сброса бомб
    public float BombDropHeight = 10f;                  // высота сброса бомб

    AudioSource TankSound;                              // источник звука
    public float SoundValue = 0.5f;
    private float SoundTargetVolume;
    private float SoundFadeSpeed = 0f;
    bool isMoving = false;                              // текущее состояние звука
    bool wasMoving = false;                             // состояние звука в прошлом кадре

    void Start () {
		turret = gameObject.transform.Find("turret");

		barrel = turret.Find("barrel");
        barrelAngleZ = barrel.localEulerAngles.z;
        //if (barrelAngleZ > 180f) barrelAngleZ -= 360f; // 0..360 -> -180..180

        TankSound = GetComponent<AudioSource>();
        TankSound.volume = 0;
        TankSound.loop = true;
        TankSound.Stop();

        SoundTargetVolume = SoundValue;
    }
	
	void Update () {
        // движение танка + ускорение на shift + звук
        TankMovement();

        // вращение башни и дула
        TurretMovement();

        // переворот танка на гусеницы
        if (Input.GetKeyDown(KeyCode.Z))
        {
            ResetTankOrientation();
        }

        // сбрасывание бомб
        if (Input.GetKeyDown(KeyCode.X))
        {
            DropBomb();
        }

        // плавное воспроизведение звука
        PlaySound();
    }

    void TankMovement()
	{
        float z = Input.GetAxis("Vertical");            // W и S

        
        float currentSpeed = TankMoveSpeed / 10f;             // ускорение при удержании Shift
        if (Input.GetKey(KeyCode.LeftShift) || Input.GetKey(KeyCode.RightShift))
            currentSpeed *= nitroMultiplier;
        

        transform.Translate(0, 0, z * currentSpeed);    // движение вперёд-назад

        float x = Input.GetAxis("Horizontal");          // A и D
        transform.Rotate(0, x, 0);                      // поворот танка

        wasMoving = isMoving;
        isMoving = (Mathf.Abs(x) > 0.001f || Mathf.Abs(z) > 0.001f);

        if (isMoving && !wasMoving)                     // начинается движение
        {
            SoundTargetVolume = SoundValue;
            TankSound.Play();
        }
        else if (!isMoving && wasMoving)                // остановка
            SoundTargetVolume = 0f;
    }

    void TurretMovement()
    {
        float h = Input.GetAxis("Mouse X");
        turret.Rotate(0f, h * TurretRotateSpeed, 0f);   // поворот башни

        float v = Input.GetAxis("Mouse Y");
        barrelAngleZ += v * TurretRotateSpeed;
        barrelAngleZ = Mathf.Clamp(barrelAngleZ, BarrelMinAngle, BarrelMaxAngle);
        barrel.localEulerAngles = new Vector3(-barrelAngleZ, 0f, 0f);
    }

    void ResetTankOrientation()
    {
        Vector3 euler = transform.eulerAngles;
        float yaw = euler.y;

        transform.rotation = Quaternion.Euler(0f, yaw, 0f);

        Rigidbody rb = GetComponent<Rigidbody>();       // убирается инерция
        if (rb != null)
        {
            rb.velocity = Vector3.zero;
            rb.angularVelocity = Vector3.zero;
        }
    }

    void DropBomb() 
    {
        Vector3 areaCenter = transform.position + transform.forward * BombForwardOffset;    // центр зоны падения

        // случайное место падения бомбы
        float half = BombAreaSize / 2f;
        float randX = Random.Range(-half, half);
        float randZ = Random.Range(-half, half);

        Vector3 spawnPoint = new Vector3(
            areaCenter.x + randX,
            areaCenter.y + BombDropHeight,
            areaCenter.z + randZ
        );

        Instantiate(Bomb, spawnPoint, Quaternion.identity);
    }

    void PlaySound()
    {
        if (TankSound == null) return;
        TankSound.volume = Mathf.MoveTowards(TankSound.volume, SoundTargetVolume, SoundFadeSpeed + Time.deltaTime);
        if (!isMoving && TankSound.volume <= 0.001f && TankSound.isPlaying) TankSound.Stop();
    }
}
