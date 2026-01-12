-- ShooterEnemyAI.lua
-- Enemy AI for top-down shooter (chaser type)

-- Enemy stats
local speed = 3.0
local health = 20.0
local damage = 10.0
local attackRange = 0.5  -- Distance to damage player

-- AI behavior
local playerEntity = nil
local updateInterval = 0.1  -- Update AI every 0.1 seconds
local timeSinceUpdate = 0.0

function OnCreate()
    print("Enemy AI initialized")
    -- TODO: Find player entity reference
    -- For now, will track toward origin (0, 0)
end

function OnUpdate(dt)
    -- Get components
    local transform = self.entity:GetComponent_Transform()
    local rb = self.entity:GetComponent_Rigidbody2D()
    
    if not rb then
        return
    end
    
    -- Update AI less frequently for performance
    timeSinceUpdate = timeSinceUpdate + dt
    if timeSinceUpdate < updateInterval then
        return
    end
    timeSinceUpdate = 0.0
    
    -- Get player position (placeholder: assume player at origin)
    -- TODO: Get actual player entity position
    local playerPos = Vec2.new(0, 0)
    local myPos = Vec2.new(transform.Position.x, transform.Position.y)
    
    -- Calculate direction to player
    local dirX = playerPos.x - myPos.x
    local dirY = playerPos.y - myPos.y
    local distance = math.sqrt(dirX * dirX + dirY * dirY)
    
    if distance > 0.1 then
        -- Normalize direction
        dirX = dirX / distance
        dirY = dirY / distance
        
        -- Move toward player
        rb:SetVelocity(Vec2.new(dirX * speed, dirY * speed))
        
        -- Rotate to face player
        local angle = math.atan2(dirY, dirX)
        transform.Rotation.z = angle
        
        -- Check if in attack range
        if distance < attackRange then
            AttackPlayer()
        end
    else
        -- Stop if at player position
        rb:SetVelocity(Vec2.new(0, 0))
    end
end

function AttackPlayer()
    print("Enemy attacks player for " .. damage .. " damage!")
    -- TODO: Call player TakeDamage function
    -- TODO: Play attack sound
    -- TODO: Attack animation/particle
end

function TakeDamage(damageAmount)
    health = health - damageAmount
    print("Enemy took " .. damageAmount .. " damage! Health: " .. health)
    
    -- TODO: Damage flash effect
    -- TODO: Play hit sound
    
    if health <= 0 then
        OnDeath()
    end
end

function OnDeath()
    print("Enemy destroyed!")
    
    -- TODO: Death particle explosion
    -- TODO: Play explosion sound
    -- TODO: Increment player score
    -- TODO: Destroy entity
    
    -- Placeholder: Move off-screen
    local transform = self.entity:GetComponent_Transform()
    transform.Position.x = 1000
    transform.Position.y = 1000
end

function OnCollisionEnter(other)
    print("Enemy collision with: " .. other)
    -- TODO: Check if collided with player projectile
    -- If yes, take damage and destroy projectile
end

