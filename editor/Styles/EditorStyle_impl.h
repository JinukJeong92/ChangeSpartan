#pragma once


class EditorStyle_impl
{
public:
    EditorStyle_impl() = default;

    void Initialize();
    void SUBSCRIBE_TO_EVENT();
    void Shutdown();   

    void NewFrame();
    void BeginWindow(float offset_y);
    void BeginDockSpace();
    void EndWindow();

    void Draw();
    void UpdateAndRenderImGuiViewports();

};
