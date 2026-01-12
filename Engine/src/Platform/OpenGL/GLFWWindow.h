#pragma once

#include "Engine/Core/Window.h"

struct GLFWwindow;

namespace Engine {

    class GLFWWindow : public Window {
    public:
        GLFWWindow(const WindowProps& props);
        virtual ~GLFWWindow();
        
        void OnUpdate() override;
        
        uint32_t GetWidth() const override { return m_Data.Width; }
        uint32_t GetHeight() const override { return m_Data.Height; }
        
        void SetEventCallback(const EventCallbackFn& callback) override {
            m_Data.EventCallback = callback;
        }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override { return m_Data.VSync; }
        
        void* GetNativeWindow() const override { return m_Window; }
        
    private:
        void Init(const WindowProps& props);
        void Shutdown();
        
    private:
        GLFWwindow* m_Window;
        
        struct WindowData {
            std::string Title;
            uint32_t Width, Height;
            bool VSync;
            EventCallbackFn EventCallback;
        };
        
        WindowData m_Data;
    };

}

