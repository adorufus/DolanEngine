#include "Engine/Core/Application.h"
#include "Engine/Core/Logger.h"
#include "Engine/Core/Input.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Audio/AudioEngine.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/ImGui/ImGuiLayer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {

    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name) {
        GE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        
        // Initialize logger
        Logger::Init();
        
        // Create window
        WindowProps props;
        props.Title = name;
        m_Window = Window::Create(props);
        m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });
        
        // Initialize renderer
        RenderCommand::Init();
        Renderer2D::Init();
        
        // Initialize audio
        AudioEngine::Init();
        
        // Initialize scripting
        ScriptEngine::Init();
        
        GE_CORE_INFO("Application initialized: {0}", name);
    }

    Application::~Application() {
        // Clean up layers
        for (Layer* layer : m_Layers) {
            layer->OnDetach();
            delete layer;
        }
        
        Renderer2D::Shutdown();
        AudioEngine::Shutdown();
        ScriptEngine::Shutdown();
        GE_CORE_INFO("Application shutting down");
    }

    void Application::PushLayer(Layer* layer) {
        m_Layers.push_back(layer);
        layer->OnAttach();
    }

    void Application::PopLayer() {
        if (!m_Layers.empty()) {
            Layer* layer = m_Layers.back();
            m_Layers.pop_back();
            layer->OnDetach();
            delete layer;
        }
    }
    
    void Application::RemoveLayer(Layer* layer) {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end()) {
            layer->OnDetach();
            m_Layers.erase(it);
            delete layer;
        }
    }

    void Application::Run() {
        GE_CORE_INFO("Starting main loop");
        
        while (m_Running) {
            // Calculate delta time
            float time = (float)glfwGetTime();
            TimeStep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;
            
            // Update
            if (!m_Minimized) {
                // Reset renderer stats for this frame
                Renderer2D::ResetStats();
                
                // Clear screen
                RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
                RenderCommand::Clear();
                
                // Update layers
                for (Layer* layer : m_Layers) {
                    layer->OnUpdate(timestep);
                    layer->OnRender();
                }
                
                // ImGui rendering - wrapped properly
                ImGuiLayer* imguiLayer = nullptr;
                for (Layer* layer : m_Layers) {
                    if (auto* imgui = dynamic_cast<ImGuiLayer*>(layer)) {
                        imguiLayer = imgui;
                        break;
                    }
                }
                
                if (imguiLayer) {
                    imguiLayer->Begin();
                    
                    // Render ImGui for all layers
                    for (Layer* layer : m_Layers) {
                        layer->OnImGuiRender();
                    }
                    
                    imguiLayer->End();
                }
            }
            
            // Update window
            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });
        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
        
        // Propagate events to layers (in reverse order)
        for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it) {
            (*it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e) {
        if (e.GetWidth() == 0 || e.GetHeight() == 0) {
            m_Minimized = true;
            return false;
        }
        
        m_Minimized = false;
        return false;
    }

}
