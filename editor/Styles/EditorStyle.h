#pragma once

#include "../ImGui/ImGuiExtension.h"

// shapes
static const float k_roundness = 2.0f;

// font
static const float k_font_size = 16.0f;
static const float k_font_scale = 1.0f;


static void apply_style()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.ScrollbarSize = 20.0f;
    style.FramePadding = ImVec2(5, 5);
    style.ItemSpacing = ImVec2(6, 5);
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    style.WindowRounding = k_roundness;
    style.FrameRounding = k_roundness;
    style.PopupRounding = k_roundness;
    style.GrabRounding = k_roundness;
    style.ScrollbarRounding = k_roundness;
    style.Alpha = 1.0f;

    style.ScaleAllSizes(Spartan::Window::GetDpiScale());
}
