#include "Engine/Editor/SceneHierarchyPanel.h"
#include "Engine/Scene/Components.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene) {
        SetContext(scene);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene) {
        m_Context = scene;
        m_SelectionContext = {};
    }

    void SceneHierarchyPanel::SetSelectedEntity(Entity entity) {
        m_SelectionContext = entity;
    }

    void SceneHierarchyPanel::OnImGuiRender() {
        ImGui::Begin("Scene Hierarchy");

        if (m_Context) {
            try {
                auto view = m_Context->m_Registry.view<TagComponent>();
                for (auto entityID : view) {
                    Entity entity{ entityID, m_Context.get() };
                    DrawEntityNode(entity);
                }
            } catch (...) {
                ImGui::Text("Error rendering entities");
            }

            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
                m_SelectionContext = {};

            // Right-click on blank space
            if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
                if (ImGui::MenuItem("Create Empty Entity"))
                    m_Context->CreateEntity("Empty Entity");

                ImGui::EndPopup();
            }
        } else {
            ImGui::Text("No scene loaded");
        }

        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectionContext && m_SelectionContext.HasComponent<TagComponent>()) {
            try {
                DrawComponents(m_SelectionContext);
            } catch (...) {
                ImGui::Text("Error rendering components");
            }
        } else {
            ImGui::Text("No entity selected");
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());
        
        if (ImGui::IsItemClicked()) {
            m_SelectionContext = entity;
        }

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Entity"))
                entityDeleted = true;

            ImGui::EndPopup();
        }

        if (opened) {
            ImGui::TreePop();
        }

        if (entityDeleted) {
            m_Context->DestroyEntity(entity);
            if (m_SelectionContext == entity)
                m_SelectionContext = {};
        }
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        float widthEach = (ImGui::CalcItemWidth() - buttonSize.x * 3) / 3.0f;

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(widthEach);
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(widthEach);
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(widthEach);
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction) {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed 
            | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_FramePadding;
        
        if (entity.HasComponent<T>()) {
            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
            ImGui::PopStyleVar();
            
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })) {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Remove component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if (open) {
                uiFunction(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                entity.RemoveComponent<T>();
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity) {
        if (entity.HasComponent<TagComponent>()) {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
                tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent")) {
            if (ImGui::MenuItem("Camera")) {
                if (!m_SelectionContext.HasComponent<CameraComponent>())
                    m_SelectionContext.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Sprite Renderer")) {
                if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
                    m_SelectionContext.AddComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Rigidbody 2D")) {
                if (!m_SelectionContext.HasComponent<Rigidbody2DComponent>())
                    m_SelectionContext.AddComponent<Rigidbody2DComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Box Collider 2D")) {
                if (!m_SelectionContext.HasComponent<BoxCollider2DComponent>())
                    m_SelectionContext.AddComponent<BoxCollider2DComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Circle Collider 2D")) {
                if (!m_SelectionContext.HasComponent<CircleCollider2DComponent>())
                    m_SelectionContext.AddComponent<CircleCollider2DComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Script")) {
                if (!m_SelectionContext.HasComponent<ScriptComponent>())
                    m_SelectionContext.AddComponent<ScriptComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Particle Emitter")) {
                if (!m_SelectionContext.HasComponent<ParticleEmitterComponent>())
                    m_SelectionContext.AddComponent<ParticleEmitterComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Audio Source")) {
                if (!m_SelectionContext.HasComponent<AudioSourceComponent>())
                    m_SelectionContext.AddComponent<AudioSourceComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Audio Listener")) {
                if (!m_SelectionContext.HasComponent<AudioListenerComponent>())
                    m_SelectionContext.AddComponent<AudioListenerComponent>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
            DrawVec3Control("Position", component.Position);
            glm::vec3 rotation = glm::degrees(component.Rotation);
            DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale", component.Scale, 1.0f);
        });

        DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
            ImGui::Checkbox("Primary", &component.Primary);
            
            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
            const char* currentProjectionTypeString = projectionTypeStrings[0]; // Orthographic for 2D
            if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
                for (int i = 0; i < 2; i++) {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
                        currentProjectionTypeString = projectionTypeStrings[i];
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }
        });

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component) {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
        });

        DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component) {
            const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
            const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
            if (ImGui::BeginCombo("Body Type", currentBodyTypeString)) {
                for (int i = 0; i < 3; i++) {
                    bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
                    if (ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
                        currentBodyTypeString = bodyTypeStrings[i];
                        component.Type = (Rigidbody2DComponent::BodyType)i;
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
            ImGui::DragFloat("Gravity Scale", &component.GravityScale, 0.1f, -10.0f, 10.0f);
        });

        DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component) {
            ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.01f);
            ImGui::DragFloat2("Size", glm::value_ptr(component.Size), 0.01f);
            ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
        });

        DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component) {
            ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.01f);
            ImGui::DragFloat("Radius", &component.Radius, 0.01f, 0.1f, 10.0f);
            ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
        });

        DrawComponent<ScriptComponent>("Script", entity, [](auto& component) {
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, component.ScriptPath.c_str());
            if (ImGui::InputText("Script Path", buffer, sizeof(buffer))) {
                component.ScriptPath = std::string(buffer);
            }
        });

        DrawComponent<ParticleEmitterComponent>("Particle Emitter", entity, [](auto& component) {
            ImGui::Text("Emission Settings:");
            ImGui::DragFloat("Emission Rate", &component.EmissionRate, 1.0f, 0.0f, 1000.0f);
            ImGui::Checkbox("Emit", &component.Emit);
            ImGui::Checkbox("Burst Mode", &component.BurstMode);
            if (component.BurstMode) {
                ImGui::DragInt("Burst Count", (int*)&component.BurstCount, 1.0f, 1, 1000);
            }
            
            ImGui::Separator();
            ImGui::Text("Particle Properties:");
            
            // Color properties
            ImGui::ColorEdit4("Start Color", glm::value_ptr(component.Properties.ColorBegin));
            ImGui::ColorEdit4("End Color", glm::value_ptr(component.Properties.ColorEnd));
            
            // Size properties
            ImGui::DragFloat("Start Size", &component.Properties.SizeBegin, 0.01f, 0.01f, 10.0f);
            ImGui::DragFloat("End Size", &component.Properties.SizeEnd, 0.01f, 0.0f, 10.0f);
            
            // Lifetime
            ImGui::DragFloat("Lifetime", &component.Properties.LifeTime, 0.1f, 0.1f, 10.0f);
            
            // Velocity
            ImGui::DragFloat2("Velocity", glm::value_ptr(component.Properties.Velocity), 0.1f);
            ImGui::DragFloat2("Velocity Variation", glm::value_ptr(component.Properties.VelocityVariation), 0.1f);
            
            // Rotation
            ImGui::DragFloat("Rotation", &component.Properties.Rotation, 0.1f);
            ImGui::DragFloat("Angular Velocity", &component.Properties.AngularVelocity, 0.1f);
            
            // Size variation
            ImGui::DragFloat("Size Variation", &component.Properties.SizeVariation, 0.01f, 0.0f, 1.0f);
        });

        DrawComponent<AudioSourceComponent>("Audio Source", entity, [](auto& component) {
            ImGui::Checkbox("Play On Start", &component.PlayOnStart);
            ImGui::Checkbox("Is 3D", &component.Is3D);
            
            // Audio file path
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            if (!component.FilePath.empty()) {
                strcpy(buffer, component.FilePath.c_str());
            }
            if (ImGui::InputText("Audio File", buffer, sizeof(buffer))) {
                component.FilePath = std::string(buffer);
                // Optionally reload the audio buffer here
            }
            
            // Volume and pitch controls
            ImGui::SliderFloat("Volume", &component.Volume, 0.0f, 1.0f);
            ImGui::SliderFloat("Pitch", &component.Pitch, 0.5f, 2.0f);
            ImGui::Checkbox("Loop", &component.Loop);
            
            // 3D audio settings
            if (component.Is3D) {
                ImGui::Separator();
                ImGui::Text("3D Audio Settings:");
                ImGui::SliderFloat("Min Distance", &component.MinDistance, 0.1f, 100.0f);
                ImGui::SliderFloat("Max Distance", &component.MaxDistance, 1.0f, 1000.0f);
                ImGui::SliderFloat("Rolloff Factor", &component.RolloffFactor, 0.0f, 10.0f);
            }
        });

        DrawComponent<AudioListenerComponent>("Audio Listener", entity, [](auto& component) {
            ImGui::Text("Audio Listener (No properties)");
        });
    }

} // namespace Engine

