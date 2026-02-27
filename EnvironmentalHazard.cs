using Godot;
using System;

public partial class EnvironmentalHazard : StaticBody2D
{
    // Variables 
	[Export] public float ZoneTemperature = 50.0f;
	[Export] public Color DebugColor      = Colors.Red;
	[Export] public int Health            = 50;
    
    private CollisionShape2D _temperatureZone;
    private Sprite2D _sprite;

    //Functions
    public override void _Ready()
    {
        _sprite          = GetNode<Sprite2D>("Sprite2D");
        _temperatureZone = GetNode<CollisionShape2D>("TemperatureZone/CollisionShape2D");
    }

    /// <summary>
    ///     Inflicts Damage on the character and updates the health bar accordingly
    /// </summary>
    /// <param name="amount"></param>
	public async void TakeDamage(int amount)
    {
        GD.Print($"Hazard took {amount} damage");

        Health -= amount;

        GD.Print($"Hazard health: {Health}");

        if (Health <= 0)  
            Explode();
    }

    private void Explode()
    {
        GD.Print("Hazard Blew up");
        _temperatureZone.Disabled = false;
    }
}
