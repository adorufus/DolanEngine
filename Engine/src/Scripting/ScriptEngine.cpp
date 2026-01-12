#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Core/Logger.h"
#include "Engine/Core/Input.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Components.h"
#include <glm/glm.hpp>

namespace Engine {

    static sol::state* s_LuaState = nullptr;

    void ScriptEngine::Init() {
        s_LuaState = new sol::state();
        s_LuaState->open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
        
        RegisterAPIs();
        
        GE_CORE_INFO("Script Engine initialized (Lua {})", LUA_VERSION_MAJOR "." LUA_VERSION_MINOR);
    }

    void ScriptEngine::Shutdown() {
        if (s_LuaState) {
            delete s_LuaState;
            s_LuaState = nullptr;
        }
        GE_CORE_INFO("Script Engine shut down");
    }

    void ScriptEngine::ExecuteString(const std::string& code) {
        try {
            s_LuaState->script(code);
        } catch (const sol::error& e) {
            GE_CORE_ERROR("Lua error: {}", e.what());
        }
    }

    bool ScriptEngine::ExecuteFile(const std::string& filepath) {
        try {
            s_LuaState->script_file(filepath);
            return true;
        } catch (const sol::error& e) {
            GE_CORE_ERROR("Lua error in file '{}': {}", filepath, e.what());
            return false;
        }
    }

    sol::state& ScriptEngine::GetLuaState() {
        return *s_LuaState;
    }

    void ScriptEngine::RegisterAPIs() {
        RegisterCoreTypes();
        RegisterInput();
        RegisterMath();
        RegisterEntity();
        RegisterComponents();
    }

    void ScriptEngine::RegisterCoreTypes() {
        auto& lua = *s_LuaState;
        
        // KeyCode enum
        lua.new_enum("KeyCode",
            "Space", KeyCode::Space,
            "Apostrophe", KeyCode::Apostrophe,
            "Comma", KeyCode::Comma,
            "Minus", KeyCode::Minus,
            "Period", KeyCode::Period,
            "Slash", KeyCode::Slash,
            "D0", KeyCode::D0,
            "D1", KeyCode::D1,
            "D2", KeyCode::D2,
            "D3", KeyCode::D3,
            "D4", KeyCode::D4,
            "D5", KeyCode::D5,
            "D6", KeyCode::D6,
            "D7", KeyCode::D7,
            "D8", KeyCode::D8,
            "D9", KeyCode::D9,
            "A", KeyCode::A,
            "B", KeyCode::B,
            "C", KeyCode::C,
            "D", KeyCode::D,
            "E", KeyCode::E,
            "F", KeyCode::F,
            "G", KeyCode::G,
            "H", KeyCode::H,
            "I", KeyCode::I,
            "J", KeyCode::J,
            "K", KeyCode::K,
            "L", KeyCode::L,
            "M", KeyCode::M,
            "N", KeyCode::N,
            "O", KeyCode::O,
            "P", KeyCode::P,
            "Q", KeyCode::Q,
            "R", KeyCode::R,
            "S", KeyCode::S,
            "T", KeyCode::T,
            "U", KeyCode::U,
            "V", KeyCode::V,
            "W", KeyCode::W,
            "X", KeyCode::X,
            "Y", KeyCode::Y,
            "Z", KeyCode::Z,
            "Escape", KeyCode::Escape,
            "Enter", KeyCode::Enter,
            "Tab", KeyCode::Tab,
            "Backspace", KeyCode::Backspace,
            "Left", KeyCode::Left,
            "Right", KeyCode::Right,
            "Up", KeyCode::Up,
            "Down", KeyCode::Down
        );
    }

    void ScriptEngine::RegisterInput() {
        auto& lua = *s_LuaState;
        
        // Input class
        lua.new_usertype<Input>("Input",
            "IsKeyPressed", &Input::IsKeyPressed,
            "IsMouseButtonPressed", &Input::IsMouseButtonPressed,
            "GetMousePosition", &Input::GetMousePosition,
            "GetMouseX", &Input::GetMouseX,
            "GetMouseY", &Input::GetMouseY
        );
    }

    void ScriptEngine::RegisterMath() {
        auto& lua = *s_LuaState;
        
        // glm::vec2
        lua.new_usertype<glm::vec2>("Vec2",
            sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
            "x", &glm::vec2::x,
            "y", &glm::vec2::y
        );
        
        // glm::vec3
        lua.new_usertype<glm::vec3>("Vec3",
            sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z
        );
        
        // glm::vec4
        lua.new_usertype<glm::vec4>("Vec4",
            sol::constructors<glm::vec4(), glm::vec4(float), glm::vec4(float, float, float, float)>(),
            "x", &glm::vec4::x,
            "y", &glm::vec4::y,
            "z", &glm::vec4::z,
            "w", &glm::vec4::w
        );
    }

    void ScriptEngine::RegisterEntity() {
        auto& lua = *s_LuaState;
        
        // Entity
        lua.new_usertype<Entity>("Entity",
            "IsValid", &Entity::operator bool,
            "GetComponent_Transform", [](Entity& e) -> TransformComponent& {
                return e.GetComponent<TransformComponent>();
            },
            "GetComponent_SpriteRenderer", [](Entity& e) -> SpriteRendererComponent& {
                return e.GetComponent<SpriteRendererComponent>();
            },
            "GetComponent_Rigidbody2D", [](Entity& e) -> Rigidbody2DComponent& {
                return e.GetComponent<Rigidbody2DComponent>();
            },
            "HasComponent_Transform", [](Entity& e) -> bool {
                return e.HasComponent<TransformComponent>();
            },
            "HasComponent_SpriteRenderer", [](Entity& e) -> bool {
                return e.HasComponent<SpriteRendererComponent>();
            }
        );
    }

    void ScriptEngine::RegisterComponents() {
        auto& lua = *s_LuaState;
        
        // TransformComponent
        lua.new_usertype<TransformComponent>("TransformComponent",
            "Position", &TransformComponent::Position,
            "Rotation", &TransformComponent::Rotation,
            "Scale", &TransformComponent::Scale
        );
        
        // SpriteRendererComponent
        lua.new_usertype<SpriteRendererComponent>("SpriteRendererComponent",
            "Color", &SpriteRendererComponent::Color
        );
        
        // Rigidbody2DComponent
        lua.new_usertype<Rigidbody2DComponent>("Rigidbody2DComponent",
            "Type", &Rigidbody2DComponent::Type,
            "FixedRotation", &Rigidbody2DComponent::FixedRotation,
            "Velocity", &Rigidbody2DComponent::Velocity,
            "AngularVelocity", &Rigidbody2DComponent::AngularVelocity,
            "SetVelocity", &Rigidbody2DComponent::SetVelocity,
            "GetVelocity", &Rigidbody2DComponent::GetVelocity,
            "ApplyLinearImpulseToCenter", &Rigidbody2DComponent::ApplyLinearImpulseToCenter,
            "ApplyForceToCenter", &Rigidbody2DComponent::ApplyForceToCenter
        );
    }

} // namespace Engine

