/////////////////////////////////////////////////////////////////
//  Author: Devon Adams (https://github.com/devonadams)
//  License : GPLv3
//  Language: C++
//  This file serves as the base for DAGGer (crazy right?)
//    Also includes CoreConfig.h for DAGGer defines
///////////////////////
/*
    DAGGer Includes: (Thank you!)
      -  spdlog..........(logging)
      -  GLFW............(cross-platform windowing system)
      -  glad............(modern loader for OpenGL / Vulkan)
      -  glm.............(complex mathematics)
      -  ImGui...........(debug GUI)
      -  ImGuizmo........(3D gizmos)
      -  stb_image.......(load images)
      -  EnTT............(Entity Component System)
      -  yaml-cpp........(Saving/loading scenes for FORGE)

----------------------    Structure of DAGGer    -------------------------
Note: This isn't important information unless you're trying to understand the engine, or make changes to DAGGer itself.

Let's start off in chronological order:
    Entry.h (contains the int main() function)
      - Entry Initalizes logs then controls 3 main tasks
      - Startup, Runtime, Shutdown

  ------  Startup  --------------------------------------------------------------------
       Application.h CreateApplication (implemented in SandboxApp.cpp) 
         - This returns a new Sandbox and preforms a bunch of functions
           Application::Application (The application constructor)
             - Only 1 application may be created, then Window.h Create() is called
                WindowsWindow::Init() 
                  - initalizes glfw, and creates a glfwWindow then calls GraphicsContext::Create()
                    GraphicsContext::Create() 
                      - Uses the defined GraphicsAPI and calls API::Init
                        OpenGL - OpenGLContext::Init(GLFWwindow* windowHandle)
                          - Makes window context current then loads Glad
           Application::Application then calls Renderer::Init()
               Renderer::Init - Calls RenderCommand::Init() then Renderer2D::Init()
                 RenderCommand::Init() calls API::RendererAPI::Init()
                     OpenGL - OpenGLRendererAPI::Init() Initalizes OpenGL with default values
                 Renderer2D::Init() Creates API specific VertexArrays, and VertexBuffers
                     OpenGL - OpenGLVertexArray::AddVertexBuffer() adds a default vertex buffer to OpenGL
                     OpenGL - OpenGLIndexBuffer::AddIndexBuffer() adds a default index buffer to OpenGL (or an empty one)
                     OpenGL - OpenGLTexture2D is constructed and has SetData is called
                     OpenGL - OpenGLShader is constructed
                            The shader file is read, processes, and compiled
                     OpenGL - OpenGLShader::SetIntArray is called to conclude Renderer2D::Init and Renderer::Init
             Application::PushOverlay is called, and the ImGui layer is attached
       The Orthographic Camera is the created

  ------  Runtime  ------------------------------------
      Application::Run is called from entry.h
        LayerStack::OnUpdate is called
          Sandbox::OnUpdate is called
            OrthographicCameraController::OnUpdate is called
              OrthographicCamera::RecalculateViewMatrix is called
            Then Sandbox::OnUpdate is called

  ------  Shutdown  -----------------------------------
      OpenGLTexture2D is deconstructed
      WindowsWindow::Shutdown is called
      WindowsWindow is deconstructed
*/
#pragma once

#include <memory>
#include "CoreConfig.h" //  includes DAGGer defines
#include "Ref.h"

namespace DAGGer
{
	void InitalizeCore();
	void ShutdownCore();
}	//	END namespace DAGGer

#define Dr_RENDERER_OPENGL

// Platform detection
#ifdef _WIN32
    //  Windows x64/x86 //
    #ifdef _WIN64
        // Windows x64  //
        #define Dr_PLATFORM_WINDOWS
    #else
		#define Dr_PLATFORM_WINDOWS
        //#error "x86 Builds are not supported!"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)   //  Apple Platforms
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "iOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define Dr_PLATFORM_IOS     //  iOS
        #error "iOS is not supported!"
    #elif TARGET_OS_MAC == 1        MacOS
        #define Dr_PLATFORM_MACOS
        #error "MacOS is not supported!"
    #else
        #error "Unknown Apple platform!"
    #endif
    //  Android is based off the linux kernel, so __ANDROID__ must be checked first
#elif defined(__ANDROID__)          //  Android
    #define Dr_PLATFORM_ANDROID
    #error "Android is not supported!"
#elif defined(__linux__)            //  Linux
    #define Dr_PLATFORM_LINUX
    #error "Linux is not supported!"
#else
    //  Unknown compiler/platform   //
    #error "Unknown platform!"
#endif  //    END platform detection

#ifdef Dr_DEBUG
    #if defined(Dr_PLATFORM_WINDOWS)
        #define Dr_DEBUGBREAK() __debugbreak()
    #elif defined(Dr_PLATFORM_LINUX)
        #include <signal.h>
        #define Dr_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define Dr_ENABLE_ASSERTS
#else
    #define Dr_DEBUGBREAK()
#endif
