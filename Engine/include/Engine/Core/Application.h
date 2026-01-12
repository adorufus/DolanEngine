#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Core/Window.h"
#include "Engine/Core/TimeStep.h"
#include "Engine/Core/Layer.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include <vector>

namespace Engine {

    class Application {
    public:
        Application(const std::string& name = "Game Engine");
        virtual ~Application();
        
        void Run();
        
        void OnEvent(Event& e);
        
        void PushLayer(Layer* layer);
        void PopLayer();
        
        Window& GetWindow() { return *m_Window; }
        
        static Application& Get() { return *s_Instance; }
        
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        
    protected:
        Scope<Window> m_Window;
        bool m_Running = true;
        bool m_Minimized = false;
        float m_LastFrameTime = 0.0f;
        std::vector<Layer*> m_Layers;
        
        static Application* s_Instance;
    };

    // To be defined in client application
    Application* CreateApplication();

}
