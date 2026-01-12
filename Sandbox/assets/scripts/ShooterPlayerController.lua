-- ShooterPlayerController.lua
-- Top-down shooter player with mouse aiming and shooting

-- Player stats
local speed = 5.0
local health = 100.0
local maxHealth = 100.0
local fireRate = 0.2  -- Seconds between shots
local timeSinceLastShot = 0.0

-- Dash mechanic
local dashSpeed = 15.0
local dashDuration = 0.2
local dashCooldown = 2.0
local isDashing = false
local dashTimer = 0.0
local dashCooldownTimer = 0.0

-- Mouse aiming
local aimDirection = Vec2.new(1, 0)  -- Default: right

function OnCreate()
    print("ShooterPlayerController initialized!")
    print("Controls: WASD = Move, Mouse = Aim, LMB = Shoot, Space = Dash")
end

function OnUpdate(dt)
    -- Get components
    local transform = self.entity:GetComponent_Transform()
    local rb = self.entity:GetComponent_Rigidbody2D()
    
    if not rb then
        return
    end
    
    -- Update timers
    timeSinceLastShot = timeSinceLastShot + dt
    
    if isDashing then
        dashTimer = dashTimer - dt
        if dashTimer <= 0 then
            isDashing = false
        end
    end
    
    if dashCooldownTimer > 0 then
        dashCooldownTimer = dashCooldownTimer - dt
    end
    
    -- Handle dash input (Space)
    if Input.IsKeyPressed(KeyCode.Space) and not isDashing and dashCooldownTimer <= 0 then
        StartDash()
    end
    
    -- Movement (WASD)
    local moveX = 0
    local moveY = 0
    
    if Input.IsKeyPressed(KeyCode.A) then
        moveX = moveX - 1
    end
    if Input.IsKeyPressed(KeyCode.D) then
        moveX = moveX + 1
    end
    if Input.IsKeyPressed(KeyCode.W) then
        moveY = moveY + 1
    end
    if Input.IsKeyPressed(KeyCode.S) then
        moveY = moveY - 1
    end
    
    -- Normalize diagonal movement
    if moveX ~= 0 or moveY ~= 0 then
        local length = math.sqrt(moveX * moveX + moveY * moveY)
        moveX = moveX / length
        moveY = moveY / length
    end
    
    -- Apply movement (dash or normal)
    local currentSpeed = isDashing and dashSpeed or speed
    rb:SetVelocity(Vec2.new(moveX * currentSpeed, moveY * currentSpeed))
    
    -- Mouse aiming (TODO: Need mouse position API)
    -- For now, aim in movement direction
    if moveX ~= 0 or moveY ~= 0 then
        aimDirection = Vec2.new(moveX, moveY)
        
        -- Update rotation to face movement direction
        local angle = math.atan2(moveY, moveX)
        transform.Rotation.z = angle
    end
    
    -- Shooting (Left Mouse Button)
    -- TODO: Need mouse button API, using E key for now
    if Input.IsKeyPressed(KeyCode.E) and timeSinceLastShot >= fireRate then
        Shoot()
        timeSinceLastShot = 0.0
    end
    
    -- Keep player in bounds (simple arena boundary)
    local bounds = 10.0
    if transform.Position.x < -bounds then transform.Position.x = -bounds end
    if transform.Position.x > bounds then transform.Position.x = bounds end
    if transform.Position.y < -bounds then transform.Position.y = -bounds end
    if transform.Position.y > bounds then transform.Position.y = bounds end
end

function StartDash()
    isDashing = true
    dashTimer = dashDuration
    dashCooldownTimer = dashCooldown
    
    print("Dash activated!")
    
    -- TODO: Play dash sound effect
    -- TODO: Particle effect
    -- TODO: Temporary invincibility
end

function Shoot()
    print("Player shoots! Direction: " .. aimDirection.x .. ", " .. aimDirection.y)
    
    -- TODO: Spawn projectile entity
    -- For now just print and create particle effect
    
    -- Get transform to spawn projectile at player position
    local transform = self.entity:GetComponent_Transform()
    
    -- In a full implementation:
    -- 1. Spawn projectile entity at player position
    -- 2. Set projectile velocity in aim direction
    -- 3. Add collision detection script to projectile
    -- 4. Play shoot sound
    -- 5. Create muzzle flash particle
    
    -- Placeholder: Print shoot event
    print("Shot fired from position: " .. transform.Position.x .. ", " .. transform.Position.y)
end

function TakeDamage(damage)
    health = health - damage
    print("Player took " .. damage .. " damage! Health: " .. health)
    
    if health <= 0 then
        OnDeath()
    else
        -- TODO: Camera shake
        -- TODO: Play hit sound
        -- TODO: Damage indicator particle
    end
end

function OnDeath()
    print("Player died! Game Over")
    -- TODO: Trigger game over state
    -- TODO: Death particle explosion
    -- TODO: Play death sound
    -- TODO: Disable player entity
end

function OnCollisionEnter(other)
    -- Collision with enemies or enemy projectiles
    print("Player collision detected with: " .. other)
    
    -- TODO: Check collision type
    -- If enemy or enemy projectile, take damage
end

