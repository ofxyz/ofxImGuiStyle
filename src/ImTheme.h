#pragma once
#include "imgui.h"
#include <functional>

//
// ImTheme - theme tweak utilities for ImGui.
//
// Vendored and lightly adapted from pthom/hello_imgui:
//   https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/imgui_theme.h
//   https://github.com/pthom/hello_imgui/blob/master/src/hello_imgui/impl/imgui_theme.cpp
//
// Original attribution from upstream:
//   "Reuse and adaptation of imgui_theme.h and imgui_theme.cpp file is granted
//    for other projects, provided the origin of those files is stated in the
//    copied version."
//   "Some themes were adapted by themes posted by ImGui users at
//    https://github.com/ocornut/imgui/issues/707"
//
// Adaptations in this copy:
//   - namespace ImGuiTheme        -> ImTheme
//   - enum  ImGuiTheme_           -> Theme_
//   - struct ImGuiThemeTweaks     -> ThemeTweaks
//   - struct ImGuiTweakedTheme    -> TweakedTheme
//   - free fn ImGuiTheme_Name     -> Name
//   - free fn ImGuiTheme_FromName -> FromName
//   - HelloImGui::GetRunnerParams() callback hook replaced with a settable
//     std::function<void()> callback (SetThemeChangedCallback).
//
// For the runtime registry of custom (addon-contributed) themes and the
// open-ended ApplyByName(string) selector, see ImThemeRegistry.h.
//

namespace ImTheme
{
    enum Theme_
    {
        Theme_ImGuiColorsClassic = 0,
        Theme_ImGuiColorsDark,
        Theme_ImGuiColorsLight,
        Theme_MaterialFlat,
        Theme_PhotoshopStyle,
        Theme_GrayVariations,
        Theme_GrayVariations_Darker,
        Theme_MicrosoftStyle,
        Theme_Cherry,
        Theme_Darcula,
        Theme_DarculaDarker,
        Theme_LightRounded,
        Theme_SoDark_AccentBlue,
        Theme_SoDark_AccentYellow,
        Theme_SoDark_AccentRed,
        Theme_BlackIsBlack,
        Theme_WhiteIsWhite,
        Theme_Count
    };
    const char* Name(Theme_ theme);
    Theme_      FromName(const char* themeName);
    ImGuiStyle  ThemeToStyle(Theme_ theme);
    void        ApplyTheme(Theme_ theme);


    struct ThemeTweaks
    {
        // Common rounding for widgets. If < 0, this is ignored.
        float Rounding = -1.f;
        // If rounding is applied, scrollbar rounding needs to be adjusted to be visually pleasing in conjunction with other widgets roundings. Only applied if Rounding > 0.f)
        float RoundingScrollbarRatio = 4.f;
        // Change the alpha that will be applied to windows, popups, etc. If < 0, this is ignored.
        float AlphaMultiplier = -1.f;

        //
        // HSV Color tweaks
        //
        // Change the hue of all widgets (gray widgets will remain gray, since their saturation is zero). If < 0, this is ignored.
        float Hue = -1.f;
        // Multiply the saturation of all widgets (gray widgets will remain gray, since their saturation is zero). If < 0, this is ignored.
        float SaturationMultiplier = -1.f;
        // Multiply the value (luminance) of all front widgets. If < 0, this is ignored.
        float ValueMultiplierFront = -1.f;
        // Multiply the value (luminance) of all backgrounds. If < 0, this is ignored.
        float ValueMultiplierBg = -1.f;
        // Multiply the value (luminance) of text. If < 0, this is ignored.
        float ValueMultiplierText = -1.f;
        // Multiply the value (luminance) of FrameBg. If < 0, this is ignored.
        // (Background of checkbox, radio button, plot, slider, text input)
        float ValueMultiplierFrameBg = -1.f;

        ThemeTweaks() {}
    };

    struct TweakedTheme
    {
        Theme_      Theme  = Theme_DarculaDarker;
        ThemeTweaks Tweaks = ThemeTweaks();

        TweakedTheme(Theme_ theme = Theme_DarculaDarker, const ThemeTweaks& tweaks = ThemeTweaks())
            : Theme(theme), Tweaks(tweaks) {}
    };

    ImGuiStyle TweakedThemeThemeToStyle(const TweakedTheme& tweaked_theme);
    void       ApplyTweakedTheme(const TweakedTheme& tweaked_theme);

    // PushTweakedTheme() / PopTweakedTheme()
    // Push and pop a tweaked theme.
    //
    // Note: If you want the theme to apply globally to a window, you need to apply it
    //       *before* calling ImGui::Begin (i.e. callBeginEnd = false in a hosted docking
    //       window, then PushTweakedTheme -> Begin -> content -> End -> PopTweakedTheme).
    void PushTweakedTheme(const TweakedTheme& tweaked_theme);
    void PopTweakedTheme();

    // Show the theme selection listbox, the theme tweak widgets, as well as
    // ImGui::ShowStyleEditor. Returns true if modified (Warning: when using
    // ShowStyleEditor, no info about modification is transmitted.)
    bool ShowThemeTweakGui(TweakedTheme* tweaked_theme);

    // Some tweakable themes
    ImGuiStyle SoDark(float hue);
    ImGuiStyle ShadesOfGray(float rounding = 0.f, float value_multiplier_front = 1.f, float value_multiplier_bg = 1.f);
    ImGuiStyle Darcula(
        float rounding              = 1.f,
        float hue                   = -1.f,
        float saturation_multiplier = 1.f,
        float value_multiplier_front = 1.f,
        float value_multiplier_bg    = 1.f,
        float alpha_bg_transparency  = 1.f
    );

    // -------------------------------------------------------------------------
    // Adaptation hook (not in upstream)
    // -------------------------------------------------------------------------
    // Set a callback invoked whenever ApplyTheme() or the built-in theme
    // selector in ShowThemeTweakGui() changes the active style. Replaces the
    // upstream `HelloImGui::GetRunnerParams()->callbacks.ThemeChanged` plumbing.
    // Pass `nullptr` to clear.
    void SetThemeChangedCallback(std::function<void()> cb);

} // namespace ImTheme
