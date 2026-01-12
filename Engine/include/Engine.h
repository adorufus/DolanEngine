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
#include "Engine/Renderer/OrthographicCamera.h"

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

// Entry point
#include "Engine/Core/EntryPoint.h"

