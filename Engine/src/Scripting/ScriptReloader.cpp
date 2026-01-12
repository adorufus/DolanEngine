#include "Engine/Scripting/ScriptReloader.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Core/Logger.h"

namespace Engine {

    std::unordered_map<std::string, ScriptReloader::ScriptWatchInfo> ScriptReloader::s_WatchedScripts;
    ScriptReloader::ReloadCallback ScriptReloader::s_ReloadCallback;

    void ScriptReloader::Init() {
        GE_CORE_INFO("Script Reloader initialized");
    }

    void ScriptReloader::Shutdown() {
        s_WatchedScripts.clear();
    }

    void ScriptReloader::WatchScript(const std::string& filepath) {
        namespace fs = std::filesystem;
        
        if (!fs::exists(filepath)) {
            GE_CORE_WARN("Script file does not exist: {}", filepath);
            return;
        }
        
        ScriptWatchInfo info;
        info.Path = filepath;
        info.LastModified = fs::last_write_time(filepath);
        
        s_WatchedScripts[filepath] = info;
        GE_CORE_INFO("Watching script: {}", filepath);
    }

    void ScriptReloader::CheckForChanges() {
        namespace fs = std::filesystem;
        
        for (auto& [path, info] : s_WatchedScripts) {
            if (!fs::exists(path))
                continue;
            
            auto lastModified = fs::last_write_time(path);
            if (lastModified != info.LastModified) {
                GE_CORE_INFO("Script changed, reloading: {}", path);
                
                // Reload the script
                if (ScriptEngine::ExecuteFile(path)) {
                    info.LastModified = lastModified;
                    
                    // Notify callback
                    if (s_ReloadCallback) {
                        s_ReloadCallback(path);
                    }
                }
            }
        }
    }

    void ScriptReloader::SetReloadCallback(ReloadCallback callback) {
        s_ReloadCallback = callback;
    }

} // namespace Engine

