-- Player Controller Script
-- This script handles player movement and input

local speed = 5.0
local jumpForce = 3.0
local canJump = true
local jumpCooldown = 0.0

function OnCreate(self)
    print("[Lua] Player controller created!")
    
    -- Get components
    local transform = self.entity:GetComponent_Transform()
    local rb = self.entity:GetComponent_Rigidbody2D()
    
    print(string.format("[Lua] Player position: (%.2f, %.2f)", transform.Position.x, transform.Position.y))
end

function OnUpdate(self, dt)
    local transform = self.entity:GetComponent_Transform()
    local rb = self.entity:GetComponent_Rigidbody2D()
    
    -- Get current velocity
    local currentVel = rb:GetVelocity()
    
    -- Horizontal movement
    local moveX = 0.0
    if Input.IsKeyPressed(KeyCode.A) then
        moveX = -speed
    elseif Input.IsKeyPressed(KeyCode.D) then
        moveX = speed
    end
    
    -- Vertical movement (for 2D top-down movement, not platformer)
    local moveY = currentVel.y  -- Preserve Y velocity (gravity)
    if Input.IsKeyPressed(KeyCode.W) then
        moveY = speed
    elseif Input.IsKeyPressed(KeyCode.S) then
        moveY = -speed
    end
    
    -- Apply movement using SetVelocity
    rb:SetVelocity(Vec2.new(moveX, moveY))
    
    -- Jump cooldown
    if jumpCooldown > 0 then
        jumpCooldown = jumpCooldown - dt
        if jumpCooldown <= 0 then
            canJump = true
        end
    end
    
    -- Jump (using Space key) - only once per press
    if Input.IsKeyPressed(KeyCode.Space) and canJump then
        rb:ApplyLinearImpulseToCenter(Vec2.new(0, jumpForce), true)
        canJump = false
        jumpCooldown = 0.3  -- 300ms cooldown
    end
    
    -- Color change on key press
    if Input.IsKeyPressed(KeyCode.E) then
        local sprite = self.entity:GetComponent_SpriteRenderer()
        sprite.Color = Vec4.new(math.random(), math.random(), math.random(), 1.0)
    end
    
    -- Reset position on R
    if Input.IsKeyPressed(KeyCode.R) then
        transform.Position = Vec3.new(0, 0, 0)
        rb:SetVelocity(Vec2.new(0, 0))
        canJump = true
        jumpCooldown = 0
        print("[Lua] Player reset to origin!")
    end
end

function OnDestroy(self)
    print("[Lua] Player controller destroyed!")
end

