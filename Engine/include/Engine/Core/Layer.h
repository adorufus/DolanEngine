#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Core/TimeStep.h"
#include "Engine/Events/Event.h"

namespace Engine {

    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;
        
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(TimeStep ts) {}
        virtual void OnRender() {}
        virtual void OnEvent(Event& event) {}
        
        const std::string& GetName() const { return m_DebugName; }
        
    protected:
        std::string m_DebugName;
    };

}

