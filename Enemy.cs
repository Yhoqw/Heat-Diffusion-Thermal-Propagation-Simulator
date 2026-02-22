using Godot;
using System;

public partial class Enemy : CharacterBody2D
{
    private enum State
    {
        Patrol,
        Chase,
        Dead,
        Attack
    }

    // Export Variables
    [Export] public float Speed = 150f;
    [Export] public float ChaseSpeed = 220f;
    [Export] public float DetectionRadius = 200f;
    [Export] public int MaxHealth = 30;

    [Export] public float AttackRange = 300f;
    [Export] public float FireRate = 0.2f;
    [Export] public int BurstCount = 3;
    [Export] public float ReloadTime = 1.2f;
    [Export] public float WindUpTime = 0.4f;
    [Export] public int Damage = 10;
    [Export] public float MaxShootDistance = 800f;

    [Export] public float RotationSpeed = 8f;

    // Private Variables
    private State _currentState = State.Patrol;                                         // _SomeVariable "_" is naming convention for private variables
    private int _health;

    private Node2D _player;
    private Vector2 _patrolPointA;
    private Vector2 _patrolPointB;
    private Vector2 _currentTarget;
    private Sprite2D _healthBarFill;

    private float _fireCooldown = 0f;
    private float _reloadTimer = 0f;
    private float _windupTimer = 0f;
    private int _shotsRemaining;
    private bool _isWindingUp = false;

    private Sprite2D _sprite;
    private const float SPRITE_ROTATION_OFFSET = -89.6f;

    public override void _Ready()
    {
        _health = MaxHealth;

        _player = GetTree().GetFirstNodeInGroup("player") as Node2D;                    // Get player Instance
        _sprite = GetNode<Sprite2D>("Sprite2D");
        _healthBarFill = GetNode<Sprite2D>("Health_Bar/Fill");

        //simple back and forth patrol
        _patrolPointA = GlobalPosition;
        _patrolPointB = GlobalPosition + new Vector2(200, 0);
        _currentTarget = _patrolPointB;
    }

    public override void _PhysicsProcess(double delta)
    {
        float d = (float)delta;

        if (_currentState == State.Dead)
            return;

        switch (_currentState)
        {
            case State.Patrol:
                Patrol();
                CheckForPlayer();
                break;

            case State.Chase:
                Chase();
                CheckLosePlayer();
                break;

            case State.Attack:
                Attack((float)delta);
                break;
        }

        MoveAndSlide();
        RotateSpriteTowardsVelocity(d);
    }

    /// <summary>
    ///     Patrol State: Moves between two points. If player is detected, switch to Chase state.
    /// </summary>
    private void Patrol()
    {
        Vector2 direction = (_currentTarget - GlobalPosition).Normalized();
        Velocity = direction * Speed;

        if (GlobalPosition.DistanceTo(_currentTarget) < 10f)
        {
            _currentTarget = _currentTarget == _patrolPointA ? _patrolPointB : _patrolPointA;
        }
    }

    /// <summary>
    ///     Chase State: Chases Player
    /// </summary>
    private void Chase()
    {
        if (_player == null) return;

        float distance = GlobalPosition.DistanceTo(_player.GlobalPosition);

        if (distance <= AttackRange && HasLineOfSight())
        {
            EnterAttackState();
            return;
        }

        Vector2 direction = (_player.GlobalPosition - GlobalPosition).Normalized();
        Velocity = direction * ChaseSpeed;
    }

    private bool HasLineOfSight()
    {
        if (_player == null) return false;

        var spaceState = GetWorld2D().DirectSpaceState;

        var query = PhysicsRayQueryParameters2D.Create(
            GlobalPosition,
            _player.GlobalPosition
            );

        query.Exclude = new Godot.Collections.Array<Rid> { GetRid() };
        query.CollideWithAreas = false;
        query.CollideWithBodies = true;

        var result = spaceState.IntersectRay(query);

        if (result.Count == 0)
            return false;

        Node collider = result["collider"].As<Node>();

        return collider.IsInGroup("player");
    }

