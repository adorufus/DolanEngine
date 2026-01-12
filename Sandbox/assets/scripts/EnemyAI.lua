-- Enemy AI Script
-- Simple patrol behavior

local patrolSpeed = 2.0
local patrolDistance = 5.0
local startX = 0.0
local direction = 1.0

function OnCreate(self)
    print("[Lua] Enemy AI created!")
    
    local transform = self.entity:GetComponent_Transform()
    startX = transform.Position.x
    
    -- Make enemy red
    local sprite = self.entity:GetComponent_SpriteRenderer()
    sprite.Color = Vec4.new(1.0, 0.3, 0.3, 1.0)
end

function OnUpdate(self, dt)
    local transform = self.entity:GetComponent_Transform()
    local rb = self.entity:GetComponent_Rigidbody2D()
    
    -- Patrol back and forth
    local currentX = transform.Position.x
    local distFromStart = currentX - startX
    
    -- Change direction at patrol limits
    if distFromStart > patrolDistance then
        direction = -1.0
    elseif distFromStart < -patrolDistance then
        direction = 1.0
    end
    
    -- Apply movement
    local currentVel = rb:GetVelocity()
    rb:SetVelocity(Vec2.new(direction * patrolSpeed, currentVel.y))
end

function OnDestroy(self)
    print("[Lua] Enemy AI destroyed!")
end

