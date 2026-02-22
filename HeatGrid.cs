using Godot;
using System;

public partial class HeatGrid : Node2D
{
    [Export] public int GridWidth = 32;
    [Export] public int GridHeight = 32;
    [Export] public int CellSize = 32;

    [Export] public int DiffusionShift = 2;   // 1 = fast, 2 = medium, 3 = slow
    [Export] public int CoolingPerTick = 1;   // temperature decay per update
    [Export] public float UpdateInterval = 0.07f;

    // Grids
    private int[,] _grid;
    private int[,] _buffer;

    // Other Private Variables
    private bool _debugVisible = false;
    private float _updateTimer = 0f;

    private TileMapLayer _tileMapLayer;     // Not used right now

    public override void _Ready()
    {

        _tileMapLayer = GetParent().GetNode<TileMapLayer>("TileMapLayer");

        InitializeGrid(); 
        GD.Print("HeatGrid initialized with " + GridWidth + "x" + GridHeight + " cells");
    }

    /// <summary>
    ///     Adds a few test heat sources just to check if the heat grid is working
    /// </summary>
    private void AddTestHeat()
    {
        // Add a hot spot in the middle
        int centerX = GridWidth / 2;
        int centerY = GridHeight / 2;

        _grid[centerX, centerY] = 80;
        _grid[centerX + 1, centerY] = 60;
        _grid[centerX - 1, centerY] = 60;
        _grid[centerX, centerY + 1] = 60;
        _grid[centerX, centerY - 1] = 60;

        // Add a cold spot in the corner
        _grid[2, 2] = -50;
        _grid[3, 2] = -30;
        _grid[2, 3] = -30;

        GD.Print("Added test heat sources");
    }

    private void InitializeGrid()
    {
        _grid = new int[GridWidth, GridHeight];
        _buffer = new int[GridWidth, GridHeight];
    }

    public override void _Process(double delta)
    {
        _updateTimer += (float)delta;

        if (Input.IsActionJustPressed("ui_text_delete"))
        {
            _debugVisible = !_debugVisible;
            QueueRedraw();
            GD.Print("Heat visualization: " + (_debugVisible ? "ON" : "OFF"));
        }

        if (_updateTimer >= UpdateInterval)
        {
            StepSimulation();
            QueueRedraw();
            _updateTimer = 0f;
        }
    }

    /// <summary>
    ///     Performs one step of the heat diffusion simulation, including cooling and handling boundaries.
    /// </summary>
    private void StepSimulation()
    {
        int width = _grid.GetLength(0);
        int height = _grid.GetLength(1);

        // Clear the buffer
        Array.Clear(_buffer, 0, _buffer.Length);

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                // Handle boundary cells
                if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
                {
                    _buffer[x, y] = _grid[x, y];
                    continue;
                }

                // Diffusion for interior cells
                int t = _grid[x, y];
                int avg = (_grid[x - 1, y] + _grid[x + 1, y] + _grid[x, y - 1] + _grid[x, y + 1]) >> 2;

                int diffused = t + ((avg - t) >> DiffusionShift);

                // Cooling
                if (diffused > 0)
                    diffused = Mathf.Max(0, diffused - CoolingPerTick);
                else if (diffused < 0)
                    diffused = Mathf.Min(0, diffused + CoolingPerTick);

                _buffer[x, y] = diffused;
            }
        }

        // Swap buffers
        (_grid, _buffer) = (_buffer, _grid);
    }

    public override void _Draw()
    {
        if (!_debugVisible)
            return;

        for (int x = 0; x < GridWidth; x++)
        {
            for (int y = 0; y < GridHeight; y++)
            {
                int t = _grid[x, y];

                // Normalize temperature to a reasonable range
                float normalized = Mathf.Clamp(t / 100f, -1f, 1f);

                Color color;

                if (normalized > 0)
                    color = new Color(normalized, 0, 0, 0.8f);   // Red heat
                else
                    color = new Color(0, 0, -normalized, 0.8f);  // Blue cold

                DrawRect(
                    new Rect2(x * CellSize, y * CellSize, CellSize, CellSize),
                    color,
                    filled: true
                );
            }
        }

        // Draw a border around the grid so we can see its extents
        DrawRect(
            new Rect2(0, 0, GridWidth * CellSize, GridHeight * CellSize),
            new Color(1, 1, 1, 0.3f),
            filled: false,
            width: 2
        );
    }

    /// <summary>
    ///     fucntion to use code to inject heat at x,y cell coordinates
    /// </summary>
    public void AddHeatAtWorldPos(Vector2 worldPos, int amount)
    {
        Vector2 local = ToLocal(worldPos);
        int x = (int)(local.X / CellSize);
        int y = (int)(local.Y / CellSize);

        if (IsInBounds(x, y))
        {
            _grid[x, y] += amount;
            QueueRedraw();
            GD.Print($"Added {amount} heat at ({x}, {y}), now: {_grid[x, y]}");
        }
        else
        {
            GD.Print($"Click out of bounds: ({x}, {y})");
        }
    }

    /// <summary>
    ///     Sample temperature at x,y cell coordinates
    /// </summary>
    public int GetTemperatureAtWorldPos(Vector2 worldPos)
    {
        Vector2 local = ToLocal(worldPos);
        int x = (int)(local.X / CellSize);
        int y = (int)(local.Y / CellSize);

        return IsInBounds(x, y) ? _grid[x, y] : 0;
    }

    /// <summary>
    ///     Reset the entire grid to zero
    /// </summary>
    public void ClearGrid()
    {
        Array.Clear(_grid, 0, _grid.Length);
        Array.Clear(_buffer, 0, _buffer.Length);
        QueueRedraw();
        GD.Print("Grid cleared");
    }

    /// <summary>
    ///     Set a specific cell's temperature
    /// </summary>
    public void SetTemperature(int x, int y, int amount)
    {
        if (IsInBounds(x, y))
        {
            _grid[x, y] = amount;
            QueueRedraw();
        }
    }

    private bool IsInBounds(int x, int y)
    {
        return x >= 0 && y >= 0 && x < _grid.GetLength(0) && y < _grid.GetLength(1);
    }

}