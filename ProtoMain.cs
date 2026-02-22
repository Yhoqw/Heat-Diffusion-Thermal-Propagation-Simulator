using Godot;
using System;

public partial class HeatGridTest : Node2D
{
    private HeatGrid _heatGrid;

    public override void _Ready()
    {
        // Adjust path based on your scene structure
        _heatGrid = GetNode<HeatGrid>("HeatGrid");

        if (_heatGrid == null)
        {
            GD.PrintErr("HeatGrid not found! Check the node path.");
            return;
        }

        GD.Print("HeatGridTest initialized");
    }
}
