-- EnemySpawner.lua
-- Spawns enemies at random positions around the arena

-- Spawn configuration
local spawnDistance = 12.0  -- Distance from center to spawn
local spawnRate = 2.0  -- Seconds between spawns
local timeSinceLastSpawn = 0.0

-- Wave control
local enemiestoSpawn = 0
local isSpawning = false

function OnCreate()
    print("Enemy Spawner initialized")
end

function OnUpdate(dt)
    if not isSpawning then
        return
    end
    
    -- Update spawn timer
    timeSinceLastSpawn = timeSinceLastSpawn + dt
    
    if timeSinceLastSpawn >= spawnRate and enemiestoSpawn > 0 then
        SpawnEnemy()
        enemiestoSpawn = enemiestoSpawn - 1
        timeSinceLastSpawn = 0.0
        
        if enemiestoSpawn <= 0 then
            isSpawning = false
            print("Wave spawn complete!")
        end
    end
end

function SpawnEnemy()
    -- Generate random angle
    local angle = math.random() * math.pi * 2
    
    -- Calculate spawn position on circle edge
    local spawnX = math.cos(angle) * spawnDistance
    local spawnY = math.sin(angle) * spawnDistance
    
    print("Spawning enemy at: " .. spawnX .. ", " .. spawnY)
    
    -- TODO: Actually spawn enemy entity
    -- In a full implementation:
    -- 1. Clone enemy prefab
    -- 2. Set position to (spawnX, spawnY)
    -- 3. Initialize enemy script
    -- 4. Play spawn particle effect
    
    -- Placeholder for entity spawning
    -- This would need engine support for runtime entity creation from Lua
end

function StartWave(enemyCount)
    print("Spawner starting wave with " .. enemyCount .. " enemies")
    enemiestoSpawn = enemyCount
    isSpawning = true
    timeSinceLastSpawn = spawnRate  -- Spawn first enemy immediately
end

-- Example: GameManager would call this
-- spawner:StartWave(10)

