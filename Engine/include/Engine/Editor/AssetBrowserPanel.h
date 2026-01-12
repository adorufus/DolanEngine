#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Texture.h"
#include <string>
#include <vector>
#include <filesystem>

namespace Engine {

    struct AssetItem {
        std::string Name;
        std::string Path;
        std::string Extension;
        bool IsDirectory;
        Ref<Texture2D> Thumbnail; // For image files
    };

    class AssetBrowserPanel {
    public:
        AssetBrowserPanel();
        ~AssetBrowserPanel() = default;

        void OnImGuiRender();
        void SetAssetDirectory(const std::string& path);
        void Refresh();

        const std::string& GetCurrentDirectory() const { return m_CurrentDirectory; }

    private:
        void RenderDirectoryTree();
        void RenderAssetGrid();
        void LoadThumbnail(AssetItem& item);
        
        std::string m_CurrentDirectory;
        std::string m_BaseDirectory;
        std::vector<AssetItem> m_Assets;
        
        // UI state
        float m_ThumbnailSize = 80.0f;
        float m_Padding = 10.0f;
        AssetItem* m_SelectedAsset = nullptr;
        
        // Default icons (will be white squares for now, can be replaced with actual icons)
        Ref<Texture2D> m_FolderIcon;
        Ref<Texture2D> m_FileIcon;
        Ref<Texture2D> m_ScriptIcon;
        Ref<Texture2D> m_AudioIcon;
    };

} // namespace Engine

