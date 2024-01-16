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
#include "Core/Engine.h"
#include "Core/Settings.h"
#include "Core/Window.h"
#include "Styles/EditorStyle_impl.h"
#include "Styles/EditorWidget.h"
//===============================================

//= NAMESPACES =====
using namespace std;
//==================

Editor::Editor()
{
    style = make_unique<EditorStyle_impl>();
    widget = make_unique< EditorWidget>();

    // initialize the engine
    Spartan::Engine::Initialize();

    // initialize Editor Gui
    style->Initialize();
    widget->Initialize();
    style->SUBSCRIBE_TO_EVENT();

    // register ImGui as a third party library (will show up in the about window)
    Spartan::Settings::RegisterThirdPartyLib("Dear ImGui", IMGUI_VERSION, "https://github.com/ocornut/imgui");
}

Editor::~Editor()
{
    style->Shutdown();
    Spartan::Engine::Shutdown();
}


void Editor::Tick()
{
    // this is the main editor/engine loop
    while (!Spartan::Window::WantsToClose())
    {
        bool render_editor = Spartan::EngineFlags::IsFlagSet(Spartan::EngineMode::Editor);

        // imgui tick
        if (render_editor) style->NewFrame();

        // engine tick
        Spartan::Engine::Tick();

        // editor
        {
            // window
            if (render_editor)
            {
                // begin window
                style->BeginWindow(widget->GetWidgetOffsetY());
                style->BeginDockSpace();

                // tick widgets
                widget->Tick();

                style->EndWindow();
                style->Draw();

                // child windows
                style->UpdateAndRenderImGuiViewports();
            }
        }
    }
}

