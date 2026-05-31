using UnityEngine;

public class TankPanel : MonoBehaviour {

    [Header("Ссылка на скрипт движения танка игрока")]
    public tank_script tank;

    [Header("Границы скорости для слайдера")]
    public float minSpeed = 1f;
    public float maxSpeed = 10f;

    [Header("Шрифт")]
    public Font russianFont;

    GUIStyle btnStyle;
    GUIStyle boxStyle;
    GUIStyle labelStyle;

    private bool isVisible = true;

    private Rect panelRect = new Rect(15, 15, 280, 140);

    void Start()
    {
        if (tank == null) tank = FindObjectOfType<tank_script>();
    }

    void OnGUI()
    {
        btnStyle = new GUIStyle(GUI.skin.button);
        boxStyle = new GUIStyle(GUI.skin.box);
        labelStyle = new GUIStyle(GUI.skin.label);

        if (russianFont != null)
        {
            btnStyle.font = russianFont;
            boxStyle.font = russianFont;
            labelStyle.font = russianFont;
        }

        if (!isVisible)
        {
            if (GUI.Button(new Rect(15, 15, 120, 30), "показать", btnStyle)) isVisible = true;
            return;
        }

        GUI.Box(panelRect, "Панель управления", boxStyle);
        GUI.Label(new Rect(panelRect.x + 10, panelRect.y + 30, 260, 20), "скорость танка:", labelStyle);

        if (tank == null)
        {
            GUI.Label(new Rect(panelRect.x + 10, panelRect.y + 55, 260, 20), "TankMovement не найден (назначить в Inspector)", labelStyle);
        }
        else
        {
            float newSpeed = GUI.HorizontalSlider(new Rect(panelRect.x + 10, panelRect.y + 55, 260, 20), tank.TankMoveSpeed, minSpeed, maxSpeed);
            tank.TankMoveSpeed = newSpeed;
            GUI.Label(new Rect(panelRect.x + 10, panelRect.y + 75, 260, 20), "Текущее значение: " + tank.TankMoveSpeed.ToString("0.0"), labelStyle);
        }

        if (GUI.Button(new Rect(panelRect.x + 10, panelRect.y + 100, 120, 30), "скрыть", btnStyle)) isVisible = false;
        if (GUI.Button(new Rect(panelRect.x + 140, panelRect.y + 100, 120, 30), "показать", btnStyle)) isVisible = true;
    }
}
