#pragma once

#include "Engine/Core/Base.h"
#include <sol/sol.hpp>
#include <string>

namespace Engine {

    class Entity;

    // Script engine - manages Lua state and script execution
    class ScriptEngine {
    public:
        static void Init();
        static void Shutdown();
        
        // Execute Lua code
        static void ExecuteString(const std::string& code);
        static bool ExecuteFile(const std::string& filepath);
        
        // Get Lua state
        static sol::state& GetLuaState();
        
        // Register engine APIs
        static void RegisterAPIs();
        
    private:
        ScriptEngine() = default;
        
        static void RegisterCoreTypes();
        static void RegisterInput();
        static void RegisterMath();
        static void RegisterEntity();
        static void RegisterComponents();
    };

} // namespace Engine

