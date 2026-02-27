using Godot;
using System;

public partial class PlayerCamera : Camera2D
{
    public Vector2 Desired_Offset; 

    public const float MaxOffsetDistance = 100f;      //Const is the closest thing to a macro in terms of performance in C#    
    public const float MinOffsetDistance = -100f;

    private const float _mouseOffsetInfluence = 0.5f;
    private const float _verticalClampFactor  = 2f;

    public override void _Ready() => MakeCurrent();   //On Ready make this the active camera  

    public override void _Process(double delta)
    {
        // Mouse position relative to camera
        Vector2 mousePos = GetGlobalMousePosition();
        Vector2 toMouse  = (mousePos - GlobalPosition) * _mouseOffsetInfluence;

        // Clamp offset
        float clampedX = Mathf.Clamp(toMouse.X, MinOffsetDistance, MaxOffsetDistance);
        float clampedY = Mathf.Clamp(toMouse.Y, MinOffsetDistance / _verticalClampFactor, MaxOffsetDistance / _verticalClampFactor);

        Desired_Offset = new Vector2(clampedX, clampedY);

        // Move camera relative to player
        Node2D player  = GetParent<CharacterBody2D>();
        GlobalPosition = player.GlobalPosition + Desired_Offset;
    }
}