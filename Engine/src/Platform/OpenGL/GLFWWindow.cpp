#include "GLFWWindow.h"
#include "Engine/Core/Logger.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {

    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description) {
        GE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Scope<Window> Window::Create(const WindowProps& props) {
        return CreateScope<GLFWWindow>(props);
    }

    GLFWWindow::GLFWWindow(const WindowProps& props) {
        Init(props);
    }

    GLFWWindow::~GLFWWindow() {
        Shutdown();
    }

    void GLFWWindow::Init(const WindowProps& props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_Data.VSync = props.VSync;
        
        GE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
        
        // Initialize GLFW
        if (!s_GLFWInitialized) {
            int success = glfwInit();
            GE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }
        
        // Set OpenGL version (3.3 Core)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        #ifdef GE_PLATFORM_MACOS
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
        
        // Create window
        m_Window = glfwCreateWindow(
            (int)props.Width,
            (int)props.Height,
            m_Data.Title.c_str(),
            nullptr,
            nullptr
        );
        
        GE_ASSERT(m_Window, "Failed to create GLFW window!");
        
        // Make OpenGL context current
        glfwMakeContextCurrent(m_Window);
        
        // Initialize GLAD
        int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        GE_ASSERT(gladStatus, "Failed to initialize GLAD!");
        
        GE_CORE_INFO("OpenGL Info:");
        GE_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
        GE_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        GE_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
        
        // Set user pointer for callbacks
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(props.VSync);
        
        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;
            
            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });
        
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });
        
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });
        
        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });
        
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });
        
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });
        
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });
    }

    void GLFWWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void GLFWWindow::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void GLFWWindow::SetVSync(bool enabled) {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        
        m_Data.VSync = enabled;
    }

}

