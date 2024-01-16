#pragma once

#include "../ImGui/ImGuiExtension.h"


// color
static const ImVec4 k_color_text = ImVec4(192.0f / 255.0f, 192.0f / 255.0f, 192.0f / 255.0f, 1.0f);
static const ImVec4 k_color_text_disabled = ImVec4(54.0f / 255.0f, 54.0f / 255.0f, 54.0f / 255.0f, 1.0f);
static const ImVec4 k_color_dark_very = ImVec4(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 1.0f);
static const ImVec4 k_color_dark = ImVec4(21.0f / 255.0f, 21.0f / 255.0f, 21.0f / 255.0f, 1.0f);
static const ImVec4 k_color_mid = ImVec4(36.0f / 255.0f, 36.0f / 255.0f, 36.0f / 255.0f, 1.0f);
static const ImVec4 k_color_light = ImVec4(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f, 1.0f);
static const ImVec4 k_color_shadow = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
static const ImVec4 k_color_interactive = ImVec4(56.0f / 255.0f, 56.0f / 255.0f, 56.0f / 255.0f, 1.0f);
static const ImVec4 k_color_interactive_hovered = ImVec4(0.450f, 0.450f, 0.450f, 1.000f);
static const ImVec4 k_color_check = ImVec4(26.0f / 255.0f, 140.0f / 255.0f, 192.0f / 255.0f, 1.0f);


static void apply_colors()
{
    // use default dark style as a base
    ImGui::StyleColorsDark();
    ImVec4* colors = ImGui::GetStyle().Colors;

    // colors
    colors[ImGuiCol_Text] = k_color_text;
    colors[ImGuiCol_TextDisabled] = k_color_text_disabled;
    colors[ImGuiCol_WindowBg] = k_color_mid;                 // Background of normal windows
    colors[ImGuiCol_ChildBg] = k_color_mid;                 // Background of child windows
    colors[ImGuiCol_PopupBg] = k_color_dark;                // Background of popups, menus, tooltips windows
    colors[ImGuiCol_Border] = k_color_interactive;
    colors[ImGuiCol_BorderShadow] = k_color_shadow;
    colors[ImGuiCol_FrameBg] = k_color_dark_very;           // Background of checkbox, radio button, plot, slider, text input
    colors[ImGuiCol_FrameBgHovered] = k_color_interactive;
    colors[ImGuiCol_FrameBgActive] = k_color_dark_very;
    colors[ImGuiCol_TitleBg] = k_color_dark;
    colors[ImGuiCol_TitleBgActive] = k_color_dark;
    colors[ImGuiCol_TitleBgCollapsed] = k_color_light;
    colors[ImGuiCol_MenuBarBg] = k_color_dark;
    colors[ImGuiCol_ScrollbarBg] = k_color_mid;
    colors[ImGuiCol_ScrollbarGrab] = k_color_interactive;
    colors[ImGuiCol_ScrollbarGrabHovered] = k_color_interactive_hovered;
    colors[ImGuiCol_ScrollbarGrabActive] = k_color_dark_very;
    colors[ImGuiCol_CheckMark] = k_color_check;
    colors[ImGuiCol_SliderGrab] = k_color_interactive;
    colors[ImGuiCol_SliderGrabActive] = k_color_dark_very;
    colors[ImGuiCol_Button] = k_color_interactive;
    colors[ImGuiCol_ButtonHovered] = k_color_interactive_hovered;
    colors[ImGuiCol_ButtonActive] = k_color_dark_very;
    colors[ImGuiCol_Header] = k_color_light;               // Header colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    colors[ImGuiCol_HeaderHovered] = k_color_interactive_hovered;
    colors[ImGuiCol_HeaderActive] = k_color_dark_very;
    colors[ImGuiCol_Separator] = k_color_dark_very;
    colors[ImGuiCol_SeparatorHovered] = k_color_light;
    colors[ImGuiCol_SeparatorActive] = k_color_light;
    colors[ImGuiCol_ResizeGrip] = k_color_interactive;
    colors[ImGuiCol_ResizeGripHovered] = k_color_interactive_hovered;
    colors[ImGuiCol_ResizeGripActive] = k_color_dark_very;
    colors[ImGuiCol_Tab] = k_color_light;
    colors[ImGuiCol_TabHovered] = k_color_interactive_hovered;
    colors[ImGuiCol_TabActive] = k_color_dark_very;
    colors[ImGuiCol_TabUnfocused] = k_color_light;
    colors[ImGuiCol_TabUnfocusedActive] = k_color_light;               // Might be called active, but it's active only because it's it's the only tab available, the user didn't really activate it
    colors[ImGuiCol_DockingPreview] = k_color_dark_very;           // Preview overlay color when about to docking something
    colors[ImGuiCol_DockingEmptyBg] = k_color_interactive;         // Background color for empty node (e.g. CentralNode with no window docked into it)
    colors[ImGuiCol_PlotLines] = k_color_interactive;
    colors[ImGuiCol_PlotLinesHovered] = k_color_interactive_hovered;
    colors[ImGuiCol_PlotHistogram] = k_color_interactive;
    colors[ImGuiCol_PlotHistogramHovered] = k_color_interactive_hovered;
    colors[ImGuiCol_TextSelectedBg] = k_color_dark;
    colors[ImGuiCol_DragDropTarget] = k_color_interactive_hovered; // Color when hovering over target
    colors[ImGuiCol_NavHighlight] = k_color_dark;                // Gamepad/keyboard: current highlighted item
    colors[ImGuiCol_NavWindowingHighlight] = k_color_dark;                // Highlight window when using CTRL+TAB
    colors[ImGuiCol_NavWindowingDimBg] = k_color_dark;                // Darken/colorize entire screen behind the CTRL+TAB window list, when active
    colors[ImGuiCol_ModalWindowDimBg] = k_color_dark;                // Darken/colorize entire screen behind a modal window, when one is active
}
