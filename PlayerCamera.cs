using Godot;
using System;

public partial class PlayerCamera : Camera2D
{
    public Vector2 Desired_Offset; 

    public float Max_Offset_Distance = 100f;
    public float Min_Offset_Distance = -100f;

    public override void _Ready()
    {
        MakeCurrent();
    }

    public override void _Process(double delta)
    {
        // Mouse position relative to camera
        Vector2 mousePos = GetGlobalMousePosition();
        Vector2 toMouse = (mousePos - GlobalPosition) * 0.5f;

        // Clamp offset
        float clampedX = Mathf.Clamp(toMouse.X, Min_Offset_Distance, Max_Offset_Distance);
        float clampedY = Mathf.Clamp(toMouse.Y, Min_Offset_Distance / 2f, Max_Offset_Distance / 2f);

        Desired_Offset = new Vector2(clampedX, clampedY);

        // Move camera relative to player
        Node2D player = GetParent<CharacterBody2D>();
        GlobalPosition = player.GlobalPosition + Desired_Offset;

    }
}