#include "Engine/Editor/AssetBrowserPanel.h"
#include "Engine/Core/Logger.h"
#include <imgui.h>
#include <algorithm>

namespace Engine {

    AssetBrowserPanel::AssetBrowserPanel() {
        // Default to assets directory
        m_BaseDirectory = "assets";
        m_CurrentDirectory = m_BaseDirectory;
        
        // Create default icons (1x1 white textures for now)
        // In a full implementation, these would be actual icon images
        uint32_t whitePixel = 0xFFFFFFFF;
        m_FolderIcon = Texture2D::Create(1, 1);
        m_FolderIcon->SetData(&whitePixel, sizeof(uint32_t));
        
        m_FileIcon = Texture2D::Create(1, 1);
        m_FileIcon->SetData(&whitePixel, sizeof(uint32_t));
        
        m_ScriptIcon = Texture2D::Create(1, 1);
        m_ScriptIcon->SetData(&whitePixel, sizeof(uint32_t));
        
        m_AudioIcon = Texture2D::Create(1, 1);
        m_AudioIcon->SetData(&whitePixel, sizeof(uint32_t));
        
        Refresh();
    }

    void AssetBrowserPanel::OnImGuiRender() {
        ImGui::Begin("Asset Browser");

        // Toolbar
        if (ImGui::Button("<-")) {
            // Go up one directory
            if (m_CurrentDirectory != m_BaseDirectory) {
                std::filesystem::path currentPath(m_CurrentDirectory);
                m_CurrentDirectory = currentPath.parent_path().string();
                Refresh();
            }
        }
        ImGui::SameLine();
        ImGui::Text("Current: %s", m_CurrentDirectory.c_str());
        ImGui::SameLine();
        if (ImGui::Button("Refresh")) {
            Refresh();
        }
        
        ImGui::Separator();
        
        // Thumbnail size slider
        ImGui::SliderFloat("Size", &m_ThumbnailSize, 50.0f, 150.0f);
        
        ImGui::Separator();

        // Asset grid
        RenderAssetGrid();

        // Selected asset info
        if (m_SelectedAsset) {
            ImGui::Separator();
            ImGui::Text("Selected: %s", m_SelectedAsset->Name.c_str());
            ImGui::Text("Path: %s", m_SelectedAsset->Path.c_str());
            ImGui::Text("Type: %s", m_SelectedAsset->Extension.c_str());
        }

        ImGui::End();
    }

    void AssetBrowserPanel::SetAssetDirectory(const std::string& path) {
        m_BaseDirectory = path;
        m_CurrentDirectory = path;
        Refresh();
    }

    void AssetBrowserPanel::Refresh() {
        m_Assets.clear();
        
        // Check if directory exists
        if (!std::filesystem::exists(m_CurrentDirectory)) {
            GE_CORE_WARN("Asset directory does not exist: {0}", m_CurrentDirectory);
            return;
        }

        try {
            // Iterate through directory
            for (const auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
                AssetItem item;
                item.Path = entry.path().string();
                item.Name = entry.path().filename().string();
                item.Extension = entry.path().extension().string();
                item.IsDirectory = entry.is_directory();
                
                // Load thumbnails for image files
                if (!item.IsDirectory) {
                    std::string ext = item.Extension;
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                    
                    if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
                        LoadThumbnail(item);
                    }
                }
                
                m_Assets.push_back(item);
            }
            
            // Sort: directories first, then files
            std::sort(m_Assets.begin(), m_Assets.end(), [](const AssetItem& a, const AssetItem& b) {
                if (a.IsDirectory != b.IsDirectory) {
                    return a.IsDirectory > b.IsDirectory;
                }
                return a.Name < b.Name;
            });
            
        } catch (const std::exception& e) {
            GE_CORE_ERROR("Failed to read asset directory: {0}", e.what());
        }
    }

    void AssetBrowserPanel::RenderAssetGrid() {
        float cellSize = m_ThumbnailSize + m_Padding;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) columnCount = 1;

        ImGui::Columns(columnCount, nullptr, false);

        for (auto& asset : m_Assets) {
            ImGui::PushID(asset.Path.c_str());
            
            // Determine icon/thumbnail
            Ref<Texture2D> icon;
            ImVec4 tintColor = ImVec4(1, 1, 1, 1);
            
            if (asset.IsDirectory) {
                icon = m_FolderIcon;
                tintColor = ImVec4(1.0f, 0.8f, 0.2f, 1.0f); // Yellow for folders
            } else if (asset.Thumbnail) {
                icon = asset.Thumbnail;
            } else {
                // Default icons based on extension
                std::string ext = asset.Extension;
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                
                if (ext == ".lua") {
                    icon = m_ScriptIcon;
                    tintColor = ImVec4(0.3f, 0.6f, 1.0f, 1.0f); // Blue for scripts
                } else if (ext == ".wav" || ext == ".mp3" || ext == ".ogg") {
                    icon = m_AudioIcon;
                    tintColor = ImVec4(0.2f, 1.0f, 0.4f, 1.0f); // Green for audio
                } else {
                    icon = m_FileIcon;
                    tintColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // Gray for other files
                }
            }
            
            // Render thumbnail button
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
            
            if (icon) {
                uint64_t textureID = icon->GetRendererID();
                if (ImGui::ImageButton(asset.Name.c_str(), reinterpret_cast<void*>(textureID), 
                                      ImVec2(m_ThumbnailSize, m_ThumbnailSize), 
                                      ImVec2(0, 0), ImVec2(1, 1), 
                                      ImVec4(0, 0, 0, 0), tintColor)) {
                    if (asset.IsDirectory) {
                        // Navigate into directory
                        m_CurrentDirectory = asset.Path;
                        Refresh();
                    } else {
                        // Select asset
                        m_SelectedAsset = &asset;
                        GE_CORE_INFO("Selected asset: {0}", asset.Path);
                    }
                }
            } else {
                // Fallback text button if no icon
                if (ImGui::Button(asset.Name.c_str(), ImVec2(m_ThumbnailSize, m_ThumbnailSize))) {
                    if (asset.IsDirectory) {
                        m_CurrentDirectory = asset.Path;
                        Refresh();
                    } else {
                        m_SelectedAsset = &asset;
                        GE_CORE_INFO("Selected asset: {0}", asset.Path);
                    }
                }
            }
            
            ImGui::PopStyleColor();
            
            // File name (wrapped)
            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + m_ThumbnailSize);
            ImGui::TextWrapped("%s", asset.Name.c_str());
            ImGui::PopTextWrapPos();
            
            ImGui::NextColumn();
            ImGui::PopID();
        }

        ImGui::Columns(1);
    }

    void AssetBrowserPanel::LoadThumbnail(AssetItem& item) {
        try {
            item.Thumbnail = Texture2D::Create(item.Path);
        } catch (const std::exception& e) {
            GE_CORE_WARN("Failed to load thumbnail for {0}: {1}", item.Path, e.what());
        }
    }

} // namespace Engine

