#pragma once

// Main engine header - include this in client applications

// Core
#include "Engine/Core/Application.h"
#include "Engine/Core/Layer.h"
#include "Engine/Core/Logger.h"
#include "Engine/Core/TimeStep.h"
#include "Engine/Core/Window.h"
#include "Engine/Core/Input.h"

// Renderer
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/RenderCommand.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/SubTexture2D.h"
#include "Engine/Renderer/Framebuffer.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/CameraController.h"
#include "Engine/Renderer/ParticleSystem.h"

// Scene
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Components.h"
#include "Engine/Scene/SceneSerializer.h"

// Assets
#include "Engine/Assets/AssetManager.h"

// Physics
#include "Engine/Physics/Physics2D.h"
#include "Engine/Physics/PhysicsComponents.h"

// Animation
#include "Engine/Animation/AnimationClip.h"
#include "Engine/Animation/Animator.h"

// Audio
#include "Engine/Audio/AudioEngine.h"
#include "Engine/Audio/AudioBuffer.h"
#include "Engine/Audio/AudioSource.h"

// Scripting
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Scripting/ScriptReloader.h"

// ImGui
#include "Engine/ImGui/ImGuiLayer.h"
#include <imgui.h>

// Editor
#include "Engine/Editor/SceneHierarchyPanel.h"
#include "Engine/Editor/ConsolePanel.h"
#include "Engine/Editor/ProfilerPanel.h"
#include "Engine/Editor/AssetBrowserPanel.h"

// Debug
#include "Engine/Debug/Profiler.h"

// Entry point
#include "Engine/Core/EntryPoint.h"

