#include <Engine.h>

class EditorLayer : public Engine::Layer {
public:
    EditorLayer() : Engine::Layer("EditorLayer") {}

    void OnAttach() override {
        // Create framebuffer for viewport
        Engine::FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Engine::Framebuffer::Create(fbSpec);
        
        // Log some welcome messages
        Engine::ConsolePanel::AddLog(Engine::LogLevel::Info, "Editor started!");
        Engine::ConsolePanel::AddLog(Engine::LogLevel::Info, "Welcome to the Game Engine Editor");
        
        // Create demo scene
        m_Scene = Engine::CreateRef<Engine::Scene>("Editor Scene");
        
        // Create camera (centered at origin)
        auto camera = m_Scene->CreateEntity("Camera");
        auto& cameraTransform = camera.GetComponent<Engine::TransformComponent>();
        cameraTransform.Position = { 0.0f, 0.0f, 0.0f };  // Explicitly centered at origin
        auto& cameraComp = camera.AddComponent<Engine::CameraComponent>();
        cameraComp.Camera.SetProjection(-16.0f, 16.0f, -9.0f, 9.0f);  // Viewport from -16 to 16 (X), -9 to 9 (Y)
        cameraComp.Primary = true;
        
        // Create origin marker (helps see where 0,0,0 is)
        auto origin = m_Scene->CreateEntity("Origin Marker");
        auto& originTransform = origin.GetComponent<Engine::TransformComponent>();
        originTransform.Position = { 0.0f, 0.0f, 0.0f };
        originTransform.Scale = { 0.5f, 0.5f, 1.0f };
        auto& originSprite = origin.AddComponent<Engine::SpriteRendererComponent>();
        originSprite.Color = { 1.0f, 1.0f, 0.0f, 1.0f };  // Yellow marker at origin
        
        // Create some demo entities
        auto square1 = m_Scene->CreateEntity("Square 1");
        auto& sq1Transform = square1.GetComponent<Engine::TransformComponent>();
        sq1Transform.Position = { -3.0f, 0.0f, 0.0f };  // Moved left of origin
        sq1Transform.Scale = { 1.0f, 1.0f, 1.0f };
        auto& sq1Sprite = square1.AddComponent<Engine::SpriteRendererComponent>();
        sq1Sprite.Color = { 0.8f, 0.2f, 0.3f, 1.0f };
        
        auto square2 = m_Scene->CreateEntity("Square 2");
        auto& sq2Transform = square2.GetComponent<Engine::TransformComponent>();
        sq2Transform.Position = { 3.0f, 0.0f, 0.0f };  // Moved right of origin
        sq2Transform.Scale = { 1.0f, 1.0f, 1.0f };
        auto& sq2Sprite = square2.AddComponent<Engine::SpriteRendererComponent>();
        sq2Sprite.Color = { 0.2f, 0.3f, 0.8f, 1.0f };
        auto& sq2Rb = square2.AddComponent<Engine::Rigidbody2DComponent>();
        sq2Rb.Type = Engine::Rigidbody2DComponent::BodyType::Dynamic;
        sq2Rb.GravityScale = 0.0f;  // No gravity for demo
        auto& sq2Collider = square2.AddComponent<Engine::BoxCollider2DComponent>();
        sq2Collider.Size = { 0.5f, 0.5f };
        
        // Create ground
        auto ground = m_Scene->CreateEntity("Ground");
        auto& groundTransform = ground.GetComponent<Engine::TransformComponent>();
        groundTransform.Position = { 0.0f, -5.0f, 0.0f };
        groundTransform.Scale = { 20.0f, 1.0f, 1.0f };
        auto& groundSprite = ground.AddComponent<Engine::SpriteRendererComponent>();
        groundSprite.Color = { 0.5f, 0.5f, 0.5f, 1.0f };
        auto& groundRb = ground.AddComponent<Engine::Rigidbody2DComponent>();
        groundRb.Type = Engine::Rigidbody2DComponent::BodyType::Static;
        auto& groundCollider = ground.AddComponent<Engine::BoxCollider2DComponent>();
        groundCollider.Size = { 0.5f, 0.5f };
        
        // Start scene
        m_Scene->OnStart();
        
        // Setup hierarchy panel
        m_SceneHierarchyPanel.SetContext(m_Scene);
        
        GE_CORE_INFO("=== Game Engine Editor ===");
        GE_CORE_INFO("Use the Scene Hierarchy to select entities");
        GE_CORE_INFO("Use the Properties panel to edit components");
        GE_CORE_INFO("Right-click in hierarchy to create/delete entities");
        GE_CORE_INFO("========================");
    }

