using UnityEngine;

public class spot : MonoBehaviour
{
    public Light spot1;

    void OnTriggerStay(Collider other)
    {
        if (other.name == "player")
            spot1.transform.Rotate(-0.5f, 0, 0);
    }

}
