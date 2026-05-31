using UnityEngine;
using UnityEngine.EventSystems;

public class Mouse : MonoBehaviour, IPointerClickHandler {

    int forse = 3000;
    
    public void OnPointerClick(PointerEventData eventData)
    {
		float red		= Random.Range(.0f, 1f);
		float green		= Random.Range(.0f, 1f);
		float blue		= Random.Range(.0f, 1f);

		Color color = new Color(red, green, blue);

        GetComponent<Renderer>().material.color = color;

        gameObject.AddComponent<Rigidbody>();

        Vector3 target = eventData.pointerCurrentRaycast.worldPosition;
        Vector3 collid = Camera.main.transform.position;

        Vector3 direction = (collid - target).normalized;

        collid = direction * forse;

        GetComponent<Rigidbody>().AddForceAtPosition(-collid, target);
    }

}
