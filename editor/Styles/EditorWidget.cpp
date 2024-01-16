#include "EditorWidget.h"
#include "../Widgets/AssetBrowser.h"
#include "../Widgets/Console.h"
#include "../Widgets/MenuBar.h"
#include "../Widgets/ProgressDialog.h"
#include "../Widgets/Properties.h"
#include "../Widgets/Viewport.h"
#include "../Widgets/WorldViewer.h"
#include "../Widgets/ShaderEditor.h"
#include "../Widgets/ResourceViewer.h"
#include "../Widgets/Profiler.h"
#include "../Widgets/RenderOptions.h"
#include "../Widgets/TextureViewer.h"
//===============================================

//= NAMESPACES =====
using namespace std;
//==================

namespace
{
    MenuBar* widget_menu_bar = nullptr;
    Widget* widget_world = nullptr;
}

void EditorWidget::Initialize()
{
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
}

void EditorWidget::Tick()
{
    // tick widgets
    for (shared_ptr<Widget>& widget : m_widgets)
    {
        widget->Tick();
    }
}

float EditorWidget::GetWidgetOffsetY()
{
    float offset_y = widget_menu_bar ? (widget_menu_bar->GetHeight() + widget_menu_bar->GetPadding()) : 0;
    return offset_y;

}
