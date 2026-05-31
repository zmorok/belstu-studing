using UnityEngine;

public class robot : MonoBehaviour
{
    public float moveSpeed = 6f;

    void Update()
    {
        Vector3 move = Vector3.zero;

        if (Input.GetKey(KeyCode.E))
            move += Vector3.forward;     // вперёд

        if (Input.GetKey(KeyCode.R))
            move += Vector3.back;        // назад

        if (Input.GetKey(KeyCode.Y))
            move += Vector3.right;       // вправо

        if (Input.GetKey(KeyCode.T))
            move += Vector3.left;        // влево

        transform.Translate(move * moveSpeed * Time.deltaTime);
    }
}
