#pragma once

#include "Engine/Core/Base.h"
#include <string>
#include <unordered_map>
#include <filesystem>
#include <chrono>

namespace Engine {

    // Watches script files and reloads them when changed
    class ScriptReloader {
    public:
        static void Init();
        static void Shutdown();
        
        // Add a script to watch
        static void WatchScript(const std::string& filepath);
        
        // Check for changes and reload if needed
        static void CheckForChanges();
        
        // Get callback when a script is reloaded
        using ReloadCallback = std::function<void(const std::string&)>;
        static void SetReloadCallback(ReloadCallback callback);
        
    private:
        struct ScriptWatchInfo {
            std::string Path;
            std::filesystem::file_time_type LastModified;
        };
        
        static std::unordered_map<std::string, ScriptWatchInfo> s_WatchedScripts;
        static ReloadCallback s_ReloadCallback;
    };

} // namespace Engine

