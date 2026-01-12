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

    bool SceneSerializer::Serialize(const std::string& filepath) {
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
            
            if (entity.HasComponent<CameraComponent>()) {
                auto& camera = entity.GetComponent<CameraComponent>();
                out << YAML::Key << "CameraComponent";
                out << YAML::BeginMap;
                out << YAML::Key << "Primary" << YAML::Value << camera.Primary;
                out << YAML::Key << "FixedAspectRatio" << YAML::Value << camera.FixedAspectRatio;
                out << YAML::EndMap;
            }
            
            if (entity.HasComponent<ScriptComponent>()) {
                auto& script = entity.GetComponent<ScriptComponent>();
                out << YAML::Key << "ScriptComponent";
                out << YAML::BeginMap;
                out << YAML::Key << "ScriptPath" << YAML::Value << script.ScriptPath;
                out << YAML::EndMap;
            }
            
            if (entity.HasComponent<Rigidbody2DComponent>()) {
                auto& rb = entity.GetComponent<Rigidbody2DComponent>();
                out << YAML::Key << "Rigidbody2DComponent";
                out << YAML::BeginMap;
                out << YAML::Key << "BodyType" << YAML::Value << (int)rb.Type;
                out << YAML::Key << "FixedRotation" << YAML::Value << rb.FixedRotation;
                out << YAML::Key << "GravityScale" << YAML::Value << rb.GravityScale;
                out << YAML::EndMap;
            }
            
            if (entity.HasComponent<BoxCollider2DComponent>()) {
                auto& bc = entity.GetComponent<BoxCollider2DComponent>();
                out << YAML::Key << "BoxCollider2DComponent";
                out << YAML::BeginMap;
                out << YAML::Key << "Offset" << YAML::Value;
                out << YAML::Flow << YAML::BeginSeq << bc.Offset.x << bc.Offset.y << YAML::EndSeq;
                out << YAML::Key << "Size" << YAML::Value;
                out << YAML::Flow << YAML::BeginSeq << bc.Size.x << bc.Size.y << YAML::EndSeq;
                out << YAML::Key << "Density" << YAML::Value << bc.Density;
                out << YAML::Key << "Friction" << YAML::Value << bc.Friction;
                out << YAML::Key << "Restitution" << YAML::Value << bc.Restitution;
                out << YAML::EndMap;
            }
            
            if (entity.HasComponent<CircleCollider2DComponent>()) {
                auto& cc = entity.GetComponent<CircleCollider2DComponent>();
                out << YAML::Key << "CircleCollider2DComponent";
                out << YAML::BeginMap;
                out << YAML::Key << "Offset" << YAML::Value;
                out << YAML::Flow << YAML::BeginSeq << cc.Offset.x << cc.Offset.y << YAML::EndSeq;
                out << YAML::Key << "Radius" << YAML::Value << cc.Radius;
                out << YAML::Key << "Density" << YAML::Value << cc.Density;
                out << YAML::Key << "Friction" << YAML::Value << cc.Friction;
                out << YAML::Key << "Restitution" << YAML::Value << cc.Restitution;
                out << YAML::EndMap;
            }
            
            if (entity.HasComponent<ParticleEmitterComponent>()) {
                auto& pe = entity.GetComponent<ParticleEmitterComponent>();
                out << YAML::Key << "ParticleEmitterComponent";
                out << YAML::BeginMap;
                out << YAML::Key << "EmissionRate" << YAML::Value << pe.EmissionRate;
                out << YAML::Key << "Emit" << YAML::Value << pe.Emit;
                out << YAML::Key << "BurstMode" << YAML::Value << pe.BurstMode;
                out << YAML::Key << "BurstCount" << YAML::Value << pe.BurstCount;
                // Particle properties
                out << YAML::Key << "ColorBegin" << YAML::Value;
                out << YAML::Flow << YAML::BeginSeq << pe.Properties.ColorBegin.r << pe.Properties.ColorBegin.g << pe.Properties.ColorBegin.b << pe.Properties.ColorBegin.a << YAML::EndSeq;
                out << YAML::Key << "ColorEnd" << YAML::Value;
                out << YAML::Flow << YAML::BeginSeq << pe.Properties.ColorEnd.r << pe.Properties.ColorEnd.g << pe.Properties.ColorEnd.b << pe.Properties.ColorEnd.a << YAML::EndSeq;
                out << YAML::Key << "SizeBegin" << YAML::Value << pe.Properties.SizeBegin;
                out << YAML::Key << "SizeEnd" << YAML::Value << pe.Properties.SizeEnd;
                out << YAML::Key << "SizeVariation" << YAML::Value << pe.Properties.SizeVariation;
                out << YAML::Key << "LifeTime" << YAML::Value << pe.Properties.LifeTime;
                out << YAML::Key << "Velocity" << YAML::Value;
                out << YAML::Flow << YAML::BeginSeq << pe.Properties.Velocity.x << pe.Properties.Velocity.y << YAML::EndSeq;
                out << YAML::Key << "VelocityVariation" << YAML::Value;
                out << YAML::Flow << YAML::BeginSeq << pe.Properties.VelocityVariation.x << pe.Properties.VelocityVariation.y << YAML::EndSeq;
                out << YAML::Key << "Rotation" << YAML::Value << pe.Properties.Rotation;
                out << YAML::Key << "AngularVelocity" << YAML::Value << pe.Properties.AngularVelocity;
                out << YAML::EndMap;
            }
            
            if (entity.HasComponent<AudioSourceComponent>()) {
                auto& audio = entity.GetComponent<AudioSourceComponent>();
                out << YAML::Key << "AudioSourceComponent";
                out << YAML::BeginMap;
                out << YAML::Key << "PlayOnStart" << YAML::Value << audio.PlayOnStart;
                out << YAML::Key << "Is3D" << YAML::Value << audio.Is3D;
                out << YAML::EndMap;
            }
            
            if (entity.HasComponent<AudioListenerComponent>()) {
                out << YAML::Key << "AudioListenerComponent";
                out << YAML::BeginMap;
                out << YAML::EndMap;
            }
            
            out << YAML::EndMap;
        }
        
        out << YAML::EndSeq;
        out << YAML::EndMap;
        
        std::ofstream fout(filepath);
        if (!fout.is_open()) {
            GE_CORE_ERROR("Failed to open file for writing: '{0}'", filepath);
            return false;
        }
        
        fout << out.c_str();
        fout.close();
        
        GE_CORE_INFO("Scene saved to '{0}'", filepath);
        return true;
    }

    bool SceneSerializer::Deserialize(const std::string& filepath) {
        std::ifstream stream(filepath);
        if (!stream.is_open()) {
            GE_CORE_ERROR("Failed to open scene file: {0}", filepath);
            return false;
        }
        
        std::stringstream strStream;
        strStream << stream.rdbuf();
        
        // Check if file is empty
        std::string fileContent = strStream.str();
        if (fileContent.empty()) {
            GE_CORE_ERROR("Scene file is empty: {0}", filepath);
            return false;
        }
        
        YAML::Node data;
        try {
            data = YAML::Load(fileContent);
        } catch (const YAML::Exception& e) {
            GE_CORE_ERROR("Failed to parse YAML file '{0}': {1}", filepath, e.what());
            return false;
        }
        
        if (!data || !data["Scene"]) {
            GE_CORE_ERROR("Invalid scene file format: {0}", filepath);
            return false;
        }
        
        std::string sceneName = data["Scene"].as<std::string>();
        GE_CORE_INFO("Deserializing scene '{0}'", sceneName);
        
        auto entities = data["Entities"];
        if (!entities || !entities.IsSequence()) {
            GE_CORE_WARN("No entities found in scene file: {0}", filepath);
            return true; // Still valid scene, just empty
        }
        
        int entityCount = 0;
        for (auto entity : entities) {
            try {
                if (!entity["Entity"]) {
                    GE_CORE_WARN("Skipping malformed entity (missing ID)");
                    continue;
                }
                
                // uint32_t uuid = entity["Entity"].as<uint32_t>(); // Reserved for future use
                
                std::string name = "Entity";
                auto tagComponent = entity["TagComponent"];
                if (tagComponent && tagComponent["Tag"])
                    name = tagComponent["Tag"].as<std::string>();
                
                Entity deserializedEntity = m_Scene->CreateEntity(name);
                entityCount++;
                
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
                
                auto cameraComponent = entity["CameraComponent"];
                if (cameraComponent) {
                    auto& camera = deserializedEntity.AddComponent<CameraComponent>();
                    camera.Primary = cameraComponent["Primary"].as<bool>();
                    camera.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
                }
                
                auto scriptComponent = entity["ScriptComponent"];
                if (scriptComponent) {
                    auto& script = deserializedEntity.AddComponent<ScriptComponent>();
                    script.ScriptPath = scriptComponent["ScriptPath"].as<std::string>();
                }
                
                auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
                if (rigidbody2DComponent) {
                    auto& rb = deserializedEntity.AddComponent<Rigidbody2DComponent>();
                    rb.Type = (Rigidbody2DComponent::BodyType)rigidbody2DComponent["BodyType"].as<int>();
                    rb.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
                    if (rigidbody2DComponent["GravityScale"])
                        rb.GravityScale = rigidbody2DComponent["GravityScale"].as<float>();
                }
                
                auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
                if (boxCollider2DComponent) {
                    auto& bc = deserializedEntity.AddComponent<BoxCollider2DComponent>();
                    auto offset = boxCollider2DComponent["Offset"];
                    bc.Offset = { offset[0].as<float>(), offset[1].as<float>() };
                    auto size = boxCollider2DComponent["Size"];
                    bc.Size = { size[0].as<float>(), size[1].as<float>() };
                    bc.Density = boxCollider2DComponent["Density"].as<float>();
                    bc.Friction = boxCollider2DComponent["Friction"].as<float>();
                    bc.Restitution = boxCollider2DComponent["Restitution"].as<float>();
                }
                
                auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
                if (circleCollider2DComponent) {
                    auto& cc = deserializedEntity.AddComponent<CircleCollider2DComponent>();
                    auto offset = circleCollider2DComponent["Offset"];
                    cc.Offset = { offset[0].as<float>(), offset[1].as<float>() };
                    cc.Radius = circleCollider2DComponent["Radius"].as<float>();
                    cc.Density = circleCollider2DComponent["Density"].as<float>();
                    cc.Friction = circleCollider2DComponent["Friction"].as<float>();
                    cc.Restitution = circleCollider2DComponent["Restitution"].as<float>();
                }
                
                auto particleEmitterComponent = entity["ParticleEmitterComponent"];
                if (particleEmitterComponent) {
                    auto& pe = deserializedEntity.AddComponent<ParticleEmitterComponent>();
                    pe.EmissionRate = particleEmitterComponent["EmissionRate"].as<float>();
                    pe.Emit = particleEmitterComponent["Emit"].as<bool>();
                    pe.BurstMode = particleEmitterComponent["BurstMode"].as<bool>();
                    pe.BurstCount = particleEmitterComponent["BurstCount"].as<uint32_t>();
                    // Particle properties
                    auto colorBegin = particleEmitterComponent["ColorBegin"];
                    pe.Properties.ColorBegin = { colorBegin[0].as<float>(), colorBegin[1].as<float>(), colorBegin[2].as<float>(), colorBegin[3].as<float>() };
                    auto colorEnd = particleEmitterComponent["ColorEnd"];
                    pe.Properties.ColorEnd = { colorEnd[0].as<float>(), colorEnd[1].as<float>(), colorEnd[2].as<float>(), colorEnd[3].as<float>() };
                    pe.Properties.SizeBegin = particleEmitterComponent["SizeBegin"].as<float>();
                    pe.Properties.SizeEnd = particleEmitterComponent["SizeEnd"].as<float>();
                    pe.Properties.SizeVariation = particleEmitterComponent["SizeVariation"].as<float>();
                    pe.Properties.LifeTime = particleEmitterComponent["LifeTime"].as<float>();
                    auto velocity = particleEmitterComponent["Velocity"];
                    pe.Properties.Velocity = { velocity[0].as<float>(), velocity[1].as<float>() };
                    auto velocityVariation = particleEmitterComponent["VelocityVariation"];
                    pe.Properties.VelocityVariation = { velocityVariation[0].as<float>(), velocityVariation[1].as<float>() };
                    pe.Properties.Rotation = particleEmitterComponent["Rotation"].as<float>();
                    pe.Properties.AngularVelocity = particleEmitterComponent["AngularVelocity"].as<float>();
                }
                
                auto audioSourceComponent = entity["AudioSourceComponent"];
                if (audioSourceComponent) {
                    auto& audio = deserializedEntity.AddComponent<AudioSourceComponent>();
                    audio.PlayOnStart = audioSourceComponent["PlayOnStart"].as<bool>();
                    audio.Is3D = audioSourceComponent["Is3D"].as<bool>();
                }
                
                auto audioListenerComponent = entity["AudioListenerComponent"];
                if (audioListenerComponent) {
                    deserializedEntity.AddComponent<AudioListenerComponent>();
                }
            } catch (const std::exception& e) {
                GE_CORE_ERROR("Error deserializing entity: {0}", e.what());
                continue; // Skip this entity but continue with others
            }
        }
        
        GE_CORE_INFO("Scene deserialization complete: {0} entities loaded", entityCount);
        
        // Validate scene has at least one camera
        bool hasCamera = false;
        auto cameraView = m_Scene->m_Registry.view<CameraComponent>();
        for (auto e : cameraView) {
            (void)e; // Mark as intentionally unused
            hasCamera = true;
            break;
        }
        
        if (!hasCamera) {
            GE_CORE_WARN("Loaded scene has no camera! Creating default camera.");
            auto camera = m_Scene->CreateEntity("Default Camera");
            auto& cameraComp = camera.AddComponent<CameraComponent>();
            cameraComp.Camera.SetProjection(-16.0f, 16.0f, -9.0f, 9.0f);
            cameraComp.Primary = true;
        }
        
        return true;
    }

}

