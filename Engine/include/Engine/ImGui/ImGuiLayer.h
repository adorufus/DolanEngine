#pragma once

#include "Engine/Core/Layer.h"

namespace Engine {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        virtual ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(TimeStep ts) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& e) override;
        
        void Begin();
        void End();
        
        void BlockEvents(bool block) { m_BlockEvents = block; }
        
    private:
        bool m_BlockEvents = true;
        float m_Time = 0.0f;
    };

} // namespace Engine