    private void EnterAttackState()
    {
        _currentState = State.Attack;
        Velocity = Vector2.Zero;

        _isWindingUp = true;
        _windupTimer = WindUpTime;

        _shotsRemaining = BurstCount;
    }

    private void Attack(float delta)
    {
        if (_player == null)
        {
            _currentState = State.Patrol;
            return;
        }

        float distance = GlobalPosition.DistanceTo(_player.GlobalPosition);

        if (distance > AttackRange || !HasLineOfSight())
        {
            _currentState = State.Chase;
            return;
        }

        LookAt(_player.GlobalPosition);

        //Wind up
        if (_isWindingUp)
        {
            _windupTimer -= delta;

            if (_windupTimer <= 0f)
            {
                _isWindingUp = false;
                _fireCooldown = 0f;
            }

            return;
        }

        // Reload phase
        if (_shotsRemaining <= 0)
        {
            _reloadTimer -= delta;

            if (_reloadTimer <= 0f)
            {
                _shotsRemaining = BurstCount;
                _isWindingUp = true;
                _windupTimer = WindUpTime;
            }

            return;
        }

        // Fire burst
        _fireCooldown -= delta;

        if (_fireCooldown <= 0f)
        {
            Shoot();
            _shotsRemaining--;
            _fireCooldown = FireRate;

            if (_shotsRemaining <= 0)
            {
                _reloadTimer = ReloadTime;
            }
        }
    }

    private void Shoot()
    {
        Vector2 origin = GlobalPosition;
        Vector2 direction = (_player.GlobalPosition - origin).Normalized();
        Vector2 endPoint = origin + direction * MaxShootDistance;

        var spaceState = GetWorld2D().DirectSpaceState;
        var query = PhysicsRayQueryParameters2D.Create(origin, endPoint);
        query.Exclude = new Godot.Collections.Array<Rid> { GetRid() };

        var result = spaceState.IntersectRay(query);

        if (result.Count > 0)
        {
            Node collider = result["collider"].As<Node>();

            if (collider.HasMethod("TakeDamage"))
            {
                collider.Call("TakeDamage", Damage);
            }
        }
    }

    /// <summary>
    ///     Die state: Enemy is dead, stop all movement and interactions, play death animation, then remove from scene.
    /// </summary>
    private void Die()
    {
        _currentState = State.Dead;
        Velocity = Vector2.Zero;
        QueueFree();
    }

    private void CheckForPlayer()
    {
        if (_player == null) return;

        float distance = GlobalPosition.DistanceTo(_player.GlobalPosition);
        if (distance < DetectionRadius)
        {
            _currentState = State.Chase;
        }
    }

    private void CheckLosePlayer()
    {
        if (_player == null) return;

        float distance = GlobalPosition.DistanceTo(_player.GlobalPosition);
        if (distance > DetectionRadius * 1.5f)
        {
            _currentState = State.Patrol;
        }
    }

    /// <summary>
    ///     Inflicts Damage on the character and updates the health bar accordingly
    /// </summary>
    /// <param name="amount"></param>
    public async void TakeDamage(int amount)
    {
        if (_currentState == State.Dead)
            return;

        GD.Print($"Entity took {amount} damage");
        _health -= amount;

        float ratio = (float)_health / MaxHealth;
        _healthBarFill.Scale = new Vector2(ratio, 0.01f);

        _sprite.Modulate = new Color(1, 0.5f, 0.5f);                        // Flash Red
        await ToSignal(GetTree().CreateTimer(0.2f), "timeout");
        _sprite.Modulate = Colors.White;

        GD.Print($"Entity health: {_health}/{MaxHealth}");

        if (_health <= 0)
        {
            Die();
        }
    }


    private void RotateSpriteTowardsVelocity(float delta)
    {
        float targetAngle = Velocity.Angle() + SPRITE_ROTATION_OFFSET;
        float newAngle = Mathf.LerpAngle(
            _sprite.Rotation,
            targetAngle,
            RotationSpeed * delta
        );

        _sprite.Rotation = newAngle;
    }   
}
