using Godot;
using System;
using System.Threading.Tasks;

public partial class Player : CharacterBody2D
{
    // Node references
    private Area2D _pickupArea;
    private Sprite2D _sprite;
    private Marker2D _muzzle;
    private AnimatedSprite2D _weaponSprite;
    private Sprite2D _healthBarFill;

    // Export variables
    [Export] public float Speed        = 300.0f;            //Movement variables
    [Export] public float Acceleration = 2000.0f;
    [Export] public float Friction     = 250.0f;

    [Export] public float RollSpeed    = 600.0f;            //Roll variables
    [Export] public float RollDuration = 0.25f;
    [Export] public float RollCoolDown = 0.5f;

    [Export] public float FireRate        = 0.5f;           //Attack variables
    [Export] public int Damage            = 10;
    [Export] public float RecoilForce     = 120f;
    [Export] public float MaxFireDistance = 1000f;

    [Export] public int MaxHealth = 100;                    //Health variables
    private int _currentHealth;

    private const float _spriteRotationOffset = -90.0f;

    private Vector2 _velocity = Vector2.Zero;

    private bool _isRolling = false;                        //Roll variables
    private float _rollTimer = 0.0f;
    private float _rollCooldownTimer = 0.0f;
    private Vector2 _rollDirection = Vector2.Zero;

    private float _attackTimer = 0.0f;                      //Attack variables
    private float _attackCooldownTimer = 0.0f;

    private float _fireCooldown = 0f;

    public override void _Ready()
    {
        _pickupArea = GetNode<Area2D>("PickupArea");
        _sprite = GetNode<Sprite2D>("Sprite2D");
        _muzzle = GetNode<Marker2D>("Sprite2D/Muzzle");
        _weaponSprite = GetNode<AnimatedSprite2D>("Sprite2D/WeaponSprite");
        _healthBarFill = GetNode<Sprite2D>("Health_Bar/Fill");

        AddToGroup("player");

        _currentHealth = MaxHealth;
    }

    public override void _PhysicsProcess(double delta)
    {
        float d = (float)delta;
        HandleRollTimers(d);
        _fireCooldown -= (float)delta;

        if (Input.IsActionPressed("attack"))
            Fire();

        if (_isRolling)
        {
            Velocity = _rollDirection * RollSpeed;
            MoveAndSlide();
            return;
        }

        Vector2 inputDir = GetInputVector();

        if (inputDir != Vector2.Zero)
            _velocity = _velocity.MoveToward(inputDir * Speed, Acceleration * d);
        else
            _velocity = _velocity.MoveToward(Vector2.Zero, Friction * d);

        Velocity = _velocity;
        MoveAndSlide();

        RotateTowardsMouse();

        StartRoll(inputDir);
    }

    /// <summary>
    ///		Get input direction 
    /// </summary>
    /// <returns>inputDir</returns>
    private Vector2 GetInputVector()
    {
        Vector2 inputDir = Vector2.Zero;
        inputDir.X = Input.GetActionStrength("ui_right") - Input.GetActionStrength("ui_left");
        inputDir.Y = Input.GetActionStrength("ui_down") - Input.GetActionStrength("ui_up");
        return inputDir.Normalized();
    }

    /// <summary>
    ///		Rotate sprite to face mouse direction
    /// </summary>
    private void RotateTowardsMouse()
    {
        Vector2 mousePos = GetGlobalMousePosition();
        Vector2 toMouse = mousePos - _sprite.GlobalPosition;
        if (toMouse != Vector2.Zero)
        {
            float angleDeg = MathF.Atan2(toMouse.Y, toMouse.X) * (180.0f / MathF.PI);
            _sprite.GlobalRotationDegrees = angleDeg + _spriteRotationOffset;
        }
    }

    private void StartRoll(Vector2 inputDir)
    {
        // Start roll
        if (Input.IsActionJustPressed("ui_accept") && !_isRolling && _rollCooldownTimer == 0.0f)
        {
            if (inputDir != Vector2.Zero)
            {
                _isRolling = true;
                _velocity = inputDir;
                _rollTimer = RollDuration;
            }
        }
    }

    private void HandleRollTimers(float d)
    {
        if (_rollCooldownTimer > 0f)
            _rollCooldownTimer -= d;

        if (_isRolling)
        {
            _rollTimer -= d;
            if (_rollTimer <= 0f)
            {
                _isRolling = false;
                _rollCooldownTimer = RollCoolDown;
            }
        }
    }

    private async Task Fire()
    {
        if (_fireCooldown > 0f)
            return;

        Vector2 origin = _muzzle != null ? _muzzle.GlobalPosition : GlobalPosition;
        Vector2 target = GetGlobalMousePosition();

        Vector2 direction = (target - origin).Normalized();
        Vector2 endPoint = origin + direction * MaxFireDistance;

        var spaceState = GetWorld2D().DirectSpaceState;

        var query = PhysicsRayQueryParameters2D.Create(origin, endPoint);
        query.CollideWithAreas = false;
        query.CollideWithBodies = true;
        query.Exclude = new Godot.Collections.Array<Rid> { GetRid() };

        var result = spaceState.IntersectRay(query);

        if (result.Count > 0)
        {
            Vector2 hitPosition = (Vector2)result["position"];
            Node collider = result["collider"].As<Node>();

            if (collider.HasMethod("TakeDamage"))
                collider.Call("TakeDamage", Damage);
        }

        // Fire animation and recoil (Is this even working?)
        _weaponSprite.Play("Flash");
        await ToSignal(GetTree().CreateTimer(0.3), "timeout");
        _weaponSprite.Play("default");

        ApplyRecoil(direction);
        _fireCooldown = FireRate;
    }

    private void ApplyRecoil(Vector2 direction)
    {
        Velocity -= direction * RecoilForce;
    }

    /// <summary>
    ///     Inflicts Damage on the character and updates the health bar accordingly
    /// </summary>
    /// <param name="amount"></param>
	public async void TakeDamage(int amount)
    {
        GD.Print($"Player took {amount} damage");

        _currentHealth -= amount;

        float ratio = (float)_currentHealth / MaxHealth;
        _healthBarFill.Scale = new Vector2(ratio, 0.01f);

        _sprite.Modulate = new Color(1, 0.5f, 0.5f);                        // Flash Red
        await ToSignal(GetTree().CreateTimer(0.2f), "timeout");
        _sprite.Modulate = Colors.White;

        GD.Print($"Player health: {_currentHealth}/{MaxHealth}");

        if (_currentHealth <= 0)
            Die();
    }

    private void Die()
    {
        GD.Print("Player has died!");
        GetTree().ReloadCurrentScene();
    }

}
