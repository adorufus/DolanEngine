#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Components.h"
#include "Engine/Core/Logger.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Engine {

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        : m_Scene(scene) {
    }

    void SceneSerializer::Serialize(const std::string& filepath) {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << m_Scene->GetName();
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        
        // Iterate through all entities
        auto view = m_Scene->m_Registry.view<Engine::TagComponent>();
        for (auto entityID : view) {
            Entity entity = { entityID, m_Scene.get() };
            if (!entity)
                continue;
            
            out << YAML::BeginMap;
            out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entityID;
            
            if (entity.HasComponent<TagComponent>()) {
                auto& tag = entity.GetComponent<TagComponent>();
                out << YAML::Key << "TagComponent";
                out << YAML::BeginMap;
                out << YAML::Key << "Tag" << YAML::Value << tag.Tag;
                out << YAML::EndMap;
            }
            
            if (entity.HasComponent<TransformComponent>()) {
                auto& tc = entity.GetComponent<TransformComponent>();
                out << YAML::Key << "TransformComponent";
                out << YAML::BeginMap;
                
                out << YAML::Key << "Position" << YAML::Value;
                out << YAML::Flow << YAML::BeginSeq << tc.Position.x << tc.Position.y << tc.Position.z << YAML::EndSeq;
                
                out << YAML::Key << "Rotation" << YAML::Value;
                out << YAML::Flow << YAML::BeginSeq << tc.Rotation.x << tc.Rotation.y << tc.Rotation.z << YAML::EndSeq;
                
                out << YAML::Key << "Scale" << YAML::Value;
                out << YAML::Flow << YAML::BeginSeq << tc.Scale.x << tc.Scale.y << tc.Scale.z << YAML::EndSeq;
                
                out << YAML::EndMap;
            }
            
            if (entity.HasComponent<SpriteRendererComponent>()) {
                auto& src = entity.GetComponent<SpriteRendererComponent>();
                out << YAML::Key << "SpriteRendererComponent";
                out << YAML::BeginMap;
                
                out << YAML::Key << "Color" << YAML::Value;
                out << YAML::Flow << YAML::BeginSeq << src.Color.r << src.Color.g << src.Color.b << src.Color.a << YAML::EndSeq;
                
                out << YAML::Key << "TilingFactor" << YAML::Value << src.TilingFactor;
                
                out << YAML::EndMap;
            }
            
            out << YAML::EndMap;
        }
        
        out << YAML::EndSeq;
        out << YAML::EndMap;
        
        std::ofstream fout(filepath);
        fout << out.c_str();
        fout.close();
        
        GE_CORE_INFO("Scene saved to '{0}'", filepath);
    }

    bool SceneSerializer::Deserialize(const std::string& filepath) {
        std::ifstream stream(filepath);
        if (!stream.is_open()) {
            GE_CORE_ERROR("Failed to open scene file: {0}", filepath);
            return false;
        }
        
        std::stringstream strStream;
        strStream << stream.rdbuf();
        
        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Scene"]) {
            GE_CORE_ERROR("Invalid scene file: {0}", filepath);
            return false;
        }
        
        std::string sceneName = data["Scene"].as<std::string>();
        GE_CORE_INFO("Deserializing scene '{0}'", sceneName);
        
        auto entities = data["Entities"];
        if (entities) {
            for (auto entity : entities) {
                uint32_t uuid = entity["Entity"].as<uint32_t>();
                
                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent)
                    name = tagComponent["Tag"].as<std::string>();
                
                Entity deserializedEntity = m_Scene->CreateEntity(name);
                
                auto transformComponent = entity["TransformComponent"];
                if (transformComponent) {
                    auto& tc = deserializedEntity.GetComponent<TransformComponent>();
                    auto pos = transformComponent["Position"];
                    tc.Position = { pos[0].as<float>(), pos[1].as<float>(), pos[2].as<float>() };
                    
                    auto rot = transformComponent["Rotation"];
                    tc.Rotation = { rot[0].as<float>(), rot[1].as<float>(), rot[2].as<float>() };
                    
                    auto scale = transformComponent["Scale"];
                    tc.Scale = { scale[0].as<float>(), scale[1].as<float>(), scale[2].as<float>() };
                }
                
                auto spriteRendererComponent = entity["SpriteRendererComponent"];
                if (spriteRendererComponent) {
                    auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
                    auto color = spriteRendererComponent["Color"];
                    src.Color = { color[0].as<float>(), color[1].as<float>(), color[2].as<float>(), color[3].as<float>() };
                    src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
                }
            }
        }
        
        GE_CORE_INFO("Scene loaded from '{0}'", filepath);
        return true;
    }

}

