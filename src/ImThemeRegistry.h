#pragma once
//
// ImThemeRegistry
// ---------------
// Open-ended runtime registry of custom (addon-contributed) themes that sit
// on top of ImTheme's closed enum of built-in themes.
//
// Built-in themes (ImTheme::Theme_ImGuiColorsDark, ImTheme::Theme_Darcula,
// ...) live in ImTheme.h/.cpp. This file adds:
//
//   - A registry where any addon can RegisterCustom({id, label, apply}).
//   - A string-id-based ApplyByName(id) that tries the custom registry first,
//     then falls back to the built-in enum via ImTheme::FromName.
//   - A ShowSelector(currentId) widget that combines the two pools.
//   - SaveStyle/LoadStyle for binary ImGuiStyle snapshots (.bin).
//   - ApplyRandomAccent / ApplyCompactMetrics — small style helpers.
//   - Setup / SetUIScale / Commit — theme + HiDPI scale without manual
//     CaptureBaseStyle + ApplyScale at every call site.
//
// All of this is pure Dear ImGui; nothing depends on openFrameworks.
//
#include "ImTheme.h"

#include <functional>
#include <string>
#include <vector>

namespace ImTheme
{
    // -------------------------------------------------------------------------
    // Custom theme registry
    // -------------------------------------------------------------------------

    /// A theme contributed by an addon or app at runtime.
    /// `id`     - unique stable string used for persistence and ApplyByName().
    /// `label`  - human-readable name shown in ShowSelector().
    /// `apply`  - writes the theme into ImGui::GetStyle().
    struct CustomTheme
    {
        std::string           id;
        std::string           label;
        std::function<void()> apply;
    };

    /// Register a custom theme. If a theme with the same id was already
    /// registered, it is replaced (safe to call multiple times).
    void                              RegisterCustom(CustomTheme theme);

    /// Read-only view of currently registered custom themes.
    const std::vector<CustomTheme>&   Customs();

    // -------------------------------------------------------------------------
    // One-shot setup (preferred entry point)
    // -------------------------------------------------------------------------

    /// Apply a theme and wire up HiDPI scaling in one call. Picks
    /// `DetectOsScale()` when `uiScale <= 0`. Stores the scale for later
    /// `SetUIScale()` / theme switches (which re-commit automatically).
    bool        Setup(const std::string& themeId, float uiScale = 0.f);
    bool        Setup(Theme_ theme, float uiScale = 0.f);

    // -------------------------------------------------------------------------
    // String-id apply / current
    // -------------------------------------------------------------------------

    /// Apply a theme by its string id. Tries the custom registry first, then
    /// falls back to the built-in enum (via FromName).
    /// Returns true if the id matched something; false if unknown.
    /// On success, snapshots the unscaled style and applies the stored UI
    /// scale (`UIScale()`). Fires SetThemeChangedCallback() if set.
    bool        ApplyByName(const std::string& id);

    /// Name of the most recently applied theme (custom id or built-in Name()).
    /// Empty string if no theme has been applied through ApplyByName yet.
    const std::string& CurrentName();

    // -------------------------------------------------------------------------
    // Selector widget
    // -------------------------------------------------------------------------

    /// Combined selector for built-in + custom themes.
    /// `currentId` is updated in place when the user picks a different theme;
    /// the function also applies the picked theme. Returns true if changed.
    bool        ShowSelector(std::string& currentId);

    // -------------------------------------------------------------------------
    // Binary ImGuiStyle snapshot (.bin)
    // -------------------------------------------------------------------------

    /// Dump the current ImGui::GetStyle() to disk as a raw binary blob.
    bool        SaveStyle(const char* path);

    /// Restore an ImGuiStyle binary blob produced by SaveStyle.
    /// Returns false on read error; on success, the file's blob fully replaces
    /// the current style.
    bool        LoadStyle(const char* path);

    // -------------------------------------------------------------------------
    // Style helpers (originally on class ofxImGuiStyle)
    // -------------------------------------------------------------------------

    /// Reset Dear ImGui's style to the dark preset and randomise the accent
    /// hue/saturation. Useful for one-click visual experimentation.
    void        ApplyRandomAccent();

    /// Apply compact scrollbar/grab metrics on top of the current style.
    /// Defaults are tuned for HiDPI screens (smaller scrollbars than ImGui's
    /// stock metrics).
    void        ApplyCompactMetrics(float scrollbarSize     = 12.f,
                                    float scrollbarRounding = 4.f,
                                    float grabMinSize       = 8.f);

    // -------------------------------------------------------------------------
    // UI scale (stored; applied via Commit)
    // -------------------------------------------------------------------------

    /// Current UI scale (1.0 = native). Updated by Setup / SetUIScale /
    /// ApplyScale.
    float       UIScale();

    /// Set the UI scale and re-apply it from the captured baseline. No-op
    /// until a theme has been applied (Setup / ApplyByName / Commit).
    void        SetUIScale(float scale);

    /// Query the OS content scale (HiDPI / 4K) from GLFW when available;
    /// otherwise 1.0. Used by Setup() when uiScale <= 0.
    float       DetectOsScale();

    // -------------------------------------------------------------------------
    // Base-style capture & scale (advanced / manual tweaks)
    // -------------------------------------------------------------------------

    /// Snapshot the current ImGui::GetStyle() as the unscaled baseline, then
    /// apply `UIScale()`. Call after hand-editing metrics on GetStyle()
    /// (e.g. extra padding in example_LumaStudio).
    void        Commit();

    /// Snapshot only — rarely needed; ApplyByName / Setup call Commit().
    void        CaptureBaseStyle();

    bool        HasBaseStyle();

    /// Re-apply a scale factor (also updates the stored UIScale). Prefer
    /// SetUIScale() in application code.
    void        ApplyScale(float scale);

} // namespace ImTheme
