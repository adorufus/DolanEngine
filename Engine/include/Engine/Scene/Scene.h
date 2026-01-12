#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Core/TimeStep.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include <entt/entt.hpp>

namespace Engine {

    class Entity;
    class ContactListener;

    class Scene {
    public:
        Scene(const std::string& name = "Untitled Scene");
        ~Scene();
        
        Entity CreateEntity(const std::string& name = "Entity");
        void DestroyEntity(Entity entity);
        
        void OnStart();
        void OnUpdate(TimeStep ts);
        void OnRender();
        void OnStop();
        
        void OnViewportResize(uint32_t width, uint32_t height);
        
        const std::string& GetName() const { return m_Name; }
        
        // Physics
        void SetGravity(const glm::vec2& gravity);
        glm::vec2 GetGravity() const;
        
    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
        
        void OnPhysics2DStart();
        void OnPhysics2DStop();
        void OnPhysics2DUpdate(TimeStep ts);
        
    private:
        entt::registry m_Registry;
        std::string m_Name;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        
        // Physics
        void* m_PhysicsWorld = nullptr;
        ContactListener* m_ContactListener = nullptr;
        
        friend class Entity;
        friend class SceneSerializer;
    };

}

