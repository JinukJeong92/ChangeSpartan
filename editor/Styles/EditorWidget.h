#pragma once

//= INCLUDES ===================
#include <vector>
#include <memory>
#include "../Widgets/Widget.h"
//==============================

class EditorWidget
{
public:
    EditorWidget() = default;
    void Initialize();
    void Tick();
    float GetWidgetOffsetY();


    template<typename T>
    T* GetWidget()
    {
        for (const auto& widget : m_widgets)
        {
            if (T* widget_t = dynamic_cast<T*>(widget.get()))
            {
                return widget_t;
            }
        }

        return nullptr;
    }

private:
    std::vector<std::shared_ptr<Widget>> m_widgets;
};
