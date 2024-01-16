/*
Copyright(c) 2016-2024 Panos Karabelas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//= INCLUDES ====================================
#include "Editor.h"
#include "Styles/EditorStyle_impl.h"
#include "Core/Engine.h"
#include "Core/Settings.h"
#include "ImGui/ImGuiExtension.h"
#include "ImGui/Implementation/ImGui_RHI.h"
#include "ImGui/Implementation/imgui_impl_sdl2.h"
#include "Widgets/AssetBrowser.h"
#include "Widgets/Console.h"
#include "Widgets/MenuBar.h"
#include "Widgets/ProgressDialog.h"
#include "Widgets/Properties.h"
#include "Widgets/Viewport.h"
#include "Widgets/WorldViewer.h"
#include "Widgets/ShaderEditor.h"
#include "Widgets/ResourceViewer.h"
#include "Widgets/Profiler.h"
#include "Widgets/RenderOptions.h"
//===============================================

//= NAMESPACES =====
using namespace std;
//==================

namespace
{   
    MenuBar* widget_menu_bar = nullptr;
    Widget* widget_world     = nullptr;

    static void process_event(Spartan::sp_variant data)
    {
        SDL_Event* event_sdl = static_cast<SDL_Event*>(get<void*>(data));
        ImGui_ImplSDL2_ProcessEvent(event_sdl);
    }                                        
}

Editor::Editor()
{
    // initialize the engine
    Spartan::Engine::Initialize();

    // initialize ImGui
    ImGui::CreateContext();

    // configure ImGui
    ImGuiIO& io                      = ImGui::GetIO();
    io.ConfigFlags                  |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags                  |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags                  |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags                  |= ImGuiConfigFlags_NoMouseCursorChange; // cursor visibility is handled by the engine
    io.ConfigWindowsResizeFromEdges  = true;
    io.ConfigViewportsNoTaskBarIcon  = true;
    io.ConfigViewportsNoDecoration   = true; // borderless child windows but with ImGui min, max and close buttons
    io.IniFilename                   = "editor.ini";

    // load font
    string dir_fonts = Spartan::ResourceCache::GetResourceDirectory(Spartan::ResourceDirectory::Fonts) + "/";
    io.Fonts->AddFontFromFileTTF((dir_fonts + "Calibri.ttf").c_str(), k_font_size * Spartan::Window::GetDpiScale());
    io.FontGlobalScale = k_font_scale;

    // initialise ImGui backends
    SP_ASSERT_MSG(ImGui_ImplSDL2_Init(), "Failed to initialize ImGui's SDL backend");
    ImGui::RHI::Initialize();

    // apply colors and style
    apply_colors();
    apply_style();

    // initialization of some helper static classes
    IconLoader::Initialize();
    EditorHelper::Initialize(this);

    // create all ImGui widgets
    m_widgets.emplace_back(make_shared<Console>(this));
    m_widgets.emplace_back(make_shared<Profiler>(this));
    m_widgets.emplace_back(make_shared<ResourceViewer>(this));
    m_widgets.emplace_back(make_shared<ShaderEditor>(this));
    m_widgets.emplace_back(make_shared<RenderOptions>(this));
    m_widgets.emplace_back(make_shared<TextureViewer>(this));
    m_widgets.emplace_back(make_shared<MenuBar>(this));
    widget_menu_bar = static_cast<MenuBar*>(m_widgets.back().get());
    m_widgets.emplace_back(make_shared<Viewport>(this));
    m_widgets.emplace_back(make_shared<AssetBrowser>(this));
    m_widgets.emplace_back(make_shared<Properties>(this));
    m_widgets.emplace_back(make_shared<WorldViewer>(this));
    widget_world = m_widgets.back().get();
    m_widgets.emplace_back(make_shared<ProgressDialog>(this));

    // allow ImGui to get event's from the engine's event processing loop
    SP_SUBSCRIBE_TO_EVENT(Spartan::EventType::Sdl, SP_EVENT_HANDLER_VARIANT_STATIC(process_event));

    // register ImGui as a third party library (will show up in the about window)
    Spartan::Settings::RegisterThirdPartyLib("Dear ImGui", IMGUI_VERSION, "https://github.com/ocornut/imgui");
}

Editor::~Editor()
{
    if (ImGui::GetCurrentContext())
    {
        ImGui::RHI::shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    Spartan::Engine::Shutdown();
}

void Editor::Tick()
{
    // this is the main editor/engine loop
    while (!Spartan::Window::WantsToClose())
    {
        bool render_editor = Spartan::EngineFlags::IsFlagSet(Spartan::EngineMode::Editor);

        // imgui tick
        if (render_editor)
        {
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
        }

        // engine tick
        Spartan::Engine::Tick();

        // editor
        {
            // window
            if (render_editor)
            {
                // begin window
                BeginWindow();

                // tick widgets
                for (shared_ptr<Widget>& widget : m_widgets)
                {
                    widget->Tick();
                }

                // end window
                if (m_editor_begun)
                {
                    ImGui::End();
                }

                // render
                ImGui::Render();
                ImGui::RHI::render(ImGui::GetDrawData());
            }

            // child windows
            if (render_editor && ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        }
    }
}

void Editor::BeginWindow()
{
    const auto window_flags =
        ImGuiWindowFlags_MenuBar               |
        ImGuiWindowFlags_NoDocking             |
        ImGuiWindowFlags_NoTitleBar            |
        ImGuiWindowFlags_NoCollapse            |
        ImGuiWindowFlags_NoResize              |
        ImGuiWindowFlags_NoMove                |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    // set window position and size
    float offset_y = widget_menu_bar ? (widget_menu_bar->GetHeight() + widget_menu_bar->GetPadding()) : 0;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + offset_y));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - offset_y));
    ImGui::SetNextWindowViewport(viewport->ID);

    // set window style
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,   0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,    ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.0f);

    // begin window
    std::string name = "##main_window";
    bool open = true;
    m_editor_begun = ImGui::Begin(name.c_str(), &open, window_flags);
    ImGui::PopStyleVar(3);

    // begin dock space
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable && m_editor_begun)
    {
        // dock space
        const auto window_id = ImGui::GetID(name.c_str());
        if (!ImGui::DockBuilderGetNode(window_id))
        {
            // reset current docking state
            ImGui::DockBuilderRemoveNode(window_id);
            ImGui::DockBuilderAddNode(window_id, ImGuiDockNodeFlags_None);
            ImGui::DockBuilderSetNodeSize(window_id, ImGui::GetMainViewport()->Size);

            // dockBuilderSplitNode(ImGuiID node_id, ImGuiDir split_dir, float size_ratio_for_node_at_dir, ImGuiID* out_id_dir, ImGuiID* out_id_other);
            ImGuiID dock_main_id       = window_id;
            ImGuiID dock_right_id      = ImGui::DockBuilderSplitNode(dock_main_id,  ImGuiDir_Right, 0.2f,  nullptr, &dock_main_id);
            ImGuiID dock_right_down_id = ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Down,  0.6f,  nullptr, &dock_right_id);
            ImGuiID dock_down_id       = ImGui::DockBuilderSplitNode(dock_main_id,  ImGuiDir_Down,  0.25f, nullptr, &dock_main_id);
            ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_down_id,  ImGuiDir_Right, 0.6f,  nullptr, &dock_down_id);

            // dock windows
            ImGui::DockBuilderDockWindow("World",      dock_right_id);
            ImGui::DockBuilderDockWindow("Properties", dock_right_down_id);
            ImGui::DockBuilderDockWindow("Console",    dock_down_id);
            ImGui::DockBuilderDockWindow("Assets",     dock_down_right_id);
            ImGui::DockBuilderDockWindow("Viewport",   dock_main_id);

            ImGui::DockBuilderFinish(dock_main_id);
        }

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::DockSpace(window_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::PopStyleVar();
    }
}
