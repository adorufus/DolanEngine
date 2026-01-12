-- Projectile.lua
-- Projectile behavior for top-down shooter

-- Projectile properties
local speed = 15.0
local damage = 25.0
local lifetime = 3.0
local age = 0.0

-- Direction (set on spawn)
local directionX = 1.0
local directionY = 0.0

function OnCreate()
    print("Projectile created")
    -- TODO: Get initial direction from spawn parameters
end

function OnUpdate(dt)
    -- Get components
    local transform = self.entity:GetComponent_Transform()
    local rb = self.entity:GetComponent_Rigidbody2D()
    
    if not rb then
        return
    end
    
    -- Move in direction
    rb:SetVelocity(Vec2.new(directionX * speed, directionY * speed))
    
    -- Update lifetime
    age = age + dt
    if age >= lifetime then
        DestroyProjectile()
    end
    
    -- Keep rotation aligned with direction
    local angle = math.atan2(directionY, directionX)
    transform.Rotation.z = angle
end

function SetDirection(x, y)
    -- Normalize direction
    local length = math.sqrt(x * x + y * y)
    if length > 0 then
        directionX = x / length
        directionY = y / length
    end
    print("Projectile direction set: " .. directionX .. ", " .. directionY)
end

function OnCollisionEnter(other)
    print("Projectile hit: " .. other)
    
    -- TODO: Check what was hit
    -- If enemy, deal damage
    -- If wall, just destroy
    
    -- Create impact effect
    CreateImpactEffect()
    
    -- Destroy projectile
    DestroyProjectile()
end

function CreateImpactEffect()
    -- TODO: Spawn particle explosion at impact point
    -- TODO: Play impact sound
    print("Impact effect at projectile position")
end

function DestroyProjectile()
    print("Projectile destroyed")
    -- TODO: Actually destroy the entity
    -- For now, move off-screen
    local transform = self.entity:GetComponent_Transform()
    transform.Position.x = 1000
    transform.Position.y = 1000
    
    -- TODO: Disable physics
    local rb = self.entity:GetComponent_Rigidbody2D()
    if rb then
        rb:SetVelocity(Vec2.new(0, 0))
    end
end