    void OnDetach() override {
        m_Scene->OnStop();
    }

    void OnUpdate(Engine::TimeStep ts) override {
        // Update profiler with frame time
        Engine::Profiler::Get().SetFrameTime(ts.GetMilliseconds());
        
        // Resize framebuffer if needed
        auto spec = m_Framebuffer->GetSpecification();
        if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && 
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }
        
        // Update scene based on state
        if (m_SceneState == SceneState::Play) {
            m_Scene->OnUpdate(ts);
        } else if (m_SceneState == SceneState::Step) {
            m_Scene->OnUpdate(ts);
            m_SceneState = SceneState::Pause; // Step once then pause
        }
        // In Edit or Pause mode, don't update scene
        
        // Render scene to framebuffer
        m_Framebuffer->Bind();
        Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.15f, 1.0f });
        Engine::RenderCommand::Clear();
        
        m_Scene->OnRender();
        
        m_Framebuffer->Unbind();
    }
    
    void OnScenePlay() {
        m_SceneState = SceneState::Play;
        // In a full implementation, we'd save the edit-time scene state here
        m_Scene->OnStart();
        Engine::ConsolePanel::AddLog(Engine::LogLevel::Info, "Scene started (Play mode)");
    }
    
    void OnSceneStop() {
        m_SceneState = SceneState::Edit;
        m_Scene->OnStop();
        // In a full implementation, we'd restore the edit-time scene state here
        m_Scene->OnStart(); // Restart for editing
        Engine::ConsolePanel::AddLog(Engine::LogLevel::Info, "Scene stopped (Edit mode)");
    }

    void OnImGuiRender() override {
        // Create dockspace
        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        
        // Main menu bar
        if (ImGui::BeginMenuBar()) {
            // Display current scene name
            std::string sceneName = m_CurrentScenePath.empty() ? "Untitled*" : m_CurrentScenePath;
            ImGui::Text("[%s]", sceneName.c_str());
            ImGui::Separator();
            
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
                    NewScene();
                }
                if (ImGui::MenuItem("Open Scene...", "Ctrl+O")) {
                    OpenScene();
                }
                if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
                    SaveScene();
                }
                if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S")) {
                    SaveSceneAs();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) {
                    // Request application shutdown
                    m_Running = false;
                }
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "Ctrl+Z", false, false)) {}
                if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Preferences")) {}
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Viewport", nullptr, &m_ShowViewport);
                ImGui::MenuItem("Scene Hierarchy", nullptr, &m_ShowHierarchy);
                ImGui::MenuItem("Properties", nullptr, &m_ShowProperties);
                ImGui::MenuItem("Console", nullptr, &m_ShowConsole);
                ImGui::MenuItem("Stats", nullptr, &m_ShowStats);
                ImGui::MenuItem("Profiler", nullptr, &m_ShowProfiler);
                ImGui::MenuItem("Asset Browser", nullptr, &m_ShowAssetBrowser);
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Entity")) {
                if (ImGui::MenuItem("Create Empty")) {
                    m_Scene->CreateEntity("Empty Entity");
                    Engine::ConsolePanel::AddLog(Engine::LogLevel::Info, "Created Empty Entity");
                }
                if (ImGui::MenuItem("Create Sprite")) {
                    auto entity = m_Scene->CreateEntity("Sprite");
                    entity.AddComponent<Engine::SpriteRendererComponent>();
                    Engine::ConsolePanel::AddLog(Engine::LogLevel::Info, "Created Sprite Entity");
                }
                if (ImGui::MenuItem("Create Camera")) {
                    auto entity = m_Scene->CreateEntity("Camera");
                    entity.AddComponent<Engine::CameraComponent>();
                    Engine::ConsolePanel::AddLog(Engine::LogLevel::Info, "Created Camera Entity");
                }
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("About")) {
                    m_ShowAbout = true;
                }
                ImGui::EndMenu();
            }
            
            ImGui::EndMenuBar();
        }
        
        ImGui::End(); // DockSpace
        
        // About window
        if (m_ShowAbout) {
            ImGui::Begin("About Game Engine", &m_ShowAbout);
            ImGui::Text("Game Engine Editor");
            ImGui::Separator();
            ImGui::Text("Version: 1.0.0");
            ImGui::Text("Built with:");
            ImGui::BulletText("OpenGL 4.1");
            ImGui::BulletText("Dear ImGui");
            ImGui::BulletText("EnTT (ECS)");
            ImGui::BulletText("Box2D (Physics)");
            ImGui::BulletText("Lua 5.4 (Scripting)");
            ImGui::BulletText("OpenAL (Audio)");
            ImGui::End();
        }
        
        // Viewport window
        if (m_ShowViewport) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        
        // Play/Pause/Step controls
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 4, 4 });
        ImGui::BeginChild("##toolbar", ImVec2(0, 40), true);
        {
            ImGui::Text("Scene Controls:");
            ImGui::SameLine();
            
            if (m_SceneState == SceneState::Edit) {
                if (ImGui::Button("▶ Play")) {
                    OnScenePlay();
                }
            } else if (m_SceneState == SceneState::Play) {
                if (ImGui::Button("⏸ Pause")) {
                    m_SceneState = SceneState::Pause;
                }
                ImGui::SameLine();
                if (ImGui::Button("■ Stop")) {
                    OnSceneStop();
                }
            } else if (m_SceneState == SceneState::Pause) {
                if (ImGui::Button("▶ Resume")) {
                    m_SceneState = SceneState::Play;
                }
                ImGui::SameLine();
                if (ImGui::Button("⏭ Step")) {
                    m_SceneState = SceneState::Step;
                }
                ImGui::SameLine();
                if (ImGui::Button("■ Stop")) {
                    OnSceneStop();
                }
            }
            
            ImGui::SameLine();
            const char* stateText = m_SceneState == SceneState::Edit ? "[EDIT MODE]" : 
                                    m_SceneState == SceneState::Play ? "[PLAYING]" : 
                                    m_SceneState == SceneState::Pause ? "[PAUSED]" : "[STEP]";
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", stateText);
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
        
        // Scene viewport
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        
        uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        
            ImGui::End();
            ImGui::PopStyleVar();
        }
        
        // Stats window
        if (m_ShowStats) {
            ImGui::Begin("Renderer Stats", &m_ShowStats);
        auto stats = Engine::Renderer2D::GetStats();
        ImGui::Text("Renderer2D Statistics");
        ImGui::Separator();
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        ImGui::Separator();
            ImGui::Text("Viewport Size: %.0fx%.0f", m_ViewportSize.x, m_ViewportSize.y);
            ImGui::End();
        }
        
        // Scene hierarchy panel
        if (m_ShowHierarchy || m_ShowProperties) {
            m_SceneHierarchyPanel.OnImGuiRender();
        }
        
        // Console panel
        if (m_ShowConsole) {
            m_ConsolePanel.OnImGuiRender();
        }
        
        // Profiler panel
        if (m_ShowProfiler) {
            m_ProfilerPanel.OnImGuiRender();
        }
        
        // Asset browser panel
        if (m_ShowAssetBrowser) {
            m_AssetBrowserPanel.OnImGuiRender();
        }
        
        // File dialogs (must be rendered every frame to work properly)
        RenderOpenDialog();
        RenderSaveAsDialog();
    }
    
    void NewScene() {
        m_Scene = Engine::CreateRef<Engine::Scene>("Untitled Scene");
        
        // IMPORTANT: Create a camera for the new scene!
        auto camera = m_Scene->CreateEntity("Camera");
        auto& cameraTransform = camera.GetComponent<Engine::TransformComponent>();
        cameraTransform.Position = { 0.0f, 0.0f, 0.0f };  // Centered at origin
        auto& cameraComp = camera.AddComponent<Engine::CameraComponent>();
        cameraComp.Camera.SetProjection(-16.0f, 16.0f, -9.0f, 9.0f);  // Same view as editor default
        cameraComp.Primary = true;
        
        // Create origin marker to show where (0,0,0) is
        auto origin = m_Scene->CreateEntity("Origin Marker");
        auto& originTransform = origin.GetComponent<Engine::TransformComponent>();
        originTransform.Position = { 0.0f, 0.0f, 0.0f };
        originTransform.Scale = { 0.3f, 0.3f, 1.0f };
        auto& originSprite = origin.AddComponent<Engine::SpriteRendererComponent>();
        originSprite.Color = { 1.0f, 1.0f, 0.0f, 0.5f };  // Semi-transparent yellow
        
        m_Scene->OnStart();
        m_SceneHierarchyPanel.SetContext(m_Scene);
        m_CurrentScenePath.clear();
        Engine::ConsolePanel::AddLog(Engine::LogLevel::Info, "Created new scene with camera");
    }
    
    void OpenScene() {
        m_ShowOpenDialog = true;
    }
    
    void SaveScene() {
        if (m_CurrentScenePath.empty()) {
            SaveSceneAs();
            return;
        }
        
        Engine::SceneSerializer serializer(m_Scene);
        if (serializer.Serialize(m_CurrentScenePath)) {
            Engine::ConsolePanel::AddLog(Engine::LogLevel::Info, "Saved scene: " + m_CurrentScenePath);
        } else {
            Engine::ConsolePanel::AddLog(Engine::LogLevel::Error, "Failed to save scene: " + m_CurrentScenePath);
        }
    }
    
    void SaveSceneAs() {
        m_ShowSaveAsDialog = true;
    }
    
    void RenderOpenDialog() {
        // Open popup only once when flag is set
        if (m_ShowOpenDialog) {
            ImGui::OpenPopup("Open Scene");
            m_ShowOpenDialog = false;
        }
        
        // Always render the popup (it stays open until closed)
        if (ImGui::BeginPopupModal("Open Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            static char pathBuffer[256] = "assets/scenes/scene.yaml";
            
            ImGui::Text("Enter scene file path:");
            ImGui::InputText("##path", pathBuffer, sizeof(pathBuffer));
            
            if (ImGui::Button("Open", ImVec2(120, 0))) {
                std::string path(pathBuffer);
                
                // Stop current scene
                m_Scene->OnStop();
                
                // Create new scene
                m_Scene = Engine::CreateRef<Engine::Scene>("Loaded Scene");
                
                // Deserialize into new scene
                Engine::SceneSerializer serializer(m_Scene);
                if (serializer.Deserialize(path)) {
                    m_CurrentScenePath = path;
                    m_Scene->OnStart();
                    m_SceneHierarchyPanel.SetContext(m_Scene);
                    Engine::ConsolePanel::AddLog(Engine::LogLevel::Info, "Opened scene: " + path);
                } else {
                    Engine::ConsolePanel::AddLog(Engine::LogLevel::Error, "Failed to open scene: " + path);
                    // Create default scene if load fails
                    NewScene();
                }
                
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    
    void RenderSaveAsDialog() {
        // Open popup only once when flag is set
        if (m_ShowSaveAsDialog) {
            ImGui::OpenPopup("Save Scene As");
            m_ShowSaveAsDialog = false;
        }
        
        // Always render the popup (it stays open until closed)
        if (ImGui::BeginPopupModal("Save Scene As", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            static char pathBuffer[256] = "assets/scenes/new_scene.yaml";
            
            ImGui::Text("Enter scene file path:");
            ImGui::InputText("##path", pathBuffer, sizeof(pathBuffer));
            
            if (ImGui::Button("Save", ImVec2(120, 0))) {
                std::string path(pathBuffer);
                
                Engine::SceneSerializer serializer(m_Scene);
                if (serializer.Serialize(path)) {
                    m_CurrentScenePath = path;
                    Engine::ConsolePanel::AddLog(Engine::LogLevel::Info, "Saved scene as: " + path);
                } else {
                    Engine::ConsolePanel::AddLog(Engine::LogLevel::Error, "Failed to save scene: " + path);
                }
                
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    void OnEvent(Engine::Event& event) override {
        // Handle events
    }

private:
    enum class SceneState {
        Edit = 0,
        Play = 1,
        Pause = 2,
        Step = 3
    };
    
    Engine::Ref<Engine::Scene> m_Scene;
    Engine::Ref<Engine::Framebuffer> m_Framebuffer;
    Engine::SceneHierarchyPanel m_SceneHierarchyPanel;
    Engine::ConsolePanel m_ConsolePanel;
    Engine::ProfilerPanel m_ProfilerPanel;
    Engine::AssetBrowserPanel m_AssetBrowserPanel;
    
    SceneState m_SceneState = SceneState::Edit;
    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
    
    // Scene management
    std::string m_CurrentScenePath;
    
    // Panel visibility
    bool m_ShowViewport = true;
    bool m_ShowHierarchy = true;
    bool m_ShowProperties = true;
    bool m_ShowConsole = true;
    bool m_ShowStats = true;
    bool m_ShowProfiler = false;
    bool m_ShowAssetBrowser = true;
    bool m_ShowAbout = false;
    
    // File dialogs
    bool m_ShowOpenDialog = false;
    bool m_ShowSaveAsDialog = false;
    
    bool m_Running = true;
};

class EditorApp : public Engine::Application {
public:
    EditorApp() : Application("Game Engine Editor") {
        PushLayer(new Engine::ImGuiLayer());
        PushLayer(new EditorLayer());
    }

    ~EditorApp() = default;
};

Engine::Application* Engine::CreateApplication() {
    return new EditorApp();
}

