#include "EditorStyle_impl.h"
#include "EditorColor.h"
#include "EditorStyle.h"
#include "RHI/RHI_Definitions.h"
#include "../ImGui/ImGuiExtension.h"
#include "../ImGui/Implementation/ImGui_RHI.h"
#include "../ImGui/Implementation/imgui_impl_sdl2.h"

using namespace std;

namespace
{
    bool m_editor_begun = false;
    std::string name = "##main_window";

    static void process_event(Spartan::sp_variant data)
    {
        SDL_Event* event_sdl = static_cast<SDL_Event*>(get<void*>(data));
        ImGui_ImplSDL2_ProcessEvent(event_sdl);
    }
}


void EditorStyle_impl::Initialize()
{
    // initialize ImGui
    ImGui::CreateContext();

    // configure ImGui
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange; // cursor visibility is handled by the engine
    io.ConfigWindowsResizeFromEdges = true;
    io.ConfigViewportsNoTaskBarIcon = true;
    io.ConfigViewportsNoDecoration = true; // borderless child windows but with ImGui min, max and close buttons
    io.IniFilename = "editor.ini";

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
}

void EditorStyle_impl::SUBSCRIBE_TO_EVENT()
{
    // allow ImGui to get event's from the engine's event processing loop
    SP_SUBSCRIBE_TO_EVENT(Spartan::EventType::Sdl, SP_EVENT_HANDLER_VARIANT_STATIC(process_event));
}

void EditorStyle_impl::Shutdown()
{
    if (ImGui::GetCurrentContext())
    {
        ImGui::RHI::shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
}

void EditorStyle_impl::NewFrame()
{
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void EditorStyle_impl::BeginWindow(float offset_y)
{
    const auto window_flags =
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    // set window position and size
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + offset_y));
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - offset_y));
    ImGui::SetNextWindowViewport(viewport->ID);

    // set window style
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.0f);

    // begin window
    bool open = true;
    m_editor_begun = ImGui::Begin(name.c_str(), &open, window_flags);
    ImGui::PopStyleVar(3);

}

void EditorStyle_impl::BeginDockSpace()
{
    // begin dock space
    bool checkDockSpace = ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable && m_editor_begun;
    if (checkDockSpace == false) return;

    // dock space
    const auto window_id = ImGui::GetID(name.c_str());
    if (!ImGui::DockBuilderGetNode(window_id))
    {
        // reset current docking state
        ImGui::DockBuilderRemoveNode(window_id);
        ImGui::DockBuilderAddNode(window_id, ImGuiDockNodeFlags_None);
        ImGui::DockBuilderSetNodeSize(window_id, ImGui::GetMainViewport()->Size);

        // dockBuilderSplitNode(ImGuiID node_id, ImGuiDir split_dir, float size_ratio_for_node_at_dir, ImGuiID* out_id_dir, ImGuiID* out_id_other);
        ImGuiID dock_main_id = window_id;
        ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
        ImGuiID dock_right_down_id = ImGui::DockBuilderSplitNode(dock_right_id, ImGuiDir_Down, 0.6f, nullptr, &dock_right_id);
        ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);
        ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_down_id, ImGuiDir_Right, 0.6f, nullptr, &dock_down_id);

        // dock windows
        ImGui::DockBuilderDockWindow("World", dock_right_id);
        ImGui::DockBuilderDockWindow("Properties", dock_right_down_id);
        ImGui::DockBuilderDockWindow("Console", dock_down_id);
        ImGui::DockBuilderDockWindow("Assets", dock_down_right_id);
        ImGui::DockBuilderDockWindow("Viewport", dock_main_id);

        ImGui::DockBuilderFinish(dock_main_id);
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::DockSpace(window_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::PopStyleVar();
}

void EditorStyle_impl::EndWindow()
{
    // end window
    if (m_editor_begun)
        ImGui::End();
}

void EditorStyle_impl::Draw()
{
    // render
    ImGui::Render();
    ImGui::RHI::render(ImGui::GetDrawData());
}

void EditorStyle_impl::UpdateAndRenderImGuiViewports()
{
    bool checkViewport = ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable;
    if (checkViewport == false) return;

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}
