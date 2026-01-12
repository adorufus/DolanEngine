-- Simple rotator script
-- Rotates the entity continuously

local rotationSpeed = 1.5 -- radians per second (about 86 degrees/sec)

function OnCreate(self)
    print("[Lua] Rotator created!")
end

function OnUpdate(self, dt)
    local transform = self.entity:GetComponent_Transform()
    
    -- Rotate around Z-axis (in radians for Box2D)
    transform.Rotation.z = transform.Rotation.z + rotationSpeed * dt
    
    -- Wrap around 2*PI radians
    local twoPi = 2.0 * math.pi
    if transform.Rotation.z > twoPi then
        transform.Rotation.z = transform.Rotation.z - twoPi
    end
end

function OnDestroy(self)
    print("[Lua] Rotator destroyed!")
end

