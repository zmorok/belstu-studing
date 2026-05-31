using UnityEngine;

public class camera_script : MonoBehaviour
{
    public Vector3 offset = new Vector3(0, 5, -10);
    public float smoothTime = 0.25f;
    private Vector3 velocity = Vector3.zero;
    [SerializeField] private Transform target;

    public float pitch = 20f;

    void Start()
    {
        if (target == null)
            target = transform.parent.Find("body");
    }

    void LateUpdate()
    {
        if (!target) return;

        // поворот танка по Y (куда он смотрит)
        float yaw = target.eulerAngles.y;
        Quaternion yawRotation = Quaternion.Euler(0f, yaw, 0f);

        // позицию камеры: "сзади и сверху" + проворот offset вокруг танка
        Vector3 desiredPosition = target.position + yawRotation * offset;

        // плавное перемещение в нужную точку
        transform.position = Vector3.SmoothDamp(transform.position, desiredPosition, ref velocity, smoothTime);
        //transform.position = target.position + yawRotation * offset;

        // поворот камеры
        transform.rotation = Quaternion.Euler(pitch, yaw, 0f);
    }
}
