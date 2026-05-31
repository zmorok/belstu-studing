using System.Net;
using UnityEngine;

public class doors : MonoBehaviour
{
    public Transform leftDoor;
    public Transform rightDoor;

    public float openAngle = 90f;
    public float speed = 2f;

    private bool playerInside = false;

    private Vector3 leftClosedEuler;
    private Vector3 rightClosedEuler;
    private Vector3 leftOpenEuler;
    private Vector3 rightOpenEuler;

    public GameObject palka;
    public float palkaSpeed = 5f;    // скорость полёта палки
    public float palkaFlyDistance = 10f; // на какое расстояние она пролетит
    private Vector3 palkaStartPos;
    private Vector3 palkaStartRot;
    private bool palkaFlying = false;
    private Vector3 palkaDirPos; // фиксированное направление полёта палки
    private Vector3 palkaDirRot; // фиксированное направление врашение палки

    public GameObject robot;
    private bool robotDetected = false;

    private Vector3 forPointsVec;
    public Light Point1;
    public Light Point2;
    public Light Point3;
    public float maxIntensity = 5f; // максимальная яркость
    public float intensitySpeed = 1f; // скорость изменения яркости
    public float rotationSpeed = 60f; // скорость вращения поинтов вокруг player

    private float p1Start;
    private float p2Start;
    private float p3Start;

    void Start()
    {
        // закрытое положение
        leftClosedEuler = leftDoor.localEulerAngles;
        rightClosedEuler = rightDoor.localEulerAngles;

        // открытое положение
        leftOpenEuler = leftClosedEuler - new Vector3(0, openAngle, 0);
        rightOpenEuler = rightClosedEuler + new Vector3(0, openAngle, 0);

        if (palka != null)
        {
            palkaStartPos = palka.transform.position;
            palkaStartRot = palka.transform.eulerAngles;
        }

        if (Point1 != null) p1Start = Point1.intensity;
        if (Point2 != null) p2Start = Point2.intensity;
        if (Point3 != null) p3Start = Point3.intensity;
    }

    void Update()
    {
        // ворота
        Vector3 targetLeft = playerInside ? leftOpenEuler : leftClosedEuler;
        Vector3 targetRight = playerInside ? rightOpenEuler : rightClosedEuler;

        leftDoor.transform.Rotate(Vector3.up, (targetLeft.y - leftDoor.localEulerAngles.y) * Time.deltaTime * speed);
        rightDoor.transform.Rotate(Vector3.up, (targetRight.y - rightDoor.localEulerAngles.y) * Time.deltaTime * speed);
        // targetLeft и targetRight - целевые углы для левой и правой двери


        // палка
        if (palkaFlying && palka != null)
        {
            // летит вперёд по своей оси Z

            palka.transform.position += palkaDirPos * palkaSpeed * Time.deltaTime;

            //palka.transform.Translate(Vector3.forward * palkaSpeed * Time.deltaTime, Space.Self);


            ////////////////////////////////////////////////////////////////////

            palka.transform.Rotate(palkaDirRot, palkaSpeed * 200f * Time.deltaTime, Space.World);

            //Vector3 axis = palka.transform.forward;  // направление движения
            //palka.transform.Rotate(axis, palkaSpeed * 200f * Time.deltaTime, Space.World);

            //palka.transform.Rotate(Vector3.up * palkaSpeed * 200f * Time.deltaTime, Space.Self);


            if (Vector3.Distance(palkaStartPos, palka.transform.position) >= palkaFlyDistance)
            {
                palkaFlying = false;
                palka.transform.position = palkaStartPos;
                palka.transform.eulerAngles = palkaStartRot;
            }
        }
    }

    void OnTriggerEnter(Collider other)
    {
        if (other.name == "robot") robotDetected = true;

        if (other.name == "player" && robotDetected == false)
        {
            forPointsVec = other.transform.up.normalized;
            playerInside = true;
        }

        if (other.name == "player" && robotDetected == true)
        {
            forPointsVec = other.transform.up.normalized;
            playerInside = true;

            if (palka != null)
            {
                palkaDirRot = palka.transform.up;
                palkaDirPos = palka.transform.forward;
                palkaFlying = true;
                palka.transform.position = palkaStartPos;
            }
        }

    }

    void OnTriggerStay(Collider other)
    {
        if (other.name == "player")
        {
            Point1.intensity = Mathf.MoveTowards(Point1.intensity, maxIntensity, intensitySpeed * Time.deltaTime);
            Point2.intensity = Mathf.MoveTowards(Point2.intensity, maxIntensity, intensitySpeed * Time.deltaTime);
            Point3.intensity = Mathf.MoveTowards(Point3.intensity, maxIntensity, intensitySpeed * Time.deltaTime);

            // вращение вокруг player
            Point1.transform.RotateAround(other.transform.position, forPointsVec, rotationSpeed * Time.deltaTime);
            Point2.transform.RotateAround(other.transform.position, forPointsVec, rotationSpeed * Time.deltaTime);
            Point3.transform.RotateAround(other.transform.position, forPointsVec, rotationSpeed * Time.deltaTime);
        }
    }

    void OnTriggerExit(Collider other)
    {
        if (other.name == "robot") robotDetected = false;
        if (other.name == "player") playerInside = false;
        Point1.intensity = p1Start;
        Point2.intensity = p2Start;
        Point3.intensity = p3Start;
    }
}