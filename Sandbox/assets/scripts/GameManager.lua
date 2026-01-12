-- GameManager.lua
-- Manages game state, waves, scoring for top-down shooter

-- Game state
local gameState = "MENU"  -- MENU, PLAYING, PAUSED, GAME_OVER
local score = 0
local highScore = 0
local currentWave = 0
local enemiesRemaining = 0

-- Wave configuration
local baseEnemiesPerWave = 5
local enemyIncreasePerWave = 2
local waveDelay = 3.0
local waveTimer = 0.0

-- Scoring
local SCORE_BASIC_ENEMY = 10
local SCORE_FAST_ENEMY = 20
local SCORE_SHOOTER_ENEMY = 30
local SCORE_BOSS_ENEMY = 100
local SCORE_WAVE_BONUS = 50

function OnCreate()
    print("=== SPACE SURVIVOR ===")
    print("Game Manager initialized")
    print("Press ENTER to start!")
    
    -- TODO: Load high score from file
    LoadHighScore()
end

function OnUpdate(dt)
    if gameState == "MENU" then
        -- Wait for player to press start
        if Input.IsKeyPressed(KeyCode.Enter) then
            StartGame()
        end
        
    elseif gameState == "PLAYING" then
        -- Update wave timer
        if enemiesRemaining <= 0 then
            waveTimer = waveTimer + dt
            if waveTimer >= waveDelay then
                StartNextWave()
            end
        end
        
        -- Check for pause
        if Input.IsKeyPressed(KeyCode.Escape) then
            PauseGame()
        end
        
    elseif gameState == "PAUSED" then
        -- Wait for unpause
        if Input.IsKeyPressed(KeyCode.Escape) then
            ResumeGame()
        end
        
    elseif gameState == "GAME_OVER" then
        -- Wait for restart
        if Input.IsKeyPressed(KeyCode.Enter) then
            RestartGame()
        end
    end
end

function StartGame()
    print("=== GAME START ===")
    gameState = "PLAYING"
    score = 0
    currentWave = 0
    
    -- TODO: Enable player entity
    -- TODO: Clear any existing enemies
    -- TODO: Play game music
    
    StartNextWave()
end

function StartNextWave()
    currentWave = currentWave + 1
    waveTimer = 0.0
    
    -- Calculate enemies for this wave
    local enemiesToSpawn = baseEnemiesPerWave + (currentWave - 1) * enemyIncreasePerWave
    enemiesRemaining = enemiesToSpawn
    
    print("=== WAVE " .. currentWave .. " ===")
    print("Enemies: " .. enemiesToSpawn)
    
    -- TODO: Tell spawner to spawn enemies
    -- For now, just log
    SpawnWaveEnemies(enemiesToSpawn)
end

function SpawnWaveEnemies(count)
    print("Spawning " .. count .. " enemies...")
    -- TODO: Communicate with EnemySpawner entity
    -- Different enemy types based on wave number
    
    if currentWave >= 5 then
        -- Mix of enemy types
        print("  - Basic enemies")
        print("  - Fast enemies")
    end
    
    if currentWave >= 10 then
        print("  - BOSS WAVE!")
    end
end

function OnEnemyKilled(enemyType)
    enemiesRemaining = enemiesRemaining - 1
    
    -- Add score based on enemy type
    local points = SCORE_BASIC_ENEMY
    if enemyType == "fast" then
        points = SCORE_FAST_ENEMY
    elseif enemyType == "shooter" then
        points = SCORE_SHOOTER_ENEMY
    elseif enemyType == "boss" then
        points = SCORE_BOSS_ENEMY
    end
    
    AddScore(points)
    
    print("Enemy killed! Enemies remaining: " .. enemiesRemaining)
    
    -- Check if wave complete
    if enemiesRemaining <= 0 then
        OnWaveComplete()
    end
end

function OnWaveComplete()
    local waveBonus = currentWave * SCORE_WAVE_BONUS
    AddScore(waveBonus)
    
    print("=== WAVE " .. currentWave .. " COMPLETE! ===")
    print("Bonus: +" .. waveBonus .. " points")
    print("Next wave in " .. waveDelay .. " seconds...")
    
    -- TODO: Play wave complete sound
end

function AddScore(points)
    score = score + points
    print("Score: " .. score .. " (+" .. points .. ")")
    
    -- Update high score
    if score > highScore then
        highScore = score
        print("NEW HIGH SCORE!")
    end
end

function OnPlayerDeath()
    print("=== GAME OVER ===")
    print("Final Score: " .. score)
    print("High Score: " .. highScore)
    print("Waves Survived: " .. currentWave)
    print("Press ENTER to restart")
    
    gameState = "GAME_OVER"
    
    -- TODO: Save high score to file
    SaveHighScore()
    
    -- TODO: Show game over screen
    -- TODO: Stop game music
end

function PauseGame()
    print("=== PAUSED ===")
    gameState = "PAUSED"
    -- TODO: Pause all game systems
    -- TODO: Show pause menu
end

function ResumeGame()
    print("=== RESUMED ===")
    gameState = "PLAYING"
    -- TODO: Resume all game systems
end

function RestartGame()
    StartGame()
end

function LoadHighScore()
    -- TODO: Load from file
    -- For now, just set to 0
    highScore = 0
    print("High Score loaded: " .. highScore)
end

function SaveHighScore()
    -- TODO: Save to file
    print("High Score saved: " .. highScore)
end

-- Public API for other scripts to call
function GetGameState()
    return gameState
end

function GetScore()
    return score
end

function GetWave()
    return currentWave
end

